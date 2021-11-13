#ifndef PXTYPES_HH
#define PXTYPES_HH

#include <cstdint>
#include <string>
#include <vector>
#include <map>

#include <pxtypes-generated.hh>

PBSF_DECLARE_REALM(
  PandaXIVRealm, 2017,
  PBSF_REGISTER_TYPE(1, RawData),
  PBSF_REGISTER_TYPE(2, GroupData),
  PBSF_REGISTER_TYPE(3, CalibData),
  PBSF_REGISTER_TYPE(4, HitData),
  PBSF_REGISTER_TYPE(5, SignalData),
  PBSF_REGISTER_TYPE(6, PhysicalEventData),
  PBSF_REGISTER_TYPE(100, EventNumber),
  PBSF_REGISTER_TYPE(200, uint32_t),
  PBSF_REGISTER_TYPE(300, EventIndex));

#endif /* PXTYPES_HH */
