#
# CMakeLists.txt for libsm examples
#


function(exampleTarget name)
    add_executable(${name} ${name}.c)
    target_link_libraries(${name} PRIVATE libsm)
endfunction()

exampleTarget(bsmPartII)
exampleTarget(createPSM)
exampleTarget(createSPAT)
exampleTarget(validator)
exampleTarget(createTIM)
exampleTarget(decodeToJER)
