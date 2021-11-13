#include <pybind11/pybind11.h>

#include <memory>
#include <string>

#include <bs3/pbs.hh>

#include "pxtypes.hh"

namespace py = pybind11;
namespace pf = pbsf;

auto group_data_reader(const std::string &ifname) {
    auto data_file = open_sequential_input_file(ifname, PandaXIVRealm());
    return data_file.read_one_type<GroupData>();
}

struct GroupReader {
    GroupReader() = delete;
    GroupReader(const std::string &ifname)
        : data_file{std::make_shared<decltype(
              pf::open_sequential_input_file(ifname, PandaXIVRealm()))>(
              std::move(open_sequential_input_file(ifname, PandaXIVRealm())))},
          data{data_file->read_one_type<GroupData>()}, is{data.begin()} {}
    std::shared_ptr<pbsf::sequential_file<std::ifstream, PandaXIVRealm>>
        data_file;
    decltype(data_file->read_one_type<GroupData>()) data;
    decltype(data.begin()) is;
    bool begin = true;
    GroupData nextData() {
        if (begin) {
            begin = false;
            return *is;
        } else {
            ++is;
            if (is != data.end())
                return *is;
            else
                return {};
        }
    }
};

PYBIND11_MODULE(ppbs, m) {
    m.doc() = "pandax-4t pbs data reader plugin";
    using namespace pybind11::literals;
    m.def("group_data_reader", &group_data_reader,
          "a function to read group data file", "file"_a);

    py::class_<GroupReader>(m, "GroupReader")
        .def(py::init<const std::string &>())
        .def("getNext", &GroupReader::nextData);

    py::class_<GroupData>(m, "GroupData")
	.def(py::init<>())
	.def_readonly("runNumber", &GroupData::runNumber)
	.def_readonly("groupNumber", &GroupData::groupNumber)
	.def_readonly("startTime", &GroupData::startTime)
	.def_readonly("endTime", &GroupData::endTime);
}
