/*
 * pathHistory.c
 * Tests for Data Frame: DF_PathHistory
 * For BSM, this includes tests for BSM PartII, VehicleSafetyExtensions, FullPositionVector, and PathHistory
 * For PSM, this requires only PathHistory
 *
 * Did you know? Documentation for how to write more tests is at https://cpputest.github.io/manual.html
 */
#include "CppUTest/TestHarness_c.h"
#include "libsm.h"


TEST_C(path_history, getting_partIIelements)
{
    libsm_rval_e ret;
    BasicSafetyMessage_t* bsm = calloc(1, sizeof(BasicSafetyMessage_t));

    ret = libsm_init_bsm(bsm);
    CHECK_EQUAL_C_INT(LIBSM_OK, ret);

    VehicleSafetyExtensions_t* vse = libsm_get_partII_elem(
            bsm,
            BSMpartIIExtension__partII_Value_PR_VehicleSafetyExtensions);

    CHECK_C_TEXT(vse == NULL, "bsm_get_partII_elem");

    libsm_bsm_alloc_partII_if_not_exist(bsm);
    libsm_add_bsm_VehicleSafetyExtension(bsm);

    vse = libsm_get_partII_elem(bsm, BSMpartIIExtension__partII_Value_PR_VehicleSafetyExtensions);
    CHECK_C_TEXT(vse != NULL, "bsm_get_partII_elem");

    ASN_STRUCT_FREE(asn_DEF_BasicSafetyMessage, bsm);
}

TEST_C(path_history, getting_partIIelements_NULL)
{
    BasicSafetyMessage_t* bsm = NULL;

    VehicleSafetyExtensions_t* vse = libsm_get_partII_elem(
            bsm,
            BSMpartIIExtension__partII_Value_PR_VehicleSafetyExtensions);

    CHECK_C_TEXT(vse == NULL, "bsm_get_partII_elem");

    bsm = calloc(1, sizeof(BasicSafetyMessage_t));

    vse = libsm_get_partII_elem(bsm, BSMpartIIExtension__partII_Value_PR_VehicleSafetyExtensions);
    CHECK_C_TEXT(vse == NULL, "bsm_get_partII_elem");

    ASN_STRUCT_FREE(asn_DEF_BasicSafetyMessage, bsm);
}

TEST_C(path_history, test_libsm_alloc_init_DDateTime)
{
    // Create a DDateTime
    DDateTime_t* myDDateTime = libsm_alloc_init_DDateTime();

    CHECK_C_TEXT(myDDateTime != NULL, "libsm_alloc_init_DDateTime");
    ASN_STRUCT_FREE(asn_DEF_DDateTime, myDDateTime);
}

TEST_C(path_history, test_libsm_alloc_init_PathHistoryPoint)
{
    PathHistoryPoint_t* pathHistoryPoint = libsm_alloc_init_PathHistoryPoint();

    CHECK_C_TEXT(pathHistoryPoint != NULL, "libsm_alloc_init_PathHistoryPoint");

    // Test that ALL elements in pathHistoryPoint contain the correct initial ("unavailable") value
    // note: Some elements remain NULL.
    CHECK_EQUAL_C_LONG_TEXT(OffsetLL_B18_unavailable,
                            pathHistoryPoint->latOffset,
                            "pathHistoryPoint->latOffset");
    CHECK_EQUAL_C_LONG_TEXT(OffsetLL_B18_unavailable,
                            pathHistoryPoint->lonOffset,
                            "pathHistoryPoint->lonOffset");
    CHECK_EQUAL_C_LONG_TEXT(VertOffset_B12_unavailable,
                            pathHistoryPoint->elevationOffset,
                            "pHP->elevationOffset");
    CHECK_EQUAL_C_LONG_TEXT(TimeOffset_unavailable,
                            pathHistoryPoint->timeOffset,
                            "pathHistoryPoint->timeOffset");
    CHECK_C_TEXT(pathHistoryPoint->speed == NULL, "pathHistoryPoint->speed");
    CHECK_C_TEXT(pathHistoryPoint->posAccuracy == NULL, "pathHistoryPoint->posAccuracy");
    CHECK_C_TEXT(pathHistoryPoint->heading == NULL, "pathHistoryPoint->latOffset");

    ASN_STRUCT_FREE(asn_DEF_PathHistoryPoint, pathHistoryPoint);
}

TEST_C(path_history, test_libsm_alloc_init_FullPositionVector)
{
    // Create and populate a FullPositionVector to test libsm_init_fullpositionvector()
    FullPositionVector_t* initialPosition = libsm_alloc_init_FullPositionVector();

    CHECK_C_TEXT(initialPosition != NULL, "libsm_init_fullpositionvector");

    initialPosition->utcTime = libsm_alloc_init_DDateTime();
    CHECK_C_TEXT(initialPosition->utcTime != NULL, "libsm_alloc_init_DDateTime");

    // Test that ALL elements in utcTime are allocated and contain the correct initial ("unavailable") value
    CHECK_EQUAL_C_LONG_TEXT(DYear_unavailable,
                            *initialPosition->utcTime->year,
                            "initialPosition->utcTime->year");
    CHECK_EQUAL_C_LONG_TEXT(DMonth_unavailable,
                            *initialPosition->utcTime->month,
                            "initialPosition->utcTime->month");
    CHECK_EQUAL_C_LONG_TEXT(DDay_unavailable,
                            *initialPosition->utcTime->day,
                            "initialPosition->utcTime->day");
    CHECK_EQUAL_C_LONG_TEXT(DHour_unavailable,
                            *initialPosition->utcTime->hour,
                            "initialPosition->utcTime->hour");
    CHECK_EQUAL_C_LONG_TEXT(DMinute_unavailable,
                            *initialPosition->utcTime->minute,
                            "initialPosition->utcTime->minute");
    CHECK_EQUAL_C_LONG_TEXT(DSecond_unavailable,
                            *initialPosition->utcTime->second,
                            "initialPosition->utcTime->second");
    CHECK_EQUAL_C_LONG_TEXT(DOffset_unavailable,
                            *initialPosition->utcTime->offset,
                            "initialPosition->utcTime->offset");

    ASN_STRUCT_FREE(asn_DEF_FullPositionVector, initialPosition);
}

TEST_C(path_history, test_libsm_alloc_init_path_history)
{
    PathHistory_t* pathHistory = libsm_alloc_init_path_history();

    CHECK_C_TEXT(pathHistory != NULL, "libsm_alloc_init_path_history");

    CHECK_C_TEXT(pathHistory->initialPosition != NULL, "libsm_alloc_init_path_history");
    CHECK_C_TEXT(pathHistory->currGNSSstatus == NULL, "libsm_alloc_init_path_history");
    CHECK_C_TEXT(pathHistory->crumbData.list.count != 0, "libsm_alloc_init_path_history");

    ASN_STRUCT_FREE(asn_DEF_PathHistory, pathHistory);
}

TEST_C(path_history, test_libsm_init_bsm_path_history)
{
    libsm_rval_e ret;
    BasicSafetyMessage_t* test_bsm = calloc(1, sizeof(BasicSafetyMessage_t));

    ret = libsm_init_bsm(test_bsm);
    CHECK_EQUAL_C_INT(LIBSM_OK, ret);

    ret = libsm_init_bsm_path_history(test_bsm);
    CHECK_EQUAL_C_INT(LIBSM_OK, ret);

    VehicleSafetyExtensions_t* vse = libsm_get_partII_elem(
            test_bsm,
            BSMpartIIExtension__partII_Value_PR_VehicleSafetyExtensions);

    CHECK_C_TEXT(vse != NULL, "bsm_get_partII_elem");

    ASN_STRUCT_FREE(asn_DEF_BasicSafetyMessage, test_bsm);
}

TEST_C(path_history, test_libsm_init_psm_path_history)
{
    // Now test the whole setup by making a PSM and inserting a timestamp into it.
    PersonalSafetyMessage_t* test_psm = calloc(1, sizeof(PersonalSafetyMessage_t));
    libsm_rval_e ret = libsm_init_psm(test_psm);

    CHECK_C_TEXT(ret == LIBSM_OK, "PSM was not allocated");

    // Test inserting pathHistory into a PSM
    ret = libsm_init_psm_path_history(test_psm);
    CHECK_C_TEXT(ret == LIBSM_OK, "pathHistory was not successfully inserted into the PSM");

    CHECK_C_TEXT(test_psm->pathHistory != NULL,
                 "test_psm->pathHistory is NULL and was not created.");

    ASN_STRUCT_FREE(asn_DEF_PersonalSafetyMessage, test_psm);
}

TEST_C(path_history, store_and_access_psm)
{
    PersonalSafetyMessage_t* psm = calloc(1, sizeof(PersonalSafetyMessage_t));
    libsm_rval_e ret = libsm_init_psm(psm);

    CHECK_C_TEXT(ret == LIBSM_OK, "PSM was not allocated");

    ret = libsm_set_psm_path_history_DDateTime_values(psm, 2021, 6, 23, 11, 54, 42000, -5 * 60);

    DDateTime_t* date = libsm_get_psm_path_history_utc_time(psm);

    CHECK_EQUAL_C_LONG(2021, *date->year);
    CHECK_EQUAL_C_LONG(6, *date->month);
    CHECK_EQUAL_C_LONG(23, *date->day);
    CHECK_EQUAL_C_LONG(11, *date->hour);
    CHECK_EQUAL_C_LONG(54, *date->minute);
    CHECK_EQUAL_C_LONG(42000, *date->second);
    CHECK_EQUAL_C_LONG(-5 * 60, *date->offset);

    ASN_STRUCT_FREE(asn_DEF_PersonalSafetyMessage, psm);
}

TEST_C(path_history, store_and_access_bsm)
{
    BasicSafetyMessage_t* bsm = calloc(1, sizeof(BasicSafetyMessage_t));
    libsm_rval_e ret = libsm_init_bsm(bsm);

    CHECK_C_TEXT(ret == LIBSM_OK, "PSM was not allocated");

    ret = libsm_set_bsm_path_history_DDateTime_values(bsm, 2021, 6, 23, 11, 54, 42000, -5 * 60);

    DDateTime_t* date = libsm_get_bsm_path_history_utc_time(bsm);

    CHECK_EQUAL_C_LONG(2021, *date->year);
    CHECK_EQUAL_C_LONG(6, *date->month);
    CHECK_EQUAL_C_LONG(23, *date->day);
    CHECK_EQUAL_C_LONG(11, *date->hour);
    CHECK_EQUAL_C_LONG(54, *date->minute);
    CHECK_EQUAL_C_LONG(42000, *date->second);
    CHECK_EQUAL_C_LONG(-5 * 60, *date->offset);

    ASN_STRUCT_FREE(asn_DEF_BasicSafetyMessage, bsm);
}


TEST_C(path_history, store_and_access_psm_pointers)
{
    PersonalSafetyMessage_t* psm = calloc(1, sizeof(PersonalSafetyMessage_t));

    libsm_rval_e ret = libsm_init_psm(psm);

    CHECK_C(ret == LIBSM_OK);

    DDateTime_t* dt = libsm_alloc_init_DDateTime();

    *dt->year = 2021;
    *dt->month = 6;
    *dt->day = 23;
    *dt->hour = 11;
    *dt->minute = 54;
    *dt->second = 42000;
    *dt->offset = -5 * 60;


    ret = libsm_set_psm_path_history_DDateTime(psm, dt);
    DDateTime_t* date = libsm_get_psm_path_history_utc_time(psm);

    CHECK_EQUAL_C_LONG(2021, *date->year);
    CHECK_EQUAL_C_LONG(6, *date->month);
    CHECK_EQUAL_C_LONG(23, *date->day);
    CHECK_EQUAL_C_LONG(11, *date->hour);
    CHECK_EQUAL_C_LONG(54, *date->minute);
    CHECK_EQUAL_C_LONG(42000, *date->second);
    CHECK_EQUAL_C_LONG(-5 * 60, *date->offset);

    ASN_STRUCT_FREE(asn_DEF_DDateTime, dt);
    ASN_STRUCT_FREE(asn_DEF_PersonalSafetyMessage, psm);
}
