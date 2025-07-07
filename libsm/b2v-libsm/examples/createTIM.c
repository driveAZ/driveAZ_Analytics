/**
 * Example that creates and encodes a Traveler Information Message
 *
 * The user can set some of the TIM parameters and the usecase
 * (back of queue, workzone, etc). Currently only BoQ is implemented.
 *
 * Where OPTIONAL TIM fields are not needed for the usecase, they
 * will be set to NULL.
 *
 * User inputs are validated immediately and the application exits with
 * the reason.
 *
 * Usecases:
 *  * Back of queue: The very last vehicle in a line of traffic that has
 *    built up due to an incident further up the road.
 */

#include <getopt.h>
#include <stdint.h>
#include <stdlib.h>

#include "libsm-TIM.h"
#include "libsm.h"

#define RANGE_CHECK_SIMPLE(MIN, MAX, VAL) (VAL >= MIN && VAL <= MAX)


static void print_help(char** argv)
{
    printf("This application creates a MessageFrame containing a\n"
           "Traveler Information Message (TIM). The TIM is configurable.\n\n");
    printf("USAGE:  %s [options]\n", argv[0]);
    printf("Options:\n");
    printf("  -m, --msgCnt    Set the message count (0-127, default: 0)\n");
    printf("  -s  --start     Set the start time of the TIM message (0-527040, minute of the year (UTC), default: 0)\n");
    printf("  -d  --duration  Set the duration that the TIM message is valid (0-32000 minutes, default: 0)\n");
    printf("  -n  --name      Set a human-friendly descriptive name for the TIM message (1-63 characters, default: NULL)\n");
    printf("  -l, --lat       Set the latitude position (default: %d 'unavailable')\n",
           Latitude_unavailable);
    printf("  -L, --Long      Set the longitude position (default: %d 'unavailable')\n",
           Longitude_unavailable);
    printf("  -u, --usecase   Set the use case for the TIM message:\n");
    printf("                      1 - Back of Queue (default)\n");
    printf("  -v, --verbose   Enable verbose output\n\n");
}


static void print_verbose(const MessageFrame_t* mf,
                          const uint8_t* encoded_buf,
                          const size_t* buf_len)
{
    if (mf && encoded_buf && buf_len) {
        // Display the MessageFrame contents
        asn_fprint(stdout, &asn_DEF_MessageFrame, mf);
        printf("encoded:\n");
        for (size_t i = 0; i < *buf_len; ++i) {
            printf(" %02X", encoded_buf[i]);
        }
        printf("\n\nEncoded TIM MessageFrame length: %zu\n", *buf_len);
    }
}


/**
 * @brief Fill content CHOICE advisory with settings for boq and add it to TravelerDataFrame
 *
 * The ITIS Text and codes are hardcoded here, based on best guess for back-of-queue
 *
 * @param[in][out] traveler_data_frame to received the content
 *
 * @retval LIBSM_OK Operation successful
 * @retval LIBSM_ALLOC_ERR Allocation error creating ITISCodesAndText.
 */
static libsm_rval_e set_partiii_content_boq(TravelerDataFrame_t* traveler_data_frame)
{
    // Create the required Part III content, by making a CHOICE of ITIScodesAndText ("advisory")
    ITIScodesAndText_t* advisory = libsm_alloc_init_ITIScodesAndText();
    if (advisory == NULL) {
        printf("Fail: ITIScodesAndText_t alloc-init\n");
        //ASN_STRUCT_FREE(asn_DEF_MessageFrame, mf);
        return LIBSM_ALLOC_ERR;
    }
    // J2735 allows between 1...100 ITIScodesAndText__Member
    // For back-of-queue
    ITIScodesAndText__Member* itis_codes_and_text__member[4] = { 0 };
    itis_codes_and_text__member[0] = libsm_alloc_init_ITIScodesAndText__Member_itis(
            ITIScodes_stopped_traffic);
    itis_codes_and_text__member[1] = libsm_alloc_init_ITIScodesAndText__Member_text("Due to:");
    itis_codes_and_text__member[2] = libsm_alloc_init_ITIScodesAndText__Member_itis(
            ITIScodes_long_queues);
    itis_codes_and_text__member[3] = libsm_alloc_init_ITIScodesAndText__Member_text(
            "Advice: Prepare to stop");
    for (int n = 0; n < ARRAY_SIZE(itis_codes_and_text__member); n++) {
        ASN_SEQUENCE_ADD(&advisory->list, &itis_codes_and_text__member[n]->item);
    }

    // Add advisory to the TravelerDataFrame
    traveler_data_frame->content.choice.advisory = advisory;
    traveler_data_frame->content.present = TravelerDataFrame__content_PR_advisory;

    return LIBSM_OK;
}


int main(int argc, char** argv)
{
    uint8_t encoded_mf_buf[254];

    libsm_rval_e rval;

    int opt;
    int option_index = 0;

    // TIM settable default values
    Common_MsgCount_t msgCnt = 0;
    MinuteOfTheYear_t startTime = MinuteOfTheYear_unavailable;
    MinutesDuration_t durationTime = 0;
    char* descriptive_name = NULL;
    Latitude_t lat = Latitude_unavailable;
    Longitude_t Long = Longitude_unavailable;

    int verbose = 0;

    enum usecase_types {
        usecase_boq = 1,  // Back of Queue
        usecase_workzone, // FUTURE
    };

    // Only usecase option 1 "BOQ" is supported (more in future)
    int usecase = usecase_boq;

    // msgID has a CHOICE of two options
    enum msgID_CHOICE {
        choice_furtherInfoID,
        choice_roadSignID,
    };

    // This selects which msgID CHOICE is used
    int choice_msgID = choice_furtherInfoID;

    static struct option long_options[] = {
        { "help", no_argument, NULL, 'h' },        { "msgCnt", required_argument, NULL, 'm' },
        { "start", required_argument, NULL, 's' }, { "duration", required_argument, NULL, 'd' },
        { "name", required_argument, NULL, 'n' },  { "lat", required_argument, NULL, 'l' },
        { "Long", required_argument, NULL, 'L' },  { "usecase", required_argument, NULL, 'u' },
        { "verbose", no_argument, NULL, 'v' },     { NULL, 0, NULL, 0 }
    };
    while ((opt = getopt_long(argc, argv, "hm:s:d:n:l:L:u:v", long_options, &option_index))
           != -1) {
        switch (opt) {
            case 'h':
                print_help(argv);
                exit(0);
            case 'm': {
                msgCnt = atoi(optarg);
                if (!RANGE_CHECK_SIMPLE(MsgCount_min, MsgCount_max, msgCnt)) {
                    printf("FAIL INPUT msgCount %ld\n", msgCnt);
                    return LIBSM_FAIL;
                }
                break;
            }
            case 's': {
                startTime = atoi(optarg);
                if (!RANGE_CHECK(MinuteOfTheYear, startTime)) {
                    printf("FAIL INPUT startTime %ld\n", startTime);
                    return LIBSM_FAIL;
                }
                break;
            }
            case 'd': {
                durationTime = atoi(optarg);
                if (!RANGE_CHECK_SIMPLE(0, 32000, durationTime)) {
                    printf("FAIL INPUT durationTime %ld\n", durationTime);
                    return LIBSM_FAIL;
                }
                break;
            }
            case 'n': {
                size_t tmp = strlen(optarg);
                if (!RANGE_CHECK_SIMPLE(1, 63, tmp)) {
                    printf("FAIL INPUT descriptive_name '%s' len: %ld\n", optarg, tmp);
                    return LIBSM_FAIL;
                }
                descriptive_name = optarg;
                break;
            }
            case 'l': {
                lat = atoi(optarg);
                if (!RANGE_CHECK(Latitude, lat)) {
                    printf("FAIL INPUT latitude %ld\n", lat);
                    return LIBSM_FAIL;
                }
                break;
            }
            case 'L': {
                Long = atoi(optarg);
                if (!RANGE_CHECK(Longitude, Long)) {
                    printf("FAIL INPUT latitude %ld\n", Long);
                    return LIBSM_FAIL;
                }
                break;
            }
            case 'u': {
                usecase = atoi(optarg);
                // Only BoQ is supported
                if (usecase != usecase_boq) {
                    printf("FAIL INPUT usecase %d not supported\n", usecase);
                    return LIBSM_FAIL;
                }
                break;
            }
            case 'v':
                verbose = 1;
                break;
            default: /* '?' */
                print_help(argv);
                exit(LIBSM_FAIL);
        }
    }

    MessageFrame_t* mf = libsm_alloc_init_partial_mf_tim();
    if (mf == NULL) {
        printf("Failed to alloc/init a MessageFrame\n");
        ASN_STRUCT_FREE(asn_DEF_MessageFrame, mf);
        return LIBSM_ALLOC_ERR;
    }

    TravelerInformation_t* tim = libsm_get_tim(mf);
    if (tim == NULL) {
        printf("Failed to allocate a TIM within a MessageFrame");
        return LIBSM_ALLOC_ERR;
    }

    tim->msgCnt = msgCnt;
    tim->timeStamp = NULL;
    tim->packetID = NULL;
    tim->urlB = NULL;
    tim->regional = NULL;

    // Create a TravelerDataFrame and add the required CHOICEs
    TravelerDataFrame_t* traveler_data_frame = libsm_alloc_init_partial_TravelerDataFrame();
    if (traveler_data_frame == NULL) {
        ASN_STRUCT_FREE(asn_DEF_MessageFrame, mf);
        return LIBSM_FAIL;
    }
    // Likely that BoQ and workZone will be an advisory
    traveler_data_frame->frameType = TravelerInfoType_advisory;

    traveler_data_frame->startTime = startTime;
    traveler_data_frame->durationTime = durationTime;
    traveler_data_frame->priority = 0;

    // CHOICE either FurtherInfoID or RoadSignID
    if (choice_msgID == choice_furtherInfoID) {
        // CHOICE FurtherInfoID
        rval = libsm_init_TravelerDataFrame_msgId_choice_FurtherInfoID(traveler_data_frame);
        if (rval != LIBSM_OK) {
            printf("Failed FurtherInfoID\n");
            ASN_STRUCT_FREE(asn_DEF_MessageFrame, mf);
            return rval;
        }

        // This data element provides a link number to other messages
        // Use zero when unknown or not present.
        traveler_data_frame->msgId.choice.furtherInfoID.buf[0] = 0x00;
        traveler_data_frame->msgId.choice.furtherInfoID.buf[1] = 0x00;
    } else if (choice_msgID == choice_roadSignID) {
        // CHOICE roadSignID
        rval = libsm_init_TravelerDataFrame_msgId_choice_RoadSignID(traveler_data_frame);
        if (rval != LIBSM_OK) {
            printf("Failed RoadSignID\n");
            ASN_STRUCT_FREE(asn_DEF_MessageFrame, mf);
            return rval;
        }
    } else {
        // Ensure valid choices
        printf("Failed CHOICE of msgID\n");
        ASN_STRUCT_FREE(asn_DEF_MessageFrame, mf);
        return LIBSM_FAIL;
    }

    // Create a GeographicalPath and populate with user input (or defaults)
    GeographicalPath_t* geographical_path = libsm_alloc_init_GeographicalPath();

    if (descriptive_name != NULL) {
        geographical_path->name = libsm_alloc_init_DescriptiveName(descriptive_name);
    } else {
        geographical_path->name = NULL;
    }
    geographical_path->id = NULL;
    geographical_path->anchor = libsm_alloc_init_Position3D();
    geographical_path->anchor->lat = lat;
    geographical_path->anchor->Long = Long;
    geographical_path->laneWidth = NULL;
    geographical_path->directionality = NULL;
    geographical_path->closedPath = NULL;
    geographical_path->direction = NULL;
    geographical_path->description = NULL;
    // When the configuration of a GeographicalPath is complete,
    // add it to the TravelerDataFrame
    // Up to 16 different GeographicalPaths may be created and added, per J2735 spec
    ASN_SEQUENCE_ADD(&traveler_data_frame->regions.list, geographical_path);

    // Create the required Part III content, currently only BoQ is implemented
    if (usecase == usecase_boq) {
        rval = set_partiii_content_boq(traveler_data_frame);
        if (rval != LIBSM_OK) {
            ASN_STRUCT_FREE(asn_DEF_MessageFrame, mf);
            return rval;
        }
    }

    char errbuf[512];
    size_t errlen = ARRAY_SIZE(errbuf);
    // Check constraints of TravelerDataFrame
    int err = asn_check_constraints(&asn_DEF_TravelerDataFrame,
                                    traveler_data_frame,
                                    errbuf,
                                    &errlen);
    if (err != 0) {
        printf("asn_check_constraints error on TravelerDataFrame: '%s'\n", errbuf);
        ASN_STRUCT_FREE(asn_DEF_MessageFrame, mf);
        return LIBSM_FAIL_CONSTRAINT;
    }

    // The configured TravelerDataFrame is valid, add it to the TIM
    ASN_SEQUENCE_ADD(&tim->dataFrames.list, traveler_data_frame);

    // re-set errlen as it was modified
    errlen = ARRAY_SIZE(errbuf);
    // Check constraints of MessageFrame
    err = asn_check_constraints(&asn_DEF_MessageFrame, mf, errbuf, &errlen);
    if (err != 0) {
        printf("asn_check_constraints error: '%s'", errbuf);
        ASN_STRUCT_FREE(asn_DEF_MessageFrame, mf);
        return LIBSM_FAIL_CONSTRAINT;
    }

    // UPER encode the MessageFrame
    size_t len = ARRAY_SIZE(encoded_mf_buf);
    rval = libsm_encode_messageframe(mf, encoded_mf_buf, &len);
    if (rval != LIBSM_OK) {
        printf("FAILED encoding a TIM: '%s'.\n", libsm_str_err(rval));
        ASN_STRUCT_FREE(asn_DEF_MessageFrame, mf);
        return rval;
    }

    if (verbose) {
        print_verbose(mf, &encoded_mf_buf[0], &len);
    } else {
        // Output the encoded bytes of the TIM
        for (size_t i = 0; i < len; ++i) {
            printf("%02X", encoded_mf_buf[i]);
        }
    }

    // Free the MessageFrame
    ASN_STRUCT_FREE(asn_DEF_MessageFrame, mf);
    printf("\n\n");
    return 0;
}
