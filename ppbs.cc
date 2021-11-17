#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <memory>
#include <string>

#include <bs3/pbs.hh>

#include "pxtypes.hh"

namespace py = pybind11;
namespace pf = pbsf;

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

struct GroupIndexedReader {
    GroupIndexedReader() = delete;
    GroupIndexedReader(const std::string &ifname)
        : data_file{std::make_shared<decltype(
              pf::open_indexed_input_file<EventIndex>(ifname, PandaXIVRealm()))>(
              std::move(pf::open_indexed_input_file<EventIndex>(
                  ifname, PandaXIVRealm())))},
          is{data_file->begin()} {}
    std::shared_ptr<decltype(pbsf::open_indexed_input_file<EventIndex>(std::declval<std::string>(), PandaXIVRealm()))> data_file;
    decltype(data_file->begin()) is;
    bool begin = true;
    std::pair<EventIndex, GroupData> nextData() {
	if (begin) {
            if (is == data_file->end()) {
                return {{}, {}};
            }
            begin = false;
	    auto idx = is->first;
	    auto gd = is->second->template as <GroupData>();
            return {idx, gd};
        }
	++is;
        if (is != data_file->end()) {
            return {is->first, is->second->template as<GroupData>()};;
        } else {
            return {{}, {}};
        }
    }
};

struct PEIndexedReader {
    PEIndexedReader() = delete;
    PEIndexedReader(const std::string &s)
        : data_file{std::make_shared<decltype(
              pf::open_indexed_input_file<EventIndex>(s, PandaXIVRealm()))>(
              std::move(pf::open_indexed_input_file<EventIndex>(
                  s, PandaXIVRealm())))},
          is{data_file->begin()} {}
    std::shared_ptr<decltype(pbsf::open_indexed_input_file<EventIndex>(std::declval<std::string>(), PandaXIVRealm()))> data_file;
    decltype(data_file->begin()) is;
    bool begin = true;
    std::pair<EventIndex, PhysicalEventData> nextData() {
	if (begin) {
            if (is == data_file->end()) {
                return {{}, {}};
            }
            begin = false;
	    auto idx = is->first;
	    auto gd = is->second->template as <PhysicalEventData>();
            return {idx, gd};
        }
	++is;
        if (is != data_file->end()) {
            return {is->first, is->second->template as<PhysicalEventData>()};;
        } else {
            return {{}, {}};
        }
    }
};

PYBIND11_MODULE(ppbs, m) {
    m.doc() = "pandax-4t pbs data reader plugin";
    using namespace pybind11::literals;

    py::class_<GroupReader>(m, "GroupReader")
        .def(py::init<const std::string &>())
        .def("getNext", &GroupReader::nextData);

    py::class_<GroupData>(m, "GroupData")
	.def(py::init<>())
	.def_readonly("runNumber", &GroupData::runNumber)
	.def_readonly("groupNumber", &GroupData::groupNumber)
	.def_readonly("startTime", &GroupData::startTime)
	.def_readonly("endTime", &GroupData::endTime);

    py::class_<EventIndex>(m, "EventIndex")
	.def(py::init<>())
	.def_readonly("runNumber", &EventIndex::runNumber)
	.def_readonly("fileNumber", &EventIndex::fileNumber)
	.def_readonly("eventNumber", &EventIndex::eventNumber);

    py::class_<GroupIndexedReader>(m, "GroupIndexedReader")
        .def(py::init<const std::string &>())
        .def("getNext", &GroupIndexedReader::nextData);

    py::class_<EventNumber>(m, "EventNumber")
	.def(py::init<>())
	.def_readonly("eventNumber", &EventNumber::eventNumber);

    py::class_<PhysicalEventData>(m, "PhysicalEventData")
	.def(py::init<>())
	.def_readonly("runNumber", &PhysicalEventData::runNumber)
	.def_readonly("number", &PhysicalEventData::number);

    py::enum_<HitType>(m, "PhysicalEventType")
	.value("NORMAL", HitType::NORMAL)
	.value("NOISE", HitType::NOISE)
	.value("SATURATE", HitType::SATURATE)
	.value("BASELINE", HitType::BASELINE)
	.export_values();

    py::class_<PEIndexedReader>(m, "PEIndexedReader")
	.def(py::init<const std::string &>())
	.def("getNext", &PEIndexedReader::nextData);
}
