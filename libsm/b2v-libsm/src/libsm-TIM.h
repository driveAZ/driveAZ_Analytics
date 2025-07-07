/**
 * Public functions and variables for Traveler Information Messages (TIM)
 */

#ifndef B2V_LIBSM_LIBSM_TIM_H
#define B2V_LIBSM_LIBSM_TIM_H

// this must be included first because both j2735 and j2540 define ITIScodes.h
// and _ITIScodes_H_, so we need to ensure we get the j2540 one
#include "j2540/ITIScodes.h"

#include "libsm.h"


/**
 * @brief alloc-init a NodeLL data frame with CHOICE NodeOffsetPointLL
 *
 * The caller is responsible for ensuring latitude and longitude are within limits
 * for the chosen NodeOffsetPointLL data frame
 *
 * @param node_offset_point_ll_choice The chosen NodeOffsetPointLL data frame
 * @param latitude The lat offset to load into the chosen NodeOffsetPointLL
 * @param longitude The Long offset to load into the chosen NodeOffsetPointLL
 *
 * @return Pointer to NodeLL_t, else NULL
 */
NodeLL_t* libsm_alloc_init_NodeLL(NodeOffsetPointLL_PR node_offset_point_ll_choice,
                                  int latitude,
                                  int longitude);


/**
 * @brief alloc-init-partial a NodeSetLL
 *
 * A NodeSetLL is the container for 2...63 NodeLL
 * objects that the user must provide
 *
 * @return Pointer to NodeSetLL_t, else NULL
 */
NodeSetLL_t* libsm_alloc_init_partial_NodeSetLL(void);


/**
 * @brief alloc-init-partial a NodeListLL
 *
 * NodeListLL is a choice with one option (in 2020-07), NodeListLL_PR_nodes.
 * That option required a NodeSetLL that the user must provide
 *
 * @return Pointer to NodeListLL_t, else NULL
 */
NodeListLL_t* libsm_alloc_init_partial_NodeListLL(void);


/**
 * @brief init geographical path's description.choice.path
 *
 * The initialized OffsetSystem has neither CHOICE NodeListXY nor NodeListLL configured
 *
 * @param geographical_path The GeographicalPath to receive this configuration
 *
 * @retval LIBSM_OK  Successful Operation
 * @retval LIBSM_FAIL_NULL_ARG  GeographicalPath was NULL
 * @retval LIBSM_ALLOC_ERR Allocation error
 */
libsm_rval_e libsm_init_partial_GeographicalPath_description_choice_path(
        GeographicalPath_t* geographical_path);


/**
 * @brief alloc-init GeographicalPath__description
 *
 * The caller must alloc and init the CHOICE for this element and set .present accordingly
 *
 * This is a required field for GeographicalPath
 *
 * @return Pointer to struct GeographicalPath__description, else NULL
 */
struct GeographicalPath__description* libsm_alloc_init_partial_GeographicalPath__description(void);


/**
 * @brief alloc-init-partial OffsetSystem
 *
 * This is a container for a OffsetSystem__offset for
 * the user to provide
 *
 * @return A pointer to OffsetSystem_t*, else NULL
 */
OffsetSystem_t* libsm_alloc_init_partial_OffsetSystem(void);

/**
 * @brief alloc-init a HeadingSlice
 *
 * @return A pointer to a 16 bit BIT_STRING_t, else NULL
 */
HeadingSlice_t* libsm_alloc_init_HeadingSlice(void);


/**
 * @brief init an already allocated HeadingSlice
 *
 * If the heading_slice has not been initialized already,
 * then its heading_slice->buf is allocated.
 *
 * @param heading_slice The heading slice to initialize
 *
 * @return LIBSM_OK Operation Successful, else fail
 */
libsm_rval_e libsm_init_HeadingSlice(HeadingSlice_t* heading_slice);


/**
 * @brief alloc-init BOOLEAN
 *
 * defaults to false
 *
 * @return Pointer to BOOLEAN_t, else NULL
 */
BOOLEAN_t* libsm_alloc_init_BOOLEAN(void);


/**
 * @brief alloc-init DirectionOfUse
 *
 * Configured value is "unavailable"
 *
 * @return Pointer to DirectionOfUse_t, else NULL
 */
DirectionOfUse_t* libsm_alloc_init_DirectionOfUse(void);


/**
 * @brief alloc-init LaneWidth
 *
 * Configured LaneWidth value is 0
 *
 * @return Pointer to LaneWidth_t, else NULL
 */
LaneWidth_t* libsm_alloc_init_LaneWidth(void);


/**
 * @brief alloc-init Position3D
 *
 * Set to unavailable:
 *  lat
 *  Long
 *
 * OPTIONAL left uninitialized:
 *  elevation
 *  regional
 *
 * @return A pointer to Position3D_t, else NULL
 */
Position3D_t* libsm_alloc_init_Position3D(void);


/**
 * @brief alloc-init a RoadSegmentReferenceID
 *
 * @return Pointer to struct RoadSegmentReferenceID, else NULL
 */
struct RoadSegmentReferenceID* libsm_alloc_init_RoadSegmentReferenceID(void);


/**
 * @brief alloc-init a given string into a DescriptiveName_t
 *
 * @param descriptive_name The text to use
 *
 * @return A pointer to DescriptiveName_t, else NULL
 */
DescriptiveName_t* libsm_alloc_init_DescriptiveName(char* descriptive_name);


/**
 * @brief alloc-init a single GeographicalPath
 *
 * Used in TravelerDataFrame Part II, "Applicable Regions of Use"
 * All elements are "optional" and are set to NULL
 *
 * @return A pointer to a GeographicalPath struct, else NULL
 */
GeographicalPath_t* libsm_alloc_init_GeographicalPath(void);


/**
 * @brief alloc-init CommonElevation
 *
 * defaults to "Elevation_unavailable"
 *
 * @return Pointer to Common_Elevation_t, else NULL
 */
Common_Elevation_t* libsm_alloc_init_CommonElevation(void);


/**
 * @brief alloc-init a MUTCDCode
 *
 * The returned MUTCDCode is zero, "non-MUTCD information"
 *
 * @return Pointer to MUTCDCode, else NULL
 */
MUTCDCode_t* libsm_alloc_init_MUTCDCode(void);


/**
 * @brief alloc-init a ITIScodesAndText__Member for holding a numerical ITIScode
 *
 * @param item A numerical ITIScode from the J2540 standard
 *
 * @return A pointer to a configured ITIScodesAndText__Member, else NULL
 */
ITIScodesAndText__Member* libsm_alloc_init_ITIScodesAndText__Member_itis(ITIScodes_t item);


/**
 * @brief alloc-init a ITIScodesAndText__Member for holding text
 *
 * @param itis_text Simple text used with ITIS codes, len 1..500
 *
 * @return A pointer to a configured ITIScodesAndText__Member, else NULL
 */
ITIScodesAndText__Member* libsm_alloc_init_ITIScodesAndText__Member_text(char* itis_text);


/**
 * @brief alloc-init a ITIScodesAndText
 *
 * ITIScodesAndText is for holding between 1...100 ITIScodesAndText__Member
 *
 * @return ITIScodesAndText_t*
 */
ITIScodesAndText_t* libsm_alloc_init_ITIScodesAndText(void);


/**
 * @brief alloc-init CHOICE RoadSignID inside TravelerDataFrame
 *
 * This is a CHOICE for TravelerDataFrame.msgId
 * A TravelerDataFrame MUST have either RoadSignID or FurtherInfoID
 *
 * Set to unavailable:
 *  position.lat,
 *  position.Long,
 *  headingSlice
 *
 * OPTIONAL set to NULL:
 *  position.elevation,
 *  position.regional,
 *  mutcdCode,
 *  crc
 *
 * @param traveler_data_frame The data frame to receive the RoadSignID
 *
 * @retval LIBSM_OK  Operational successful
 * @retval LIBSM_ALLOC_ERR  Allocation Error
 */
libsm_rval_e libsm_init_TravelerDataFrame_msgId_choice_RoadSignID(
        TravelerDataFrame_t* traveler_data_frame);


/**
 * @brief Alloc-init required fields for CHOICE FurtherInfoID and insert into TravelerDataFrame
 *
 * The OCTET_STRING is initialized to zero, defined as "unknown or not present"
 *
 * This is a CHOICE for TravelerDataFrame.msgId
 * A TravelerDataFrame MUST have either RoadSignID or FurtherInfoID
 *
 * @param traveler_data_frame The data frame to receive the FurtherInfoID
 *
 * @retval LIBSM_OK  Operational successful
 * @retval LIBSM_ALLOC_ERR  Allocation Error
 */
libsm_rval_e libsm_init_TravelerDataFrame_msgId_choice_FurtherInfoID(
        TravelerDataFrame_t* traveler_data_frame);


/**
 * @brief Allocate and initialize (to "unavailable") a DYear
 *
 * This is an OPTIONAL in a TravelerDataFrame
 *
 * @return A pointer to DYear_t, else NULL
 */
DYear_t* libsm_alloc_init_DYear(void);


/**
 * @brief Allocate and initialize a URL_Short field
 *
 * This is an OPTIONAL in a TravelerDataFrame
 *
 * A valid internet style URI/URL in the form of a text string which
 * will be used as the final portion of a compound string which,
 * when combined with the URL-Base data element, will link
 * to the designated resource
 *
 * See J2735 for additional information
 *
 * @param url The URL text, SIZE(1..15)
 *
 * @return URL_Short_t*
 */
URL_Short_t* libsm_alloc_init_URL_Short(char* url);

/**
 * @brief Alloc-init a bare-bones TravelerDataFrame
 *
 * Required: Initialized to zero, "unavailable", or "unknown"
 *  frameType
 *  msgID (CHOICE) User must provide
 *  durationTime
 *  startTime
 *  priority
 *  regions    User must provide a minimum of 1
 *  content (CHOICE) User must provide
 *
 * OPTIONAL, set to NULL
 *  startYear
 *  url
 *
 * A TravelerDataFrame is added to a TIM using
 * ASN_SEQUENCE_ADD(&tim->dataFrames.list, traveler_data_frame)
 *
 * @return A pointer to TravelerDataFrame_t, else NULL
 */
TravelerDataFrame_t* libsm_alloc_init_partial_TravelerDataFrame(void);


/**
 * @brief Allocate and initialize a URL_Base field
 *
 * This is an OPTIONAL in a TIM
 *
 * A valid internet style URI/URL in the form of a text string which
 * will form the base of a compound string which, when combined with
 * the URL-short data element, will link to the designated resource.
 *
 * See J2735 for additional information
 *
 * @param url The URL text to use, SIZE(1..45)
 *
 * @return Pointer to URL_Base_t, else NULL
 */
URL_Base_t* libsm_alloc_init_URL_Base(char* url);


/**
 * @brief Allocate and initialize (to "invalid") a MinuteOfTheYear
 *
 * This is an OPTIONAL in a TIM
 *
 * @return Pointer to MinuteOfTheYear_t, else NULL
 */
MinuteOfTheYear_t* libsm_alloc_init_MinuteOfTheYear(void);


/**
 * @brief Return the TIM object
 *
 * @param mf A MessageFrame containing a TIM
 *
 * @return A pointer to TravelerInformation, else NULL
 */
TravelerInformation_t* libsm_get_tim(MessageFrame_t* const mf);


/**
 * @brief Allocate a MessageFrame and configure it to hold a TIM.
 *
 * The user must add dataFrames for it to pass constraints
 *
 * Required TIM fields:
 *  msgCount (initialized to zero),
 *  dataFrames (not initialized here)
 *
 * OPTIONAL fields set to NULL:
 *  timeStamp,
 *  packetID,
 *  urlB,
 *  regional
 *
 * @return Pointer to MessageFrame containing a TIM, else NULL
 */
MessageFrame_t* libsm_alloc_init_partial_mf_tim(void);


/**
 * @brief Check a MessageFrame contains a TIM
 *
 * @param mf The MessageFrame to check
 *
 * @retval true The MessageFrame contains a TIM
 * @retval false The MessageFrame does not contain a TIM, or is NULL
 */
bool libsm_mf_has_tim(MessageFrame_t const* const mf);

#endif // B2V_LIBSM_LIBSM_TIM_H
