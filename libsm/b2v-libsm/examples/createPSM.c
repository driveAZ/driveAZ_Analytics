/*
 * createPSM.c
 * Example to create a PSM and encode it with a messageframe as a UPER array
 */

#include "libsm.h"
#include <getopt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    uint8_t buf[128];
    size_t len = ARRAY_SIZE(buf);
    libsm_rval_e ret;

    int opt;
    int option_index = 0;

    // PSM Defaults
    e_PersonalDeviceUserType basicType = PersonalDeviceUserType_aPEDESTRIAN;
    DSecond_t secMark = 0;
    Common_MsgCount_t msgCnt = 0;
    uint32_t message_id = 0;
    Latitude_t lat = Latitude_unavailable;
    Longitude_t Long = Longitude_unavailable;
    int verbose = 0;

    static struct option long_options[] = { { "help", no_argument, NULL, 'h' },
                                            { "basicType", required_argument, NULL, 'b' },
                                            { "secMark", required_argument, NULL, 's' },
                                            { "msgCnt", required_argument, NULL, 'm' },
                                            { "message_id", required_argument, NULL, 'i' },
                                            { "lat", required_argument, NULL, 'l' },
                                            { "Long", required_argument, NULL, 'L' },
                                            { "verbose", no_argument, NULL, 'v' },
                                            { NULL, 0, NULL, 0 } };

    while ((opt = getopt_long(argc, argv, "hb:s:m:i:l:L:v", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'h':
                printf("This is an example to create a PSM and encode it with a messageframe as UPER arrays.\n");
                printf("USAGE:  %s [options]\n", argv[0]);
                printf("Options:\n");
                printf("  -b, --basicType\tSet the basic type (default: 1 - pedestrian)\n");
                printf("  -s, --secMark\tSet the secMark value (0-65535, default: 0)\n");
                printf("  -m, --msgCnt\tSet the message count (0-127, default: 0)\n");
                printf("  -i, --message_id\tSet the message ID (32-bit integer)\n");
                printf("  -l, --lat\tSet the latitude position (default: 900000001)\n");
                printf("  -L, --Long\tSet the longitude position (default: 1800000001)\n");
                printf("  -v, --verbose\tEnable verbose output\n");
                exit(0);
            case 'b':
                basicType = atoi(optarg);
                break;
            case 's':
                secMark = atoi(optarg);
                break;
            case 'm':
                msgCnt = atoi(optarg);
                break;
            case 'i':
                message_id = (uint32_t)strtoul(optarg, NULL, 0);
                break;
            case 'l':
                lat = atoi(optarg);
                break;
            case 'L':
                Long = atoi(optarg);
                break;
            case 'v':
                verbose = 1;
                break;
            default:
                exit(2);
        }
    }

    MessageFrame_t* mf = calloc(1, sizeof(MessageFrame_t));
    mf->messageId = DSRCmsgID_personalSafetyMessage;
    mf->value.present = MessageFrame__value_PR_PersonalSafetyMessage;
    PersonalSafetyMessage_t* psm = &mf->value.choice.PersonalSafetyMessage;

    ret = libsm_init_psm(psm);
    if (ret != LIBSM_OK) {
        printf("FAILED making a PSM.\n");
        return 1;
    }

    psm->basicType = basicType;
    psm->secMark = secMark;
    psm->msgCnt = msgCnt;

    psm->id.buf[0] = message_id >> 24;
    psm->id.buf[1] = (message_id >> 16) & 0xFF;
    psm->id.buf[2] = (message_id >> 8) & 0xFF;
    psm->id.buf[3] = message_id & 0xFF;

    psm->position.lat = lat;
    psm->position.Long = Long;

    if (verbose) {
        asn_fprint(stdout, &asn_DEF_MessageFrame, mf);
    }

    ret = libsm_encode_messageframe(mf, buf, &len);
    ASN_STRUCT_RESET(asn_DEF_MessageFrame, mf);
    if (ret != LIBSM_OK) {
        printf("FAILED encoding a PSM: '%s'.\n", libsm_str_err(ret));
        return 1;
    }

    if (verbose) {
        printf("encoded:\n");
        for (size_t i = 0; i < len; ++i) {
            printf(" %02X", buf[i]);
        }
        printf("\n");
    } else {
        for (size_t i = 0; i < len; ++i) {
            printf("%02X", buf[i]);
        }
        printf("\n");
    }

    return 0;
}
