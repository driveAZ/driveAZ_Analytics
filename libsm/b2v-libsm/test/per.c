// https://cpputest.github.io/manual.html
#include <stdio.h>
#include <string.h>

#include "CppUTest/TestHarness_c.h"

#include "libsm-per.h"

/**
 * Live data gathered from 1 device for crosscheck with unit testing
 * ---| 1st  last  R       T       Ex    |---PER:005----
 * ---| 103, 113, 010, 1624902159, 011   |--- missing 1
 * ---| 114, 123, 010, 1624902160, 010   |---
 * ---| 124, 006, 010, 1624902161, 011   |--- missing 1
 * ---| 007, 016, 010, 1624902162, 010   |---
 * ---| 017, 027, 010, 1624902163, 011   |--- missing 1
 * ---|  E    R    M   diff  |---              expectedSubWindow total = 53
 * ---| 053, 050, 003,  000  |---
 *
 * "outside_window"
 * ---| 1st  last  R       T       Ex    |---PER:007----
 * ---| 077, 087, 010, 1624902208, 011   |--- missing 1
 * ---| 088, 097, 009, 1624902209, 010   |--- missing 1
 * ---| 098, 107, 010, 1624902210, 010   |---
 * ---| 108, 116, 009, 1624902211, 009   |---
 * ---| 118, 000, 010, 1624902212, 011   |--- missing 1 outside subwindow ("117") AND missing 1 in subwindow (118, xxx, 120, 121, 122, 123, 124, 125, 126, 127, 000)
 * ---|  E    R    M   diff  |---             expectedSubWindow total = 51
 * ---| 052, 048, 004,  000  |---
 *
 * ---| 1st  last  R       T       Ex    |---PER:009----
 * ---| 123, 005, 009, 1624902367, 011   |--- missing 2
 * ---| 006, 015, 010, 1624902368, 010   |---
 * ---| 016, 026, 010, 1624902369, 011   |--- missing 1
 * ---| 027, 036, 010, 1624902370, 010   |---
 * ---| 038, 047, 009, 1624902371, 010   |--- missing 1 outside window, AND missing 1 in subwindow
 * ---|  E    R    M   diff  |---
 * ---| 053, 048, 005,  000  |---
 *
 * "two_duplicates"
 * ---| 1st  last  R       T       Ex    |---PER:003----
 * ---| 015, 023, 010, 1624902433, 009*  |--- duplicate 1
 * ---| 024, 033, 010, 1624902434, 010   |---
 * ---| 034, 043, 009, 1624902435, 010   |--- missing 1
 * ---| 044, 054, 010, 1624902436, 011   |--- missing 1
 * ---| 055, 063, 010, 1624902437, 009*  |--- duplicate 1
 * ---|  E    R    M   diff  |---             expectedSubWindow total = 49 + 2 (for the duplicates) = 51 total expected
 * ---| 051, 049, 002,  001  |---
 *
 * "one_duplicate"
 * ---| 1st  last  R       T       Ex    |---PER:005---
 * ---| 002, 011, 010, 1624902817, 010   |---
 * ---| 012, 022, 010, 1624902819, 011   |--- missing 1
 * ---| 023, 032, 010, 1624902820, 010   |---
 * ---| 033, 043, 009, 1624902821, 011   |--- missing 2
 * ---| 044, 052, 010, 1624902822, 009*  |--- duplicate 1
 * ---|  E    R    M   diff  |---             expectedSubWindow total = 51 + 1 (for the duplicate) = 52 total expected
 * ---| 052, 049, 003,  001  |---
 *
 * "perfect"
 * -------| Direct from a sender, showing 000% PER |----------
 * ---| 1st  last  R       T       Ex    |---PER:000---
 * ---| 081, 090, 010, 1624902066, 010   |---
 * ---| 091, 100, 010, 1624902067, 010   |---
 * ---| 101, 111, 011, 1624902068, 011   |---
 * ---| 112, 122, 011, 1624902069, 011   |---
 * ---| 123, 004, 010, 1624902071, 010   |---
 * ---|  E    R    M   diff  |---
 * ---| 052, 052, 000,  000  |---
 *
 * "duplicates_between_subwindows"
 * ---| 1st  last  R  expected |---
 * ---| 000, 010, 011,  011 |---
 * ---| 010, 020, 011,  011 |---
 * ---| 020, 030, 011,  011 |---
 * ---| 030, 040, 011,  011 |---
 * ---| 040, 050, 011,  011 |---
 *
 */


/**
 * Test against a perfectly received interval
 * Actual data taken from a Nordic board in Sender mode
 */
TEST_C(per_calculate, perfect)
{
    PERSlidingInterval_t perSlidingInterval[PER_SUBINTERVAL_COUNT] = { { 0 } };

    perSlidingInterval[0].first = 0;
    perSlidingInterval[0].last = 9;
    perSlidingInterval[0].received = 10;

    perSlidingInterval[1].first = 10;
    perSlidingInterval[1].last = 19;
    perSlidingInterval[1].received = 10;

    perSlidingInterval[2].first = 20;
    perSlidingInterval[2].last = 29;
    perSlidingInterval[2].received = 10;

    perSlidingInterval[3].first = 30;
    perSlidingInterval[3].last = 39;
    perSlidingInterval[3].received = 10;

    perSlidingInterval[4].first = 40;
    perSlidingInterval[4].last = 49;
    perSlidingInterval[4].received = 10;

    uint16_t per = libsm_per_calculate(perSlidingInterval);

    CHECK_EQUAL_C_INT(0, per);
}

TEST_C(per_calculate, half_contiguous)
{
    PERSlidingInterval_t perSlidingInterval[PER_SUBINTERVAL_COUNT] = { { 0 } };

    perSlidingInterval[0].first = 0;
    perSlidingInterval[0].last = 9;
    perSlidingInterval[0].received = 5;

    perSlidingInterval[1].first = 10;
    perSlidingInterval[1].last = 19;
    perSlidingInterval[1].received = 5;

    perSlidingInterval[2].first = 20;
    perSlidingInterval[2].last = 29;
    perSlidingInterval[2].received = 5;

    perSlidingInterval[3].first = 30;
    perSlidingInterval[3].last = 39;
    perSlidingInterval[3].received = 5;

    perSlidingInterval[4].first = 40;
    perSlidingInterval[4].last = 49;
    perSlidingInterval[4].received = 5;

    uint16_t per = libsm_per_calculate(perSlidingInterval);

    CHECK_EQUAL_C_INT(50, per);
}

TEST_C(per_calculate, half_noncontiguous)
{
    PERSlidingInterval_t perSlidingInterval[PER_SUBINTERVAL_COUNT] = { { 0 } };

    perSlidingInterval[0].first = 0;
    perSlidingInterval[0].last = 4;
    perSlidingInterval[0].received = 5;

    perSlidingInterval[1].first = 10;
    perSlidingInterval[1].last = 14;
    perSlidingInterval[1].received = 5;

    perSlidingInterval[2].first = 20;
    perSlidingInterval[2].last = 24;
    perSlidingInterval[2].received = 5;

    perSlidingInterval[3].first = 30;
    perSlidingInterval[3].last = 34;
    perSlidingInterval[3].received = 5;

    perSlidingInterval[4].first = 45;
    perSlidingInterval[4].last = 49;
    perSlidingInterval[4].received = 5;

    uint16_t per = libsm_per_calculate(perSlidingInterval);

    CHECK_EQUAL_C_INT(50, per);
}

TEST_C(per_calculate, two_per_window)
{
    PERSlidingInterval_t perSlidingInterval[PER_SUBINTERVAL_COUNT] = { { 0 } };

    perSlidingInterval[0].first = 0;
    perSlidingInterval[0].last = 1;
    perSlidingInterval[0].received = 2;

    perSlidingInterval[1].first = 10;
    perSlidingInterval[1].last = 11;
    perSlidingInterval[1].received = 2;

    perSlidingInterval[2].first = 20;
    perSlidingInterval[2].last = 21;
    perSlidingInterval[2].received = 2;

    perSlidingInterval[3].first = 30;
    perSlidingInterval[3].last = 31;
    perSlidingInterval[3].received = 2;

    perSlidingInterval[4].first = 48;
    perSlidingInterval[4].last = 49;
    perSlidingInterval[4].received = 2;

    uint16_t per = libsm_per_calculate(perSlidingInterval);

    CHECK_EQUAL_C_INT(80, per);
}

TEST_C(per_calculate, single_packet)
{
    PERSlidingInterval_t perSlidingInterval[PER_SUBINTERVAL_COUNT] = { { 0 } };

    perSlidingInterval[0].first = 1;
    perSlidingInterval[0].last = 1;
    perSlidingInterval[0].received = 1;

    uint16_t per = libsm_per_calculate(perSlidingInterval);

    CHECK_EQUAL_C_INT(PER_UNAVAILABLE, per);
}

// These tests are from J2945/1 2020-04, A.8.3 PER Calculation Special Cases,
// figures A21 and A22 (these figures may be mislabelled in the PDF)
TEST_C(per_calculate, j2945_1_A83_FA21a)
{
    PERSlidingInterval_t perSlidingInterval[PER_SUBINTERVAL_COUNT] = { { 0 } };

    perSlidingInterval[0].first = 1;
    perSlidingInterval[0].last = 8;
    perSlidingInterval[0].received = 2;

    perSlidingInterval[1].first = 0;
    perSlidingInterval[1].last = 0;
    perSlidingInterval[1].received = 0;

    perSlidingInterval[2].first = 0;
    perSlidingInterval[2].last = 0;
    perSlidingInterval[2].received = 0;

    perSlidingInterval[3].first = 0;
    perSlidingInterval[3].last = 0;
    perSlidingInterval[3].received = 0;

    perSlidingInterval[4].first = 0;
    perSlidingInterval[4].last = 0;
    perSlidingInterval[4].received = 0;

    uint16_t per = libsm_per_calculate(perSlidingInterval);

    CHECK_EQUAL_C_INT(75, per);
}

TEST_C(per_calculate, j2945_1_A83_FA21b)
{
    PERSlidingInterval_t perSlidingInterval[PER_SUBINTERVAL_COUNT] = { { 0 } };

    perSlidingInterval[0].first = 0;
    perSlidingInterval[0].last = 0;
    perSlidingInterval[0].received = 0;

    perSlidingInterval[1].first = 0;
    perSlidingInterval[1].last = 0;
    perSlidingInterval[1].received = 0;

    perSlidingInterval[2].first = 0;
    perSlidingInterval[2].last = 0;
    perSlidingInterval[2].received = 0;

    perSlidingInterval[3].first = 0;
    perSlidingInterval[3].last = 0;
    perSlidingInterval[3].received = 0;

    perSlidingInterval[4].first = 0;
    perSlidingInterval[4].last = 0;
    perSlidingInterval[4].received = 0;

    uint16_t per = libsm_per_calculate(perSlidingInterval);

    CHECK_EQUAL_C_INT(PER_UNAVAILABLE, per);
}

TEST_C(per_calculate, j2945_1_A83_FA22a)
{
    PERSlidingInterval_t perSlidingInterval[PER_SUBINTERVAL_COUNT] = { { 0 } };

    perSlidingInterval[0].first = 1;
    perSlidingInterval[0].last = 1;
    perSlidingInterval[0].received = 1;

    perSlidingInterval[1].first = 11;
    perSlidingInterval[1].last = 11;
    perSlidingInterval[1].received = 1;

    perSlidingInterval[2].first = 0;
    perSlidingInterval[2].last = 0;
    perSlidingInterval[2].received = 0;

    perSlidingInterval[3].first = 0;
    perSlidingInterval[3].last = 0;
    perSlidingInterval[3].received = 0;

    perSlidingInterval[4].first = 0;
    perSlidingInterval[4].last = 0;
    perSlidingInterval[4].received = 0;

    uint16_t per = libsm_per_calculate(perSlidingInterval);
    // PER = 81.8181839, rounded to 82
    CHECK_EQUAL_C_INT(82, per);
}

TEST_C(per_calculate, j2945_1_A83_FA22b)
{
    PERSlidingInterval_t perSlidingInterval[PER_SUBINTERVAL_COUNT] = { { 0 } };

    perSlidingInterval[0].first = 11;
    perSlidingInterval[0].last = 11;
    perSlidingInterval[0].received = 1;

    perSlidingInterval[1].first = 0;
    perSlidingInterval[1].last = 0;
    perSlidingInterval[1].received = 0;

    perSlidingInterval[2].first = 0;
    perSlidingInterval[2].last = 0;
    perSlidingInterval[2].received = 0;

    perSlidingInterval[3].first = 0;
    perSlidingInterval[3].last = 0;
    perSlidingInterval[3].received = 0;

    perSlidingInterval[4].first = 0;
    perSlidingInterval[4].last = 0;
    perSlidingInterval[4].received = 0;

    uint16_t per = libsm_per_calculate(perSlidingInterval);

    CHECK_EQUAL_C_INT(PER_UNAVAILABLE, per);
}

TEST_C(per_calculate, rollover_all)
{
    PERSlidingInterval_t perSlidingInterval[PER_SUBINTERVAL_COUNT] = { { 0 } };

    perSlidingInterval[0].first = 110;
    perSlidingInterval[0].last = 119;
    perSlidingInterval[0].received = 10;

    perSlidingInterval[1].first = 120;
    perSlidingInterval[1].last = 1;
    perSlidingInterval[1].received = 10;

    perSlidingInterval[2].first = 2;
    perSlidingInterval[2].last = 11;
    perSlidingInterval[2].received = 10;

    perSlidingInterval[3].first = 12;
    perSlidingInterval[3].last = 21;
    perSlidingInterval[3].received = 10;

    perSlidingInterval[4].first = 22;
    perSlidingInterval[4].last = 31;
    perSlidingInterval[4].received = 10;

    uint16_t per = libsm_per_calculate(perSlidingInterval);

    CHECK_EQUAL_C_INT(0, per);
}

TEST_C(per_calculate, rollover_half)
{
    PERSlidingInterval_t perSlidingInterval[PER_SUBINTERVAL_COUNT] = { { 0 } };

    perSlidingInterval[0].first = 110;
    perSlidingInterval[0].last = 119;
    perSlidingInterval[0].received = 5;

    perSlidingInterval[1].first = 120;
    perSlidingInterval[1].last = 1;
    perSlidingInterval[1].received = 5;

    perSlidingInterval[2].first = 2;
    perSlidingInterval[2].last = 11;
    perSlidingInterval[2].received = 5;

    perSlidingInterval[3].first = 12;
    perSlidingInterval[3].last = 21;
    perSlidingInterval[3].received = 5;

    perSlidingInterval[4].first = 22;
    perSlidingInterval[4].last = 31;
    perSlidingInterval[4].received = 5;

    uint16_t per = libsm_per_calculate(perSlidingInterval);

    CHECK_EQUAL_C_INT(50, per);
}

TEST_C(per_calculate, rollover_between_window)
{
    PERSlidingInterval_t perSlidingInterval[PER_SUBINTERVAL_COUNT] = { { 0 } };

    perSlidingInterval[0].first = 110;
    perSlidingInterval[0].last = 119;
    perSlidingInterval[0].received = 10;

    perSlidingInterval[1].first = 120;
    perSlidingInterval[1].last = 127;
    perSlidingInterval[1].received = 8;

    perSlidingInterval[2].first = 0;
    perSlidingInterval[2].last = 11;
    perSlidingInterval[2].received = 12;

    perSlidingInterval[3].first = 12;
    perSlidingInterval[3].last = 21;
    perSlidingInterval[3].received = 10;

    perSlidingInterval[4].first = 22;
    perSlidingInterval[4].last = 31;
    perSlidingInterval[4].received = 10;

    uint16_t per = libsm_per_calculate(perSlidingInterval);

    CHECK_EQUAL_C_INT(0, per);
}

TEST_C(per_calculate, two_duplicates)
{
    PERSlidingInterval_t perSlidingInterval[PER_SUBINTERVAL_COUNT] = { { 0 } };

    perSlidingInterval[0].first = 15;
    perSlidingInterval[0].last = 23;
    perSlidingInterval[0].received = 10;

    perSlidingInterval[1].first = 24;
    perSlidingInterval[1].last = 33;
    perSlidingInterval[1].received = 10;

    perSlidingInterval[2].first = 34;
    perSlidingInterval[2].last = 43;
    perSlidingInterval[2].received = 9;

    perSlidingInterval[3].first = 44;
    perSlidingInterval[3].last = 54;
    perSlidingInterval[3].received = 10;

    perSlidingInterval[4].first = 55;
    perSlidingInterval[4].last = 63;
    perSlidingInterval[4].received = 10;

    uint16_t per = libsm_per_calculate(perSlidingInterval);

    // PER = 3.92156887, rounded to 4
    CHECK_EQUAL_C_INT(4, per);
}

TEST_C(per_calculate, one_duplicate)
{
    PERSlidingInterval_t perSlidingInterval[PER_SUBINTERVAL_COUNT] = { { 0 } };

    perSlidingInterval[0].first = 2;
    perSlidingInterval[0].last = 11;
    perSlidingInterval[0].received = 10;

    perSlidingInterval[1].first = 12;
    perSlidingInterval[1].last = 22;
    perSlidingInterval[1].received = 10;

    perSlidingInterval[2].first = 23;
    perSlidingInterval[2].last = 32;
    perSlidingInterval[2].received = 10;

    perSlidingInterval[3].first = 33;
    perSlidingInterval[3].last = 43;
    perSlidingInterval[3].received = 9;

    perSlidingInterval[4].first = 44;
    perSlidingInterval[4].last = 52;
    perSlidingInterval[4].received = 10;

    uint16_t per = libsm_per_calculate(perSlidingInterval);

    // PER = 5.76923084, rounded to 6
    CHECK_EQUAL_C_INT(6, per);
}

TEST_C(per_calculate, subharthi)
{
    PERSlidingInterval_t perSlidingInterval[PER_SUBINTERVAL_COUNT] = { { 0 } };

    perSlidingInterval[0].first = 22;
    perSlidingInterval[0].last = 31;
    perSlidingInterval[0].received = 10;

    perSlidingInterval[1].first = 32;
    perSlidingInterval[1].last = 40;
    perSlidingInterval[1].received = 10;

    perSlidingInterval[2].first = 41;
    perSlidingInterval[2].last = 51;
    perSlidingInterval[2].received = 9;

    perSlidingInterval[3].first = 51;
    perSlidingInterval[3].last = 61;
    perSlidingInterval[3].received = 10;

    perSlidingInterval[4].first = 62;
    perSlidingInterval[4].last = 72;
    perSlidingInterval[4].received = 10;

    uint16_t per = libsm_per_calculate(perSlidingInterval);

    // PER = 7.54716969, rounded to 8
    CHECK_EQUAL_C_INT(8, per);
}

TEST_C(per_calculate, duplicates_between_subwindows)
{
    PERSlidingInterval_t perSlidingInterval[PER_SUBINTERVAL_COUNT] = { { 0 } };

    perSlidingInterval[0].first = 0;
    perSlidingInterval[0].last = 10;
    perSlidingInterval[0].received = 11;

    perSlidingInterval[1].first = 10;
    perSlidingInterval[1].last = 20;
    perSlidingInterval[1].received = 11;

    perSlidingInterval[2].first = 20;
    perSlidingInterval[2].last = 30;
    perSlidingInterval[2].received = 11;

    perSlidingInterval[3].first = 30;
    perSlidingInterval[3].last = 40;
    perSlidingInterval[3].received = 11;

    perSlidingInterval[4].first = 40;
    perSlidingInterval[4].last = 50;
    perSlidingInterval[4].received = 11;

    uint16_t per = libsm_per_calculate(perSlidingInterval);

    CHECK_EQUAL_C_INT(0, per);
}

TEST_C(per_calculate, roundDown)
{
    PERSlidingInterval_t perSlidingInterval[PER_SUBINTERVAL_COUNT] = { { 0 } };

    // 0, 1, 3, 4, 5, 6, 9        received:7 expected:10 missing:3
    // 10, 11, 12, 13, 14, 19     received:6 expected:10 missing:4
    // 20                         received:1 expected:1 missing:0
    // 21                         received:1 expected:1 missing:0
    // 22                         received:1 expected:1 missing:0
    // received: 16
    // expected 23
    // missing: 7
    // per: 7/23 = 30.4 = 30
    perSlidingInterval[0].first = 0;
    perSlidingInterval[0].last = 9;
    perSlidingInterval[0].received = 7;

    perSlidingInterval[1].first = 10;
    perSlidingInterval[1].last = 19;
    perSlidingInterval[1].received = 6;

    perSlidingInterval[2].first = 20;
    perSlidingInterval[2].last = 20;
    perSlidingInterval[2].received = 1;

    perSlidingInterval[3].first = 21;
    perSlidingInterval[3].last = 21;
    perSlidingInterval[3].received = 1;

    perSlidingInterval[4].first = 22;
    perSlidingInterval[4].last = 22;
    perSlidingInterval[4].received = 1;

    uint16_t per = libsm_per_calculate(perSlidingInterval);

    CHECK_EQUAL_C_INT(30, per);
}

TEST_C(per_calculate, roundUp)
{
    PERSlidingInterval_t perSlidingInterval[PER_SUBINTERVAL_COUNT] = { { 0 } };

    //  0,  1,  3,  4,  5,  9     received:6 expected:10 missing:4
    // 10, 11, 12, 13, 14, 19     received:6 expected:10 missing:4
    // 20                         received:1  expected:1 missing:0
    // 21                         received:1  expected:1 missing:0
    // 22                         received:1  expected:1 missing:0
    // received: 14
    // expected 23
    // missing: 8
    // per: 8/23 = 34.782608 rounded to 35
    perSlidingInterval[0].first = 0;
    perSlidingInterval[0].last = 9;
    perSlidingInterval[0].received = 6;

    perSlidingInterval[1].first = 10;
    perSlidingInterval[1].last = 19;
    perSlidingInterval[1].received = 6;

    perSlidingInterval[2].first = 20;
    perSlidingInterval[2].last = 20;
    perSlidingInterval[2].received = 1;

    perSlidingInterval[3].first = 21;
    perSlidingInterval[3].last = 21;
    perSlidingInterval[3].received = 1;

    perSlidingInterval[4].first = 22;
    perSlidingInterval[4].last = 22;
    perSlidingInterval[4].received = 1;

    uint16_t per = libsm_per_calculate(perSlidingInterval);

    CHECK_EQUAL_C_INT(35, per);
}


//---------------------------| Storage Tests |-------------------------------------
TEST_C(storage_per, one_in_each_window)
{
    PERSlidingInterval_t arr[PER_SUBINTERVAL_COUNT] = { { 0 } };
    uint16_t per = 0;

    struct timespec* now = calloc(1, sizeof(struct timespec));

    now->tv_sec = (time_t)1623110160;
    now->tv_nsec = (long)954582000;

    struct timespec* w0 = calloc(1, sizeof(struct timespec));
    memcpy(w0, now, sizeof(struct timespec));
    w0->tv_sec -= (time_t)5;
    arr[0].window_start.tv_sec = w0->tv_sec - 1;

    struct timespec* w1 = calloc(1, sizeof(struct timespec));
    memcpy(w1, now, sizeof(struct timespec));
    w1->tv_sec -= (time_t)4;
    arr[1].window_start.tv_sec = w1->tv_sec - 1;

    struct timespec* w2 = calloc(1, sizeof(struct timespec));
    memcpy(w2, now, sizeof(struct timespec));
    w2->tv_sec -= (time_t)3;
    arr[2].window_start.tv_sec = w2->tv_sec - 1;

    struct timespec* w3 = calloc(1, sizeof(struct timespec));
    memcpy(w3, now, sizeof(struct timespec));
    w3->tv_sec -= (time_t)2;
    arr[3].window_start.tv_sec = w3->tv_sec - 1;

    struct timespec* w4 = calloc(1, sizeof(struct timespec));
    memcpy(w4, now, sizeof(struct timespec));
    w4->tv_sec -= (time_t)1;
    arr[4].window_start.tv_sec = w4->tv_sec - 1;

    libsm_per_store_recalculate(&per, arr, 0, w0);
    libsm_per_store_recalculate(&per, arr, 10, w1);
    libsm_per_store_recalculate(&per, arr, 20, w2);
    libsm_per_store_recalculate(&per, arr, 30, w3);
    libsm_per_store_recalculate(&per, arr, 40, w4);

    for (size_t i = 0; i < PER_SUBINTERVAL_COUNT; ++i) {
        //printf("w%ld: %ld-%ld, %ld\n", i, arr[i].first, arr[i].last, arr[i].received);
        CHECK_EQUAL_C_INT(1, arr[i].received);
        CHECK_EQUAL_C_INT(i * 10, arr[i].first);
        CHECK_EQUAL_C_INT(i * 10, arr[i].last);
    }

    free(now);
    free(w0);
    free(w1);
    free(w2);
    free(w3);
    free(w4);
}

TEST_C(storage_per, two_in_each_window)
{
    PERSlidingInterval_t arr[PER_SUBINTERVAL_COUNT] = { { 0 } };
    uint16_t per = 0;

    struct timespec* now = calloc(1, sizeof(struct timespec));

    now->tv_sec = (time_t)1623110160;
    now->tv_nsec = (long)954582000;

    struct timespec* w0 = calloc(1, sizeof(struct timespec));
    memcpy(w0, now, sizeof(struct timespec));
    w0->tv_sec -= (time_t)5;
    arr[0].window_start.tv_sec = w0->tv_sec - 1;

    struct timespec* w1 = calloc(1, sizeof(struct timespec));
    memcpy(w1, now, sizeof(struct timespec));
    w1->tv_sec -= (time_t)4;
    arr[1].window_start.tv_sec = w1->tv_sec - 1;

    struct timespec* w2 = calloc(1, sizeof(struct timespec));
    memcpy(w2, now, sizeof(struct timespec));
    w2->tv_sec -= (time_t)3;
    arr[2].window_start.tv_sec = w2->tv_sec - 1;

    struct timespec* w3 = calloc(1, sizeof(struct timespec));
    memcpy(w3, now, sizeof(struct timespec));
    w3->tv_sec -= (time_t)2;
    arr[3].window_start.tv_sec = w3->tv_sec - 1;

    struct timespec* w4 = calloc(1, sizeof(struct timespec));
    memcpy(w4, now, sizeof(struct timespec));
    w4->tv_sec -= (time_t)1;
    arr[4].window_start.tv_sec = w4->tv_sec - 1;

    libsm_per_store_recalculate(&per, arr, 0, w0);
    libsm_per_store_recalculate(&per, arr, 1, w0);
    libsm_per_store_recalculate(&per, arr, 10, w1);
    libsm_per_store_recalculate(&per, arr, 11, w1);
    libsm_per_store_recalculate(&per, arr, 20, w2);
    libsm_per_store_recalculate(&per, arr, 21, w2);
    libsm_per_store_recalculate(&per, arr, 30, w3);
    libsm_per_store_recalculate(&per, arr, 31, w3);
    libsm_per_store_recalculate(&per, arr, 40, w4);
    libsm_per_store_recalculate(&per, arr, 41, w4);

    for (size_t i = 0; i < PER_SUBINTERVAL_COUNT; ++i) {
        //printf("w%ld: %ld-%ld, %ld\n", i, arr[i].first, arr[i].last, arr[i].received);
        CHECK_EQUAL_C_INT(2, arr[i].received);
        CHECK_EQUAL_C_INT(i * 10, arr[i].first);
        CHECK_EQUAL_C_INT(i * 10 + 1, arr[i].last);
    }

    free(now);
    free(w0);
    free(w1);
    free(w2);
    free(w3);
    free(w4);
}


TEST_C(per_store_and_calculate, noDuplicate)
{
    PERSlidingInterval_t arr[PER_SUBINTERVAL_COUNT] = { { 0 } };
    uint16_t per = 0;

    struct timespec* now = calloc(1, sizeof(struct timespec));

    now->tv_sec = (time_t)1623110160;
    now->tv_nsec = (long)954582000;

    struct timespec* w0 = calloc(1, sizeof(struct timespec));
    memcpy(w0, now, sizeof(struct timespec));
    w0->tv_sec -= (time_t)5;
    arr[0].window_start.tv_sec = w0->tv_sec - 1;

    struct timespec* w1 = calloc(1, sizeof(struct timespec));
    memcpy(w1, now, sizeof(struct timespec));
    w1->tv_sec -= (time_t)4;
    arr[1].window_start.tv_sec = w1->tv_sec - 1;

    struct timespec* w2 = calloc(1, sizeof(struct timespec));
    memcpy(w2, now, sizeof(struct timespec));
    w2->tv_sec -= (time_t)3;
    arr[2].window_start.tv_sec = w2->tv_sec - 1;

    struct timespec* w3 = calloc(1, sizeof(struct timespec));
    memcpy(w3, now, sizeof(struct timespec));
    w3->tv_sec -= (time_t)2;
    arr[3].window_start.tv_sec = w3->tv_sec - 1;

    struct timespec* w4 = calloc(1, sizeof(struct timespec));
    memcpy(w4, now, sizeof(struct timespec));
    w4->tv_sec -= (time_t)1;
    arr[4].window_start.tv_sec = w4->tv_sec - 1;

    // 0, 1, 3, 4, 5, 6, 9        received:7 expected:10 missing:3
    // 10, 11, 12, 13, 14, 19     received:6 expected:10 missing:4
    // 20                         received:1 expected:1 missing:0
    // 21                         received:1 expected:1 missing:0
    // 22                         received:1 expected:1 missing:0
    // received: 16
    // expected 23
    // missing: 7
    // per: 7/23 = 30.4 = 30
    libsm_per_store_recalculate(&per, arr, 0, w0);
    libsm_per_store_recalculate(&per, arr, 1, w0);
    libsm_per_store_recalculate(&per, arr, 3, w0);
    libsm_per_store_recalculate(&per, arr, 4, w0);
    libsm_per_store_recalculate(&per, arr, 5, w0);
    libsm_per_store_recalculate(&per, arr, 6, w0);
    libsm_per_store_recalculate(&per, arr, 9, w0);
    libsm_per_store_recalculate(&per, arr, 10, w1);
    libsm_per_store_recalculate(&per, arr, 11, w1);
    libsm_per_store_recalculate(&per, arr, 12, w1);
    libsm_per_store_recalculate(&per, arr, 13, w1);
    libsm_per_store_recalculate(&per, arr, 14, w1);
    libsm_per_store_recalculate(&per, arr, 19, w1);
    libsm_per_store_recalculate(&per, arr, 20, w2);
    libsm_per_store_recalculate(&per, arr, 21, w3);
    libsm_per_store_recalculate(&per, arr, 22, w4);


    CHECK_EQUAL_C_INT(0, arr[0].first);
    CHECK_EQUAL_C_INT(9, arr[0].last);
    CHECK_EQUAL_C_INT(7, arr[0].received);

    CHECK_EQUAL_C_INT(10, arr[1].first);
    CHECK_EQUAL_C_INT(19, arr[1].last);
    CHECK_EQUAL_C_INT(6, arr[1].received);

    CHECK_EQUAL_C_INT(20, arr[2].first);
    CHECK_EQUAL_C_INT(20, arr[2].last);
    CHECK_EQUAL_C_INT(1, arr[2].received);

    CHECK_EQUAL_C_INT(21, arr[3].first);
    CHECK_EQUAL_C_INT(21, arr[3].last);
    CHECK_EQUAL_C_INT(1, arr[3].received);

    CHECK_EQUAL_C_INT(22, arr[4].first);
    CHECK_EQUAL_C_INT(22, arr[4].last);
    CHECK_EQUAL_C_INT(1, arr[4].received);

    // At this point, arr has been fully loaded with
    // realistic PER data, but to 'flush' out the PER value
    // based on the arr data requires either:
    // 1) call libsm_per_calculate(arr) here, or,
    // 2) update W0 with a timespec later than "now" to
    // force the final PER calculation for the subwindow.
    // (Using this method requires rewriting the arr tests
    // in a way that isn't intuitative and makes the
    // test code harder to understand.)
    per = libsm_per_calculate(arr);

    CHECK_EQUAL_C_INT_TEXT(30, per, "The PER Value isn't what's expected.");

    free(now);
    free(w0);
    free(w1);
    free(w2);
    free(w3);
    free(w4);
}


TEST_C(per_store_and_calculate, duplicate)
{
    PERSlidingInterval_t arr[PER_SUBINTERVAL_COUNT] = { { 0 } };
    uint16_t per = 0;

    struct timespec* now = calloc(1, sizeof(struct timespec));

    now->tv_sec = (time_t)1623110160;
    now->tv_nsec = 0L; //(long)954582000;

    struct timespec* w0 = calloc(1, sizeof(struct timespec));
    memcpy(w0, now, sizeof(struct timespec));
    w0->tv_sec -= (time_t)5;
    arr[0].window_start.tv_sec = w0->tv_sec - 1;

    struct timespec* w1 = calloc(1, sizeof(struct timespec));
    memcpy(w1, now, sizeof(struct timespec));
    w1->tv_sec -= (time_t)4;
    arr[1].window_start.tv_sec = w1->tv_sec - 1;

    struct timespec* w2 = calloc(1, sizeof(struct timespec));
    memcpy(w2, now, sizeof(struct timespec));
    w2->tv_sec -= (time_t)3;
    arr[2].window_start.tv_sec = w2->tv_sec - 1;

    struct timespec* w3 = calloc(1, sizeof(struct timespec));
    memcpy(w3, now, sizeof(struct timespec));
    w3->tv_sec -= (time_t)2;
    arr[3].window_start.tv_sec = w3->tv_sec - 1;

    struct timespec* w4 = calloc(1, sizeof(struct timespec));
    memcpy(w4, now, sizeof(struct timespec));
    w4->tv_sec -= (time_t)1;
    arr[4].window_start.tv_sec = w4->tv_sec - 1;


    // We've been told to just assume duplicates counter missing data.
    // 0, 1, 3, 4, 5, 6, 6, 6, 9        received:9 expected:10 missing:1
    // 10, 11, 12, 13, 14, 14, 14, 19   received:8 expected:10 missing:2
    // 20                               received:1 expected:1 missing:0
    // 21                               received:1 expected:1 missing:0
    // 22                               received:1 expected:1 missing:0
    // received: 20
    // expected 23
    // missing: 3
    // per: 3/23 = 13
    libsm_per_store_recalculate(&per, arr, 0, w0);
    libsm_per_store_recalculate(&per, arr, 1, w0);
    libsm_per_store_recalculate(&per, arr, 3, w0);
    libsm_per_store_recalculate(&per, arr, 4, w0);
    libsm_per_store_recalculate(&per, arr, 5, w0);
    libsm_per_store_recalculate(&per, arr, 6, w0);
    libsm_per_store_recalculate(&per, arr, 6, w0);
    libsm_per_store_recalculate(&per, arr, 6, w0);
    libsm_per_store_recalculate(&per, arr, 9, w0);
    libsm_per_store_recalculate(&per, arr, 10, w1);
    libsm_per_store_recalculate(&per, arr, 11, w1);
    libsm_per_store_recalculate(&per, arr, 12, w1);
    libsm_per_store_recalculate(&per, arr, 13, w1);
    libsm_per_store_recalculate(&per, arr, 14, w1);
    libsm_per_store_recalculate(&per, arr, 14, w1);
    libsm_per_store_recalculate(&per, arr, 14, w1);
    libsm_per_store_recalculate(&per, arr, 19, w1);
    libsm_per_store_recalculate(&per, arr, 20, w2);
    libsm_per_store_recalculate(&per, arr, 21, w3);
    libsm_per_store_recalculate(&per, arr, 22, w4);

    CHECK_EQUAL_C_INT(0, arr[0].first);
    CHECK_EQUAL_C_INT(9, arr[0].last);
    CHECK_EQUAL_C_INT(9, arr[0].received);

    CHECK_EQUAL_C_INT(10, arr[1].first);
    CHECK_EQUAL_C_INT(19, arr[1].last);
    CHECK_EQUAL_C_INT(8, arr[1].received);

    CHECK_EQUAL_C_INT(20, arr[2].first);
    CHECK_EQUAL_C_INT(20, arr[2].last);
    CHECK_EQUAL_C_INT(1, arr[2].received);

    CHECK_EQUAL_C_INT(21, arr[3].first);
    CHECK_EQUAL_C_INT(21, arr[3].last);
    CHECK_EQUAL_C_INT(1, arr[3].received);

    CHECK_EQUAL_C_INT(22, arr[4].first);
    CHECK_EQUAL_C_INT(22, arr[4].last);
    CHECK_EQUAL_C_INT(1, arr[4].received);

    // At this point, arr has been fully loaded with
    // realistic PER data, but to 'flush' out the PER value
    // based on the arr data requires either:
    // 1) call libsm_per_calculate(arr) here, or,
    // 2) update W0 with a timespec later than "now" to
    // force the final PER calculation for the subwindow.
    // (Using this method requires rewriting the arr tests
    // in a way that isn't intuitative and makes the
    // test code harder to understand.)
    per = libsm_per_calculate(arr);

    CHECK_EQUAL_C_INT_TEXT(13, per, "The PER Value isn't what's expected.");

    free(now);
    free(w0);
    free(w1);
    free(w2);
    free(w3);
    free(w4);
}
