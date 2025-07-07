/**
 * @brief Helper functions to ease the use of OCTET_STRING_t variables
 */

#include "octet-helpers.h"

#include "libsm.h"

OCTET_STRING_t* libsm_alloc_init_octet_string(size_t length)
{
    OCTET_STRING_t* ret = calloc(1, sizeof(OCTET_STRING_t) + sizeof(uint8_t) * length);
    if (ret == NULL) {
        return NULL;
    }
    libsm_rval_e err = libsm_init_octet_string(ret, length);
    if (err != LIBSM_OK) {
        free(ret);
        return NULL;
    }
    return ret;
}


libsm_rval_e libsm_init_octet_string(OCTET_STRING_t* s, size_t length)
{
    if (s == NULL) {
        return LIBSM_FAIL_NULL_ARG;
    }
    s->buf = calloc(length, sizeof(uint8_t));
    if (s->buf == NULL) {
        return LIBSM_ALLOC_ERR;
    }
    s->size = length;
    return LIBSM_OK;
}


libsm_rval_e libsm_init_octet_string_from_str(OCTET_STRING_t* s, char* str)
{
    libsm_rval_e rval = libsm_init_octet_string(s, strlen(str));
    if (rval) {
        return rval;
    }
    memcpy(s->buf, str, strlen(str));
    return rval;
}


OCTET_STRING_t* libsm_alloc_init_octet_string_from_str(char* str)
{
    OCTET_STRING_t* octet_string = libsm_alloc_init_octet_string(strlen(str));
    if (octet_string == NULL) {
        return NULL;
    }

    memcpy(octet_string->buf, str, strlen(str));
    return octet_string;
}
