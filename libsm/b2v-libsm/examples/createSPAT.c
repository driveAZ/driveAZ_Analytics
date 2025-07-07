/*
 * createSPAT.c
 * Example to create a SPAT and encode it with a messageframe as a UPER array
 */

#include <getopt.h>
#include <stdint.h>
#include <stdlib.h>

#include "libsm.h"


int main(int argc, char** argv)
{
    uint8_t buf[128]; //Buffer is the size of message it can take in
    size_t len = ARRAY_SIZE(buf);
    libsm_rval_e ret; //error type return message
    int opt;
    int option_index = 0;
    static struct option long_options[] = { { "help", no_argument, NULL, 'h' },
                                            { NULL, 0, NULL, 0 } };

    while ((opt = getopt_long(argc, argv, "h", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'h':
                printf("This is an example to create a SPAT message encode it with a messageframe as UPER arrays.\n");
                printf("USAGE:  %s\n", argv[0]);
                exit(0);
            default: /* '?' */
                exit(2);
        }
    }

    MessageFrame_t* mf = calloc(1, sizeof(MessageFrame_t));
    mf->messageId
            = DSRCmsgID_signalPhaseAndTimingMessage; //makes messageID 19 which marks it as a spat message
    mf->value.present = MessageFrame__value_PR_SPAT; //
    SPAT_t* spat = &mf->value.choice.SPAT;

    ret = libsm_init_spat(spat);
    if (ret != LIBSM_OK) {
        printf("FAILED making a SPAT.\n");
        return 1;
    }

    // Add a second state to the SPAT message
    IntersectionState_t* intersection = spat->intersections.list.array[0];
    MovementState_t* state = libsm_add_spat_intersectionState_movementState(intersection);
    if (state == NULL) {
        printf("FAILED adding a state to a SPAT.\n");
        return 1;
    }

    // Set the values for the SPAT intersection
    spat->intersections.list.array[0]->id.id = 1001;
    spat->intersections.list.array[0]->revision = 1;
    spat->intersections.list.array[0]->status.buf[0] = 0x0;
    spat->intersections.list.array[0]->status.buf[0]
            |= (1 << (15 - IntersectionStatusObject_fixedTimeOperation));
    ;

    // Set the values for the signal groups and event states for both states in the SPAT
    spat->intersections.list.array[0]->states.list.array[0]->signalGroup = 1;
    spat->intersections.list.array[0]->states.list.array[1]->signalGroup = 2;

    spat->intersections.list.array[0]->states.list.array[0]->state_time_speed.list.array[0]->eventState
            = MovementPhaseState_stop_And_Remain;
    spat->intersections.list.array[0]->states.list.array[1]->state_time_speed.list.array[0]->eventState
            = MovementPhaseState_protected_Movement_Allowed;

    asn_fprint(stdout, &asn_DEF_MessageFrame, mf);


    ret = libsm_encode_messageframe(mf, buf, &len);
    ASN_STRUCT_FREE(asn_DEF_MessageFrame, mf);
    if (ret != LIBSM_OK) {
        printf("FAILED encoding a SPAT: '%s'.\n", libsm_str_err(ret));
        return 1;
    }


    printf("encoded:\n");
    for (size_t i = 0; i < len; ++i) {
        printf(" %02X", buf[i]);
    }
    printf("\n");
}


/*
 * value MessageFrame :: =
 * {
 *  messageId 19, //base msg I
 *  {
 *      intersections
 *      { //base intersections
 *          {//first intersection of this list of intersections
 *              IntersectionID_t const id =1001,
 *              Common_MsgCount_t const rev = 1;, //intersections revision number
 *              IntersectionStatusObject_t const status = 5, //intersections status
 *              states //intersections states
 *              {
 *                  MovementState_t movementState = libsm_init_spat_states(SignalGroupID_t const signalGroup = 1;)
 *                      {
 *                          MovementEvent_t sts= libsm_init_spat_sts(&eventState)
 *                          //timing
 *                      },
 *                  MovementState_t movementState2 = libsm_init_spat_states(SignalGroupID_t const signalGroup = 1;)
 *                      {
 *                          MovementEvent_t sts= libsm_init_spat_sts(&eventState)
 *                      }
 *              }
 *              }
 *          }
 *      }
 *  }
 * }
 *
 * /
 * Data we need
 *
 *
 * value MessageFrame :: =
 * {
 *  messageId 19, //base msg ID
 *  value SPAT :
 *  {
 *      intersections
 *      { //base intersections
 *          {//first intersection of this list of intersections
 *              id //intersections ID
 *              {
 *                  id 1001 //id's ID
 *              },
 *              revision 1, //intersections revision number
 *              status {trafficDependentOperation}, //intersections status
 *
 *              states //intersections states
 *              {
 *                  {//first signal group/state in the first intersection
 *                      signalGroup 1, //states signal group
 *                      state - time - speed //states state-time-speed
 *                      {
 *                          {
 *                              //movement phase 3
 *                              eventState stop - And - Remain, //state-time-speed's event state
 *                          }
 *                      }
 *                  },
 *                  {//second signal group/stat in the first intersection
 *                      signalGroup 2, //states signal group
 *                      state - time - speed
 *                      {
 *                          {
 *                              //movement phase 6
 *                              eventState protected - Movement - Allowed, //
 *                          }
 *                      }
 *                  }
 *              }
 *          }
 *      }
 *  }
 * }
 *
 * Intersection: Medical Foundation Dr at El Camino Real, Palo Alto, California
 * SPaT payload size 85
 * payload = 0013520018800001F48102004EAF77B9D06001043C29A22A742A74001021E14D115AAD5AAC01011B0A688A96C
 * A96C00A0878534454E854E8006043C29A22ABA2B00006023614D114E294E2803810F0A688AAE8AC00
 * gov.usdot.asn1.generated.j2735.dsrc.MessageFrame
 * value MessageFrame :: = {
 *  messageId 19,
 *  value SPAT : {
 *      intersections {
 *          {
 *              id {
 *                  region 0,
 *                  id 1001
 *              },
 *              revision 1,
 *              status {trafficDependentOperation},
 *              moy 322295,
 *              timeStamp 31645,
 *              states {
 *                  {
 *                      signalGroup 1,
 *                      state - time - speed {
 *                          {
 *                              eventState stop - And - Remain,
 *                              timing {
 *                                  startTime 21316,
 *                                  minEndTime 21736,
 *                                  maxEndTime 21736
 *                              }
 *                          }
 *                      }
 *                  },
 *                  {
 *                      signalGroup 2,
 *                      state - time - speed {
 *                          {
 *                              eventState protected - Movement - Allowed,
 *                              timing {
 *                                  startTime 21316,
 *                                  minEndTime 22187,
 *                                  maxEndTime 22187
 *                              }
 *                          }
 *                      }
 *                  },
 *                  {
 *                      signalGroup 4,
 *                      state - time - speed {
 *                          {
 *                          eventState protected - Movement - Allowed,
 *                              timing {
 *                                  startTime 21316,
 *                                  minEndTime 21686,
 *                                  maxEndTime 21686
 *                              }
 *                          }
 *                      }
 *                  },
 *                  {
 *                      signalGroup 5,
 *                      state - time - speed {
 *                          {
 *                              eventState stop - And - Remain,
 *                              timing {
 *                                  startTime 21316,
 *                                  minEndTime 21736,
 *                                  maxEndTime 21736
 *                              }
 *                          }
 *                      }
 *                  },
 *                  {
 *                      signalGroup 6,
 *                      state - time - speed {
 *                          {
 *                              eventState stop - And - Remain,
 *                              timing {
 *                                  startTime 21316,
 *                                  minEndTime 21876,
 *                                  maxEndTime 22016
 *                              }
 *                          }
 *                      }
 *                  },
 *                  {
 *                      signalGroup 12,
 *                      state - time - speed {
 *                          {
 *                              eventState protected - Movement - Allowed,
 *                              timing {
 *                                  startTime 21316,
 *                                  minEndTime 21386,
 *                                  maxEndTime 21386
 *                              }
 *                          }
 *                      }
 *                  },
 *                  {
 *                      signalGroup 14,
 *                      state - time - speed {
 *                          {
 *                              eventState stop - And - Remain,
 *                              timing {
 *                                  startTime 21316,
 *                                  minEndTime 21876,
 *                                  maxEndTime 22016
 *                              }
 *                          }
 *                      }
 *                  }
 *              }
 *          }
 *      }
 *  }
 * }
 */
