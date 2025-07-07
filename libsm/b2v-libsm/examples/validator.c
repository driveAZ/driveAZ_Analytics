#include <ctype.h>
#include <getopt.h>
#include <stdint.h>
#include <string.h>

#include "libsm.h"

#define BUFFER_SIZE 512


static void usage(char* name)
{
    printf("Usage: %s [OPTION]... <UPER-encoded MessageFrame bytes>\n"
           "  <UPER-encoded MessageFrame bytes>   a string of hex bytes in ascii (no spaces).\n"
           "                                      For example, like this valid UPER-encoded MessageFrame:\n"
           "                                      00201A000004001415090909084EF7F79139BA8622FFFFFFFF005010E0\n"
           "Returns:\n"
           "  0 if MessageFrame is valid. Non-zero if MessageFrame is not valid.\n"
           "Available options:\n"
           "  -v, --verbose   increase debugging\n"
           "  -q, --quiet     decrease debugging; Don't print the decoded message\n"
           "  -h, --help      Show this help\n"
           "  -V, --version   Show version (It's %s)\n",
           name,
           VERSION_LIBSM);
}


static int parseArgs(int argc, char* argv[], uint8_t* msgf, size_t* len, int* debug)
{
    char buf[8];

    int opt;
    int option_index = 0;

    static struct option long_options[] = { { "help", no_argument, NULL, 'h' },
                                            { "verbose", no_argument, NULL, 'v' },
                                            { "version", no_argument, NULL, 'V' },
                                            { "quiet", no_argument, NULL, 'q' },
                                            { NULL, 0, NULL, 0 } };

    while ((opt = getopt_long(argc, argv, "hvVq", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'h':
                usage(argv[0]);
                exit(0);
            case 'v':
                (*debug)++;
                break;
            case 'V':
                printf("%s version %s\n", argv[0], VERSION_LIBSM);
                exit(0);
            case 'q':
                *debug = 0;
                break;
            default: /* '?' */
                usage(argv[0]);
                exit(2);
        }
    }

    if (optind >= argc) {
        usage(argv[0]);
        return LIBSM_FAIL_NULL_ARG;
    }

    // Collect the MessageFrame from the command line and covert into uint8_t.
    int msgLen = strlen(argv[optind]);

    if ((size_t)msgLen / 2 > *len) {
        fprintf(stderr,
                "your messageframe is too powerful, make the buffer bigger or paste less garbage\n");
        exit(3);
    }

    *len = 0;
    msgLen = strlen(argv[optind]);
    const char* msg = argv[optind];

    for (int i = 0; i < msgLen; i += 2) {
        sprintf(buf, "%c%c", msg[i], msg[i + 1]);
        uint8_t byte = strtol(buf, NULL, 16);
        msgf[(*len)++] = byte;
    }

    if (*debug > 1) {
        printf("\nThe byte array read:\n");
        for (int i = 0; i < (int)*len; ++i) {
            printf(" %02X", msgf[i]);
        }
        printf("\n");
    }

    return LIBSM_OK;
}


int main(int argc, char* argv[])
{
    uint8_t encoded[BUFFER_SIZE];
    size_t len = BUFFER_SIZE;
    libsm_rval_e ret;
    int debug = 1;

    MessageFrame_t* mf = calloc(1, sizeof(MessageFrame_t));

    ret = parseArgs(argc, argv, encoded, &len, &debug);
    if (ret != LIBSM_OK) {
        if (debug > 1) {
            printf("parse args: %s\n", libsm_str_err(ret));
        }
        ASN_STRUCT_FREE(asn_DEF_MessageFrame, mf);
        return ret;
    }

    // The decode function also validates the MessageFrame.
    ret = libsm_decode_messageframe(encoded, len, mf);
    if (ret != LIBSM_OK) {
        if (debug) {
            printf("decode: %s\n", libsm_str_err(ret));
        }
        ASN_STRUCT_FREE(asn_DEF_MessageFrame, mf);
        return 1;
    }

    if (debug) {
        printf("\nPASSED DECODING AND VALIDATION:\n");
        asn_fprint(stdout, &asn_DEF_MessageFrame, mf);
    }

    // REQUIRED: FREE THE STRUCT
    ASN_STRUCT_FREE(asn_DEF_MessageFrame, mf);

    return 0;
}
