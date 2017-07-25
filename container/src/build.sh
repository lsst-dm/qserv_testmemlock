#!/bin/sh

# Build memory_lock.cc

# @author Fabrice JAMMES, IN2P3

set -e
# set -x

DIR=$(cd "$(dirname "$0")"; pwd -P)

FILES=$(find "$DIR" -name "*.cc")

for f in $FILES; do
    OUT=$(basename "$f" .cc)
    g++ -std=c++11 "$f" -o "$DIR/$OUT"
done
