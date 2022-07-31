#include "acunit.h"
#include <acetimec.h>

ACU_TEST(test_zonedb_sizes)
{
  // These numbers are correct for TZDB 2022a
  ACU_ASSERT(sizeof(kAtcZoneRegistry) / sizeof(struct AtcZoneInfo*) == 377);
  ACU_ASSERT(sizeof(kAtcZoneAndLinkRegistry) / sizeof(struct AtcZoneInfo*)
      == 594);
  ACU_ASSERT(sizeof(kAtcLinkRegistry) / sizeof(struct AtcLinkEntry)
      == 217);
  ACU_PASS();
}

//---------------------------------------------------------------------------

ACU_PARAMS();

int main()
{
  ACU_RUN_TEST(test_zonedb_sizes);
  ACU_SUMMARY();
}
