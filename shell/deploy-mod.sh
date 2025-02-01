#!/bin/bash

echo "Generating PAK file"
echo ""

CWD=$(pwd)

TITLE="ironstock"
TARGET="release-linux-x86_64"
BUILD_PATH="$CWD/build/$TARGET/$TITLE"
PAK_SRC="$CWD/mod_data/pak"
PAK_DST="$BUILD_PATH/pak0.pk3"
rm $PAK_DST
cd $PAK_SRC
zip -r "$PAK_DST" .
cd $CWD

echo "Done"
echo ""


SRC_CONFIGS="$CWD/mod_data/configs"

echo "Deploying mod description"
cp "$SRC_CONFIGS/description.txt" "$BUILD_PATH/description.txt"

echo "Deploying default config"
cp "$SRC_CONFIGS/default.cfg" "$BUILD_PATH/default.cfg"

echo "Deploying auto exec config"
cp "$SRC_CONFIGS/autoexec.cfg" "$BUILD_PATH/autoexec.cfg"

echo ""
echo "Deployment complete"
