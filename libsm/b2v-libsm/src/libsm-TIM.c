/**
 * Functions to generate, validate, and parse elements in a
 * J2735 Traveler Information Message (TIM)
 *
 * Because of a TIM complexity, not every TIM element
 * specified in J2735 is implemented here
 *
 * References:
 * SAE J2735_202007.pdf (July 2020)
 * SAE J2540_2_202012.pdf (December 2020)
 */

#include <getopt.h>
#include <stdint.h>
#include <stdlib.h>

#include "libsm-TIM.h"

NodeLL_t* libsm_alloc_init_NodeLL(NodeOffsetPointLL_PR node_offset_point_ll_choice,
                                  int latitude,
                                  int longitude)
{
    NodeLL_t* node_ll = calloc(1, sizeof(NodeLL_t));
    if (node_ll == NULL) {
        return NULL;
    }

    // OPTIONAL
    node_ll->attributes = NULL;

    switch (node_offset_point_ll_choice) {
        case NodeOffsetPointLL_PR_node_LL1: {
            // lat/Long range is OffsetLL-B12: -2048..2047
            struct Node_LL_24B* node_LL_x = calloc(1, sizeof(struct Node_LL_24B));
            if (node_LL_x == NULL) {
                free(node_ll);
                return NULL;
            }
            node_LL_x->lat = latitude;
            node_LL_x->lon = longitude;
            node_ll->delta.choice.node_LL1 = node_LL_x;
        } break;
        case NodeOffsetPointLL_PR_node_LL2: {
            // lat/Long range is OffsetLL-B14 -8192..8191
            struct Node_LL_28B* node_LL_x = calloc(1, sizeof(struct Node_LL_28B));
            if (node_LL_x == NULL) {
                free(node_ll);
                return NULL;
            }
            node_LL_x->lat = latitude;
            node_LL_x->lon = longitude;
            node_ll->delta.choice.node_LL2 = node_LL_x;
        } break;
        case NodeOffsetPointLL_PR_node_LL3: {
            // lat/Long range is OffsetLL-B16 ::= INTEGER (-32768..32767)
            struct Node_LL_32B* node_LL_x = calloc(1, sizeof(struct Node_LL_32B));
            if (node_LL_x == NULL) {
                free(node_ll);
                return NULL;
            }
            node_LL_x->lat = latitude;
            node_LL_x->lon = longitude;
            node_ll->delta.choice.node_LL3 = node_LL_x;

        } break;
        case NodeOffsetPointLL_PR_node_LL4: {
            // lat/Long range is OffsetLL-B18 ::= INTEGER (-131072..131071)
            struct Node_LL_36B* node_LL_x = calloc(1, sizeof(struct Node_LL_36B));
            if (node_LL_x == NULL) {
                free(node_ll);
                return NULL;
            }
            node_LL_x->lat = latitude;
            node_LL_x->lon = longitude;
            node_ll->delta.choice.node_LL4 = node_LL_x;

        } break;
        case NodeOffsetPointLL_PR_node_LL5: {
            // lat/Long range is OffsetLL-B22 ::= INTEGER (-2097152..2097151)
            struct Node_LL_44B* node_LL_x = calloc(1, sizeof(struct Node_LL_44B));
            if (node_LL_x == NULL) {
                free(node_ll);
                return NULL;
            }
            node_LL_x->lat = latitude;
            node_LL_x->lon = longitude;
            node_ll->delta.choice.node_LL5 = node_LL_x;

        } break;
        case NodeOffsetPointLL_PR_node_LL6: {
            // lat/Long range is OffsetLL-B24 ::= INTEGER (-8388608..8388607)
            struct Node_LL_48B* node_LL_x = calloc(1, sizeof(struct Node_LL_48B));
            if (node_LL_x == NULL) {
                free(node_ll);
                return NULL;
            }
            node_LL_x->lat = latitude;
            node_LL_x->lon = longitude;
            node_ll->delta.choice.node_LL6 = node_LL_x;

        } break;
        case NodeOffsetPointLL_PR_node_LatLon: {
            // Longitude ::= INTEGER (-1799999999..1800000001)
            // Latitude ::= INTEGER (-900000000..900000001)
            struct Node_LLmD_64b* node_LL_x = calloc(1, sizeof(struct Node_LLmD_64b));
            if (node_LL_x == NULL) {
                free(node_ll);
                return NULL;
            }
            node_LL_x->lat = latitude;
            node_LL_x->lon = longitude;
            node_ll->delta.choice.node_LatLon = node_LL_x;
        } break;
        case NodeOffsetPointLL_PR_regional: {
            // struct Reg_NodeOffsetPointLL	*regional;
            printf("NodeOffsetPointLL_PR_regional not implemented\n");
            free(node_ll);
            return NULL;
        } break;
        case NodeOffsetPointLL_PR_NOTHING:
        default:
            node_ll->delta.present = NodeOffsetPointLL_PR_NOTHING;
            free(node_ll);
            return NULL;
    }

    node_ll->delta.present = node_offset_point_ll_choice;
    return node_ll;
}


NodeSetLL_t* libsm_alloc_init_partial_NodeSetLL(void)
{
    NodeSetLL_t* nodes = calloc(1, sizeof(NodeSetLL_t));
    if (nodes == NULL) {
        return NULL;
    }

    return nodes;
}


// nodes not valid as null, need to call alloc init node set ll
NodeListLL_t* libsm_alloc_init_partial_NodeListLL(void)
{
    NodeListLL_t* ll = calloc(1, sizeof(NodeListLL_t));
    if (ll == NULL) {
        return NULL;
    }
    // There is only one CHOICE
    ll->present = NodeListLL_PR_nodes;
    ll->choice.nodes = NULL;

    return ll;
}


struct GeographicalPath__description* libsm_alloc_init_partial_GeographicalPath__description(void)
{
    struct GeographicalPath__description* description
            = calloc(1, sizeof(struct GeographicalPath__description));
    if (description == NULL) {
        return NULL;
    }
    description->present = GeographicalPath__description_PR_NOTHING;

    return description;
}


OffsetSystem_t* libsm_alloc_init_partial_OffsetSystem(void)
{
    OffsetSystem_t* offset_system = calloc(1, sizeof(OffsetSystem_t));
    if (offset_system == NULL) {
        return NULL;
    }

    // OPTIONAL
    offset_system->scale = NULL;

    // Caller must set .present accordingly when OffsetSystem CHOICE is made
    offset_system->offset.present = OffsetSystem__offset_PR_NOTHING;
    return offset_system;
}


libsm_rval_e libsm_init_partial_GeographicalPath_description_choice_path(
        GeographicalPath_t* geographical_path)
{
    if (geographical_path == NULL) {
        return LIBSM_FAIL_NULL_ARG;
    }
    // if geographical_path->description, allocate and initialize it
    if (geographical_path->description == NULL) {
        geographical_path->description = libsm_alloc_init_partial_GeographicalPath__description();
        if (geographical_path->description == NULL) {
            return LIBSM_ALLOC_ERR;
        }
    }

    if (geographical_path->description->present != GeographicalPath__description_PR_NOTHING) {
        return LIBSM_FAIL_NO_VALID_PARAMETER;
    }

    OffsetSystem_t* path = libsm_alloc_init_partial_OffsetSystem();
    if (!path) {
        return LIBSM_ALLOC_ERR;
    }

    // Add this OffsetSystem to the GeographicalPath
    geographical_path->description->choice.path = path;
    geographical_path->description->present = GeographicalPath__description_PR_path;

    return LIBSM_OK;
}


HeadingSlice_t* libsm_alloc_init_HeadingSlice(void)
{
    BIT_STRING_t* heading_slice = libsm_alloc_init_bit_string(16);
    if (heading_slice == NULL) {
        return NULL;
    }

    libsm_rval_e rval = libsm_init_HeadingSlice(heading_slice);
    if (rval != LIBSM_OK) {
        free(heading_slice);
        return NULL;
    }
    return heading_slice;
}

libsm_rval_e libsm_init_HeadingSlice(HeadingSlice_t* heading_slice)
{
    if (heading_slice->buf == NULL) {
        libsm_rval_e rval = libsm_init_bit_string(heading_slice, 16);
        if (rval != LIBSM_OK) {
            return rval;
        }
    }
    // In a bit string, the leftmost bit is bit 0
    heading_slice->buf[0] = 0x00;
    heading_slice->buf[1] = 0x00;

    return LIBSM_OK;
}


BOOLEAN_t* libsm_alloc_init_BOOLEAN(void)
{
    BOOLEAN_t* a_boolean = calloc(1, sizeof(BOOLEAN_t));
    if (!a_boolean) {
        return NULL;
    }
    *a_boolean = false;
    return a_boolean;
}


DirectionOfUse_t* libsm_alloc_init_DirectionOfUse(void)
{
    DirectionOfUse_t* directionality = calloc(1, sizeof(DirectionOfUse_t));
    if (directionality == NULL) {
        return NULL;
    }
    *directionality = DirectionOfUse_unavailable;
    return directionality;
}


LaneWidth_t* libsm_alloc_init_LaneWidth(void)
{
    LaneWidth_t* lane_width_cm = calloc(1, sizeof(LaneWidth_t));
    if (!lane_width_cm) {
        return NULL;
    }
    *lane_width_cm = 0;
    return lane_width_cm;
}


Position3D_t* libsm_alloc_init_Position3D(void)
{
    Position3D_t* anchor = calloc(1, sizeof(Position3D_t));
    if (anchor == NULL) {
        return NULL;
    }
    // Lat and Long are required
    anchor->lat = Latitude_unavailable;
    anchor->Long = Longitude_unavailable;

    // OPTIONAL
    anchor->elevation = NULL;
    // OPTIONAL
    anchor->regional = NULL;

    return anchor;
}


struct RoadSegmentReferenceID* libsm_alloc_init_RoadSegmentReferenceID(void)
{
    struct RoadSegmentReferenceID* road_segment_reference_id
            = calloc(1, sizeof(struct RoadSegmentReferenceID));
    if (road_segment_reference_id == NULL) {
        return NULL;
    }
    road_segment_reference_id->region = NULL;
    road_segment_reference_id->id = 0L;

    return road_segment_reference_id;
}


DescriptiveName_t* libsm_alloc_init_DescriptiveName(char* descriptive_name)
{
    return (DescriptiveName_t*)libsm_alloc_init_octet_string_from_str(descriptive_name);
}


GeographicalPath_t* libsm_alloc_init_GeographicalPath(void)
{
    GeographicalPath_t* geographical_path = calloc(1, sizeof(GeographicalPath_t));
    if (!geographical_path) {
        return NULL;
    }

    geographical_path->name = NULL;
    geographical_path->id = NULL;
    geographical_path->anchor = NULL;
    geographical_path->laneWidth = NULL;
    geographical_path->directionality = NULL;
    geographical_path->closedPath = NULL;
    geographical_path->direction = NULL;
    geographical_path->description = NULL;
    geographical_path->regional = NULL;

    return geographical_path;
}


// ---------------| TravelerDataFrame |-----------------


Common_Elevation_t* libsm_alloc_init_CommonElevation(void)
{
    Common_Elevation_t* elevation = calloc(1, sizeof(Common_Elevation_t));
    if (elevation == NULL) {
        return NULL;
    }
    *elevation = Elevation_unavailable;
    return elevation;
}


MUTCDCode_t* libsm_alloc_init_MUTCDCode(void)
{
    MUTCDCode_t* mutcd_code = calloc(1, sizeof(MUTCDCode_t));
    if (mutcd_code == NULL) {
        return NULL;
    }
    *mutcd_code = 0; // "non-MUTCD information"
    return mutcd_code;
}


ITIScodesAndText__Member* libsm_alloc_init_ITIScodesAndText__Member_itis(ITIScodes_t item)
{
    ITIScodesAndText__Member* itis = calloc(1, sizeof(ITIScodesAndText__Member));
    if (itis == NULL) {
        return NULL;
    }
    itis->item.present = ITIScodesAndText__Member__item_PR_itis;
    itis->item.choice.itis = item;

    return itis;
}


ITIScodesAndText__Member* libsm_alloc_init_ITIScodesAndText__Member_text(char* itis_text)
{
    if (itis_text == NULL) {
        return NULL;
    }

    ITIScodesAndText__Member* text = calloc(1, sizeof(ITIScodesAndText__Member));
    if (text == NULL) {
        return NULL;
    }

    text->item.present = ITIScodesAndText__Member__item_PR_text;

    libsm_rval_e ret = libsm_init_octet_string_from_str(&text->item.choice.text, itis_text);
    if (ret) {
        free(text);
        return NULL;
    }
    return text;
}


ITIScodesAndText_t* libsm_alloc_init_ITIScodesAndText(void)
{
    ITIScodesAndText_t* itis_codes_and_text = calloc(1, sizeof(ITIScodesAndText_t));
    if (itis_codes_and_text == NULL) {
        return NULL;
    }

    return itis_codes_and_text;
}


libsm_rval_e libsm_init_TravelerDataFrame_msgId_choice_RoadSignID(
        TravelerDataFrame_t* traveler_data_frame)
{
    struct RoadSignID* road_sign_id = calloc(1, sizeof(struct RoadSignID));
    if (road_sign_id == NULL) {
        return LIBSM_ALLOC_ERR;
    }
    // position, required
    road_sign_id->position.lat = Latitude_unavailable;
    road_sign_id->position.Long = Longitude_unavailable;
    // elevation OPTIONAL
    road_sign_id->position.elevation = NULL;
    // regional OPTIONAL
    road_sign_id->position.regional = NULL;

    // viewAngle, required
    libsm_rval_e rval = libsm_init_HeadingSlice(&road_sign_id->viewAngle);
    if (rval != LIBSM_OK) {
        return rval;
    }

    // mutcdCode OPTIONAL
    road_sign_id->mutcdCode = NULL;

    // crc OPTIONAL
    road_sign_id->crc = NULL;

    // Set "msgId present" to the roadSignID CHOICE
    traveler_data_frame->msgId.present = TravelerDataFrame__msgId_PR_roadSignID;
    traveler_data_frame->msgId.choice.roadSignID = road_sign_id;

    return LIBSM_OK;
}


libsm_rval_e libsm_init_TravelerDataFrame_msgId_choice_FurtherInfoID(
        TravelerDataFrame_t* traveler_data_frame)
{
    traveler_data_frame->msgId.present = TravelerDataFrame__msgId_PR_furtherInfoID;
    libsm_rval_e ret = libsm_init_octet_string(&traveler_data_frame->msgId.choice.furtherInfoID,
                                               2);
    return ret;
}


DYear_t* libsm_alloc_init_DYear(void)
{
    DYear_t* d_year = calloc(1, sizeof(DYear_t));
    if (d_year == NULL) {
        return NULL;
    }
    *d_year = DYear_unavailable;
    return d_year;
}


TravelerDataFrame_t* libsm_alloc_init_partial_TravelerDataFrame(void)
{
    TravelerDataFrame_t* traveler_data_frame = calloc(1, sizeof(TravelerDataFrame_t));
    if (traveler_data_frame == NULL) {
        return NULL;
    }

    // Part I, Frame Header

    // Placeholder, to be set later
    traveler_data_frame->frameType = TravelerInfoType_unknown;

    // msgID CHOICE, to be added later
    traveler_data_frame->msgId.present = TravelerDataFrame__msgId_PR_NOTHING;

    // OPTIONAL
    traveler_data_frame->startYear = NULL;

    // Range of minutes 0..527040, Specified here as "unavailable"
    traveler_data_frame->startTime = MinuteOfTheYear_unavailable;

    // Duration is 0...32000 minutes
    traveler_data_frame->durationTime = 0;

    // The relative importance of the sign, on a scale
    // from 0 (least important) to 7 (most important).
    traveler_data_frame->priority = 0;

    // Part II, Applicable Regions of Use
    // regions: At least one GeographicalPath is required, to be added later

    // Part III, Content
    // content CHOICE to be added later
    traveler_data_frame->content.present = TravelerDataFrame__content_PR_NOTHING;

    // OPTIONAL
    traveler_data_frame->url = NULL;

    return traveler_data_frame;
}

// ----------------| TravelerInformationMessage (TIM) |--------------

MinuteOfTheYear_t* libsm_alloc_init_MinuteOfTheYear(void)
{
    MinuteOfTheYear_t* time_stamp = calloc(1, sizeof(MinuteOfTheYear_t));
    if (!time_stamp) {
        return NULL;
    }
    // Use J2735-DEFINES for "invalid"
    *time_stamp = MinuteOfTheYear_unavailable;
    return time_stamp;
}


TravelerInformation_t* libsm_get_tim(MessageFrame_t* const mf)
{
    if (libsm_mf_has_tim(mf)) {
        return &mf->value.choice.TravelerInformation;
    }
    return NULL;
}


bool libsm_mf_has_tim(MessageFrame_t const* const mf)
{
    if (mf == NULL) {
        return false;
    }
    return mf->messageId == DSRCmsgID_travelerInformation
           && mf->value.present == MessageFrame__value_PR_TravelerInformation;
}


MessageFrame_t* libsm_alloc_init_partial_mf_tim(void)
{
    MessageFrame_t* mf = calloc(1, sizeof(MessageFrame_t));
    if (mf == NULL) {
        return NULL;
    }
    mf->messageId = DSRCmsgID_travelerInformation;
    mf->value.present = MessageFrame__value_PR_TravelerInformation;
    TravelerInformation_t* tim = &mf->value.choice.TravelerInformation;

    tim->msgCnt = 0;

    // tim->dataFrames.list must be added before encoding

    // OPTIONAL
    tim->timeStamp = NULL;
    // OPTIONAL
    tim->packetID = NULL;
    // OPTIONAL
    tim->urlB = NULL;
    // OPTIONAL
    tim->regional = NULL;

    return mf;
}

// ---------------| URL functions |----------------

URL_Base_t* libsm_alloc_init_URL_Base(char* url)
{
    return (URL_Base_t*)libsm_alloc_init_octet_string_from_str(url);
}


URL_Short_t* libsm_alloc_init_URL_Short(char* url)
{
    return (URL_Short_t*)libsm_alloc_init_octet_string_from_str(url);
}
