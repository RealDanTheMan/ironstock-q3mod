#!/bin/bash

CWD=$(pwd)
TARGET="release-linux-x86_64"
BUILD_PATH="$CWD/build/$TARGET"
XFILE="$BUILD_PATH/ioquake3.x86_64"

"$XFILE" +set fs_basegame ironstock +fs_game ironstock +set developer 1 +set logfile 2 +set fs_debug 1
