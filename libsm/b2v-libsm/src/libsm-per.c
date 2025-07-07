#include "libsm-per.h"
#include <math.h>
#include <string.h>


// calculate time in ms between start and end
static int diff_ms(struct timespec* start, struct timespec* end)
{
    if (start == NULL || end == NULL) {
        fprintf(stderr, "someone passed null to diff_ms, don't do that\n");
        abort();
    }
    return ((end->tv_sec - start->tv_sec) * 1000) + (end->tv_nsec - start->tv_nsec) / 1000000;
}


uint16_t libsm_per_calculate(PERSlidingInterval_t arr[PER_SUBINTERVAL_COUNT])
{
    // This rollover formula is a modified from J2945/1 to look at
    // the breadth of the interval "delta-k"
    int8_t first_subwindow = -1, last_subwindow = first_subwindow;
    uint8_t expectedTotal = 0;

    int receivedTotal = 0;
    uint8_t expectedSubWindow = 0;
    uint8_t diff = 0;

    for (size_t n = 0; n < PER_SUBINTERVAL_COUNT; n++) {
        // Process subwindows if it has actually received something
        if (arr[n].received > 0) {
            receivedTotal += arr[n].received;
            // Now calculate 'expected' for just this subwindow.
            // If received > expectedSubWindow, then increase expected
            // to allow for duplicates.
            expectedSubWindow = (arr[n].last - arr[n].first) + 1;
            expectedSubWindow = expectedSubWindow % 128;
            if (arr[n].received > expectedSubWindow) {
                diff = (arr[n].received - expectedSubWindow);
                expectedTotal += diff;
            }
            // Look for duplicates between subWindows,
            // and increment expectedTotal accordingly.
            if (n > 0) {
                if (arr[n - 1].last == arr[n].first) {
                    expectedTotal++;
                }
            }
            // If "n" is the first subwindow that received messages
            // then note it as the first (and last subWindow),
            // in the event the rest of delta-K has no other received messages.
            if (first_subwindow < 0) {
                first_subwindow = n;
                last_subwindow = n;
            } else {
                // Save the recent subwindow that has received messages.
                last_subwindow = n;
            }
        }
        // DEBUG (uncomment whilst working on tests)
        // printf("%ld first: %03ld last: %03ld received: %03ld expectedSubWindow: %03d expectedTotal: %03d\n",
        //        n,
        //        arr[n].first,
        //        arr[n].last,
        //        arr[n].received,
        //        expectedSubWindow,
        //        expectedTotal);
        // DEBUG END
    }
    // if we have zero or one packet in delta-k then PER is unavailable
    // as required by 6.3.8.1 and A.8.3,Figure A22(b)
    if (receivedTotal < 2) {
        return PER_UNAVAILABLE;
    }
    // Calculate the total expected messages across the span of delta-K
    uint8_t expectedTotalSpan
            = (uint8_t)(arr[last_subwindow].last - (int)arr[first_subwindow].first) % 128 + 1;
    expectedTotal += expectedTotalSpan;
    uint8_t missed = expectedTotal - receivedTotal;

    // DEBUG
    // Uncomment whilst working with storage_per and viewing the Monitor display
    //printf("\033[25;0H---|  E    R    M   |---\n");
    //printf("---| %03d, %03d, %03d  |---\n", expectedTotal, received, missed);
    // DEBUG END

    // DEBUG
    // Uncomment whilst working on tests
    // printf("---|  E    R    M   |---\n");
    // printf("---| %03d, %03d, %03d  |---\n", expectedTotal, receivedTotal, missed);
    // printf("-----------------------------\n");
    // DEBUG END


    // Keep PER between 0 and 100, and use rounding.
    // It was requested that the PER value is rounded.
    double per_tmp = round((((double)missed / (double)expectedTotal) * 100.0));
    uint16_t per = (uint16_t)per_tmp;
    if (per > 100) {
        per = 100;
    }
    // DEBUG
    // printf("PER: %03d\n", per);
    // DEBUG END
    return per;
    //}
}


void libsm_per_store_recalculate(uint16_t* devicePER,
                                 PERSlidingInterval_t arr[PER_SUBINTERVAL_COUNT],
                                 Common_MsgCount_t msgCnt,
                                 struct timespec* now)
{
    struct timespec* tmpTS = &arr[PER_NEWEST_SUBINTERVAL].window_start;
    int diffTime = diff_ms(tmpTS, now);

    // diffTime will be negative when .window_start is 0 (starting up)
    if (diffTime < 0) {
        // Initialize the newest element
        arr[PER_NEWEST_SUBINTERVAL].first = msgCnt;
        arr[PER_NEWEST_SUBINTERVAL].last = msgCnt;
        arr[PER_NEWEST_SUBINTERVAL].received = 1;
        memcpy(&arr[PER_NEWEST_SUBINTERVAL].window_start, now, sizeof(struct timespec));
        return;
    }

    // If we've reached the end of the subinterval time, calculate the PER based on the
    // current values, then shift all elements down, losing the oldest,
    // and insert the newest at the top.
    if (diffTime >= V_PER_SUBINTERVAL_MS) {
        *devicePER = libsm_per_calculate(arr);

        // DEBUG Print the current subinterval and PER
        // uint8_t expectedSubWindow;
        // bool expectedFlag;
        // //printf("\033[19;0H---| 1st  last  rcv     T       Exp   |---PER:%03d---\n", *devicePER);
        // printf("---| 1st  last  rcv     T       Exp   |---PER:%03d---\n", *devicePER);
        // for (size_t n = 0; n < PER_SUBINTERVAL_COUNT; n++) {
        //     expectedSubWindow = ((arr[n].last - arr[n].first) + 1);
        //     expectedSubWindow = expectedSubWindow % 128;
        //     if (arr[n].received > expectedSubWindow) {
        //         expectedFlag = true;
        //     } else {
        //         expectedFlag = false;
        //     }
        //     printf("---| %03ld, %03ld, %03ld, %ld, %03d%s  |---\n",
        //            arr[n].first, arr[n].last, arr[n].received,
        //            arr[n].window_start.tv_sec, expectedSubWindow, expectedFlag ? "*" : " ");
        // }
        // DEBUG END

        for (size_t n = 1; n < PER_SUBINTERVAL_COUNT; n++) {
            memcpy(&arr[n - 1], &arr[n], sizeof(PERSlidingInterval_t));
        }

        // Re-initialize the newest element
        arr[PER_NEWEST_SUBINTERVAL].first = msgCnt;
        arr[PER_NEWEST_SUBINTERVAL].last = msgCnt;
        arr[PER_NEWEST_SUBINTERVAL].received = 1;
        memcpy(&arr[PER_NEWEST_SUBINTERVAL].window_start, now, sizeof(struct timespec));
        return;
    }
    // Update the newest element
    arr[PER_NEWEST_SUBINTERVAL].last = msgCnt;
    arr[PER_NEWEST_SUBINTERVAL].received += 1;
}
