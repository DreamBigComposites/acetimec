/*
 * MIT License
 * Copyright (c) 2022 Brian T. Park
 */

#include "local_date_time.h"
#include "zone_processing.h"
#include "zoned_date_time.h"

static void atc_zoned_date_time_set_error(struct AtcZonedDateTime *zdt)
{
  zdt->zone_info = NULL;
}

void atc_zoned_date_time_from_epoch_seconds(
    struct AtcZoneProcessing *processing,
    const struct AtcZoneInfo *zone_info,
    atc_time_t epoch_seconds,
    struct AtcZonedDateTime *zdt)
{
  atc_zoned_date_time_set_error(zdt);
  if (epoch_seconds == kAtcInvalidEpochSeconds) return;

  bool status = atc_processing_offset_date_time_from_epoch_seconds(
      processing,
      zone_info,
      epoch_seconds,
      (struct AtcOffsetDateTime *) zdt);
  if (! status) return;

  zdt->zone_info = zone_info;
}

atc_time_t atc_zoned_date_time_to_epoch_seconds(
    const struct AtcZonedDateTime *zdt)
{
  return atc_offset_date_time_to_epoch_seconds(
      (const struct AtcOffsetDateTime*) zdt);
}
