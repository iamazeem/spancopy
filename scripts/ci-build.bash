#!/bin/bash

set -e

echo "[INF] Running $0"

if [[ -z $TAG ]]; then
  echo "[ERR] Set TAG env var!" 1>&2
  exit 1
fi

if [[ -z $PREFIX ]]; then
  echo "[ERR] Set PREFIX env var!" 1>&2
  exit 1
fi

ARTIFACT_DIR=${ARTIFACT_DIR:-.artifacts}
mkdir -p "$ARTIFACT_DIR"

CXX=${CXX:-g++}
STRIP=${STRIP:-strip}

TAR="spancopy-$TAG-$PREFIX.tar.gz"
ZIP="spancopy-$TAG-$PREFIX.zip"

echo "[INF] CXX:      $CXX"
echo "[INF] STRIP:    $STRIP"
echo "[INF] TAG:      $TAG"
echo "[INF] PREFIX:   $PREFIX"
echo "[INF] TAR:      $TAR"
echo "[INF] ZIP:      $ZIP"

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

echo "[INF] Listing build directory [$PREFIX]"
ls -hl ./"$PREFIX"/spancopy*

echo "[INF] Moving build artifacts to artifacts directory [$ARTIFACT_DIR]"
mv ./"$PREFIX"/"$TAR" ./"$PREFIX"/"$ZIP" "$ARTIFACT_DIR"

echo "[INF] Listing artifacts directory [$ARTIFACT_DIR]"
ls -hl ./"$ARTIFACT_DIR"

echo "[INF] --- [DONE] ---"
