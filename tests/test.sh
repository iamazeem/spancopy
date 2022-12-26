#!/bin/bash

set -e

BUILD_DIR=${BUILD_DIR:-build}

SOURCE_DIR="./tests/source"
EXPECTED_DEST_DIR="./tests/expected-destination"
ACTUAL_DEST_DIR="/tmp/actual-destination"

rm -rf $ACTUAL_DEST_DIR
mkdir -p $ACTUAL_DEST_DIR

"$BUILD_DIR"/spancopy --threshold 100 --source $SOURCE_DIR --destination $ACTUAL_DEST_DIR

EXPECTED_DEST_SUBDIR=$(ls $EXPECTED_DEST_DIR | tr -d '\n')
ACTUAL_DEST_SUBDIR=$(ls $ACTUAL_DEST_DIR | tr -d '\n')

if diff -r "$EXPECTED_DEST_DIR/$EXPECTED_DEST_SUBDIR" "$ACTUAL_DEST_DIR/$ACTUAL_DEST_SUBDIR"; then
  rm -rf $ACTUAL_DEST_DIR
fi
