/**
 * Tests for Traveler Information Message (TIM)
 *
 * REFERENCE: https://cpputest.github.io/manual.html
 */

#include <stdint.h>
#include <stdlib.h>

#include "CppUTest/TestHarness_c.h"
#include "libsm.h"

char buf[512];
size_t buf_len = sizeof(buf);

char errbuf[128];
size_t errlen = sizeof(errbuf);

TEST_GROUP_C_SETUP(test_tim)
{
    errlen = sizeof(errbuf);
}
TEST_C(test_tim, init_tim_traveler_data_frame_roadSignID)
{
    TravelerDataFrame_t* traveler_data_frame = libsm_alloc_init_partial_TravelerDataFrame();

    CHECK_C_TEXT(traveler_data_frame != NULL, "traveler_data_frame was NULL");

    // Insert arbitrary values into the fields, to show we can
    traveler_data_frame->frameType = TravelerInfoType_advisory;
    traveler_data_frame->startTime = 45701;
    traveler_data_frame->durationTime = 110;
    traveler_data_frame->priority = 0;

    // Choose FurtherInfoID
    libsm_rval_e rval = libsm_init_TravelerDataFrame_msgId_choice_RoadSignID(traveler_data_frame);
    CHECK_C_TEXT(rval == LIBSM_OK, "Fail: Alloc RoadSignID");

    // Put arbitrary values into FurtherInfoID
    traveler_data_frame->msgId.choice.furtherInfoID.buf[0] = 0x08;
    traveler_data_frame->msgId.choice.furtherInfoID.buf[1] = 0x77;

    // Create a bare-bones GeographicalPath
    GeographicalPath_t* geographical_path = libsm_alloc_init_GeographicalPath();
    ASN_SEQUENCE_ADD(&traveler_data_frame->regions.list, geographical_path);

    // Create the required Part III content, by making a CHOICE of ITIScodesAndText ("advisory")
    ITIScodesAndText_t* advisory = libsm_alloc_init_ITIScodesAndText();
    if (advisory == NULL) {
        ASN_STRUCT_FREE(asn_DEF_TravelerDataFrame, traveler_data_frame);
        sprintf(buf, "Fail: ITIScodesAndText_t alloc-init");
        FAIL_TEXT_C(buf);
    }
    // J2735 allows between 1...100 ITIScodesAndText__Member
    ITIScodesAndText__Member* itis_codes_and_text__member
            = libsm_alloc_init_ITIScodesAndText__Member_text("Stopped Traffic Ahead");
    ASN_SEQUENCE_ADD(&advisory->list, &itis_codes_and_text__member->item);

    // Add advisory to the TravelerDataFrame
    traveler_data_frame->content.choice.advisory = advisory;
    traveler_data_frame->content.present = TravelerDataFrame__content_PR_advisory;

    // Check constraints
    int err = asn_check_constraints(&asn_DEF_TravelerDataFrame,
                                    traveler_data_frame,
                                    errbuf,
                                    &errlen);
    if (err != 0) {
        sprintf(buf, "asn_check_constraints error: '%s'", errbuf);
        FAIL_TEXT_C(buf);
    }

    ASN_STRUCT_FREE(asn_DEF_TravelerDataFrame, traveler_data_frame);
}


TEST_C(test_tim, init_tim_traveler_data_frame_furtherInfoID)
{
    TravelerDataFrame_t* traveler_data_frame = libsm_alloc_init_partial_TravelerDataFrame();

    CHECK_C_TEXT(traveler_data_frame != NULL, "traveler_data_frame was NULL");

    // Insert arbitrary values into the fields, to show we can
    traveler_data_frame->frameType = TravelerInfoType_advisory;
    traveler_data_frame->startTime = 45701;
    traveler_data_frame->durationTime = 110;
    traveler_data_frame->priority = 0;

    // Choose FurtherInfoID
    libsm_rval_e rval = libsm_init_TravelerDataFrame_msgId_choice_FurtherInfoID(
            traveler_data_frame);
    CHECK_C_TEXT(rval == LIBSM_OK, "Fail: Alloc FurtherInfoID");

    // Put arbitrary values into FurtherInfoID
    traveler_data_frame->msgId.choice.furtherInfoID.buf[0] = 0x08;
    traveler_data_frame->msgId.choice.furtherInfoID.buf[1] = 0x77;

    // Create a bare-bones GeographicalPath
    GeographicalPath_t* geographical_path = libsm_alloc_init_GeographicalPath();
    ASN_SEQUENCE_ADD(&traveler_data_frame->regions.list, geographical_path);

    // Create the required Part III content, by making a CHOICE of ITIScodesAndText ("advisory")
    ITIScodesAndText_t* advisory = libsm_alloc_init_ITIScodesAndText();
    if (advisory == NULL) {
        ASN_STRUCT_FREE(asn_DEF_TravelerDataFrame, traveler_data_frame);
        sprintf(buf, "Fail: ITIScodesAndText_t alloc-init");
        FAIL_TEXT_C(buf);
    }
    // J2735 allows between 1...100 ITIScodesAndText__Member
    ITIScodesAndText__Member* itis_codes_and_text__member
            = libsm_alloc_init_ITIScodesAndText__Member_text("Stopped Traffic Ahead");
    ASN_SEQUENCE_ADD(&advisory->list, &itis_codes_and_text__member->item);

    // Add advisory to the TravelerDataFrame
    traveler_data_frame->content.choice.advisory = advisory;
    traveler_data_frame->content.present = TravelerDataFrame__content_PR_advisory;

    // Check constraints
    int err = asn_check_constraints(&asn_DEF_TravelerDataFrame,
                                    traveler_data_frame,
                                    errbuf,
                                    &errlen);
    if (err != 0) {
        sprintf(buf, "asn_check_constraints error: '%s'", errbuf);
        FAIL_TEXT_C(buf);
    }

    ASN_STRUCT_FREE(asn_DEF_TravelerDataFrame, traveler_data_frame);
}


TEST_C(test_tim, init_tim_minimum_viable)
{
    uint8_t encode_buf[512] = { 0 };
    size_t encode_buf_len = sizeof(encode_buf);

    // Test mf and get pointer to TIM
    MessageFrame_t* mf = libsm_alloc_init_partial_mf_tim();
    CHECK_C_TEXT(mf != NULL, "Ought not to be NULL");
    TravelerInformation_t* tim = libsm_get_tim(mf);
    CHECK_C_TEXT(tim != NULL, "Ought not to be NULL");

    // Create a TravelerDataFrame, finish setting it up later
    TravelerDataFrame_t* traveler_data_frame = libsm_alloc_init_partial_TravelerDataFrame();
    CHECK_C_TEXT(traveler_data_frame != NULL, "traveler_data_frame ought not to be NULL");

    // CHOICE msgId.FurtherInfoID
    libsm_rval_e rval = libsm_init_TravelerDataFrame_msgId_choice_FurtherInfoID(
            traveler_data_frame);
    CHECK_C_TEXT(rval == LIBSM_OK, "Fail: Alloc FurtherInfoID");

    // Create a bare-bones GeographicalPath
    GeographicalPath_t* geographical_path = libsm_alloc_init_GeographicalPath();
    ASN_SEQUENCE_ADD(&traveler_data_frame->regions.list, geographical_path);

    // Create the required Part III content, CHOICE ITIScodesAndText ("advisory")
    ITIScodesAndText_t* advisory = libsm_alloc_init_ITIScodesAndText();
    CHECK_C_TEXT(advisory != NULL, "Failed ITIS alloc");

    // Minimum 1 ITIScodesAndText__Member is required
    ITIScodesAndText__Member* itis_codes_and_text__member
            = libsm_alloc_init_ITIScodesAndText__Member_text("Stop sign ahead");
    ASN_SEQUENCE_ADD(&advisory->list, &itis_codes_and_text__member->item);

    // Add advisory to the TravelerDataFrame
    traveler_data_frame->content.choice.advisory = advisory;
    traveler_data_frame->content.present = TravelerDataFrame__content_PR_advisory;

    // Add the traveler_data_frame to the TIM
    ASN_SEQUENCE_ADD(&tim->dataFrames.list, traveler_data_frame);

    // Check constraints
    int err = asn_check_constraints(&asn_DEF_MessageFrame, mf, errbuf, &errlen);
    if (err != 0) {
        sprintf(buf, "asn_check_constraints error: '%s'", errbuf);
        FAIL_TEXT_C(buf);
    }
    // UPER encode the MessageFrame
    rval = libsm_encode_messageframe(mf, encode_buf, &encode_buf_len);
    CHECK_C_TEXT(rval == LIBSM_OK, "Fail: encode an incomplete TIM");


    ASN_STRUCT_FREE(asn_DEF_MessageFrame, mf);
}
