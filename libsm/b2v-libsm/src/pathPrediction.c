#include "pathPrediction.h"
#include "j2945-defines.h"
#include <math.h>


double libsm_pp_radius_filter(double curvature, libsm_pp_filter_state_t* const state)
{
    const double w0 = (M_PI * 2.0 * curvatureCutoffFrequency);
    const double w02_Ts2 = ((w0 * w0) * (curvatureSamplingPeriod * curvatureSamplingPeriod));
    const double two_w0_Zeta_Ts = (2.0 * w0 * curvatureDampingFactor * curvatureSamplingPeriod);
    const double maxCurvature = 1.0 / curvatureMaxRadius;

    double filteredCurvature;

    if (state->initCount < 2) {
        state->initCount++;
        filteredCurvature = curvature;
    } else {
        filteredCurvature = (2.0 + two_w0_Zeta_Ts) * state->Y_n_minus1;
        filteredCurvature += w02_Ts2 * curvature;
        filteredCurvature -= state->Y_n_minus2;
        filteredCurvature /= (1.0 + two_w0_Zeta_Ts + w02_Ts2);
    }

    state->Y_n_minus2 = state->Y_n_minus1;
    state->Y_n_minus1 = filteredCurvature;

    bool const signIsNegative = (filteredCurvature < 0.0);
    filteredCurvature = fmax(fabs(filteredCurvature), maxCurvature);
    if (signIsNegative) {
        filteredCurvature = -filteredCurvature;
    }

    return 1.0 / filteredCurvature;
}


// J2945/1 2020-04 Figure A19/A20
double libsm_pp_confidence_filter(double yawRate, libsm_pp_filter_state_t* const state)
{
    const double w0 = (M_PI * 2.0 * confidenceCutoffFrequency);
    const double w02_Ts2 = ((w0 * w0) * (confidenceSamplingPeriod * confidenceSamplingPeriod));
    const double w02_Ts = ((w0 * w0) * confidenceSamplingPeriod);
    const double two_w0_Zeta_Ts = (2.0 * w0 * confidenceDampingFactor * confidenceSamplingPeriod);

    double filteredConfidence;

    if (state->initCount < 2) {
        state->initCount++;
        filteredConfidence = yawRate;
    } else {
        filteredConfidence = ((2.0 + two_w0_Zeta_Ts) * state->Y_n_minus1);
        filteredConfidence += w02_Ts * (yawRate - state->u_n_minus1);
        filteredConfidence -= state->Y_n_minus2;
        filteredConfidence /= (w02_Ts2 + two_w0_Zeta_Ts + 1.0);
    }

    state->u_n_minus1 = yawRate;
    state->Y_n_minus2 = state->Y_n_minus1;
    state->Y_n_minus1 = filteredConfidence;

    return confidenceLookup(fabs(filteredConfidence));
}


double confidenceLookup(double confidenceFiltered)
{
    static double const thresholds[11] = { 25.0, 20.0, 15.0, 10.0, 5.0, 2.5,
                                           2.0,  1.5,  1.0,  0.5,  0.0 };
    static double const values[11] = { 0.0,  10.0, 20.0, 30.0, 40.0, 50.0,
                                       60.0, 70.0, 80.0, 90.0, 100.0 };

    // Linear interpolation
    for (int i = 0; i < 11; i++) {
        if (confidenceFiltered >= thresholds[i]) {
            if (i == 0) {
                return values[i];
            } else {
                double const lowerConf = values[i - 1];
                double const upperConf = values[i];
                double const lowerBound = thresholds[i - 1];
                double const upperBound = thresholds[i];
                double finalConfidence = confidenceFiltered - lowerBound;
                finalConfidence *= (upperConf - lowerConf);
                finalConfidence /= (upperBound - lowerBound);
                finalConfidence += lowerConf;
                return finalConfidence;
            }
        }
    }
    return 100.0;
}


void libsm_calculatePathPrediction(PathPrediction_t* const pp, double speed, double yawRate)
{
    static libsm_pp_filter_state_t radiusState = { 0 };
    static libsm_pp_filter_state_t confidenceState = { 0 };
    double const YawRateRadPerS = yawRate * (M_PI / 180.0);

    double curvature = 1.0 / (double)vruMaxCurveRadius;

    if (speed < vruStationarySpeedThresh) {
        curvature = YawRateRadPerS / speed;
    }

    double FilteredRadius_Raw = libsm_pp_radius_filter(curvature, &radiusState);
    ;
    double FilteredConfidence = libsm_pp_confidence_filter(yawRate, &confidenceState);

    long scaledRadius = (RadiusOfCurvature_t)(curvatureRadiusUnit * FilteredRadius_Raw);
    if (labs(scaledRadius) >= vruMaxCurveRadius) {
        pp->radiusOfCurve = RadiusOfCurvature_straight;
    } else {
        pp->radiusOfCurve = scaledRadius;
    }
    pp->confidence = (Confidence_t)(curvatureConfidenceUnit * FilteredConfidence);
}
