/**
 * @brief Helper functions to ease the use of OCTET_STRING_t variables
 */


#ifndef OCTET_HELPERS_H
#define OCTET_HELPERS_H

#include "libsm.h"

#include <OCTET_STRING.h>

#include <stddef.h>


/**
 * @brief Initialize an already allocated OCTET_STRING_t, including
 * allocating octet_string->buf
 *
 * @param s Pointer to an allocated OCTET_STRING_t
 * @param length Desired length of octet_string->buf
 * @retval LIBSM_OK  Operation successful
 * @retval LIBSM_FAIL_NULL_ARG  A NULL OCTET_STRING_t was passed
 * @retval LIBSM_ALLOC_ERR  Unable to allocate space for octet_string->buf
 */
libsm_rval_e libsm_init_octet_string(OCTET_STRING_t* s, size_t length);


/**
 * @brief Allocate and initialize an OctetString of size length
 *
 * @param length Size of the OctetString
 *
 * @return Pointer to an OCTET_STRING_t, else NULL
 */
OCTET_STRING_t* libsm_alloc_init_octet_string(size_t length);


/**
 * @brief Allocate and init an OCTET_STRING_t containing a string
 *
 * @param str The string
 *
 * @return Pointer to an OCTET_STRING_t, else NULL
 */
OCTET_STRING_t* libsm_alloc_init_octet_string_from_str(char* str);


/**
 * @brief init an allocated OCTET_STRING with a string
 *
 * @param s The existing OCTET_STRING
 * @param str The string to convert into the OCTET_STRING
 *
 * @return LIBSM_OK Operation successful
 *
 */
libsm_rval_e libsm_init_octet_string_from_str(OCTET_STRING_t* s, char* str);

#endif // OCTET_HELPERS_H
