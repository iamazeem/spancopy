#!/bin/bash

set -e

if [[ -z $BUILD_DIR || -z $TAR ]]; then
  echo "[ERR] Set BUILD_DIR and TAR env vars!"
  exit 1
fi

CXX=${CXX:-gcc}
STRIP=${STRIP:-strip}

echo "[INF] CXX:        $CXX"
echo "[INF] STRIP:      $STRIP"
echo "[INF] BUILD_DIR:  $BUILD_DIR"
echo "[INF] TAR:        $TAR"

echo "[INF] Building"
rm -rf ./"$BUILD_DIR"
cmake -S . -B "$BUILD_DIR"
cmake --build "$BUILD_DIR"
$STRIP ./"$BUILD_DIR"/spancopy*

echo "[INF] Compressing"
cd ./"$BUILD_DIR"
tar -czvf "$TAR" spancopy*
cd ..
ls -Gghl ./"$BUILD_DIR"/spancopy*

echo "--- [DONE] ---"
