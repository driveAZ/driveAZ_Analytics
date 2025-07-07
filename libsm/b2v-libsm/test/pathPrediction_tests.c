#include "CppUTest/TestHarness_c.h"
#include "pathPrediction.h"

TEST_C(path_prediction, radius_filter_constant)
{
    double const tolerance = 1e8;
    libsm_pp_filter_state_t radiusState = { 0 };
    double const fixedRadius = 8.0;
    double const fixedCurvature = 1.0 / fixedRadius;

    for (int i = 0; i < 6; i++) {
        double const output = libsm_pp_radius_filter(fixedCurvature, &radiusState);
        CHECK_EQUAL_C_REAL(fixedRadius, output, tolerance);
    }
}

TEST_C(path_prediction, radius_filter)
{
    double const tolerance = 0.5;
    libsm_pp_filter_state_t radiusState = { 0 };
    double const maxCurvature = 1.0 / curvatureMaxRadius;
    double const fixedCurvature = 0.0017453292519943296;
    double const fixedRadius = 1.0 / fixedCurvature;
    double output;

    (void)libsm_pp_radius_filter(maxCurvature, &radiusState);
    (void)libsm_pp_radius_filter(maxCurvature, &radiusState);
    for (int i = 0; i < 50; i++) {
        output = libsm_pp_radius_filter(fixedCurvature, &radiusState);
    }
    CHECK_EQUAL_C_REAL(fixedRadius, output, tolerance);
}

TEST_C(path_prediction, radius_filter_max_radius)
{
    double const tolerance = 0.01;
    libsm_pp_filter_state_t radiusState = { 0 };
    double const maxCurvature = 1.0 / curvatureMaxRadius;
    double const output = libsm_pp_radius_filter(maxCurvature, &radiusState);

    CHECK_EQUAL_C_REAL(curvatureMaxRadius, output, tolerance);
}

TEST_C(path_prediction, confidence_filter_constant)
{
    double const tolerance = 1e8;
    libsm_pp_filter_state_t radiusState = { 0 };
    double const targetYawRate = 0.0;
    double const expectedResult = 100.0;
    double output;

    for (int i = 0; i < 6; i++) {
        output = libsm_pp_confidence_filter(targetYawRate, &radiusState);
    }
    CHECK_EQUAL_C_REAL(expectedResult, output, tolerance);
}

TEST_C(path_prediction, confidence_filter)
{
    double const tolerance = 0.5;
    libsm_pp_filter_state_t radiusState = { 0 };
    double const initialYawRate = 25.0;
    double const targetYawRate = 0.0;
    double const expectedResult = 100.0;
    double output;

    (void)libsm_pp_confidence_filter(initialYawRate, &radiusState);
    (void)libsm_pp_confidence_filter(initialYawRate, &radiusState);
    for (int i = 0; i < 25; i++) {
        output = libsm_pp_confidence_filter(targetYawRate, &radiusState);
    }
    CHECK_EQUAL_C_REAL(expectedResult, output, tolerance);
}

TEST_C(path_prediction, confidence_filter_negative)
{
    double const tolerance = 1e8;
    libsm_pp_filter_state_t radiusState = { 0 };
    double const targetYawRate = -0.0;
    double const expectedResult = -100.0;
    double output;

    for (int i = 0; i < 6; i++) {
        output = libsm_pp_confidence_filter(targetYawRate, &radiusState);
    }
    CHECK_EQUAL_C_REAL(expectedResult, output, tolerance);
}

TEST_C(path_prediction, confidenceLookup_min)
{
    double const tolerance = 0.1;
    double const largeValue = 100.0;
    double const expectedResult = 0.0;
    double const actualResult = confidenceLookup(largeValue);

    CHECK_EQUAL_C_REAL(expectedResult, actualResult, tolerance);
}

TEST_C(path_prediction, confidenceLookup_max)
{
    double const tolerance = 0.1;
    double const negativeValue = -10.0;
    double const expectedResult = 100.0;
    double const actualResult = confidenceLookup(negativeValue);

    CHECK_EQUAL_C_REAL(expectedResult, actualResult, tolerance);
}

TEST_C(path_prediction, confidenceLookup_interpolate)
{
    double const tolerance = 0.1;
    double const negativeValue = 12.5;
    double const expectedResult = 25.0;
    double const actualResult = confidenceLookup(negativeValue);

    CHECK_EQUAL_C_REAL(expectedResult, actualResult, tolerance);
}
