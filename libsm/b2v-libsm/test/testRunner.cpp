/*
* testMain.c
* Unit test initializer for libsm.c
* All test groups and tests are declared here
*/

#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness_c.h"

TEST_GROUP_C_WRAPPER(smoketest){};

TEST_C_WRAPPER(smoketest, init_psm)
TEST_C_WRAPPER(smoketest, init_psm_NULL)
TEST_C_WRAPPER(smoketest, init_bsm)
TEST_C_WRAPPER(smoketest, init_bsm_NULL)
TEST_C_WRAPPER(smoketest, decode_messageframe)
TEST_C_WRAPPER(smoketest, decode_messageframe_bad_data)
TEST_C_WRAPPER(smoketest, decode_messageframe_outside_constraints)
TEST_C_WRAPPER(smoketest, encode_messageframe_psm)
TEST_C_WRAPPER(smoketest, encode_messageframe_psm_err_val_out_of_range)
TEST_C_WRAPPER(smoketest, encode_messageframe_bsm)
TEST_C_WRAPPER(smoketest, encode_messageframe_bsm_err_val_out_of_range)
TEST_C_WRAPPER(smoketest, set_basic_vehicle_class_smoke)
TEST_C_WRAPPER(smoketest, set_basic_vehicle_class_noPartII)
TEST_C_WRAPPER(smoketest, set_basic_vehicle_class_noSupplementalVehicleExtensions)
TEST_C_WRAPPER(smoketest, get_basic_vehicle_class_smoke)
TEST_C_WRAPPER(smoketest, get_basic_vehicle_class_noPartII)
TEST_C_WRAPPER(smoketest, path_prediction_smoke)
TEST_C_WRAPPER(smoketest, path_prediction_noPartII)
TEST_C_WRAPPER(smoketest, path_prediction_noVehicleSafetyExtensions)
TEST_C_WRAPPER(smoketest, lism_str_personal_device_user_type)
TEST_C_WRAPPER(smoketest, encode_jer)
TEST_C_WRAPPER(smoketest, alloc_init_mf_psm)
TEST_C_WRAPPER(smoketest, alloc_init_mf_bsm)


TEST_GROUP_C_WRAPPER(bitstring){};
TEST_C_WRAPPER(bitstring, valid)
TEST_C_WRAPPER(bitstring, invalid)
TEST_C_WRAPPER(bitstring, bitstringBig)
TEST_C_WRAPPER(bitstring, multiSize)


TEST_GROUP_C_WRAPPER(path_history){};
TEST_C_WRAPPER(path_history, getting_partIIelements)
TEST_C_WRAPPER(path_history, getting_partIIelements_NULL)
TEST_C_WRAPPER(path_history, test_libsm_alloc_init_DDateTime)
TEST_C_WRAPPER(path_history, test_libsm_alloc_init_PathHistoryPoint)
TEST_C_WRAPPER(path_history, test_libsm_alloc_init_FullPositionVector)
TEST_C_WRAPPER(path_history, test_libsm_alloc_init_path_history)
TEST_C_WRAPPER(path_history, test_libsm_init_bsm_path_history)
TEST_C_WRAPPER(path_history, test_libsm_init_psm_path_history)
TEST_C_WRAPPER(path_history, store_and_access_psm)
TEST_C_WRAPPER(path_history, store_and_access_bsm)
TEST_C_WRAPPER(path_history, store_and_access_psm_pointers)

TEST_GROUP_C_WRAPPER(j2735_rangeCoercion){};
TEST_C_WRAPPER(j2735_rangeCoercion, acceleration_valid)
TEST_C_WRAPPER(j2735_rangeCoercion, acceleration_above)
TEST_C_WRAPPER(j2735_rangeCoercion, acceleration_below)
TEST_C_WRAPPER(j2735_rangeCoercion, acceleration_unavailable)
TEST_C_WRAPPER(j2735_rangeCoercion, latitude_valid)
TEST_C_WRAPPER(j2735_rangeCoercion, latitude_above)
TEST_C_WRAPPER(j2735_rangeCoercion, latitude_below)
TEST_C_WRAPPER(j2735_rangeCoercion, latitude_unavailable)
TEST_C_WRAPPER(j2735_rangeCoercion, yaw_valid)
TEST_C_WRAPPER(j2735_rangeCoercion, yaw_invalid)

TEST_GROUP_C_WRAPPER(versionCheck){};
TEST_C_WRAPPER(versionCheck, valid)

TEST_GROUP_C_WRAPPER(per_calculate) { };
TEST_C_WRAPPER(per_calculate, perfect)
TEST_C_WRAPPER(per_calculate, half_contiguous)
TEST_C_WRAPPER(per_calculate, half_noncontiguous)
TEST_C_WRAPPER(per_calculate, two_per_window)
TEST_C_WRAPPER(per_calculate, single_packet)
TEST_C_WRAPPER(per_calculate, j2945_1_A83_FA21a)
TEST_C_WRAPPER(per_calculate, j2945_1_A83_FA21b)
TEST_C_WRAPPER(per_calculate, j2945_1_A83_FA22a)
TEST_C_WRAPPER(per_calculate, j2945_1_A83_FA22b)
TEST_C_WRAPPER(per_calculate, rollover_all)
TEST_C_WRAPPER(per_calculate, rollover_half)
TEST_C_WRAPPER(per_calculate, rollover_between_window)
TEST_C_WRAPPER(per_calculate, two_duplicates)
TEST_C_WRAPPER(per_calculate, one_duplicate)
TEST_C_WRAPPER(per_calculate, subharthi)
TEST_C_WRAPPER(per_calculate, duplicates_between_subwindows)
TEST_C_WRAPPER(per_calculate, roundUp)
TEST_C_WRAPPER(per_calculate, roundDown)

TEST_GROUP_C_WRAPPER(storage_per) { };
TEST_C_WRAPPER(storage_per, one_in_each_window)
TEST_C_WRAPPER(storage_per, two_in_each_window)

TEST_GROUP_C_WRAPPER(per_store_and_calculate) { };
TEST_C_WRAPPER(per_store_and_calculate, noDuplicate)
TEST_C_WRAPPER(per_store_and_calculate, duplicate)

TEST_GROUP_C_WRAPPER(test_spat) { };
TEST_C_WRAPPER(test_spat, init_spat)
TEST_C_WRAPPER(test_spat, init_spat_NULL)
TEST_C_WRAPPER(test_spat, init_spat_add_state)

TEST_GROUP_C_WRAPPER(test_tim) {
    TEST_GROUP_C_SETUP_WRAPPER(test_tim);
};
TEST_C_WRAPPER(test_tim, init_tim_traveler_data_frame_roadSignID)
TEST_C_WRAPPER(test_tim, init_tim_traveler_data_frame_furtherInfoID)
TEST_C_WRAPPER(test_tim, init_tim_minimum_viable)

int main(int ac, char** av)
{
    return RUN_ALL_TESTS(ac, av);
}
