#!/usr/bin/env bash
set -euo pipefail
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
ROOT=$(realpath "$DIR/../")

. "$DIR"/asn1c.sh

# shellcheck disable=SC2120
h () {
    # if arguments, print them
    [ $# == 0 ] || echo "$*"

  cat <<EOF
Usage: $0 [OPTION]... <J2735.ASN|ASN_DIR|ASN_FILE...>
Regenerate src/j2735 from ASN using mouse07410/asn1c
  -h, --help       display this help and exit
  -k, --keep       keep temp directory around
  -d, --dir=[DIR]  use specific dir instead of generated tempdir, implies -k
EOF

    # if args, exit 1 else exit 0
    [ $# == 0 ] || exit 1
    exit 0
}

# getopt short options go together, long options have commas
TEMP=$(getopt -o hd:k --long help,dir:,keep -n "$0" -- "$@")
#shellcheck disable=SC2181
if [ $? != 0 ] ; then
    die "something wrong with getopt"
fi
eval set -- "$TEMP"

dir=""
keep=false
while true ; do
    case "$1" in
        -h|--help) h ;;
        -d|--dir) dir="$2" ; keep=true ; shift 2 ;;
        -k|--keep) keep=true ; shift ;;
        --) shift ; break ;;
        *) echo "Internal error, unexpected argument '$0'!" ; exit 1 ;;
    esac
done

if [ $# -eq 0 ]; then
    echo "need to pass in asn1 file(s)"
    exit 1
fi

if [[ $# -eq 1 ]] && [[ -d "$1" ]]; then
    shopt -s nullglob
    #shellcheck disable=SC2206
    inputFiles=("$1"/*)
else
    inputFiles=("$@")
fi



tmpDir=$(chooseTmpDir "$dir")
handleTmpDir "$tmpDir" "$keep"
buildASN1c "$tmpDir"

compileASN "$tmpDir" j2735 "${inputFiles[@]}"

rm -rf "$ROOT/src/j2735"
cp -r "$tmp_dir/output" "$ROOT/src/j2735"
