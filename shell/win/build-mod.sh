#!/bin/bash


make ARCH=x86 WINDRES='windres -F pe-i386' -j$(nproc) BASEGAME='ironstock-q3mod' BUILD_STANDALONE=1 BUILD_MISSIONPACK=0 BUILD_GAME_QVM=1
