#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <memory>
#include <string>

#include <bs3/pbs.hh>

#include "ppbs.hh"

#include "pxtypes.hh"

namespace py = pybind11;
namespace pf = pbsf;

using RawDataReader = SequentialReader<RawData, PandaXIVRealm>;
using RawDataEventIndexReader = IndexedReader<EventIndex, RawData, PandaXIVRealm>;
using RawDataUint32Reader = IndexedReader<uint32_t, RawData, PandaXIVRealm>;

using GroupDataReader = SequentialReader<GroupData, PandaXIVRealm>;
using GroupDataEventIndexReader = IndexedReader<EventIndex, GroupData, PandaXIVRealm>;
using GroupDataUint32Reader = IndexedReader<uint32_t, GroupData, PandaXIVRealm>;

using CalibDataReader = SequentialReader<CalibData, PandaXIVRealm>;
using CalibDataEventIndexReader = IndexedReader<EventIndex, CalibData, PandaXIVRealm>;
using CalibDataUint32Reader = IndexedReader<uint32_t, CalibData, PandaXIVRealm>;

using HitDataReader = SequentialReader<HitData, PandaXIVRealm>;
using HitDataEventIndexReader = IndexedReader<EventIndex, HitData, PandaXIVRealm>;
using HitDataUint32Reader = IndexedReader<uint32_t, HitData, PandaXIVRealm>;

using SignalDataReader = SequentialReader<SignalData, PandaXIVRealm>;
using SignalDataEventIndexReader = IndexedReader<EventIndex, SignalData, PandaXIVRealm>;
using SignalDataUint32Reader = IndexedReader<uint32_t, SignalData, PandaXIVRealm>;

using PhysicalEventDataReader = SequentialReader<PhysicalEventData, PandaXIVRealm>;
using PhysicalEventDataEventIndexReader = IndexedReader<EventIndex, PhysicalEventData, PandaXIVRealm>;
using PhysicalEventDataUint32Reader = IndexedReader<uint32_t, PhysicalEventData, PandaXIVRealm>;

PYBIND11_MODULE(ppbs, m) {
    m.doc() = "pbs data reader";
    using namespace pybind11::literals;

    declare_reader<RawDataReader>(m, "RawDataReader");
    declare_reader<GroupDataReader>(m, "GroupDataReader");
    declare_reader<CalibDataReader>(m, "CalibDataReader");
    declare_reader<HitDataReader>(m, "HitDataReader");
    declare_reader<SignalDataReader>(m, "SignalDataReader");
    declare_reader<PhysicalEventDataReader>(m, "PhysicalEventDataReader");
    declare_indexed_reader<RawDataEventIndexReader>(m, "RawDataEventIndexReader");
    declare_indexed_reader<RawDataUint32Reader>(m, "RawDataUint32Reader");
    declare_indexed_reader<GroupDataEventIndexReader>(m, "GroupDataEventIndexReader");
    declare_indexed_reader<GroupDataUint32Reader>(m, "GroupDataUint32Reader");
    declare_indexed_reader<CalibDataEventIndexReader>(m, "CalibDataEventIndexReader");
    declare_indexed_reader<CalibDataUint32Reader>(m, "CalibDataUint32Reader");
    declare_indexed_reader<HitDataEventIndexReader>(m, "HitDataEventIndexReader");
    declare_indexed_reader<HitDataUint32Reader>(m, "HitDataUint32Reader");
    declare_indexed_reader<SignalDataEventIndexReader>(m, "SignalDataEventIndexReader");
    declare_indexed_reader<SignalDataUint32Reader>(m, "SignalDataUint32Reader");
    declare_indexed_reader<PhysicalEventDataEventIndexReader>(m, "PhysicalEventDataEventIndexReader");
    declare_indexed_reader<PhysicalEventDataUint32Reader>(m, "PhysicalEventDataUint32Reader");

    py::class_<EventNumber>(m, "EventNumber")
        .def(py::init<>())
        .def_readonly("eventNumber", &EventNumber::eventNumber);

    py::class_<EventIndex>(m, "EventIndex")
        .def(py::init<>())
        .def_readonly("runNumber", &EventIndex::runNumber)
        .def_readonly("fileNumber", &EventIndex::fileNumber)
        .def_readonly("eventNumber", &EventIndex::eventNumber);

    py::class_<RawPmtSegment>(m, "RawPmtSegment")
        .def(py::init<>())
        .def_readonly("startTime", &RawPmtSegment::startTime)
        .def_readonly("channelNumber", &RawPmtSegment::channelNumber)
        .def_readonly("adcValue", &RawPmtSegment::adcValue);

    py::class_<CalibPmtSegment>(m, "CalibPmtSegment")
        .def(py::init<>())
        .def_readonly("startTime", &CalibPmtSegment::startTime)
        .def_readonly("channelNumber", &CalibPmtSegment::channelNumber)
        .def_readonly("peValue", &CalibPmtSegment::peValue);

    py::class_<RawData>(m, "RawData")
        .def(py::init<>())
        .def_readonly("packageId", &RawData::packageId)
        .def_readonly("packageNumber", &RawData::packageNumber)
        .def_readonly("packageTime", &RawData::packageTime)
        .def_readonly("packageEndTime", &RawData::packageEndTime)
        .def_readonly("rawSegments", &RawData::rawSegments);

    py::class_<GroupData>(m, "GroupData")
        .def(py::init<>())
        .def_readonly("runNumber", &GroupData::runNumber)
        .def_readonly("groupNumber", &GroupData::groupNumber)
        .def_readonly("startTime", &GroupData::startTime)
        .def_readonly("endTime", &GroupData::endTime)
        .def_readonly("segments", &GroupData::segments);

    py::class_<CalibData>(m, "CalibData")
        .def(py::init<>())
        .def_readonly("runNumber", &CalibData::runNumber)
        .def_readonly("groupNumber", &CalibData::groupNumber)
        .def_readonly("startTime", &CalibData::startTime)
        .def_readonly("endTime", &CalibData::endTime)
        .def_readonly("segments", &CalibData::segments);

    py::enum_<HitType>(m, "HitType")
        .value("NORMAL", HitType::NORMAL)
        .value("NOISE", HitType::NOISE)
        .value("SATURATE", HitType::SATURATE)
        .value("BASELINE", HitType::BASELINE)
        .export_values();

    py::class_<SingleHit>(m, "SingleHit")
        .def(py::init<>())
        .def_readonly("channelNumber", &SingleHit::channelNumber)
        .def_readonly("startTime", &SingleHit::startTime)
        .def_readonly("peakTime", &SingleHit::peakTime)
        .def_readonly("width", &SingleHit::width)
        .def_readonly("area", &SingleHit::area)
        .def_readonly("height", &SingleHit::height)
        .def_readonly("preBaseline", &SingleHit::preBaseline)
        .def_readonly("postBaseline", &SingleHit::postBaseline)
        .def_readonly("rmsPreBaseline", &SingleHit::rmsPreBaseline)
        .def_readonly("rmsPostBaseline", &SingleHit::rmsPostBaseline)
        .def_readonly("hitSearchThreshold", &SingleHit::hitSearchThreshold)
        .def_readonly("type", &SingleHit::type)
        .def_readonly("segment", &SingleHit::segment);

    py::class_<HitData>(m, "HitData")
        .def(py::init<>())
        .def_readonly("runNumber", &HitData::runNumber)
        .def_readonly("groupNumber", &HitData::groupNumber)
        .def_readonly("hitsVec", &HitData::hitsVec);

    py::enum_<SignalType>(m, "SignalType")
        .value("S1", SignalType::S1)
        .value("S2", SignalType::S2)
        .value("NOISE", SignalType::NOISE)
        .value("UNKNOWN", SignalType::UNKNOWN)
        .value("SPARKING", SignalType::SPARKING)
        .value("S1_WITH_VETO", SignalType::S1_WITH_VETO)
        .value("S2_WITH_VETO", SignalType::S2_WITH_VETO)
        .value("PMT_SPARKING", SignalType::PMT_SPARKING)
        .export_values();

    py::class_<Signal>(m, "Signal")
        .def(py::init<>())
        .def_readonly("startTime", &Signal::startTime)
        .def_readonly("endTime", &Signal::endTime)
        .def_readonly("width", &Signal::width)
        .def_readonly("height", &Signal::height)
        .def_readonly("type", &Signal::type)
        .def_readonly("hits", &Signal::hits)
        .def_readonly("waveform", &Signal::waveform)
        .def_readonly("fPars", &Signal::fPars)
        .def_readonly("iPars", &Signal::iPars)
        .def_readonly("vetoHits", &Signal::vetoHits);

    py::class_<SignalData>(m, "SignalData")
        .def(py::init<>())
        .def_readonly("runNumber", &SignalData::runNumber)
        .def_readonly("groupNumber", &SignalData::groupNumber)
        .def_readonly("signals", &SignalData::signals);

    py::enum_<PhysicalEventType>(m, "PhysicalEventType")
        .value("UNKNOWN", PhysicalEventType::UNKNOWN)
        .value("SS", PhysicalEventType::SS)
        .value("MS", PhysicalEventType::MS)
        .value("BI_PO_214", PhysicalEventType::BI_PO_214)
        .value("BI_PO_212", PhysicalEventType::BI_PO_212)
        .value("KR_85", PhysicalEventType::KR_85)
        .value("OTHER", PhysicalEventType::OTHER)
        .value("S1_ONLY", PhysicalEventType::S1_ONLY)
        .value("S2_ONLY", PhysicalEventType::S2_ONLY)
        .export_values();

    py::class_<PhysicalEventData>(m, "PhysicalEventData")
        .def(py::init<>())
        .def_readonly("runNumber", &PhysicalEventData::runNumber)
        .def_readonly("number", &PhysicalEventData::number)
        .def_readonly("type", &PhysicalEventData::type)
        .def_readonly("signals", &PhysicalEventData::signals);

}
