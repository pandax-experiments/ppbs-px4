struct EventNumber;
struct EventIndex;
struct RawPmtSegment;
struct CalibPmtSegment;
struct RawData;
struct GroupData;
struct CalibData;
struct SingleHit;
struct HitData;
struct Signal;
struct SignalData;
struct PhysicalEventData;

struct EventNumber {
    uint32_t eventNumber;

    bool operator==(const EventNumber& other) const
    {
        return (eventNumber==other.eventNumber);
    }

    template <class Other>
    auto operator<(const Other& other) const
        -> typename std::enable_if<std::is_same<Other, EventNumber>::value, decltype(bool(eventNumber<other.eventNumber))>::type
    {
        if (eventNumber != other.eventNumber) return eventNumber < other.eventNumber;
        return false;
    }

    PBSS_TUPLE_MEMBERS(PBSS_TUPLE_MEMBER(&EventNumber::eventNumber));
};

struct EventIndex {
    uint32_t runNumber;
    uint32_t fileNumber;
    uint32_t eventNumber;

    bool operator==(const EventIndex& other) const
    {
        return (runNumber==other.runNumber) && (fileNumber==other.fileNumber) && (eventNumber==other.eventNumber);
    }

    template <class Other>
    auto operator<(const Other& other) const
        -> typename std::enable_if<std::is_same<Other, EventIndex>::value, decltype(bool(runNumber<other.runNumber), bool(fileNumber<other.fileNumber), bool(eventNumber<other.eventNumber))>::type
    {
        if (runNumber != other.runNumber) return runNumber < other.runNumber;
        if (fileNumber != other.fileNumber) return fileNumber < other.fileNumber;
        if (eventNumber != other.eventNumber) return eventNumber < other.eventNumber;
        return false;
    }

    PBSS_TUPLE_MEMBERS(PBSS_TUPLE_MEMBER(&EventIndex::runNumber), PBSS_TUPLE_MEMBER(&EventIndex::fileNumber), PBSS_TUPLE_MEMBER(&EventIndex::eventNumber));
};

struct RawPmtSegment {
    uint64_t startTime;
    uint32_t channelNumber;
    std::vector<uint16_t> adcValue;

    bool operator==(const RawPmtSegment& other) const
    {
        return (startTime==other.startTime) && (channelNumber==other.channelNumber) && (adcValue==other.adcValue);
    }

    template <class Other>
    auto operator<(const Other& other) const
        -> typename std::enable_if<std::is_same<Other, RawPmtSegment>::value, decltype(bool(startTime<other.startTime), bool(channelNumber<other.channelNumber), bool(adcValue<other.adcValue))>::type
    {
        if (startTime != other.startTime) return startTime < other.startTime;
        if (channelNumber != other.channelNumber) return channelNumber < other.channelNumber;
        if (adcValue != other.adcValue) return adcValue < other.adcValue;
        return false;
    }

    PBSS_TUPLE_MEMBERS(PBSS_TUPLE_MEMBER(&RawPmtSegment::startTime), PBSS_TUPLE_MEMBER(&RawPmtSegment::channelNumber), PBSS_TUPLE_MEMBER(&RawPmtSegment::adcValue));
};

struct CalibPmtSegment {
    uint64_t startTime;
    uint32_t channelNumber;
    std::vector<float> peValue;

    bool operator==(const CalibPmtSegment& other) const
    {
        return (startTime==other.startTime) && (channelNumber==other.channelNumber) && (peValue==other.peValue);
    }

    template <class Other>
    auto operator<(const Other& other) const
        -> typename std::enable_if<std::is_same<Other, CalibPmtSegment>::value, decltype(bool(startTime<other.startTime), bool(channelNumber<other.channelNumber), bool(peValue<other.peValue))>::type
    {
        if (startTime != other.startTime) return startTime < other.startTime;
        if (channelNumber != other.channelNumber) return channelNumber < other.channelNumber;
        if (peValue != other.peValue) return peValue < other.peValue;
        return false;
    }

    PBSS_TUPLE_MEMBERS(PBSS_TUPLE_MEMBER(&CalibPmtSegment::startTime), PBSS_TUPLE_MEMBER(&CalibPmtSegment::channelNumber), PBSS_TUPLE_MEMBER(&CalibPmtSegment::peValue));
};

struct RawData {
    uint32_t packageId;
    uint32_t packageNumber;
    uint64_t packageTime;
    uint64_t packageEndTime;
    std::vector<RawPmtSegment> rawSegments;

    bool operator==(const RawData& other) const
    {
        return (packageId==other.packageId) && (packageNumber==other.packageNumber) && (packageTime==other.packageTime) && (packageEndTime==other.packageEndTime) && (rawSegments==other.rawSegments);
    }

    PBSS_TAGGED_STRUCT(PBSS_TAG_MEMBER(1, &RawData::packageId), PBSS_TAG_MEMBER(2, &RawData::packageNumber), PBSS_TAG_MEMBER(3, &RawData::packageTime), PBSS_TAG_MEMBER(4, &RawData::packageEndTime), PBSS_TAG_MEMBER(5, &RawData::rawSegments));
};

struct GroupData {
    uint32_t runNumber;
    uint32_t groupNumber;
    uint64_t startTime;
    uint64_t endTime;
    std::vector<RawPmtSegment> segments;

    bool operator==(const GroupData& other) const
    {
        return (runNumber==other.runNumber) && (groupNumber==other.groupNumber) && (startTime==other.startTime) && (endTime==other.endTime) && (segments==other.segments);
    }

    PBSS_TAGGED_STRUCT(PBSS_TAG_MEMBER(1, &GroupData::runNumber), PBSS_TAG_MEMBER(2, &GroupData::groupNumber), PBSS_TAG_MEMBER(3, &GroupData::startTime), PBSS_TAG_MEMBER(4, &GroupData::endTime), PBSS_TAG_MEMBER(5, &GroupData::segments));
};

struct CalibData {
    uint32_t runNumber;
    uint32_t groupNumber;
    uint64_t startTime;
    uint64_t endTime;
    std::vector<CalibPmtSegment> segments;

    bool operator==(const CalibData& other) const
    {
        return (runNumber==other.runNumber) && (groupNumber==other.groupNumber) && (startTime==other.startTime) && (endTime==other.endTime) && (segments==other.segments);
    }

    PBSS_TAGGED_STRUCT(PBSS_TAG_MEMBER(1, &CalibData::runNumber), PBSS_TAG_MEMBER(2, &CalibData::groupNumber), PBSS_TAG_MEMBER(3, &CalibData::startTime), PBSS_TAG_MEMBER(4, &CalibData::endTime), PBSS_TAG_MEMBER(5, &CalibData::segments));
};

enum class HitType : uint8_t {
    NORMAL=1, NOISE, SATURATE, BASELINE
};

struct SingleHit {
    uint32_t channelNumber;
    uint64_t startTime;
    uint64_t peakTime;
    uint64_t width;
    float area;
    float height;
    float preBaseline;
    float postBaseline;
    float rmsPreBaseline;
    float rmsPostBaseline;
    float hitSearchThreshold;
    HitType type;
    CalibPmtSegment segment;

    bool operator==(const SingleHit& other) const
    {
        return (channelNumber==other.channelNumber) && (startTime==other.startTime) && (peakTime==other.peakTime) && (width==other.width) && (area==other.area) && (height==other.height) && (preBaseline==other.preBaseline) && (postBaseline==other.postBaseline) && (rmsPreBaseline==other.rmsPreBaseline) && (rmsPostBaseline==other.rmsPostBaseline) && (hitSearchThreshold==other.hitSearchThreshold) && (type==other.type) && (segment==other.segment);
    }

    PBSS_TAGGED_STRUCT(PBSS_TAG_MEMBER(1, &SingleHit::channelNumber), PBSS_TAG_MEMBER(2, &SingleHit::startTime), PBSS_TAG_MEMBER(3, &SingleHit::peakTime), PBSS_TAG_MEMBER(4, &SingleHit::width), PBSS_TAG_MEMBER(5, &SingleHit::area), PBSS_TAG_MEMBER(6, &SingleHit::height), PBSS_TAG_MEMBER(7, &SingleHit::preBaseline), PBSS_TAG_MEMBER(8, &SingleHit::postBaseline), PBSS_TAG_MEMBER(9, &SingleHit::rmsPreBaseline), PBSS_TAG_MEMBER(10, &SingleHit::rmsPostBaseline), PBSS_TAG_MEMBER(11, &SingleHit::hitSearchThreshold), PBSS_TAG_MEMBER(12, &SingleHit::type), PBSS_TAG_MEMBER(13, &SingleHit::segment));
};

struct HitData {
    uint32_t runNumber;
    uint32_t groupNumber;
    std::vector<SingleHit> hitsVec;

    bool operator==(const HitData& other) const
    {
        return (runNumber==other.runNumber) && (groupNumber==other.groupNumber) && (hitsVec==other.hitsVec);
    }

    PBSS_TAGGED_STRUCT(PBSS_TAG_MEMBER(1, &HitData::runNumber), PBSS_TAG_MEMBER(2, &HitData::groupNumber), PBSS_TAG_MEMBER(3, &HitData::hitsVec));
};

enum class SignalType : uint8_t {
    S1=1, S2, NOISE, UNKNOWN, SPARKING, S1_WITH_VETO, S2_WITH_VETO, PMT_SPARKING
};

struct Signal {
    uint64_t startTime;
    uint64_t endTime;
    uint64_t width;
    float height;
    SignalType type;
    std::vector<SingleHit> hits;
    std::map<uint32_t, CalibPmtSegment> waveform;
    std::map<std::string, float> fPars;
    std::map<std::string, int> iPars;
    std::vector<SingleHit> vetoHits;

    bool operator==(const Signal& other) const
    {
        return (startTime==other.startTime) && (endTime==other.endTime) && (width==other.width) && (height==other.height) && (type==other.type) && (hits==other.hits) && (waveform==other.waveform) && (fPars==other.fPars) && (iPars==other.iPars) && (vetoHits==other.vetoHits);
    }

    PBSS_TAGGED_STRUCT(PBSS_TAG_MEMBER(1, &Signal::startTime), PBSS_TAG_MEMBER(2, &Signal::endTime), PBSS_TAG_MEMBER(3, &Signal::width), PBSS_TAG_MEMBER(4, &Signal::height), PBSS_TAG_MEMBER(5, &Signal::type), PBSS_TAG_MEMBER(6, &Signal::hits), PBSS_TAG_MEMBER(7, &Signal::waveform), PBSS_TAG_MEMBER(8, &Signal::fPars), PBSS_TAG_MEMBER(9, &Signal::iPars), PBSS_TAG_MEMBER(10, &Signal::vetoHits));
};

struct SignalData {
    uint32_t runNumber;
    uint32_t groupNumber;
    std::vector<Signal> signals;

    bool operator==(const SignalData& other) const
    {
        return (runNumber==other.runNumber) && (groupNumber==other.groupNumber) && (signals==other.signals);
    }

    PBSS_TAGGED_STRUCT(PBSS_TAG_MEMBER(1, &SignalData::runNumber), PBSS_TAG_MEMBER(2, &SignalData::groupNumber), PBSS_TAG_MEMBER(3, &SignalData::signals));
};

enum class PhysicalEventType : uint8_t {
    UNKNOWN=1, SS, MS, BI_PO_214, BI_PO_212, KR_85, OTHER, S1_ONLY, S2_ONLY
};

struct PhysicalEventData {
    uint32_t runNumber;
    EventNumber number;
    PhysicalEventType type;
    std::vector<Signal> signals;

    bool operator==(const PhysicalEventData& other) const
    {
        return (runNumber==other.runNumber) && (number==other.number) && (type==other.type) && (signals==other.signals);
    }

    PBSS_TAGGED_STRUCT(PBSS_TAG_MEMBER(1, &PhysicalEventData::runNumber), PBSS_TAG_MEMBER(2, &PhysicalEventData::number), PBSS_TAG_MEMBER(3, &PhysicalEventData::type), PBSS_TAG_MEMBER(4, &PhysicalEventData::signals));
};
