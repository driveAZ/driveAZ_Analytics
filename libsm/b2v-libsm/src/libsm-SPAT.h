#ifndef B2V_LIBSM_LIBSM_SPAT_H
#define B2V_LIBSM_LIBSM_SPAT_H

#include "BIT_STRING.h"
#include "MessageFrame.h"
#include "SPAT.h"
#include "j2735-defines.h"
#include "j2945-defines.h"
#include "libsm-error.h"
#include "libsm-per.h"


/**
 * Allocate and initialize a SPaT message inside a messageframe
 */
MessageFrame_t* libsm_alloc_init_mf_spat(void);


libsm_rval_e libsm_init_spat(SPAT_t* spat);


IntersectionState_t* libsm_add_spat_intersectionState(SPAT_t* spat);


MovementState_t* libsm_add_spat_intersectionState_movementState(IntersectionState_t* intersection);


MovementEvent_t* libsm_add_spat_movementState_movementEvent(MovementState_t* state);


libsm_rval_e libsm_encode_messageframe_spat(void* data, uint8_t* encoded, size_t* len);


bool libsm_mf_has_spat(MessageFrame_t const* const mf);


SPAT_t* libsm_get_spat(MessageFrame_t* const mf);


#endif //B2V_LIBSM_LIBSM_SPAT_H
