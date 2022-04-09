/*
 * MIT License
 * Copyright (c) 2022 Brian T. Park
 */

#ifndef ACE_TIME_C_EPOCH_CONVERTER_HINNANT2_H
#define ACE_TIME_C_EPOCH_CONVERTER_HINNANT2_H

#include <stdint.h>

/** Base year of the AceTimeC epoch. */
#define ATC_EPOCH_YEAR 2000

/**
 * Return the number of epoch days for the (year_tiny, month, day) triple.
 */
int32_t atc_to_epoch_days(int8_t year_tiny, uint8_t month, uint8_t day);

/**
 * Extract the (year_tiny, month, day) fields from AceTime epoch_days.
 *
 * No input validation is performed. The behavior is undefined if the
 * parameters are outside their expected range.
 *
 * @param epoch_days number of days from AceTime Epoch of 2000-01-01
 * @param year_tiny year offset since 2000 [-127, 127], with -128 indicating
 *        an error condition
 * @param month month integer [1, 12]
 * @param day day of month integer[1, 31]
 */
void atc_from_epoch_days(
    int32_t epoch_days,
    int8_t *year_tiny,
    uint8_t *month,
    uint8_t *day);

#endif
