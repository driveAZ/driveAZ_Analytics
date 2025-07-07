#include "libsm-error.h"


const char* libsm_str_err(libsm_rval_e err)
{
    static const char* errStrings[] = { "LIBSM_OK",
                                        "LIBSM_OK_CONSTRAINT",
                                        "LIBSM_FAIL",
                                        "LIBSM_ALLOC_ERR",
                                        "LIBSM_FAIL_CONSTRAINT",
                                        "LIBSM_FAIL_NULL_ARG",
                                        "LIBSM_FAIL_NO_VALID_PARAMETER",
                                        "LIBSM_FAIL_DECODING",
                                        "LIBSM_FAIL_DECODING_BUFF_SIZE",
                                        "LIBSM_FAIL_ENCODING",
                                        "LIBSM_FAIL_ENCODING_BUFF_SIZE",
                                        "LIBSM_FAIL_NO_ARGUMENT" };

    return errStrings[err];
}
