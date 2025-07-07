// https://cpputest.github.io/manual.html

#include "CppUTest/TestHarness_c.h"
#include "libsm.h"


TEST_C(bitstring, valid)
{
    libsm_rval_e ret;
    uint8_t buf[128];
    size_t len = (sizeof(buf) / sizeof(*buf));

    BasicSafetyMessage_t* bsm = calloc(1, sizeof(BasicSafetyMessage_t));

    ret = libsm_init_bsm(bsm);
    if (ret != LIBSM_OK) {
        FAIL_TEXT_C("Can not test encoding when bsm fails to initialize.");
    }

    // bitstring bit zero is leftmost bit in a byte
    bsm->coreData.brakes.wheelBrakes.buf[0] = 0x80;

    ret = libsm_encode_messageframe_bsm(bsm, buf, &len);
    ASN_STRUCT_FREE(asn_DEF_BasicSafetyMessage, bsm);

    if (ret != LIBSM_OK) {
        printf("FAILED encoding a BSM: '%s'.\n", libsm_str_err(ret));
        FAIL_C();
    }
}

TEST_C(bitstring, invalid)
{
    libsm_rval_e ret;
    uint8_t buf[128];
    size_t len = (sizeof(buf) / sizeof(*buf));

    BasicSafetyMessage_t* bsm = calloc(1, sizeof(BasicSafetyMessage_t));

    ret = libsm_init_bsm(bsm);
    if (ret != LIBSM_OK) {
        FAIL_TEXT_C("Can not test encoding when bsm fails to initialize.");
    }

    // smallest bit is "bit 7" in a bit string
    // This bitstring only has 5 things so this is out of range and does not work
    bsm->coreData.brakes.wheelBrakes.buf[0] = 0x01;

    ret = libsm_encode_messageframe_bsm(bsm, buf, &len);
    ASN_STRUCT_FREE(asn_DEF_BasicSafetyMessage, bsm);

    //should fail
    if (ret == LIBSM_OK) {
        FAIL_TEXT_C("Encoded invalid bitstring");
    }
}


TEST_C(bitstring, bitstringBig)
{
    BIT_STRING_t* bit_string = libsm_alloc_init_bit_string(27);

    if (bit_string == NULL) {
        FAIL_TEXT_C("failed to alloc init bit string");
    }

    for (size_t i = 0; i < 4; ++i) {
        CHECK_EQUAL_C_INT_TEXT(0x0, bit_string->buf[i], "bit string value");
    }
    CHECK_EQUAL_C_INT_TEXT(4, bit_string->size, "bit string size");
    CHECK_EQUAL_C_INT_TEXT(5, bit_string->bits_unused, "bit string bits unused");

    ASN_STRUCT_FREE(asn_DEF_BIT_STRING, bit_string);
}


void testMultiSize(size_t bits, size_t size, size_t bits_unused)
{
    BIT_STRING_t* bit_string = libsm_alloc_init_bit_string(bits);

    CHECK_EQUAL_C_INT_TEXT(size, bit_string->size, "bit string size");
    CHECK_EQUAL_C_INT_TEXT(bits_unused, bit_string->bits_unused, "bit string bits unused");
    ASN_STRUCT_FREE(asn_DEF_BIT_STRING, bit_string);
}


TEST_C(bitstring, multiSize)
{
    testMultiSize(27, 4, 5);
    testMultiSize(14, 2, 2);
    testMultiSize(15, 2, 1);
    testMultiSize(16, 2, 0);
    testMultiSize(17, 3, 7);
}
