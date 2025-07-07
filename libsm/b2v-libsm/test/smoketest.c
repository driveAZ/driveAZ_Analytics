// https://cpputest.github.io/manual.html
/*
 * smoketest.c
 * scattershot tests for libsm
 * Do not add tests here, start making new files
 * Documentation for how to write more tests is at https://cpputest.github.io/manual.html
 */

#include "CppUTest/TestHarness_c.h"
#include "libsm.h"

static uint8_t encoded_bsm_mf_valid_nopartII[]
        = { 0x00, 0x14, 0x25, 0x00, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF5, 0xA4, 0xE9, 0x00,
            0xEB, 0x49, 0xD2, 0x00, 0x00, 0x00, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x80,
            0xFD, 0xFA, 0x1F, 0xA1, 0x00, 0x7F, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00 };
static uint8_t encoded_bsm_mf_valid_noSupplementalVehicleExtensions[]
        = { 0x00, 0x14, 0x2B, 0x40, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF5, 0xA4,
            0xE9, 0x00, 0xEB, 0x49, 0xD2, 0x00, 0x00, 0x00, 0x7F, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xF0, 0x80, 0xFD, 0xFA, 0x1F, 0xA1, 0x00, 0x7F, 0xFF, 0x80,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x48, 0x00, 0x40, 0x20 };
static uint8_t encoded_bsm_mf_valid_noVehicleSafetyExtensions[] = {
    0x00, 0x14, 0x2A, 0x40, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF5, 0xA4, 0xE9, 0x00, 0xEB,
    0x49, 0xD2, 0x00, 0x00, 0x00, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x80, 0xFD, 0xFA,
    0x1F, 0xA1, 0x00, 0x7F, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x08, 0x0D, 0x00, 0x2A, 0x80
};
static uint8_t encoded_bsm_mf_valid[] = { 0x00, 0x14, 0x30, 0x40, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF,
                                          0xFF, 0xF5, 0xA4, 0xE9, 0x00, 0xEB, 0x49, 0xD2, 0x00,
                                          0x00, 0x00, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0,
                                          0x80, 0xFD, 0xFA, 0x1F, 0xA1, 0x00, 0x7F, 0xFF, 0x80,
                                          0x00, 0x00, 0x00, 0x01, 0x00, 0x10, 0x48, 0x00, 0x40,
                                          0x20, 0x20, 0x34, 0x00, 0xAA, 0x00 };
static uint8_t encoded_psm_mf_valid[] = { 0x00, 0x20, 0x1A, 0x00, 0x00, 0x04, 0x00, 0x14,
                                          0x15, 0x09, 0x09, 0x09, 0x08, 0x4E, 0xF7, 0xF7,
                                          0x91, 0x39, 0xBA, 0x86, 0x22, 0xFF, 0xFF, 0xFF,
                                          0xFF, 0x00, 0x50, 0x10, 0xE0 };
static uint8_t encoded_psm_mf_invalid[] = { 0x10, 0x30, 0x1A, 0x00, 0x00, 0x05, 0x00, 0x14,
                                            0x15, 0x09, 0x09, 0x09, 0x08, 0x4E, 0xF7, 0xF7,
                                            0x01, 0x39, 0xBA, 0x86, 0x22, 0xFF, 0xFF, 0xFF,
                                            0xFF, 0x00, 0x50, 0x10, 0xE0 };

TEST_C(smoketest, init_psm)
{
    PersonalSafetyMessage_t* psm = calloc(1, sizeof(PersonalSafetyMessage_t));

    libsm_rval_e result = libsm_init_psm(psm);

    CHECK_EQUAL_C_INT(LIBSM_OK, result);

    ASN_STRUCT_FREE(asn_DEF_PersonalSafetyMessage, psm);
}

TEST_C(smoketest, init_psm_NULL)
{
    PersonalSafetyMessage_t* psm = NULL;

    libsm_rval_e result = libsm_init_psm(psm);

    if (result == LIBSM_OK) {
        FAIL_TEXT_C("init_psm succeeded when it should have failed.");
    }

    ASN_STRUCT_FREE(asn_DEF_PersonalSafetyMessage, psm);
}

TEST_C(smoketest, init_bsm)
{
    BasicSafetyMessage_t* bsm = calloc(1, sizeof(BasicSafetyMessage_t));

    libsm_rval_e result = libsm_init_bsm(bsm);

    CHECK_EQUAL_C_INT(LIBSM_OK, result);

    ASN_STRUCT_FREE(asn_DEF_BasicSafetyMessage, bsm);
}

TEST_C(smoketest, init_bsm_NULL)
{
    BasicSafetyMessage_t* bsm = NULL;

    libsm_rval_e result = libsm_init_bsm(bsm);

    if (result == LIBSM_OK) {
        FAIL_TEXT_C("init_bsm succeeded when it should have failed.");
    }
}

TEST_C(smoketest, decode_messageframe)
{
    libsm_rval_e ret;

    MessageFrame_t* mf = calloc(1, sizeof(MessageFrame_t));

    ret = libsm_decode_messageframe(encoded_psm_mf_valid, ARRAY_SIZE(encoded_psm_mf_valid), mf);
    if (ret != LIBSM_OK) {
        FAIL_TEXT_C("Failed to decode.");
    }

    ASN_STRUCT_FREE(asn_DEF_MessageFrame, mf);
}

TEST_C(smoketest, decode_messageframe_bad_data)
{
    libsm_rval_e ret;

    MessageFrame_t* mf = calloc(1, sizeof(MessageFrame_t));

    ret = libsm_decode_messageframe(encoded_psm_mf_invalid,
                                    ARRAY_SIZE(encoded_psm_mf_invalid),
                                    mf);
    if (ret == LIBSM_OK) {
        FAIL_TEXT_C("decoding message frame passed when it should have failed.");
    }

    ASN_STRUCT_FREE(asn_DEF_MessageFrame, mf);
}

// this messageframe has a been edited so position.lat is 900000002
// asn_decode only returns fail, not constraint fail for this
TEST_C(smoketest, decode_messageframe_outside_constraints)
{
    libsm_rval_e ret;
    uint8_t outside_constraints[] = { 0x00, 0x20, 0x1F, 0x10, 0x00, 0x01, 0xFF, 0xFE, 0x03,
                                      0xFF, 0xFF, 0xFF, 0xFD, 0x6B, 0x49, 0xD2, 0x02, 0xD6,
                                      0x93, 0xA4, 0x00, 0x10, 0x05, 0xFF, 0xFF, 0xFF, 0xFF,
                                      0xFF, 0xFF, 0x08, 0x04, 0x00, 0x20, 0x10 };
    MessageFrame_t* mf = calloc(1, sizeof(MessageFrame_t));

    ret = libsm_decode_messageframe(outside_constraints, ARRAY_SIZE(outside_constraints), mf);

    CHECK_EQUAL_C_INT(LIBSM_FAIL_DECODING, ret);

    ASN_STRUCT_FREE(asn_DEF_MessageFrame, mf);
}

TEST_C(smoketest, encode_messageframe_psm)
{
    PersonalSafetyMessage_t* psm = calloc(1, sizeof(PersonalSafetyMessage_t));
    uint8_t buf[128];
    size_t len = (sizeof(buf) / sizeof(*buf));
    libsm_rval_e ret;

    ret = libsm_init_psm(psm);
    if (ret != LIBSM_OK) {
        FAIL_TEXT_C("Can not test encoding when psm fails to initialize.");
    }
    psm->pathPrediction = calloc(1, sizeof(PathPrediction_t));
    assert(psm->pathPrediction);

    psm->pathPrediction->radiusOfCurve = 5;
    psm->pathPrediction->confidence = 2;

    psm->position.elevation = calloc(1, sizeof(Common_Elevation_t));
    assert(psm->position.elevation);

    (*psm->position.elevation) = 5;

    ret = libsm_encode_messageframe_psm(psm, buf, &len);
    ASN_STRUCT_FREE(asn_DEF_PersonalSafetyMessage, psm);
    if (ret != LIBSM_OK) {
        printf("FAILED encoding a PSM: '%s'.\n", libsm_str_err(ret));
        FAIL_C();
    }
}

TEST_C(smoketest, encode_messageframe_psm_err_val_out_of_range)
{
    PersonalSafetyMessage_t* psm = calloc(1, sizeof(PersonalSafetyMessage_t));
    uint8_t buf[128];
    size_t len = (sizeof(buf) / sizeof(*buf));
    libsm_rval_e ret;

    ret = libsm_init_psm(psm);
    if (ret != LIBSM_OK) {
        FAIL_TEXT_C("Can not test encoding when psm fails to initialize.");
    }
    psm->pathPrediction = calloc(1, sizeof(PathPrediction_t));
    assert(psm->pathPrediction);

    psm->pathPrediction->confidence = -1;

    ret = libsm_encode_messageframe_psm(psm, buf, &len);
    ASN_STRUCT_FREE(asn_DEF_PersonalSafetyMessage, psm);
    if (ret == LIBSM_OK) {
        FAIL_TEXT_C("Encoding a psm message frame succeeded when it should have failed.");
    }
}

TEST_C(smoketest, encode_messageframe_bsm)
{
    libsm_rval_e ret;
    uint8_t buf[128];
    size_t len = (sizeof(buf) / sizeof(*buf));

    BasicSafetyMessage_t* bsm = calloc(1, sizeof(BasicSafetyMessage_t));

    ret = libsm_init_bsm(bsm);
    if (ret != LIBSM_OK) {
        FAIL_TEXT_C("Can not test encoding when bsm fails to initialize.");
    }

    ret = libsm_encode_messageframe_bsm(bsm, buf, &len);
    ASN_STRUCT_FREE(asn_DEF_BasicSafetyMessage, bsm);

    if (ret != LIBSM_OK) {
        printf("FAILED encoding a BSM: '%s'.\n", libsm_str_err(ret));
        FAIL_C();
    }
}

TEST_C(smoketest, encode_messageframe_bsm_err_val_out_of_range)
{
    libsm_rval_e ret;
    uint8_t buf[128];
    size_t len = (sizeof(buf) / sizeof(*buf));

    BasicSafetyMessage_t* bsm = calloc(1, sizeof(BasicSafetyMessage_t));

    ret = libsm_init_bsm(bsm);
    if (ret != LIBSM_OK) {
        FAIL_TEXT_C("Can not test encoding when bsm fails to initialize.");
    }

    bsm->coreData.secMark = -1;
    ret = libsm_encode_messageframe_bsm(bsm, buf, &len);
    ASN_STRUCT_FREE(asn_DEF_BasicSafetyMessage, bsm);

    if (ret == LIBSM_OK) {
        FAIL_TEXT_C("Encoding a bsm message frame succeeded when it should have failed.");
    }
}

TEST_C(smoketest, get_basic_vehicle_class_smoke)
{
    libsm_rval_e ret;

    MessageFrame_t* mf = calloc(1, sizeof(MessageFrame_t));

    ret = libsm_decode_messageframe(encoded_bsm_mf_valid, ARRAY_SIZE(encoded_bsm_mf_valid), mf);
    if (ret != LIBSM_OK) {
        FAIL_TEXT_C("Failed to decode.");
    }

    //asn_fprint(stdout, &asn_DEF_MessageFrame, mf);
    BasicVehicleClass_t vehicleClass = libsm_get_basic_vehicle_class(
            &mf->value.choice.BasicSafetyMessage);

    CHECK_EQUAL_C_LONG(BasicVehicleClass_otherTraveler_Bicycle, vehicleClass);

    ASN_STRUCT_FREE(asn_DEF_MessageFrame, mf);
}

TEST_C(smoketest, get_basic_vehicle_class_noPartII)
{
    libsm_rval_e ret;

    MessageFrame_t* mf = calloc(1, sizeof(MessageFrame_t));

    ret = libsm_decode_messageframe(encoded_bsm_mf_valid_nopartII,
                                    ARRAY_SIZE(encoded_bsm_mf_valid_nopartII),
                                    mf);
    if (ret != LIBSM_OK) {
        FAIL_TEXT_C("Failed to decode.");
    }

    //asn_fprint(stdout, &asn_DEF_MessageFrame, mf);
    BasicVehicleClass_t vehicleClass = libsm_get_basic_vehicle_class(
            &mf->value.choice.BasicSafetyMessage);

    CHECK_EQUAL_C_LONG(BasicVehicleClass_unknownVehicleClass, vehicleClass);

    ASN_STRUCT_FREE(asn_DEF_MessageFrame, mf);
}

TEST_C(smoketest, set_basic_vehicle_class_smoke)
{
    libsm_rval_e ret;

    MessageFrame_t* mf = calloc(1, sizeof(MessageFrame_t));

    ret = libsm_decode_messageframe(encoded_bsm_mf_valid, ARRAY_SIZE(encoded_bsm_mf_valid), mf);
    if (ret != LIBSM_OK) {
        FAIL_TEXT_C("Failed to decode.");
    }

    ret = libsm_set_basic_vehicle_class(&mf->value.choice.BasicSafetyMessage,
                                        BasicVehicleClass_otherTraveler_Pedestrian);
    if (ret != LIBSM_OK) {
        FAIL_TEXT_C("Failed to set vehicle class.");
    }

    BasicVehicleClass_t vehicleClass = libsm_get_basic_vehicle_class(
            &mf->value.choice.BasicSafetyMessage);

    CHECK_EQUAL_C_LONG(BasicVehicleClass_otherTraveler_Pedestrian, vehicleClass);

    ASN_STRUCT_FREE(asn_DEF_MessageFrame, mf);
}

TEST_C(smoketest, set_basic_vehicle_class_noPartII)
{
    libsm_rval_e ret;

    MessageFrame_t* mf = calloc(1, sizeof(MessageFrame_t));

    ret = libsm_decode_messageframe(encoded_bsm_mf_valid_nopartII,
                                    ARRAY_SIZE(encoded_bsm_mf_valid_nopartII),
                                    mf);
    if (ret != LIBSM_OK) {
        FAIL_TEXT_C("Failed to decode.");
    }

    ret = libsm_set_basic_vehicle_class(&mf->value.choice.BasicSafetyMessage,
                                        BasicVehicleClass_otherTraveler_Bicycle);
    if (ret != LIBSM_OK) {
        FAIL_TEXT_C("Failed to set vehicle class.");
    }
    BasicVehicleClass_t vehicleClass = libsm_get_basic_vehicle_class(
            &mf->value.choice.BasicSafetyMessage);

    CHECK_EQUAL_C_LONG(BasicVehicleClass_otherTraveler_Bicycle, vehicleClass);

    ASN_STRUCT_FREE(asn_DEF_MessageFrame, mf);
}

TEST_C(smoketest, set_basic_vehicle_class_noSupplementalVehicleExtensions)
{
    libsm_rval_e ret;

    MessageFrame_t* mf = calloc(1, sizeof(MessageFrame_t));

    ret = libsm_decode_messageframe(
            encoded_bsm_mf_valid_noSupplementalVehicleExtensions,
            ARRAY_SIZE(encoded_bsm_mf_valid_noSupplementalVehicleExtensions),
            mf);
    if (ret != LIBSM_OK) {
        FAIL_TEXT_C("Failed to decode.");
    }

    ret = libsm_set_basic_vehicle_class(&mf->value.choice.BasicSafetyMessage,
                                        BasicVehicleClass_otherTraveler_Bicycle);
    if (ret != LIBSM_OK) {
        FAIL_TEXT_C("Failed to set vehicle class.");
    }
    BasicVehicleClass_t vehicleClass = libsm_get_basic_vehicle_class(
            &mf->value.choice.BasicSafetyMessage);

    CHECK_EQUAL_C_LONG(BasicVehicleClass_otherTraveler_Bicycle, vehicleClass);

    ASN_STRUCT_FREE(asn_DEF_MessageFrame, mf);
}


static void comparePathPrediction(Confidence_t expectedConf,
                                  RadiusOfCurvature_t expectedRadius,
                                  PathPrediction_t* actual)
{
    CHECK_EQUAL_C_LONG(expectedConf, actual->confidence);
    CHECK_EQUAL_C_LONG(expectedRadius, actual->radiusOfCurve);
}


TEST_C(smoketest, path_prediction_smoke)
{
    libsm_rval_e ret;

    MessageFrame_t* mf = calloc(1, sizeof(MessageFrame_t));

    ret = libsm_decode_messageframe(encoded_bsm_mf_valid, ARRAY_SIZE(encoded_bsm_mf_valid), mf);
    if (ret != LIBSM_OK) {
        FAIL_TEXT_C("Failed to decode.");
    }

    Confidence_t confidence = 10;
    RadiusOfCurvature_t radiusOfCurve = 15;

    ret = libsm_set_path_prediction(&mf->value.choice.BasicSafetyMessage,
                                    confidence,
                                    radiusOfCurve);
    if (ret != LIBSM_OK) {
        FAIL_TEXT_C("Failed to set path prediction");
    }

    PathPrediction_t* getPath = libsm_get_path_prediction(&mf->value.choice.BasicSafetyMessage);

    comparePathPrediction(confidence, radiusOfCurve, getPath);

    ASN_STRUCT_FREE(asn_DEF_MessageFrame, mf);
}

TEST_C(smoketest, path_prediction_noPartII)
{
    libsm_rval_e ret;

    MessageFrame_t* mf = calloc(1, sizeof(MessageFrame_t));

    ret = libsm_decode_messageframe(encoded_bsm_mf_valid_nopartII,
                                    ARRAY_SIZE(encoded_bsm_mf_valid_nopartII),
                                    mf);
    if (ret != LIBSM_OK) {
        FAIL_TEXT_C("Failed to decode.");
    }

    Confidence_t confidence = 10;
    RadiusOfCurvature_t radiusOfCurve = 15;

    ret = libsm_set_path_prediction(&mf->value.choice.BasicSafetyMessage,
                                    confidence,
                                    radiusOfCurve);
    if (ret != LIBSM_OK) {
        FAIL_TEXT_C("Failed to set path prediction");
    }
    PathPrediction_t* getPath = libsm_get_path_prediction(&mf->value.choice.BasicSafetyMessage);

    comparePathPrediction(confidence, radiusOfCurve, getPath);

    ASN_STRUCT_FREE(asn_DEF_MessageFrame, mf);
}

TEST_C(smoketest, path_prediction_noVehicleSafetyExtensions)
{
    libsm_rval_e ret;

    MessageFrame_t* mf = calloc(1, sizeof(MessageFrame_t));

    ret = libsm_decode_messageframe(encoded_bsm_mf_valid_noVehicleSafetyExtensions,
                                    ARRAY_SIZE(encoded_bsm_mf_valid_noVehicleSafetyExtensions),
                                    mf);
    if (ret != LIBSM_OK) {
        FAIL_TEXT_C("Failed to decode.");
    }

    Confidence_t confidence = 10;
    RadiusOfCurvature_t radiusOfCurve = 15;

    ret = libsm_set_path_prediction(&mf->value.choice.BasicSafetyMessage,
                                    confidence,
                                    radiusOfCurve);
    if (ret != LIBSM_OK) {
        FAIL_TEXT_C("Failed to set path prediction");
    }
    PathPrediction_t* getPath = libsm_get_path_prediction(&mf->value.choice.BasicSafetyMessage);

    comparePathPrediction(confidence, radiusOfCurve, getPath);

    ASN_STRUCT_FREE(asn_DEF_MessageFrame, mf);
}

TEST_C(smoketest, lism_str_personal_device_user_type)
{
    const char* result;
    char buf[64];

    for (long i = -5; i < 10; ++i) {
        result = libsm_str_personal_device_user_type(i);
        //printf("i %d '%s'\n", i, result);
        if (result == NULL) {
            sprintf(buf, "lism_str_personal_device_user_type index %ld returned null", i);
            FAIL_TEXT_C(buf);
        }
    }
}

TEST_C(smoketest, encode_jer)
{
    libsm_rval_e ret;

    MessageFrame_t* mf = calloc(1, sizeof(MessageFrame_t));

    ret = libsm_decode_messageframe(encoded_psm_mf_valid, ARRAY_SIZE(encoded_psm_mf_valid), mf);
    if (ret != LIBSM_OK) {
        FAIL_TEXT_C("Failed to decode.");
    }

    asn_encode_to_new_buffer_result_t res
            = asn_encode_to_new_buffer(0, ATS_JER, &asn_DEF_MessageFrame, mf);

    if (!res.buffer) {
        fprintf(stderr,
                "Failed to encode %s, estimated %zd bytes\n",
                asn_DEF_MessageFrame.name,
                res.result.encoded);
    } else {
        free(res.buffer);
    }

    ASN_STRUCT_FREE(asn_DEF_MessageFrame, mf);
}


TEST_C(smoketest, alloc_init_mf_psm)
{
    MessageFrame_t* mf = libsm_alloc_init_mf_psm();

    PersonalSafetyMessage_t* psm = libsm_get_psm(mf);

    psm->msgCnt = 5;


    ASN_STRUCT_FREE(asn_DEF_MessageFrame, mf);
}
TEST_C(smoketest, alloc_init_mf_bsm)
{
    MessageFrame_t* mf = libsm_alloc_init_mf_bsm();

    BasicSafetyMessage_t* bsm = libsm_get_bsm(mf);

    bsm->coreData.msgCnt = 5;


    ASN_STRUCT_FREE(asn_DEF_MessageFrame, mf);
}
