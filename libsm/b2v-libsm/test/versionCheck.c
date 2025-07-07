// https://cpputest.github.io/manual.html
#include "CppUTest/TestHarness_c.h"
#include "libsm.h"

TEST_C(versionCheck, valid)
{
    //printf("version is %s\n", VERSION_LIBSM);

    CHECK_C(strlen(VERSION_LIBSM) > 0);
    CHECK_C(strlen(VERSION_LIBSM_MAJOR) > 0);
    CHECK_C(strlen(VERSION_LIBSM_MINOR) > 0);
    CHECK_C(strlen(VERSION_LIBSM_PATCH) > 0);
    CHECK_C(strlen(VERSION_LIBSM_GIT) > 0);
}
