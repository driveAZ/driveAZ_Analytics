/*
 * psmPartII.c
 * Example to create a BSM with part II
 */

#include <getopt.h>
#include <stdint.h>
#include <stdlib.h>

#include "libsm.h"


int main(int argc, char** argv)
{
    BasicSafetyMessage_t* bsm;

    uint8_t buf[128];
    size_t len = ARRAY_SIZE(buf);
    libsm_rval_e ret;

    int opt;
    int option_index = 0;
    static struct option long_options[] = { { "help", no_argument, NULL, 'h' },
                                            { NULL, 0, NULL, 0 } };

    while ((opt = getopt_long(argc, argv, "h", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'h':
                printf("This is an example to create a BSM with part II extension.\n");
                printf("USAGE:  %s\n", argv[0]);
                exit(0);
            default: /* '?' */
                exit(2);
        }
    }

    bsm = calloc(1, sizeof(BasicSafetyMessage_t));
    assert(bsm);

    ret = libsm_init_bsm(bsm);
    if (ret != LIBSM_OK) {
        printf("FAILED making a BSM.\n");
        return 1;
    }

    //allocate part II holder
    bsm->partII = calloc(1, sizeof(struct BasicSafetyMessage__partII));
    assert(bsm->partII);


    // do a vehicle safety extension for path prediction
    BSMpartIIExtension_t* vehicleSafetyExt = calloc(1, sizeof(BSMpartIIExtension_t));

    assert(vehicleSafetyExt);

    vehicleSafetyExt->partII_Id = PartII_Id_vehicleSafetyExt;
    vehicleSafetyExt->partII_Value.present
            = BSMpartIIExtension__partII_Value_PR_VehicleSafetyExtensions;

    vehicleSafetyExt->partII_Value.choice.VehicleSafetyExtensions.pathPrediction
            = calloc(1, sizeof(PathPrediction_t));
    assert(vehicleSafetyExt->partII_Value.choice.VehicleSafetyExtensions.pathPrediction);

    vehicleSafetyExt->partII_Value.choice.VehicleSafetyExtensions.pathPrediction->radiusOfCurve
            = 5;
    vehicleSafetyExt->partII_Value.choice.VehicleSafetyExtensions.pathPrediction->confidence = 2;

    ret = ASN_SEQUENCE_ADD(&bsm->partII->list, vehicleSafetyExt);
    assert(ret == 0);


    //do  a supplemental vehicle extension for VRU type
    BSMpartIIExtension_t* supplementalVehicleExt = calloc(1, sizeof(BSMpartIIExtension_t));

    assert(supplementalVehicleExt);

    supplementalVehicleExt->partII_Id = PartII_Id_supplementalVehicleExt;
    supplementalVehicleExt->partII_Value.present
            = BSMpartIIExtension__partII_Value_PR_SupplementalVehicleExtensions;

    supplementalVehicleExt->partII_Value.choice.SupplementalVehicleExtensions.classification
            = calloc(1, sizeof(BasicVehicleClass_t));
    assert(supplementalVehicleExt->partII_Value.choice.SupplementalVehicleExtensions
                   .classification);

    *(supplementalVehicleExt->partII_Value.choice.SupplementalVehicleExtensions.classification)
            = BasicVehicleClass_otherTraveler_Bicycle;

    ret = ASN_SEQUENCE_ADD(&bsm->partII->list, supplementalVehicleExt);
    assert(ret == 0);


    asn_fprint(stdout, &asn_DEF_BasicSafetyMessage, bsm);

    ret = libsm_encode_messageframe_bsm(bsm, buf, &len);
    ASN_STRUCT_FREE(asn_DEF_BasicSafetyMessage, bsm);
    if (ret != LIBSM_OK) {
        printf("FAILED encoding a BSM: '%s'.\n", libsm_str_err(ret));
        return 1;
    }

    printf("encoded:\n");
    for (size_t i = 0; i < len; ++i) {
        printf(" %02X", buf[i]);
    }
    printf("\n");
}
