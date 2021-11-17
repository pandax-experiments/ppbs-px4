#include "ppbs.hh"

#include "pxtypes.hh"

using GroupReader = SequentialReader<GroupData, PandaXIVRealm>;
using PhysicalEventReader = SequentialReader<PhysicalEventData, PandaXIVRealm>;

using IndexedGroupReader = IndexedReader<EventIndex, GroupData, PandaXIVRealm>;
using IndexedPEReader = IndexedReader<EventIndex, PhysicalEventData, PandaXIVRealm>;

PYBIND11_MODULE(ppbsn, m) {
    m.doc() = "pandax-4t pbs data reader plugin";
    using namespace pybind11::literals;
    declare_reader<GroupReader>(m, "GroupReader");
    declare_indexed_reader<IndexedPEReader>(m, "IndexedPEReader");
}
