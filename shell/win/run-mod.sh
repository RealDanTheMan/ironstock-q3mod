#!/bin/bash

CWD=$(pwd)
BUILD_PATH="$CWD/build/release-mingw32-x86"
XFILE="$BUILD_PATH/ioquake3.x86"

"$XFILE" +set fs_basegame ironstock-q3mod +fs_game ironstock-q3mod +set developer 1 +set logfile 2 +set fs_debug 1
