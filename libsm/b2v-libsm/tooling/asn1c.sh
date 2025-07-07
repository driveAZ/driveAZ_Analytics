#!/usr/bin/env bash
# helpers to compile ASN.1 sources with ans1c

if [[ "$(basename -- "$0")" == "script.sh" ]]; then
    >&2 echo "Don't run $0, source it"
    exit 1
fi


ASN1C_REPO=git@github.com:mouse07410/asn1c.git
ASN1C_COMMIT=a20d29aeb6a794e0754397eb7af2e95c346c63db
ASN1C_FLAGS=(-fcompound-names -no-gen-example -no-gen-BER -no-gen-XER -no-gen-OER -no-gen-APER -fincludes-quoted -findirect-choice)

if [[ "$OSTYPE" == "darwin"* ]] && ! sed --version >/dev/null; then
    die "sed --version doesn't work so you probably have bsd sed, try 'brew install gnu-sed'"
fi

msg() {
    echo >&2 -e "${*-}"
}

die() {
    local msg=$1
    local code=${2-1} # default exit status 1
    msg "$msg"
    exit "$code"
}

chooseTmpDir() {
    dir=$1
    if [ -z "$dir" ]; then
        tmp_dir=$(mktemp -d -t libsm-asn1c-compilation-XXXXXXXXXX)
    else
        tmp_dir="$dir"
    fi
    echo "$tmp_dir"
}
handleTmpDir() {
    tmp_dir=$1
    keep=$2

    # shellcheck disable=SC2317
    function cleanup() {
        set +x
        if [ ! "$keep" = true ]; then
            rm -rf "$tmp_dir"
        else
            msg "kept $tmp_dir"
        fi
    }
    trap "cleanup" ERR EXIT
    mkdir -p "$tmp_dir"
}

# pass in working dir
# will result in working dir/asn1c/asn1c/asn1c binary and working dir/asn1c/skeletons/ dir
buildASN1c() {
    tmp_dir=$1

    cd "$tmp_dir" || die "can't cd"
    msg "building stuff in $tmp_dir"

    clone=false
    build=false
    if [ ! -d "asn1c" ]; then
        msg "asn1c not cloned"
        clone=true
    else
        msg "asn1c already cloned"
        cd asn1c || die "can't cd"
        if [[ "$(git config --get remote.origin.url)" != "$ASN1C_REPO" ]]; then
            msg "asn1c wrong remote, clongin again"
            rm -rf ./asn1c
            clone=true
        else
            git fetch >/dev/null 2>&1
            startCommit=$(git rev-parse HEAD)
            git checkout $ASN1C_COMMIT >/dev/null 2>&1
            # if it's a branch, pull
            if git symbolic-ref HEAD 2>/dev/null ; then
                git pull
            fi
            endCommit=$(git rev-parse HEAD)
            if [[ "$startCommit" != "$endCommit" ]]; then
                build=true
            else
                msg "asn1c on correct version, assuming it's built"
            fi
        fi
        cd ..
    fi

    if [ "$clone" = true ]; then
        msg "cloning asn1c"
        git clone $ASN1C_REPO
        cd asn1c || die "can't cd"
        git checkout $ASN1C_COMMIT
        cd .. || die "can't cd"
        build=true
    fi
    if [ "$build" = true ]; then
        msg "building asn1c"
        cd asn1c || die "can't cd"
        test -f configure || autoreconf -iv
        ./configure
        make -j8
        cd .. || die "can't cd"
    fi
}
compileASN() {
    tmp_dir=$1
    input_name=$2
    shift 2
    inputFiles=("$@")

    inputDir="$tmp_dir/$input_name"
    outputDir="$tmp_dir/output"
    rm -rf "$outputDir"
    mkdir "$outputDir"
    rm -rf "$inputDir"
    mkdir "$inputDir"
    cp "${inputFiles[@]}" "$inputDir/"

    # we want to be in output dir and not specify it, because the output
    # Makefile.am.libasncodec will do names reletaive to current dir
    pushd "$outputDir" || die "cd fail"
    set -x
    "../asn1c/asn1c/asn1c" \
        -S "../asn1c/skeletons/" \
        "${ASN1C_FLAGS[@]}" \
        "../$input_name"/*
    set +x
    popd || die "cd fail"

    # the nordic version of fatfs has a integer.h. apple's HFS is by default case insensitive, and cannot cope.
    sed -i "s/<INTEGER.h>/\"INTEGER.h\"/" "$outputDir"/*.[ch]

    # our unit test framework redefines free to track memory allocation/free
    # asn1c uses the word free to not mean free.
    # change what word asn1c uses
    sed -i 's/\*free/*asn_free/; s/->free/->asn_free/' "$outputDir"/asn_SE*.[ch]


    # make CMakeLists.files.txt
    echo "set(${input_name^}_HEADERS" > "$outputDir"/CMakeLists.files.txt
    #shellcheck disable=SC2129,SC2016
    grep ".*\.h" "$outputDir"/Makefile.am.libasncodec | sed 's/ASN_MODULE_.*+=//; s/\t\(.*\.[ch]\)\t\?.*/\1/; s/\(.*\)/        \1/' >> "$outputDir"/CMakeLists.files.txt
    echo ")" >> "$outputDir"/CMakeLists.files.txt
    echo "set(${input_name^}_SRCS" >> "$outputDir"/CMakeLists.files.txt
    #shellcheck disable=SC2016
    grep ".*\.c" "$outputDir"/Makefile.am.libasncodec | sed 's/ASN_MODULE_.*+=//; s/\t\(.*\.[ch]\)\t\?.*/\1/; s/\(.*\)/        \1/' >> "$outputDir"/CMakeLists.files.txt
    echo ")" >> "$outputDir"/CMakeLists.files.txt

    cat > "$outputDir/CMakeLists.txt" << EOF
#
# CMakeLists.txt for ${input_name} library files
# GENERATED FILE
#

include(\${PROJECT_SOURCE_DIR}/tooling/misc.cmake)
include(\${PROJECT_SOURCE_DIR}/tooling/colours.cmake)

include(CMakeLists.files.txt)
include(CheckCompilerFlag)


# you just do "cmake -DJ2735_EMIT_DEBUG=ON .." to set
option(${input_name^}_EMIT_DEBUG "enable ASN_EMIT_DEBUG for ${input_name^}" OFF)

add_library(${input_name} SHARED \${${input_name^}_SRCS} \${${input_name^}_HEADERS})

filter_supported_flags(${input_name^}_CFLAGS "C"
        -Wall
        -Wextra
        -Werror
        -Wno-missing-field-initializers
        -Wno-unused-parameter
        -Wno-parentheses-equality
        -Wno-unused-but-set-variable
        -Wno-array-bounds
)
target_compile_options(${input_name} PRIVATE \${${input_name^}_CFLAGS})

if(${input_name^}_EMIT_DEBUG)
    message("\${ttyCy}-- Using ASN_EMIT_DEBUG for ${input_name}\${ttyN}")
    target_compile_options(${input_name} PRIVATE -DASN_EMIT_DEBUG=1)
else()
    target_compile_options(${input_name} PRIVATE -DASN_EMIT_DEBUG=0)
endif()


target_include_directories(${input_name} PUBLIC \${CMAKE_CURRENT_SOURCE_DIR})
EOF
}
