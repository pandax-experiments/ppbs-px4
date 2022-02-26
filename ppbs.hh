#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include <bs3/pbs.hh>

namespace py = pybind11;

template <typename T>
void declare_reader(py::module &m, const std::string &data_name) {
    py::class_<T>(m, data_name.c_str())
        .def(py::init<const std::string &>())
        .def("next", &T::next);
}

template <typename T>
void declare_indexed_reader(py::module &m, const std::string &data_name) {
    py::class_<T>(m, data_name.c_str())
        .def(py::init<const std::string &>())
        .def("next", &T::next)
	.def("previous", &T::previous)
	.def("get", &T::get)
	.def("indices", &T::indices);
}

template <typename T, typename Realm> struct SequentialReader {
    SequentialReader() = delete;
    SequentialReader(const std::string &s)
        : data_file{std::make_shared<decltype(pbsf::open_sequential_input_file(
              s, Realm()))>(std::move(open_sequential_input_file(s, Realm())))},
          data{data_file->template read_one_type<T>()}, is{data.begin()} {}
    std::shared_ptr<pbsf::sequential_file<std::ifstream, Realm>> data_file;
    decltype(data_file->template read_one_type<T>()) data;
    decltype(data.begin()) is;
    bool begin = true;
    T next() {
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

template <typename Index, typename T, typename Realm> struct IndexedReader {
    IndexedReader() = delete;
    IndexedReader(const std::string &ifname)
        : data_file{std::make_shared<decltype(
              pbsf::open_indexed_input_file<Index>(ifname, Realm()))>(
              std::move(
                  pbsf::open_indexed_input_file<Index>(ifname, Realm())))},
          is{data_file->begin()} {}
    std::shared_ptr<decltype(pbsf::open_indexed_input_file<Index>(
        std::declval<std::string>(), Realm()))>
        data_file;
    decltype(data_file->begin()) is;
    bool begin = true;

    std::pair<Index, T> next() {
        if (begin) {
            if (is == data_file->end()) {
                return {{}, {}};
            }
            begin = false;
            auto idx = is->first;
            auto gd = is->second->template as<T>();
            return {idx, gd};
        }
        ++is;
        if (is != data_file->end()) {
            return {is->first, is->second->template as<T>()};
            ;
        } else {
            return {{}, {}};
        }
    }

    std::pair<Index, T> previous() {
        if (begin || is == data_file->begin() || data_file->begin() == data_file->end()) {
	    begin = true;
	    return {{}, {}};
	}
	--is;
        if (is == data_file->begin()) {
            begin = true;
        }
	auto idx = is->first;
	auto gd = is->second->template as<T>();
	return {idx, gd};
    }

    T get(Index idx) {
	auto iss = data_file->find(idx);
	if (iss == data_file->end()) {
            return {};
        }
	is = iss;
	return iss->second->template as<T>();
    }

    std::vector<Index> indices() {
	return data_file->indices();
    }
};
