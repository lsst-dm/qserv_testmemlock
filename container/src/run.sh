#!/bin/sh

# Check memory lock feture is enabled for large files (~10 GB)

# @author Fabrice JAMMES, IN2P3

set -e
# set -x

DIR=$(cd "$(dirname "$0")"; pwd -P)

LARGE_FILE=/qserv/data/mysql/LSST/Source_0.MYD
SMALL_FILE=/qserv/data/mysql/LSST/Object_0.frm

for f in $SMALL_FILE $LARGE_FILE;
do
    echo "Checking memory lock feature for $f"
    "$DIR"/memory_lock -i "$f"
done

