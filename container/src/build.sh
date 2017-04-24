#!/bin/sh

# Build memory_lock.cc

# @author Fabrice JAMMES, IN2P3

set -e
# set -x

DIR=$(cd "$(dirname "$0")"; pwd -P)

g++ -std=c++11 "$DIR"/memory_lock.cc -o "$DIR"/memory_lock

