// https://cpputest.github.io/manual.html
/*
 * testSPAT.c
 * Test file used for SPAT demo
 */

#include <getopt.h>
#include <stdint.h>
#include <stdlib.h>

#include "CppUTest/TestHarness_c.h"
#include "libsm.h"

TEST_C(test_spat, init_spat)
{
    SPAT_t* spat = calloc(1, sizeof(SPAT_t));

    libsm_rval_e result = libsm_init_spat(spat);

    CHECK_EQUAL_C_INT(LIBSM_OK, result);

    CHECK_C(spat->intersections.list.array[0] != NULL);
    CHECK_C(spat->intersections.list.array[0]->states.list.array[0] != NULL);
    CHECK_C(spat->intersections.list.array[0]->states.list.array[0]->state_time_speed.list.array[0]
            != NULL);

    ASN_STRUCT_FREE(asn_DEF_SPAT, spat);
}

TEST_C(test_spat, init_spat_NULL)
{
    SPAT_t* spat = NULL;

    libsm_rval_e result = libsm_init_spat(spat);

    CHECK_C_TEXT(result != LIBSM_OK, "init_spat succeeded when it should have failed.");

    ASN_STRUCT_FREE(asn_DEF_SPAT, spat);
}

TEST_C(test_spat, init_spat_add_state)
{
    SPAT_t* spat = calloc(1, sizeof(SPAT_t));

    libsm_rval_e result = libsm_init_spat(spat);

    CHECK_EQUAL_C_INT(LIBSM_OK, result);

    CHECK_C(spat->intersections.list.array[0] != NULL);
    CHECK_C(spat->intersections.list.array[0]->states.list.array[0] != NULL);
    CHECK_C(spat->intersections.list.array[0]->states.list.array[0]->state_time_speed.list.array[0]
            != NULL);

    MovementState_t* state = libsm_add_spat_intersectionState_movementState(
            spat->intersections.list.array[0]);
    CHECK_C(state != NULL);

    CHECK_C(spat->intersections.list.array[0]->states.list.array[1] != NULL);
    CHECK_C(spat->intersections.list.array[0]->states.list.array[1]->state_time_speed.list.array[0]
            != NULL);

    ASN_STRUCT_FREE(asn_DEF_SPAT, spat);
}
