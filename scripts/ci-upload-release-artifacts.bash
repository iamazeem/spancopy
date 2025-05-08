#!/bin/bash

set -e

echo "[INF] Running $0"

if [[ -z $CI || $CI != true ]]; then
  echo "[ERR] Run $0 in GitHub Actions CI!" 1>&2
  exit 1
fi

if [[ -z $TAG || -z $ARTIFACT_DIR ]]; then
  echo "[ERR] Set TAG and ARTIFACT_DIR before running $0 script." 1>&2
  exit 1
fi

echo "[INF] Listing release artifacts"
ls -hl "$ARTIFACT_DIR/spancopy-$TAG-"*

for ARTIFACT in "$ARTIFACT_DIR/spancopy-$TAG-"*; do
  if [[ -f $ARTIFACT ]]; then
    echo "[INF] Uploading artifact... [$ARTIFACT]"
    gh release upload "$GITHUB_REF_NAME" "$ARTIFACT"
    echo "[INF] Artifact uploaded successfully! [$ARTIFACT]"
  fi
done

echo "[INF] --- [DONE] ---"
