#include "libsm-pathHistory.h"


/**
 * Function that returns a path history utcTime object for a given PSM
 * Default returns null if not found
 */
DDateTime_t* libsm_get_psm_path_history_utc_time(PersonalSafetyMessage_t* psm)
{
    if (psm->pathHistory) {
        if (psm->pathHistory->initialPosition) {
            if (psm->pathHistory->initialPosition->utcTime) {
                return psm->pathHistory->initialPosition->utcTime;
            }
        }
    }
    return NULL;
}


/**
 * Function that returns a path history utcTime object for a given BSM
 * Default returns null if not found
 */
DDateTime_t* libsm_get_bsm_path_history_utc_time(BasicSafetyMessage_t* bsm)
{
    if (bsm->partII) {
        for (int i = 0; i < bsm->partII->list.count; i++) {
            BSMpartIIExtension_t* partIIExt = bsm->partII->list.array[i];
            if (partIIExt->partII_Value.present
                        == BSMpartIIExtension__partII_Value_PR_VehicleSafetyExtensions
                && partIIExt->partII_Value.choice.VehicleSafetyExtensions.pathHistory
                && partIIExt->partII_Value.choice.VehicleSafetyExtensions.pathHistory
                           ->initialPosition
                && partIIExt->partII_Value.choice.VehicleSafetyExtensions.pathHistory
                           ->initialPosition->utcTime) {
                return partIIExt->partII_Value.choice.VehicleSafetyExtensions.pathHistory
                        ->initialPosition->utcTime;
            }
        }
    }
    return NULL;
}


libsm_rval_e libsm_set_path_history_DDateTime_values(PathHistory_t* pathHistory,
                                                     DYear_t year,
                                                     DMonth_t month,
                                                     DDay_t day,
                                                     DHour_t hour,
                                                     DMinute_t minute,
                                                     DSecond_t second,
                                                     DOffset_t offset)
{
    if (pathHistory == NULL) {
        return LIBSM_FAIL_NO_ARGUMENT;
    }
    // Allocate a DDateTime struct and its elements
    pathHistory->initialPosition->utcTime = libsm_alloc_init_DDateTime();
    if (pathHistory->initialPosition->utcTime == NULL) {
        return LIBSM_FAIL;
    }
    // Load the parameters into DDateTime
    *pathHistory->initialPosition->utcTime->year = year;
    *pathHistory->initialPosition->utcTime->month = month;
    *pathHistory->initialPosition->utcTime->day = day;
    *pathHistory->initialPosition->utcTime->hour = hour;
    *pathHistory->initialPosition->utcTime->minute = minute;
    *pathHistory->initialPosition->utcTime->second = second;
    *pathHistory->initialPosition->utcTime->offset = offset;

    return LIBSM_OK;
}


/**
 * Given an allocated BSM and the timestamp parameters, insert the time parameters
 * into PartII->VehicleSafetyExtension->pathHistory->initialPosition->utcTime.
 * Checks are made for the existence of these dataframes and they are
 * allocated and initialized if they don't exist.
 * Returns: LIBSM_OK if the timestamp has been inserted successfully.
 */
libsm_rval_e libsm_set_bsm_path_history_DDateTime(BasicSafetyMessage_t* bsm, DDateTime_t* date)
{
    return libsm_set_bsm_path_history_DDateTime_values(bsm,
                                                       *date->year,
                                                       *date->month,
                                                       *date->day,
                                                       *date->hour,
                                                       *date->minute,
                                                       *date->second,
                                                       *date->offset);
}


libsm_rval_e libsm_set_bsm_path_history_DDateTime_values(BasicSafetyMessage_t* bsm,
                                                         DYear_t year,
                                                         DMonth_t month,
                                                         DDay_t day,
                                                         DHour_t hour,
                                                         DMinute_t minute,
                                                         DSecond_t second,
                                                         DOffset_t offset)

{
    libsm_rval_e err;

    // Sanity check for a BSM
    if (bsm == NULL) {
        return LIBSM_FAIL_NO_ARGUMENT;
    }

    // Check for pathHistory and create it and PartII if it doesn't exist.
    err = libsm_init_bsm_path_history(bsm);
    if (err != LIBSM_OK) {
        return err;
    }

    // Get a pointer to the VehicleSafetyExtensions
    VehicleSafetyExtensions_t* vse = libsm_get_partII_elem(
            bsm,
            BSMpartIIExtension__partII_Value_PR_VehicleSafetyExtensions);

    err = libsm_set_path_history_DDateTime_values(vse->pathHistory,
                                                  year,
                                                  month,
                                                  day,
                                                  hour,
                                                  minute,
                                                  second,
                                                  offset);
    return err;
}


/**
 * Given an allocated PSM and the timestamp parameters, insert the time parameters
 * into psm->pathHistory->initialPosition->utcTime.
 * Checks are made for the existence of these dataframes and they are
 * allocated and initialized if they don't exist.
 * Returns: LIBSM_OK if the timestamp has been inserted successfully.
 */
libsm_rval_e libsm_set_psm_path_history_DDateTime(PersonalSafetyMessage_t* psm, DDateTime_t* date)
{
    return libsm_set_psm_path_history_DDateTime_values(psm,
                                                       *date->year,
                                                       *date->month,
                                                       *date->day,
                                                       *date->hour,
                                                       *date->minute,
                                                       *date->second,
                                                       *date->offset);
}


libsm_rval_e libsm_set_psm_path_history_DDateTime_values(PersonalSafetyMessage_t* psm,
                                                         DYear_t year,
                                                         DMonth_t month,
                                                         DDay_t day,
                                                         DHour_t hour,
                                                         DMinute_t minute,
                                                         DSecond_t second,
                                                         DOffset_t offset)
{
    libsm_rval_e err;

    if (psm == NULL) {
        return LIBSM_FAIL_NO_ARGUMENT;
    }
    err = libsm_init_psm_path_history(psm);
    if (err != LIBSM_OK) {
        return err;
    }
    err = libsm_set_path_history_DDateTime_values(psm->pathHistory,
                                                  year,
                                                  month,
                                                  day,
                                                  hour,
                                                  minute,
                                                  second,
                                                  offset);
    return err;
}
