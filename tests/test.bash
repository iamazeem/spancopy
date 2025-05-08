#!/bin/bash

set -e

PREFIX=${PREFIX:-build}

SOURCE_DIR="./tests/source"
EXPECTED_DEST_DIR="./tests/expected-destination"
ACTUAL_DEST_DIR="/tmp/actual-destination"

rm -rf $ACTUAL_DEST_DIR
mkdir -p $ACTUAL_DEST_DIR

echo "[INF] Running tests..."

echo "[INF] TEST: no arguments (success)"
"$PREFIX"/spancopy >/dev/null

echo "[INF] TEST: version flag (success)"
"$PREFIX"/spancopy --version >/dev/null

echo "[INF] TEST: help flag (success)"
"$PREFIX"/spancopy --help >/dev/null

echo "[INF] TEST: invalid threshold (failure)"
"$PREFIX"/spancopy --threshold 10 --source $SOURCE_DIR --destination $ACTUAL_DEST_DIR || true

echo "[INF] TEST: valid threshold (success)"
"$PREFIX"/spancopy --threshold 100 --source $SOURCE_DIR --destination $ACTUAL_DEST_DIR

EXPECTED_DEST_SUBDIR=$(ls $EXPECTED_DEST_DIR | tr -d '\n')
ACTUAL_DEST_SUBDIR=$(ls $ACTUAL_DEST_DIR | tr -d '\n')

if diff -r "$EXPECTED_DEST_DIR/$EXPECTED_DEST_SUBDIR" "$ACTUAL_DEST_DIR/$ACTUAL_DEST_SUBDIR"; then
  rm -rf $ACTUAL_DEST_DIR
fi

echo "[INF] --- [DONE] ---"
