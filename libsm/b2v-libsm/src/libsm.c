/**
 * PSM.c
 * Example code to generate, validate, encode, and decode a
 * J2735 Personal Safety Message, using the asn1c-generated
 * source.
 * Additional functions added to UPER-encode a BasicSafetyMessage.
 * This code was tested on a Win7 in the Cygwin environment,
 * on MacOS, and the results were validated
 * using http://asn1-playground.oss.com/
 *
 * 20180710
 * rich@tomesoftware.com
 */

#include "libsm.h"

#include <BasicSafetyMessage.h>
#include <MessageFrame.h>
#include <PersonalSafetyMessage.h>
#include <SPAT.h>
#include <asn_system.h>

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


libsm_rval_e libsm_init_TemporaryID(TemporaryID_t* id)
{
    return libsm_init_octet_string(id, 4);
}


/**
 * Takes a pointer to a PSM structure you have allocated,
 * allocates any additional things, and fills with "unavailable" values
 * If successful, returns LIBSM_OK.
 * You are responsible for calling ASN_STRUCT_FREE
 * If psm is not initialized, returns LIBSM_FAIL_NULL_ARG
 */
libsm_rval_e libsm_init_psm(PersonalSafetyMessage_t* psm)
{
    if (psm == NULL) {
        return LIBSM_FAIL_NULL_ARG;
    }

    //alloc id
    if (libsm_init_TemporaryID(&psm->id) != 0) {
        return LIBSM_ALLOC_ERR;
    }

    psm->basicType = PersonalDeviceUserType_unavailable;
    psm->secMark = DSecond_unavailable;
    psm->msgCnt = 0;

    psm->position.lat = Latitude_unavailable;
    psm->position.Long = Longitude_unavailable;

    psm->accuracy.semiMajor = SemiMajorAxisAccuracy_unavailable;
    psm->accuracy.semiMinor = SemiMinorAxisAccuracy_unavailable;
    psm->accuracy.orientation = SemiMajorAxisOrientation_unavailable;

    psm->speed = Velocity_unavailable;
    psm->heading = Heading_unavailable;

    return LIBSM_OK;
}


/**
 * Takes a pointer to a BSM structure you have allocated, allocates any additional coreData items,
 * and fills with "unavailable" values.
 * If successful, returns LIBSM_OK.
 * You are responsible for calling ASN_STRUCT_FREE
 * If bsm is not initialized, returns LIBSM_FAIL_NULL_ARG
 */
libsm_rval_e libsm_init_bsm(BasicSafetyMessage_t* bsm)
{
    if (bsm == NULL) {
        return LIBSM_FAIL_NULL_ARG;
    }

    //alloc id
    if (libsm_init_TemporaryID(&bsm->coreData.id) != 0) {
        return LIBSM_ALLOC_ERR;
    }

    bsm->coreData.msgCnt = 0;
    bsm->coreData.secMark = DSecond_unavailable;
    bsm->coreData.lat = Latitude_unavailable;
    bsm->coreData.Long = Longitude_unavailable;
    bsm->coreData.elev = Elevation_unavailable;

    bsm->coreData.accuracy.semiMajor = SemiMajorAxisAccuracy_unavailable;
    bsm->coreData.accuracy.semiMinor = SemiMinorAxisAccuracy_unavailable;
    bsm->coreData.accuracy.orientation = SemiMajorAxisOrientation_unavailable;

    bsm->coreData.transmission = TransmissionState_unavailable;
    bsm->coreData.speed = Velocity_unavailable;
    bsm->coreData.heading = Heading_unavailable;
    bsm->coreData.angle = SteeringWheelAngle_unavailable;

    bsm->coreData.accelSet.Long = Acceleration_unavailable;
    bsm->coreData.accelSet.lat = Acceleration_unavailable;
    bsm->coreData.accelSet.vert = VerticalAcceleration_unavailable;
    bsm->coreData.accelSet.yaw = 0; // no unavailable, so hopefully a YawRate of 0 is fine


    if (libsm_init_bit_string(&bsm->coreData.brakes.wheelBrakes, 5)) {
        return LIBSM_ALLOC_ERR;
    }
    //in a bit string, the leftmost bit is bit 0
    //so unavailable for BrakeAppliedStatus should be 0x80, or 1 << 7
    bsm->coreData.brakes.wheelBrakes.buf[0] = 0x0;
    bsm->coreData.brakes.wheelBrakes.buf[0] |= (1 << (7 - BrakeAppliedStatus_unavailable));

    bsm->coreData.brakes.traction = TractionControlStatus_unavailable;
    bsm->coreData.brakes.abs = AntiLockBrakeStatus_unavailable;
    bsm->coreData.brakes.scs = StabilityControlStatus_unavailable;
    bsm->coreData.brakes.brakeBoost = BrakeBoostApplied_unavailable;
    bsm->coreData.brakes.auxBrakes = AuxiliaryBrakeStatus_unavailable;

    bsm->coreData.size.width = VehicleWidth_unavailable;
    bsm->coreData.size.length = VehicleLength_unavailable;

    return LIBSM_OK;
}


BIT_STRING_t* libsm_alloc_init_bit_string(size_t bits)
{
    BIT_STRING_t* bit_string = calloc(1, sizeof(BIT_STRING_t));

    if (bit_string == NULL) {
        return NULL;
    }
    if (LIBSM_OK != libsm_init_bit_string(bit_string, bits)) {
        free(bit_string);
        bit_string = NULL;
    }
    return bit_string;
}


libsm_rval_e libsm_init_bit_string(BIT_STRING_t* bit_string, size_t bits)
{
    // integer division but round up
    size_t const bytes = (bits + 7) / 8;

    bit_string->buf = calloc(bytes, sizeof(uint8_t));
    if (bit_string->buf == NULL) {
        return LIBSM_ALLOC_ERR;
    }
    bit_string->size = bytes;
    bit_string->bits_unused = bytes * 8 - bits;
    return LIBSM_OK;
}


/**
 * Function that accepts a UPER-encoded MessageFrame in the buffer, decodes it, and then validates it.
 * mf must be allocated before calling.
 * To access the PSM, use something like: PersonalSafetyMessage_t PSM = mf->value.choice.PersonalSafetyMessage;
 * NOTE: The caller is responsible for freeing mf with ASN_STRUCT_FREE
 */
libsm_rval_e libsm_decode_messageframe(const uint8_t* encoded, size_t len, MessageFrame_t* mf)
{
    asn_dec_rval_t rval;

    if (mf == NULL) {
        return LIBSM_FAIL_NULL_ARG;
    }
    if (len == 0) {
        return LIBSM_FAIL_DECODING_BUFF_SIZE;
    }

    rval = asn_decode(0,
                      ATS_UNALIGNED_BASIC_PER,
                      &asn_DEF_MessageFrame,
                      (void**)&mf,
                      encoded,
                      len);

    switch (rval.code) {
        case RC_OK:
            if (rval.consumed == 0) {
                return LIBSM_FAIL_DECODING;
            }
            break;
        case RC_WMORE:
        case RC_FAIL:
            return LIBSM_FAIL_DECODING;
    }
    return LIBSM_OK;
}


libsm_rval_e libsm_encode_messageframe(MessageFrame_t* mf, uint8_t* encoded, size_t* len)
{
    asn_enc_rval_t enc_res = asn_encode_to_buffer(0,
                                                  ATS_UNALIGNED_BASIC_PER,
                                                  &asn_DEF_MessageFrame,
                                                  mf,
                                                  encoded,
                                                  *len);

    if (enc_res.encoded == -1) {
        //printf("Failed to encode MessageFrame of type %s\n", enc_res.failed_type->name);
        switch (errno) {
            case EINVAL:
                //printf("MessageFrame encode to buffer EINVAL: Incorrect parameters to the function, such as NULLs\n");
                break;
            case ENOENT:
                //printf("MessageFrame encode to buffer ENOENT: Encoding transfer syntax is not defined (for this type)\n");
                break;
            case EBADF:
                //printf("MessageFrame encode to buffer EBADF: The structure has invalid form or content constraint failed\n");
                return LIBSM_FAIL_CONSTRAINT;
                break;
            default:
                //printf("MessageFrame encode to buffer ERRNO IS BROKEN\n");
                break;
        }
        return LIBSM_FAIL_ENCODING;
    } else if ((size_t)enc_res.encoded > *len) {
        return LIBSM_FAIL_ENCODING_BUFF_SIZE;
    }
    // Number of encoded bytes is returned in buff_size
    *len = enc_res.encoded;
    return LIBSM_OK;
}


/**
 * Given a psm/bsm and buffer, wrap the PSM in a MessageFrame, validate the whole message, then UPER-encode it.
 * If LIBSM_OK is returned, the UPER-encoded message is in encodec and the size of encoded message is *len.
 */
static libsm_rval_e libsm_encode_messageframe_both_bsm_psm(bool bsm,
                                                           void* data,
                                                           uint8_t* encoded,
                                                           size_t* len)
{
    // we don't allocate anything in it, it has pointers to data so we don't need to ASN_STRUCT_FREE it
    MessageFrame_t mf;

    if (*len == 0) {
        return LIBSM_FAIL_ENCODING_BUFF_SIZE;
    }

    // Build the MessageFrame and load the PSM/BSM into it.
    if (bsm) {
        mf.messageId = DSRCmsgID_basicSafetyMessage;
        mf.value.present = MessageFrame__value_PR_BasicSafetyMessage;
        mf.value.choice.BasicSafetyMessage = *(BasicSafetyMessage_t*)data;
    } else {
        mf.messageId = DSRCmsgID_personalSafetyMessage;
        mf.value.present = MessageFrame__value_PR_PersonalSafetyMessage;
        mf.value.choice.PersonalSafetyMessage = *(PersonalSafetyMessage_t*)data;
    }

    return libsm_encode_messageframe(&mf, encoded, len);
}


/**
 * Given a psm and buffer, wrap the PSM in a MessageFrame, validate the whole message, then UPER-encode it.
 * If LIBSM_OK is returned, the UPER-encoded message is in encodec and the size of encoded message is *len.
 */
libsm_rval_e libsm_encode_messageframe_psm(PersonalSafetyMessage_t* psm,
                                           uint8_t* encoded,
                                           size_t* len)
{
    return libsm_encode_messageframe_both_bsm_psm(false, (void*)psm, encoded, len);
}


/**
 * Given a bsm and buffer, wrap the BSM in a MessageFrame, validate the whole message, then UPER-encode it.
 * If LIBSM_OK is returned, the UPER-encoded message is in encodec and the size of encoded message is *len.
 */
libsm_rval_e libsm_encode_messageframe_bsm(BasicSafetyMessage_t* bsm,
                                           uint8_t* encoded,
                                           size_t* len)
{
    return libsm_encode_messageframe_both_bsm_psm(true, (void*)bsm, encoded, len);
}


/**
 * Function that accepts a long based on values defined in J2735 returns the basic type string
 * Default 0 mapping for unknown if type not found
 * BasicVehicleClass_t is a long
 */
const char* libsm_str_basic_vehicle_class(BasicVehicleClass_t type)
{
    switch (type) {
        case BasicVehicleClass_specialVehicleClass:
            return "specialVehicleClass";
        case BasicVehicleClass_passenger_Vehicle_TypeUnknown:
            return "passenger-Vehicle-TypeUnknown";
        case BasicVehicleClass_passenger_Vehicle_TypeOther:
            return "passenger-Vehicle-TypeOther";
        case BasicVehicleClass_lightTruck_Vehicle_TypeUnknown:
            return "lightTruck-Vehicle-TypeUnknown";
        case BasicVehicleClass_lightTruck_Vehicle_TypeOther:
            return "lightTruck-Vehicle-TypeOther";
        case BasicVehicleClass_truck_Vehicle_TypeUnknown:
            return "truck-Vehicle-TypeUnknown";
        case BasicVehicleClass_truck_Vehicle_TypeOther:
            return "truck-Vehicle-TypeOther";
        case BasicVehicleClass_truck_axleCnt2:
            return "truck-axleCnt2";
        case BasicVehicleClass_truck_axleCnt3:
            return "truck-axleCnt3";
        case BasicVehicleClass_truck_axleCnt4:
            return "truck-axleCnt4";
        case BasicVehicleClass_truck_axleCnt4Trailer:
            return "truck-axleCnt4Trailer";
        case BasicVehicleClass_truck_axleCnt5Trailer:
            return "truck-axleCnt5Trailer";
        case BasicVehicleClass_truck_axleCnt6Trailer:
            return "truck-axleCnt6Trailer";
        case BasicVehicleClass_truck_axleCnt5MultiTrailer:
            return "truck-axleCnt5MultiTrailer";
        case BasicVehicleClass_truck_axleCnt6MultiTrailer:
            return "truck-axleCnt6MultiTrailer";
        case BasicVehicleClass_truck_axleCnt7MultiTrailer:
            return "truck-axleCnt7MultiTrailer";
        case BasicVehicleClass_motorcycle_TypeUnknown:
            return "motorcycle-TypeUnknown";
        case BasicVehicleClass_motorcycle_TypeOther:
            return "motorcycle-TypeOther";
        case BasicVehicleClass_motorcycle_Cruiser_Standard:
            return "motorcycle-Cruiser-Standard";
        case BasicVehicleClass_motorcycle_SportUnclad:
            return "motorcycle-SportUnclad";
        case BasicVehicleClass_motorcycle_SportTouring:
            return "motorcycle-SportTouring";
        case BasicVehicleClass_motorcycle_SuperSport:
            return "motorcycle-SuperSport";
        case BasicVehicleClass_motorcycle_Touring:
            return "motorcycle-Touring";
        case BasicVehicleClass_motorcycle_Trike:
            return "motorcycle-Trike";
        case BasicVehicleClass_motorcycle_wPassengers:
            return "motorcycle-wPassengers";
        case BasicVehicleClass_transit_TypeUnknown:
            return "transit-TypeUnknown";
        case BasicVehicleClass_transit_TypeOther:
            return "transit-TypeOther";
        case BasicVehicleClass_transit_BRT:
            return "transit-BRT";
        case BasicVehicleClass_transit_ExpressBus:
            return "transit-ExpressBus";
        case BasicVehicleClass_transit_LocalBus:
            return "transit-LocalBus";
        case BasicVehicleClass_transit_SchoolBus:
            return "transit-SchoolBus";
        case BasicVehicleClass_transit_FixedGuideway:
            return "transit-FixedGuideway";
        case BasicVehicleClass_transit_Paratransit:
            return "transit-Paratransit";
        case BasicVehicleClass_transit_Paratransit_Ambulance:
            return "transit-Paratransit-Ambulance";
        case BasicVehicleClass_emergency_TypeUnknown:
            return "emergency-TypeUnknown";
        case BasicVehicleClass_emergency_TypeOther:
            return "emergency-TypeOther";
        case BasicVehicleClass_emergency_Fire_Light_Vehicle:
            return "emergency-Fire-Light-Vehicle";
        case BasicVehicleClass_emergency_Fire_Heavy_Vehicle:
            return "emergency-Fire-Heavy-Vehicle";
        case BasicVehicleClass_emergency_Fire_Paramedic_Vehicle:
            return "emergency-Fire-Paramedic-Vehicle";
        case BasicVehicleClass_emergency_Fire_Ambulance_Vehicle:
            return "emergency-Fire-Ambulance-Vehicle";
        case BasicVehicleClass_emergency_Police_Light_Vehicle:
            return "emergency-Police-Light-Vehicle";
        case BasicVehicleClass_emergency_Police_Heavy_Vehicle:
            return "emergency-Police-Heavy-Vehicle";
        case BasicVehicleClass_emergency_Other_Responder:
            return "emergency-Other-Responder";
        case BasicVehicleClass_emergency_Other_Ambulance:
            return "emergency-Other-Ambulance";
        case BasicVehicleClass_otherTraveler_TypeUnknown:
            return "otherTraveler-TypeUnknown";
        case BasicVehicleClass_otherTraveler_TypeOther:
            return "otherTraveler-TypeOther";
        case BasicVehicleClass_otherTraveler_Pedestrian:
            return "otherTraveler-Pedestrian";
        case BasicVehicleClass_otherTraveler_Visually_Disabled:
            return "otherTraveler-Visually-Disabled";
        case BasicVehicleClass_otherTraveler_Physically_Disabled:
            return "otherTraveler-Physically-Disabled";
        case BasicVehicleClass_otherTraveler_Bicycle:
            return "otherTraveler-Bicycle";
        case BasicVehicleClass_otherTraveler_Vulnerable_Roadworker:
            return "otherTraveler-Vulnerable-Roadworker";
        case BasicVehicleClass_infrastructure_TypeUnknown:
            return "infrastructure-TypeUnknown";
        case BasicVehicleClass_infrastructure_Fixed:
            return "infrastructure-Fixed";
        case BasicVehicleClass_infrastructure_Movable:
            return "infrastructure-Movable";
        case BasicVehicleClass_equipped_CargoTrailer:
            return "equipped-CargoTrailer";
        case BasicVehicleClass_unknownVehicleClass:
        default:
            return "unknownVehicleClass";
    }
}


/**
 * Function that accepts basic type string returns a long based on values defined in J2735
 * Default 0 mapping for unknown if type not found
 * BasicVehicleClass_t is a long
 */
BasicVehicleClass_t libsm_basic_vehicle_class_from_str(char* type)
{
    if (strcmp(type, "specialVehicleClass") == 0) {
        return BasicVehicleClass_specialVehicleClass;
    }
    if (strcmp(type, "passenger_Vehicle_TypeUnknown") == 0
        || strcmp(type, "passenger-Vehicle-TypeUnknown") == 0) {
        return BasicVehicleClass_passenger_Vehicle_TypeUnknown;
    }
    if (strcmp(type, "passenger_Vehicle_TypeOther") == 0
        || strcmp(type, "passenger-Vehicle-TypeOther") == 0) {
        return BasicVehicleClass_passenger_Vehicle_TypeOther;
    }
    if (strcmp(type, "lightTruck_Vehicle_TypeUnknown") == 0
        || strcmp(type, "lightTruck-Vehicle-TypeUnknown") == 0) {
        return BasicVehicleClass_lightTruck_Vehicle_TypeUnknown;
    }
    if (strcmp(type, "lightTruck_Vehicle_TypeOther") == 0
        || strcmp(type, "lightTruck-Vehicle-TypeOther") == 0) {
        return BasicVehicleClass_lightTruck_Vehicle_TypeOther;
    }
    if (strcmp(type, "truck_Vehicle_TypeUnknown") == 0
        || strcmp(type, "truck-Vehicle-TypeUnknown") == 0) {
        return BasicVehicleClass_truck_Vehicle_TypeUnknown;
    }
    if (strcmp(type, "truck_Vehicle_TypeOther") == 0
        || strcmp(type, "truck-Vehicle-TypeOther") == 0) {
        return BasicVehicleClass_truck_Vehicle_TypeOther;
    }
    if (strcmp(type, "truck_axleCnt2") == 0 || strcmp(type, "truck-axleCnt2") == 0) {
        return BasicVehicleClass_truck_axleCnt2;
    }
    if (strcmp(type, "truck_axleCnt3") == 0 || strcmp(type, "truck-axleCnt3") == 0) {
        return BasicVehicleClass_truck_axleCnt3;
    }
    if (strcmp(type, "truck_axleCnt4") == 0 || strcmp(type, "truck-axleCnt4") == 0) {
        return BasicVehicleClass_truck_axleCnt4;
    }
    if (strcmp(type, "truck_axleCnt4Trailer") == 0
        || strcmp(type, "truck-axleCnt4Trailer") == 0) {
        return BasicVehicleClass_truck_axleCnt4Trailer;
    }
    if (strcmp(type, "truck_axleCnt5Trailer") == 0
        || strcmp(type, "truck-axleCnt5Trailer") == 0) {
        return BasicVehicleClass_truck_axleCnt5Trailer;
    }
    if (strcmp(type, "truck_axleCnt6Trailer") == 0
        || strcmp(type, "truck-axleCnt6Trailer") == 0) {
        return BasicVehicleClass_truck_axleCnt6Trailer;
    }
    if (strcmp(type, "truck_axleCnt5MultiTrailer") == 0
        || strcmp(type, "truck-axleCnt5MultiTrailer") == 0) {
        return BasicVehicleClass_truck_axleCnt5MultiTrailer;
    }
    if (strcmp(type, "truck_axleCnt6MultiTrailer") == 0
        || strcmp(type, "truck-axleCnt6MultiTrailer") == 0) {
        return BasicVehicleClass_truck_axleCnt6MultiTrailer;
    }
    if (strcmp(type, "truck_axleCnt7MultiTrailer") == 0
        || strcmp(type, "truck-axleCnt7MultiTrailer") == 0) {
        return BasicVehicleClass_truck_axleCnt7MultiTrailer;
    }
    if (strcmp(type, "motorcycle_TypeUnknown") == 0
        || strcmp(type, "motorcycle-TypeUnknown") == 0) {
        return BasicVehicleClass_motorcycle_TypeUnknown;
    }
    if (strcmp(type, "motorcycle_TypeOther") == 0 || strcmp(type, "motorcycle-TypeOther") == 0) {
        return BasicVehicleClass_motorcycle_TypeOther;
    }
    if (strcmp(type, "motorcycle_Cruiser_Standard") == 0
        || strcmp(type, "motorcycle-Cruiser-Standard") == 0) {
        return BasicVehicleClass_motorcycle_Cruiser_Standard;
    }
    if (strcmp(type, "motorcycle_SportUnclad") == 0
        || strcmp(type, "motorcycle-SportUnclad") == 0) {
        return BasicVehicleClass_motorcycle_SportUnclad;
    }
    if (strcmp(type, "motorcycle_SportTouring") == 0
        || strcmp(type, "motorcycle-SportTouring") == 0) {
        return BasicVehicleClass_motorcycle_SportTouring;
    }
    if (strcmp(type, "motorcycle_SuperSport") == 0
        || strcmp(type, "motorcycle-SuperSport") == 0) {
        return BasicVehicleClass_motorcycle_SuperSport;
    }
    if (strcmp(type, "motorcycle_Touring") == 0 || strcmp(type, "motorcycle-Touring") == 0) {
        return BasicVehicleClass_motorcycle_Touring;
    }
    if (strcmp(type, "motorcycle_Trike") == 0 || strcmp(type, "motorcycle-Trike") == 0) {
        return BasicVehicleClass_motorcycle_Trike;
    }
    if (strcmp(type, "motorcycle_wPassengers") == 0
        || strcmp(type, "motorcycle-wPassengers") == 0) {
        return BasicVehicleClass_motorcycle_wPassengers;
    }
    if (strcmp(type, "transit_TypeUnknown") == 0 || strcmp(type, "transit-TypeUnknown") == 0) {
        return BasicVehicleClass_transit_TypeUnknown;
    }
    if (strcmp(type, "transit_TypeOther") == 0 || strcmp(type, "transit-TypeOther") == 0) {
        return BasicVehicleClass_transit_TypeOther;
    }
    if (strcmp(type, "transit_BRT") == 0 || strcmp(type, "transit-BRT") == 0) {
        return BasicVehicleClass_transit_BRT;
    }
    if (strcmp(type, "transit_ExpressBus") == 0 || strcmp(type, "transit-ExpressBus") == 0) {
        return BasicVehicleClass_transit_ExpressBus;
    }
    if (strcmp(type, "transit_LocalBus") == 0 || strcmp(type, "transit-LocalBus") == 0) {
        return BasicVehicleClass_transit_LocalBus;
    }
    if (strcmp(type, "transit_SchoolBus") == 0 || strcmp(type, "transit-SchoolBus") == 0) {
        return BasicVehicleClass_transit_SchoolBus;
    }
    if (strcmp(type, "transit_FixedGuideway") == 0
        || strcmp(type, "transit-FixedGuideway") == 0) {
        return BasicVehicleClass_transit_FixedGuideway;
    }
    if (strcmp(type, "transit_Paratransit") == 0 || strcmp(type, "transit-Paratransit") == 0) {
        return BasicVehicleClass_transit_Paratransit;
    }
    if (strcmp(type, "transit_Paratransit_Ambulance") == 0
        || strcmp(type, "transit-Paratransit-Ambulance") == 0) {
        return BasicVehicleClass_transit_Paratransit_Ambulance;
    }
    if (strcmp(type, "emergency_TypeUnknown") == 0
        || strcmp(type, "emergency-TypeUnknown") == 0) {
        return BasicVehicleClass_emergency_TypeUnknown;
    }
    if (strcmp(type, "emergency_TypeOther") == 0 || strcmp(type, "emergency-TypeOther") == 0) {
        return BasicVehicleClass_emergency_TypeOther;
    }
    if (strcmp(type, "emergency_Fire_Light_Vehicle") == 0
        || strcmp(type, "emergency-Fire-Light-Vehicle") == 0) {
        return BasicVehicleClass_emergency_Fire_Light_Vehicle;
    }
    if (strcmp(type, "emergency_Fire_Heavy_Vehicle") == 0
        || strcmp(type, "emergency-Fire-Heavy-Vehicle") == 0) {
        return BasicVehicleClass_emergency_Fire_Heavy_Vehicle;
    }
    if (strcmp(type, "emergency_Fire_Paramedic_Vehicle") == 0
        || strcmp(type, "emergency-Fire-Paramedic-Vehicle") == 0) {
        return BasicVehicleClass_emergency_Fire_Paramedic_Vehicle;
    }
    if (strcmp(type, "emergency_Fire_Ambulance_Vehicle") == 0
        || strcmp(type, "emergency-Fire-Ambulance-Vehicle") == 0) {
        return BasicVehicleClass_emergency_Fire_Ambulance_Vehicle;
    }
    if (strcmp(type, "emergency_Police_Light_Vehicle") == 0
        || strcmp(type, "emergency-Police-Light-Vehicle") == 0) {
        return BasicVehicleClass_emergency_Police_Light_Vehicle;
    }
    if (strcmp(type, "emergency_Police_Heavy_Vehicle") == 0
        || strcmp(type, "emergency-Police-Heavy-Vehicle") == 0) {
        return BasicVehicleClass_emergency_Police_Heavy_Vehicle;
    }
    if (strcmp(type, "emergency_Other_Responder") == 0
        || strcmp(type, "emergency-Other-Responder") == 0) {
        return BasicVehicleClass_emergency_Other_Responder;
    }
    if (strcmp(type, "emergency_Other_Ambulance") == 0
        || strcmp(type, "emergency-Other-Ambulance") == 0) {
        return BasicVehicleClass_emergency_Other_Ambulance;
    }
    if (strcmp(type, "otherTraveler_TypeUnknown") == 0
        || strcmp(type, "otherTraveler-TypeUnknown") == 0) {
        return BasicVehicleClass_otherTraveler_TypeUnknown;
    }
    if (strcmp(type, "otherTraveler_TypeOther") == 0
        || strcmp(type, "otherTraveler-TypeOther") == 0) {
        return BasicVehicleClass_otherTraveler_TypeOther;
    }
    if (strcmp(type, "otherTraveler_Pedestrian") == 0
        || strcmp(type, "otherTraveler-Pedestrian") == 0) {
        return BasicVehicleClass_otherTraveler_Pedestrian;
    }
    if (strcmp(type, "otherTraveler_Visually_Disabled") == 0
        || strcmp(type, "otherTraveler-Visually-Disabled") == 0) {
        return BasicVehicleClass_otherTraveler_Visually_Disabled;
    }
    if (strcmp(type, "otherTraveler_Physically_Disabled") == 0
        || strcmp(type, "otherTraveler-Physically-Disabled") == 0) {
        return BasicVehicleClass_otherTraveler_Physically_Disabled;
    }
    if (strcmp(type, "otherTraveler_Bicycle") == 0
        || strcmp(type, "otherTraveler-Bicycle") == 0) {
        return BasicVehicleClass_otherTraveler_Bicycle;
    }
    if (strcmp(type, "otherTraveler_Vulnerable_Roadworker") == 0
        || strcmp(type, "otherTraveler-Vulnerable-Roadworker") == 0) {
        return BasicVehicleClass_otherTraveler_Vulnerable_Roadworker;
    }
    if (strcmp(type, "infrastructure_TypeUnknown") == 0
        || strcmp(type, "infrastructure-TypeUnknown") == 0) {
        return BasicVehicleClass_infrastructure_TypeUnknown;
    }
    if (strcmp(type, "infrastructure_Fixed") == 0 || strcmp(type, "infrastructure-Fixed") == 0) {
        return BasicVehicleClass_infrastructure_Fixed;
    }
    if (strcmp(type, "infrastructure_Movable") == 0
        || strcmp(type, "infrastructure-Movable") == 0) {
        return BasicVehicleClass_infrastructure_Movable;
    }
    if (strcmp(type, "equipped_CargoTrailer") == 0
        || strcmp(type, "equipped-CargoTrailer") == 0) {
        return BasicVehicleClass_equipped_CargoTrailer;
    }

    return BasicVehicleClass_unknownVehicleClass;
}


/**
 * Function that accepts a long based on values defined in J2735 returns the basic type string
 * Default 0 mapping for unavailable if type not found
 * PersonalDeviceUserType_t is a long
 */
const char* libsm_str_personal_device_user_type(PersonalDeviceUserType_t type)
{
    static char* types[] = { "unavailable",
                             "aPEDESTRIAN",
                             "aPEDALCYCLIST",
                             "aPUBLICSAFETYWORKER",
                             "anANIMAL" };

    // size_t is unsigned, so negative numbers are just really big numbers
    if ((size_t)type >= ARRAY_SIZE(types)) {
        type = 0;
    }
    return types[type];
}


/**
 * Function that will convert a string to its corresponding PersonalDeviceUserType
 * Accepts a string and returns PersonalDeviceUserType_t
 */
PersonalDeviceUserType_t libsm_personal_device_user_type_from_str(char* deviceType)
{
    if (strcasecmp(deviceType, "unavailable") == 0) {
        return PersonalDeviceUserType_unavailable;
    }
    if (strcmp(deviceType, "pedestrian") == 0 || strcmp(deviceType, "aPEDESTRIAN") == 0) {
        return PersonalDeviceUserType_aPEDESTRIAN;
    }
    if (strcmp(deviceType, "pedalcyclist") == 0 || strcmp(deviceType, "aPEDALCYCLIST") == 0) {
        return PersonalDeviceUserType_aPEDALCYCLIST;
    }
    if (strcmp(deviceType, "public safety worker") == 0
        || strcmp(deviceType, "aPUBLICSAFETYWORKER") == 0) {
        return PersonalDeviceUserType_aPUBLICSAFETYWORKER;
    }
    if (strcmp(deviceType, "animal") == 0 || strcmp(deviceType, "anANIMAL") == 0) {
        return PersonalDeviceUserType_anANIMAL;
    }
    return PersonalDeviceUserType_unavailable;
}


libsm_rval_e libsm_bsm_alloc_partII_if_not_exist(BasicSafetyMessage_t* bsm)
{
    if (bsm->partII == NULL) {
        //allocate part II holder
        bsm->partII = calloc(1, sizeof(struct BasicSafetyMessage__partII));
        if (bsm->partII == NULL) {
            return LIBSM_ALLOC_ERR;
        }
    }
    return LIBSM_OK;
}


void* libsm_get_partII_elem(BasicSafetyMessage_t* bsm,
                            BSMpartIIExtension__partII_Value_PR searchFor)
{
    if (bsm == NULL || bsm->partII == NULL) {
        return NULL;
    }
    for (int i = 0; i < bsm->partII->list.count; i++) {
        if (bsm->partII->list.array[i]->partII_Value.present == searchFor) {
            // cast to void because choice is a union, and we don't know which one
            // they want but the pointer is the same
            return (void*)&bsm->partII->list.array[i]->partII_Value.choice.VehicleSafetyExtensions;
        }
    }
    return NULL;
}


/**
 * Function that returns the basic vehicle type for a given BSM
 * Default 0 mapping for unavailable if type not found
 */
BasicVehicleClass_t libsm_get_basic_vehicle_class(BasicSafetyMessage_t* bsm)
{
    if (bsm->partII) {
        for (int i = 0; i < bsm->partII->list.count; i++) {
            BSMpartIIExtension_t* partIIExt = bsm->partII->list.array[i];
            if (partIIExt->partII_Value.present
                == BSMpartIIExtension__partII_Value_PR_SupplementalVehicleExtensions) {
                // classification is optional, so we have to dereference its pointer
                return *partIIExt->partII_Value.choice.SupplementalVehicleExtensions
                                .classification;
            }
        }
    }
    return BasicVehicleClass_unknownVehicleClass;
}


static SupplementalVehicleExtensions_t* libsm_add_bsm_SupplementalVehicleExtension(
        BasicSafetyMessage_t* bsm)
{
    BSMpartIIExtension_t* supplementalVehicleExt = calloc(1, sizeof(BSMpartIIExtension_t));

    if (supplementalVehicleExt == NULL) {
        return NULL;
    }

    supplementalVehicleExt->partII_Id = PartII_Id_supplementalVehicleExt;
    supplementalVehicleExt->partII_Value.present
            = BSMpartIIExtension__partII_Value_PR_SupplementalVehicleExtensions;

    supplementalVehicleExt->partII_Value.choice.SupplementalVehicleExtensions.classification
            = calloc(1, sizeof(BasicVehicleClass_t));
    if (supplementalVehicleExt->partII_Value.choice.SupplementalVehicleExtensions.classification
        == NULL) {
        free(supplementalVehicleExt);
        return NULL;
    }

    if (ASN_SEQUENCE_ADD(&bsm->partII->list, supplementalVehicleExt)) {
        free(supplementalVehicleExt);
        return NULL;
    }

    return &supplementalVehicleExt->partII_Value.choice.SupplementalVehicleExtensions;
}


/**
 * Function to set the basic vehicle type for a given BSM
 * Accepts the bsm and vehicle type and returns the success or error code accordingly
 * Creates the PartII if no PartII exists on the BSM
 */
libsm_rval_e libsm_set_basic_vehicle_class(BasicSafetyMessage_t* bsm,
                                           BasicVehicleClass_t vehicleType)
{
    libsm_rval_e ret = libsm_bsm_alloc_partII_if_not_exist(bsm);

    if (ret != LIBSM_OK) {
        return ret;
    }

    SupplementalVehicleExtensions_t* sve = libsm_get_partII_elem(
            bsm,
            BSMpartIIExtension__partII_Value_PR_SupplementalVehicleExtensions);

    if (sve == NULL) {
        sve = libsm_add_bsm_SupplementalVehicleExtension(bsm);
        if (sve == NULL) {
            return LIBSM_ALLOC_ERR;
        }
    }

    *sve->classification = vehicleType;
    return LIBSM_OK;
}


/**
 * Function that returns a path prediction object for a given BSM
 * Default returns null if type not found
 */
PathPrediction_t* libsm_get_path_prediction(BasicSafetyMessage_t* bsm)
{
    if (bsm && bsm->partII) {
        for (int i = 0; i < bsm->partII->list.count; i++) {
            BSMpartIIExtension_t* partIIExt = bsm->partII->list.array[i];
            if (partIIExt->partII_Value.present
                == BSMpartIIExtension__partII_Value_PR_VehicleSafetyExtensions) {
                if (partIIExt->partII_Value.choice.VehicleSafetyExtensions.pathPrediction) {
                    return partIIExt->partII_Value.choice.VehicleSafetyExtensions.pathPrediction;
                }
            }
        }
    }
    //return null if no partII or no VehicleSafetyExtensions
    return NULL;
}


//returns index of vehicle safety extension in part II
VehicleSafetyExtensions_t* libsm_add_bsm_VehicleSafetyExtension(BasicSafetyMessage_t* bsm)
{
    // It doesn't exist, allocate the VehicleSafetyExtensions
    BSMpartIIExtension_t* vehicleSafetyExt = calloc(1, sizeof(BSMpartIIExtension_t));

    if (vehicleSafetyExt == NULL) {
        return NULL;
    }

    vehicleSafetyExt->partII_Id = PartII_Id_vehicleSafetyExt;
    vehicleSafetyExt->partII_Value.present
            = BSMpartIIExtension__partII_Value_PR_VehicleSafetyExtensions;

    if (ASN_SEQUENCE_ADD(&bsm->partII->list, vehicleSafetyExt)) {
        free(vehicleSafetyExt);
        return NULL;
    }

    // Confirm we have a Part II VehicleSafetyExtensions; J2735 specifies only ONE VehicleSafetyExtensions.
    return libsm_get_partII_elem(bsm,
                                 BSMpartIIExtension__partII_Value_PR_VehicleSafetyExtensions);
}


/**
 * Function to set path prediction for a BSM
 * Accepts bsm and the path prediction information
 * If needed, will allocate the Part II VehicleSafetyExtensions,
 * and the pathPrediction data frame.
 */
libsm_rval_e libsm_set_path_prediction(BasicSafetyMessage_t* bsm,
                                       Confidence_t confidence,
                                       RadiusOfCurvature_t radius)
{
    //allocate part II if it doesn't exist
    int err = libsm_bsm_alloc_partII_if_not_exist(bsm);

    if (err != 0) {
        return LIBSM_ALLOC_ERR;
    }

    // get vehicle safety extensions out of part II if it exists
    VehicleSafetyExtensions_t* vse = libsm_get_partII_elem(
            bsm,
            BSMpartIIExtension__partII_Value_PR_VehicleSafetyExtensions);

    if (vse == NULL) {
        //bsm part II didn't have vehicle safety extensions, add it
        vse = libsm_add_bsm_VehicleSafetyExtension(bsm);
        if (vse == NULL) {
            return LIBSM_ALLOC_ERR;
        }
    }

    // Check if pathPrediction has been allocated, if not, allocate it.
    if (vse->pathPrediction == NULL) {
        vse->pathPrediction = calloc(1, sizeof(PathPrediction_t));
        if (vse->pathPrediction == NULL) {
            return LIBSM_ALLOC_ERR;
        }
    }

    vse->pathPrediction->confidence = confidence;
    vse->pathPrediction->radiusOfCurve = radius;

    return LIBSM_OK;
}


/**
 * Allocates a PathHistory and initializes
 * the elements with "unavailable".
 * Returns a pointer to the allocated and initialized
 * PathHistory, else returns NULL.
 */
PathHistory_t* libsm_alloc_init_path_history(void)
{
    PathHistory_t* pathHistory = calloc(1, sizeof(PathHistory_t));

    if (pathHistory == NULL) {
        return NULL;
    }

    // currGNSSstatus is OPTIONAL and is set to NULL
    pathHistory->currGNSSstatus = NULL;

    //ensure pathHistory->initialPosition allocated and initialized
    if (pathHistory->initialPosition == NULL) {
        // Create and init the initialPosition dataframe within pathHistory:
        pathHistory->initialPosition = libsm_alloc_init_FullPositionVector();
        if (pathHistory->initialPosition == NULL) {
            return NULL;
        }
    }

    // If no PathHistoryPoints are within the PathHistoryPointList ("crumbData"),
    // create one PathHistoryPoint and add it to crumbData.
    if (pathHistory->crumbData.list.count == 0) {
        // Create a PathHistoryPointList (crumbData)
        PathHistoryPoint_t* pathHistoryPoint = libsm_alloc_init_PathHistoryPoint();
        if (pathHistoryPoint == NULL) {
            return NULL;
        }

        if (ASN_SEQUENCE_ADD(&pathHistory->crumbData.list, pathHistoryPoint)) {
            return NULL;
        }
    }
    return pathHistory;
}


/**
 * Given an allocated PSM, allocate a pathHistory dataframe and initialize it
 * if pathHistory does not already exist within the PSM.
 * Returns LIBSM_OK if the PSM already has a pathHistory, or if the insertion
 * of a new pathHistory into the PSM is successful.
 */
libsm_rval_e libsm_init_psm_path_history(PersonalSafetyMessage_t* psm)
{
    if (psm == NULL) {
        return LIBSM_FAIL_NO_ARGUMENT;
    }
    psm->pathHistory = libsm_alloc_init_path_history();
    if (psm->pathHistory == NULL) {
        return LIBSM_ALLOC_ERR;
    }
    return LIBSM_OK;
}


/**
 * Given an allocated BSM, allocate a pathHistory dataframe and initialize it
 * if pathHistory does not already exist within the BSM.
 * Because a BSM's pathHistory exists within BSM Part II->VehicleSafetyExtensions,
 * these additional dataframes are allocated and initialized if they do not already
 * exist.
 * Returns LIBSM_OK if the BSM already has a pathHistory, or if the insertion
 * of a new pathHistory into the BSM is successful.
 */
libsm_rval_e libsm_init_bsm_path_history(BasicSafetyMessage_t* bsm)
{
    if (bsm == NULL) {
        return LIBSM_FAIL_NO_ARGUMENT;
    }
    //allocate part II if it doesn't exist
    int err = libsm_bsm_alloc_partII_if_not_exist(bsm);

    if (err != 0) {
        return LIBSM_ALLOC_ERR;
    }

    // get vehicle safety extensions out of part II if it exists
    VehicleSafetyExtensions_t* vse = libsm_get_partII_elem(
            bsm,
            BSMpartIIExtension__partII_Value_PR_VehicleSafetyExtensions);

    if (vse == NULL) {
        //bsm part II didn't have vehicle safety extensions, add it
        vse = libsm_add_bsm_VehicleSafetyExtension(bsm);
        if (vse == NULL) {
            return LIBSM_ALLOC_ERR;
        }
    }

    vse->pathHistory = libsm_alloc_init_path_history();
    if (vse->pathHistory == NULL) {
        return LIBSM_ALLOC_ERR;
    }
    return LIBSM_OK;
}


/**
 * Allocates a DDateTime dataframe and initializes
 * the elements with "unavailable".
 * Returns a pointer to the allocated and initialized
 * DDateTime dataframe, else returns NULL.
 */
DDateTime_t* libsm_alloc_init_DDateTime(void)
{
    DDateTime_t* utcTime = calloc(1, sizeof(DDateTime_t));

    if (utcTime == NULL) {
        return NULL;
    }

    // check these all allocated
    utcTime->year = calloc(1, sizeof(DYear_t));
    utcTime->month = calloc(1, sizeof(DMonth_t));
    utcTime->day = calloc(1, sizeof(DDay_t));
    utcTime->hour = calloc(1, sizeof(DHour_t));
    utcTime->minute = calloc(1, sizeof(DMinute_t));
    utcTime->second = calloc(1, sizeof(DSecond_t));
    utcTime->offset = calloc(1, sizeof(DOffset_t));


    *utcTime->year = DYear_unavailable;
    *utcTime->month = DMonth_unavailable;
    *utcTime->day = DDay_unavailable;
    *utcTime->hour = DHour_unavailable;
    *utcTime->minute = DMinute_unavailable;
    *utcTime->second = DSecond_unavailable;
    *utcTime->offset = DOffset_unavailable;
    return utcTime;
}


/**
 * Allocates a FullPositionVector dataframe and initializes
 * required elements with "unavailable", and OPTIONAL elements with NULL.
 * Returns a pointer to the allocated and initialized
 * FullPositionVector dataframe, else returns NULL.
 */
FullPositionVector_t* libsm_alloc_init_FullPositionVector(void)
{
    FullPositionVector_t* fullPositionVector = calloc(1, sizeof(FullPositionVector_t));

    if (fullPositionVector == NULL) {
        return NULL;
    }

    fullPositionVector->utcTime = NULL;
    fullPositionVector->Long = Longitude_unavailable;
    fullPositionVector->lat = Latitude_unavailable;
    fullPositionVector->elevation = NULL;
    fullPositionVector->heading = NULL;
    fullPositionVector->speed = NULL;
    fullPositionVector->posAccuracy = NULL;
    fullPositionVector->timeConfidence = NULL;
    fullPositionVector->posConfidence = NULL;
    fullPositionVector->speedConfidence = NULL;
    return fullPositionVector;
}


/**
 * Allocates a PathHistoryPoint dataframe and initializes
 * required elements with "unavailable", and set OPTIONAL elements to NULL.
 * Returns a pointer to the allocated and initialized
 * PathHistoryPoint dataframe, else returns NULL.
 */
PathHistoryPoint_t* libsm_alloc_init_PathHistoryPoint()
{
    PathHistoryPoint_t* pathHistoryPoint = calloc(1, sizeof(PathHistoryPoint_t));

    if (pathHistoryPoint == NULL) {
        return NULL;
    }
    pathHistoryPoint->latOffset = OffsetLL_B18_unavailable;
    pathHistoryPoint->lonOffset = OffsetLL_B18_unavailable;
    pathHistoryPoint->elevationOffset = VertOffset_B12_unavailable;
    pathHistoryPoint->timeOffset = TimeOffset_unavailable;
    pathHistoryPoint->speed = NULL;
    pathHistoryPoint->posAccuracy = NULL;
    pathHistoryPoint->heading = NULL;
    return pathHistoryPoint;
}


/**
 * Allocates a PathPrediction dataframe and initializes
 * the elements with "unavailable".
 * Returns a pointer to the allocated and initialized
 * PathPrediction dataframe, else returns NULL.
 */
PathPrediction_t* libsm_alloc_init_PathPrediction(void)
{
    PathPrediction_t* pathPrediction = calloc(1, sizeof(PathPrediction_t));

    if (pathPrediction == NULL) {
        return NULL;
    }

    pathPrediction->radiusOfCurve = RadiusOfCurvature_straight;
    pathPrediction->confidence = 0;
    return pathPrediction;
}


/**
 * Allocates a AccelerationSet4Way dataframe and initializes
 * the elements with "unavailable".
 * Returns a pointer to the allocated and initialized
 * AccelerationSet4Way dataframe, else returns NULL.
 */
AccelerationSet4Way_t* libsm_alloc_init_AccelerationSet4Way(void)
{
    AccelerationSet4Way_t* accelSet = calloc(1, sizeof(AccelerationSet4Way_t));

    if (accelSet == NULL) {
        return NULL;
    }

    accelSet->Long = Acceleration_unavailable;
    accelSet->lat = Acceleration_unavailable;
    accelSet->vert = VerticalAcceleration_unavailable;
    accelSet->yaw = 0; // no unavailable, so hopefully a YawRate of 0 is fine
    return accelSet;
}


MessageFrame_t* libsm_alloc_init_mf_psm(void)
{
    MessageFrame_t* mf = calloc(1, sizeof(MessageFrame_t));

    if (mf == NULL) {
        return NULL;
    }

    mf->messageId = DSRCmsgID_personalSafetyMessage;
    mf->value.present = MessageFrame__value_PR_PersonalSafetyMessage;

    libsm_rval_e ret = libsm_init_psm(&mf->value.choice.PersonalSafetyMessage);
    if (ret != LIBSM_OK) {
        ASN_STRUCT_FREE(asn_DEF_MessageFrame, mf);
        return NULL;
    }
    return mf;
}


MessageFrame_t* libsm_alloc_init_mf_bsm(void)
{
    MessageFrame_t* mf = calloc(1, sizeof(MessageFrame_t));

    if (mf == NULL) {
        return NULL;
    }

    mf->messageId = DSRCmsgID_basicSafetyMessage;
    mf->value.present = MessageFrame__value_PR_BasicSafetyMessage;

    libsm_rval_e ret = libsm_init_bsm(&mf->value.choice.BasicSafetyMessage);
    if (ret != LIBSM_OK) {
        ASN_STRUCT_FREE(asn_DEF_MessageFrame, mf);
        return NULL;
    }
    return mf;
}


bool libsm_mf_has_bsm(MessageFrame_t const* const mf)
{
    return mf->messageId == DSRCmsgID_basicSafetyMessage
           && mf->value.present == MessageFrame__value_PR_BasicSafetyMessage;
}


bool libsm_mf_has_psm(MessageFrame_t const* const mf)
{
    return mf->messageId == DSRCmsgID_personalSafetyMessage
           && mf->value.present == MessageFrame__value_PR_PersonalSafetyMessage;
}


BasicSafetyMessage_t* libsm_get_bsm(MessageFrame_t* const mf)
{
    if (libsm_mf_has_bsm(mf)) {
        return &mf->value.choice.BasicSafetyMessage;
    }
    return NULL;
}


PersonalSafetyMessage_t* libsm_get_psm(MessageFrame_t* const mf)
{
    if (libsm_mf_has_psm(mf)) {
        return &mf->value.choice.PersonalSafetyMessage;
    }
    return NULL;
}
