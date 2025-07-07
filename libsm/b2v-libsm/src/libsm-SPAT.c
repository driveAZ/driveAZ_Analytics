/**
 * Example code to generate, validate, encode, and decode a
 * J2735 Signal Phase and Timing Message, using the asn1c-generated
 * source.
 *
 * perellil@msu.edu
 */

#include "asn_system.h"
#include "libsm.h"
#include <stdbool.h>
#include <stdlib.h>


/**
 * Takes a pointer to a SPAT structure you have allocated,
 * allocates any additional things, and fills with "unavailable" values
 * If successful, returns LIBSM_OK.
 * You are responsible for calling ASN_STRUCT_FREE
 * If spat is not initialized, returns LIBSM_FAIL_NULL_ARG
 */
libsm_rval_e libsm_init_spat(SPAT_t* spat)
{
    if (spat == NULL) {
        return LIBSM_FAIL_NULL_ARG;
    }

    // Initialize an intersection and other nested variables for spat
    IntersectionState_t* intersection = libsm_add_spat_intersectionState(spat);
    if (intersection == NULL) {
        return LIBSM_ALLOC_ERR;
    }

    return LIBSM_OK;
}


MessageFrame_t* libsm_alloc_init_mf_spat(void)
{
    MessageFrame_t* mf = calloc(1, sizeof(MessageFrame_t));

    if (mf == NULL) {
        return NULL;
    }

    mf->messageId = DSRCmsgID_signalPhaseAndTimingMessage;
    mf->value.present = MessageFrame__value_PR_SPAT;

    libsm_rval_e ret = libsm_init_spat(&mf->value.choice.SPAT);
    if (ret != LIBSM_OK) {
        ASN_STRUCT_FREE(asn_DEF_MessageFrame, mf);
        return NULL;
    }
    return mf;
}


/**
 * Function to initialize and add an intersection variable to a SPAT.
 * This function takes the spat message.
 */
IntersectionState_t* libsm_add_spat_intersectionState(SPAT_t* spat)
{
    // Initialize the intersection
    IntersectionState_t* intersection = calloc(1, sizeof(IntersectionState_t));

    if (intersection == NULL) {
        return NULL;
    }

    // Set required variable with default values
    intersection->id.id = 0;
    intersection->revision = 0;

    if (LIBSM_OK != libsm_init_bit_string(&intersection->status, 16)) {
        free(intersection);
        return NULL;
    }

    // Initialize a state for the intersection
    MovementState_t* state = libsm_add_spat_intersectionState_movementState(intersection);
    if (state == NULL) {
        free(intersection);
        return NULL;
    }

    // Add the intersection to the spat
    if (ASN_SEQUENCE_ADD(&spat->intersections.list, intersection)) {
        free(intersection);
        return NULL;
    }

    return intersection;
}


/**
 * Function to initialize and add the states variable to a SPAT intersection.
 * This function takes the spat message and index for the specified intersection.
 */
MovementState_t* libsm_add_spat_intersectionState_movementState(IntersectionState_t* intersection)
{
    // Initialize the state
    MovementState_t* state = calloc(1, sizeof(MovementState_t));

    if (state == NULL) {
        return NULL;
    }

    // Set required variable with default
    state->signalGroup = 0;

    // Initialize a state-time-speed for the state
    MovementEvent_t* sts = libsm_add_spat_movementState_movementEvent(state);
    if (sts == NULL) {
        return NULL;
    }

    if (ASN_SEQUENCE_ADD(&intersection->states.list, state)) {
        free(state);
        return NULL;
    }

    return state;
}


/**
 * Function to initialize and add the state-time-speed variable to a SPAT state.
 * This function takes the spat message, and indices for the specified
 * intersection and states objects.
 */
MovementEvent_t* libsm_add_spat_movementState_movementEvent(MovementState_t* state)
{
    // Initialize the state-time-speed
    MovementEvent_t* sts = calloc(1, sizeof(MovementEvent_t));

    if (sts == NULL) {
        return NULL;
    }

    // Set required variable with unavailable
    sts->eventState = MovementPhaseState_unavailable;

    // Add the state-time-speed to the state
    if (ASN_SEQUENCE_ADD(&state->state_time_speed.list, sts)) {
        free(sts);
        return NULL;
    }

    return sts;
}


/**
 * Checks if the given Message Frame contains a SPAT message.
 */
bool libsm_mf_has_spat(MessageFrame_t const* const mf)
{
    return mf->messageId == DSRCmsgID_signalPhaseAndTimingMessage
           && mf->value.present == MessageFrame__value_PR_SPAT;
}


/**
 * Getter that returns the SPAT message in the provided Message Frame.
 */
SPAT_t* libsm_get_spat(MessageFrame_t* const mf)
{
    if (libsm_mf_has_spat(mf)) {
        return &mf->value.choice.SPAT;
    }
    return NULL;
}
