#include <stdlib.h> // exit()
#include <stdio.h>
#include <acetimec.h>

AtcZoneProcessing processing_la; // Los Angeles
AtcZoneProcessing processing_ny; // New York

// initialize the time zone processing workspace
void setup()
{
  atc_processing_init(&processing_la);
  atc_processing_init(&processing_ny);
}

void print_dates()
{
  fprintf(stderr, "======== ZonedDateTime from epoch seconds\n");

  atc_time_t seconds = 3432423;
  fprintf(stderr, "Epoch Seconds: %ld\n", (long) seconds);

  // Convert epoch seconds to date/time components for given time zone.
  AtcTimeZone tzla = {&kAtcZoneAmerica_Los_Angeles, &processing_la};
  AtcZonedDateTime zdtla;
  int8_t err = atc_zoned_date_time_from_epoch_seconds(&zdtla, seconds, tzla);
  if (err) { 
    fprintf(stderr,
        "ERROR: Unable to create ZonedDateTime from epoch seconds\n");
    exit(1);
  }

  // Print the date for Los Angeles.
  char buf[80];
  struct AtcStringBuffer sb;
  atc_buf_init(&sb, buf, 80);
  atc_zoned_date_time_print(&zdtla, &sb);
  atc_buf_close(&sb);
  fprintf(stderr, "Los Angeles: %s\n", sb.p);

  // Convert zoned_date_time to back to epoch seconds.
  atc_time_t epoch_seconds = atc_zoned_date_time_to_epoch_seconds(&zdtla);
  if (epoch_seconds == kAtcInvalidEpochSeconds) {
    fprintf(stderr,
        "ERROR: Unable to convert ZonedDateTime to epoch seconds\n");
    exit(1);
  }
  if (seconds != epoch_seconds) {
    fprintf(stderr,
        "ERROR: Converted seconds (%ld) != original seconds (%ld)\n",
        (long) epoch_seconds, (long) seconds);
    exit(1);
  }
  fprintf(stderr, "Converted Seconds: %ld\n", (long) epoch_seconds);

  fprintf(stderr, "======== ZonedDateTime from LocalDateTime\n");

  // Start with a LocalDateTime in an overlap.
  AtcLocalDateTime ldt = {2022, 11, 6, 1, 30, 0};
  atc_buf_reset(&sb);
  atc_local_date_time_print(&ldt, &sb);
  atc_buf_close(&sb);
  fprintf(stderr, "LocalDateTime: %s\n", sb.p);
  fprintf(stderr, "fold: 1\n");

  // Convert components to zoned_date_time. 2022-11-06 01:30 occurred twice. Set
  // fold=1 to select the second occurrence.
  err = atc_zoned_date_time_from_local_date_time(
      &zdtla, &ldt, 1 /*fold*/, tzla);
  if (err) { 
    fprintf(stderr,
        "ERROR: Unable to create ZonedDateTime from LocalDateTime\n");
    exit(1);
  }

  // Print the date time.
  atc_buf_reset(&sb);
  atc_zoned_date_time_print(&zdtla, &sb);
  atc_buf_close(&sb);
  fprintf(stderr, "Los Angeles: %s\n", sb.p);
  epoch_seconds = atc_zoned_date_time_to_epoch_seconds(&zdtla);
  fprintf(stderr, "Epoch Seconds: %ld\n", (long) epoch_seconds);

  fprintf(stderr, "======== ZonedDateTime to different time zone\n");

  // convert America/Los_Angles to America/New_York
  AtcTimeZone tzny = {&kAtcZoneAmerica_New_York, &processing_ny};
  AtcZonedDateTime zdtny;
  err = atc_zoned_date_time_convert(&zdtla, tzny, &zdtny);
  if (err) { 
    fprintf(stderr,
        "ERROR: Unable to convert ZonedDateTime to New York time zone\n");
    exit(1);
  }

  atc_buf_reset(&sb);
  atc_zoned_date_time_print(&zdtny, &sb);
  atc_buf_close(&sb);
  fprintf(stderr, "New York: %s\n", sb.p);
  epoch_seconds = atc_zoned_date_time_to_epoch_seconds(&zdtla);
  fprintf(stderr, "Epoch Seconds: %ld\n", (long) epoch_seconds);
}

int main(int argc, char **argv)
{
  (void) argc;
  (void) argv;
  setup();
  print_dates();
}
