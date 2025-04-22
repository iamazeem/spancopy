#!/bin/bash

set -e

if [[ -z $PREFIX || -z $TAR || -z $ZIP ]]; then
  echo "[ERR] Set PREFIX and TAR env vars!"
  exit 1
fi

CXX=${CXX:-g++}
STRIP=${STRIP:-strip}

echo "[INF] CXX:        $CXX"
echo "[INF] STRIP:      $STRIP"
echo "[INF] PREFIX:     $PREFIX"
echo "[INF] TAR:        $TAR"
echo "[INF] ZIP:        $ZIP"

echo "[INF] Building"
rm -rf ./"$PREFIX"
cmake -S . -B "$PREFIX"
cmake --build "$PREFIX"
$STRIP ./"$PREFIX"/spancopy*

echo "[INF] Compressing"
cd ./"$PREFIX"
EXE="spancopy"
if [[ -f "spancopy.exe" ]]; then
  EXE="spancopy.exe"
fi
tar -czvf "$TAR" "$EXE"
zip "$ZIP" "$EXE"
cd ..
ls -Gghl ./"$PREFIX"/spancopy*

echo "--- [DONE] ---"
