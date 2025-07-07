#
# CMakeLists for tests
#

include(${PROJECT_SOURCE_DIR}/tooling/misc.cmake)

# cpputest has some warnings on, and we don't want to see them
filter_supported_flags(CPPUTEST_CXXFLAGS "CXX"
    -Wno-c++98-compat
    -Wno-c++98-compat-pedantic
    -Wno-suggest-override
    -Wno-suggest-destructor-override
)

target_compile_options(CppUTest PRIVATE ${CPPUTEST_CXXFLAGS})
target_compile_options(CppUTestExt PRIVATE ${CPPUTEST_CXXFLAGS})

add_executable(test_libsm
    testRunner.cpp

    bitstring.c
    per.c
    rangeCoercion.c
    smoketest.c
    testPathHistory.c
    versionCheck.c
    testSPAT.c
    testTIM.c
)

target_include_directories(test_libsm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(test_libsm PRIVATE
    CppUTest
    CppUTestExt
    libsm
)
