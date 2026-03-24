#!/usr/bin/env bash

set -euo pipefail

LOCAL_PROFILES="$HOME/.conan2/profiles"
REPO_PROFILES="../profiles/cmake"

echo "Installing Conan profiles"

rm -rf "$LOCAL_PROFILES"
mkdir "$LOCAL_PROFILES"
cp -rf "$REPO_PROFILES/." "$LOCAL_PROFILES"

echo "Conan profiles successfully installed"
