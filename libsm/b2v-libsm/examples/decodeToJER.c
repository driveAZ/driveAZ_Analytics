/*
 * decodeToJER.c
 * Example to decode a UPER MF from an encoded UPER array with JSON output
 */

#include "libsm.h"
#include <errno.h>
#include <getopt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
    MessageFrame_t* mf;
    libsm_rval_e libsm_ret;
    uint8_t* buf;
    size_t len = 0;
    int verbose = 0;
    int ret = 0;
    int opt;
    int option_index = 0;

    static struct option long_options[] = { { "help", no_argument, NULL, 'h' },
                                            { "input", required_argument, NULL, 'i' },
                                            { "verbose", no_argument, NULL, 'v' },
                                            { NULL, 0, NULL, 0 } };

    while ((opt = getopt_long(argc, argv, "hi:v", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'h':
                printf("This is an example to decode a UPER MF from an encoded UPER array.\n");
                printf("USAGE:  %s [options]\n", argv[0]);
                printf("Options:\n");
                printf("  -i, --input\tSpecify the encoded UPER MF hex string\n");
                printf("  -v, --verbose\tEnable verbose output\n");
                exit(0);

            case 'i':
                len = strlen(optarg) / 2;
                buf = calloc(len, sizeof(uint8_t));

                /* Convert input hex string to byte array */
                for (size_t i = 0; i < len; ++i) {
                    sscanf(&optarg[i * 2], "%2hhx", &buf[i]);
                }

                break;

            case 'v':
                verbose = 1;
                break;

            default:
                ret = -EINVAL;
        }
    }

    if (verbose) {
        printf("Decoding %d bytes from input hex string: ", (int)len);
    }

    if (len == 0) {
        fprintf(stderr,
                "No input provided. Use -i option to provide an encoded UPER MF hex string.\n");
        ret = -EINVAL;
    }

    if (0 == ret) {
        mf = (MessageFrame_t*)calloc(1, sizeof(MessageFrame_t));
        /* Allocate a fresh MessageFrame structure */
        if (!mf) {
            fprintf(stderr, "Failed to allocate memory for MessageFrame.\n");
            ret = -ENOMEM;
        }
    }


    /* Decode the hex array as a UPER-encoded MessageFrame */
    if (0 == ret) {
        libsm_ret = libsm_decode_messageframe(buf, len, mf);
        if (libsm_ret != LIBSM_OK) {
            fprintf(stderr, "Failed to decode UPER MF: '%s'\n", libsm_str_err(libsm_ret));
            free(mf);
            ret = libsm_ret;
        }
    } else if (verbose) {
        printf("Successfully decoded UPER MF. Now encoding to JSON...\n");
    }

    /* Now encode the internal representation to JSON (JER) */
    if (0 == ret) {
        asn_encode_to_new_buffer_result_t json_obj = asn_encode_to_new_buffer(
                NULL,    /* No special callback (use built-in) */
                ATS_JER, /* JSON Encoding Rules */
                &asn_DEF_MessageFrame,
                mf);
        if (json_obj.result.encoded < 0) {
            fprintf(stderr, "Failed to encode UPER MF to JSON.\n");
            ret = json_obj.result.encoded;
        } else if (json_obj.buffer != NULL) {
            /* Print the resulting JSON to stdout */
            fwrite(json_obj.buffer, 1, json_obj.result.encoded, stdout);
            printf("\n");
        }
        free(json_obj.buffer);
    }

    /* Clean up the MessageFrame structure */
    ASN_STRUCT_RESET(asn_DEF_MessageFrame, mf);
    free(mf);
    free(buf);

    return ret;
}
