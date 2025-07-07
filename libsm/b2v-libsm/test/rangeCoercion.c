#include "CppUTest/TestHarness_c.h"
#include "libsm.h"

TEST_C(j2735_rangeCoercion, acceleration_valid)
{
    CHECK_EQUAL_C_LONG(42, RANGE_COERCE(Acceleration, 42));
}
TEST_C(j2735_rangeCoercion, acceleration_above)
{
    CHECK_EQUAL_C_LONG(Acceleration_max, RANGE_COERCE(Acceleration, 2021));
}
TEST_C(j2735_rangeCoercion, acceleration_below)
{
    CHECK_EQUAL_C_LONG(Acceleration_min, RANGE_COERCE(Acceleration, -2021));
}
TEST_C(j2735_rangeCoercion, acceleration_unavailable)
{
    CHECK_EQUAL_C_LONG(Acceleration_unavailable,
                       RANGE_COERCE(Acceleration, Acceleration_unavailable));
}

TEST_C(j2735_rangeCoercion, latitude_valid)
{
    CHECK_EQUAL_C_LONG(424874144, RANGE_COERCE(Latitude, 424874144));
}
TEST_C(j2735_rangeCoercion, latitude_above)
{
    CHECK_EQUAL_C_LONG(Latitude_unavailable, RANGE_COERCE(Latitude, 900000003));
}
TEST_C(j2735_rangeCoercion, latitude_below)
{
    CHECK_EQUAL_C_LONG(Latitude_unavailable, RANGE_COERCE(Latitude, -900000003));
}
TEST_C(j2735_rangeCoercion, latitude_unavailable)
{
    CHECK_EQUAL_C_LONG(Latitude_unavailable, RANGE_COERCE(Latitude, Latitude_unavailable));
}

// yaw is weird because it doesn't have an unavailable or instructions for what to do
// We just set it to 0 if it's out of range and hope
TEST_C(j2735_rangeCoercion, yaw_valid)
{
    CHECK_EQUAL_C_LONG(42, RANGE_COERCE_ACTUAL(YawRate_min, YawRate_max, 0, true, 42));
}
TEST_C(j2735_rangeCoercion, yaw_invalid)
{
    CHECK_EQUAL_C_LONG(0, RANGE_COERCE_ACTUAL(YawRate_min, YawRate_max, 0, true, 32768));
}
