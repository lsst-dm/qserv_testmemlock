#!/bin/sh

# Check memory lock feature is enabled for small and large files (~10 GB)

# @author Fabrice JAMMES, IN2P3

set -e
set -x

LARGE_FILE=/qserv/data/mysql/LSST/Source_0.MYD
SMALL_FILE=/qserv/data/mysql/LSST/Object_0.frm

OPT="-i"

usage() {
  cat << EOD

  Usage: $(basename "$0") [options]

  Available options:
    -s          Small file name
    -l          Large file name
    -f          Only lock large file and wait for user to hit enter

    Check memory lock feature is enabled for small and large files (~10 GB)
    WARN: Spaces in filenames are not supported

EOD
}

# get the options
while getopts fl:s:h c ; do
    case $c in
            f) OPT="" ;;
            s) SMALL_FILE="$OPTARG" ;;
            l) LARGE_FILE="$OPTARG" ;;
            h) usage ; exit 0 ;;
            \?) usage ; exit 2 ;;
    esac
done
shift "$((OPTIND-1))"

if [ $# -ne 0 ] ; then
    usage
    exit 2
fi

DIR=$(cd "$(dirname "$0")"; pwd -P)

if [ "$OPT" ]; then
    FILES="$SMALL_FILE $LARGE_FILE"
else
    FILES=$LARGE_FILE
fi

for f in $FILES;
do
    echo "Checking memory lock feature for $f"
    "$DIR"/memory_lock $OPT "$f"
done

