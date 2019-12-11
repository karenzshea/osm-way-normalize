#!/usr/bin/env bash

set -o errexit
set -o pipefail
set -o nounset

readonly LIBOSMIUM_URL="https://github.com/osmcode/libosmium/archive/v2.15.0.tar.gz"
readonly LIBOSMIUM_DIR="libosmium"
readonly PROTOZERO_URL="https://github.com/mapbox/protozero/archive/v1.6.5.tar.gz"
readonly PROTOZERO_DIR="protozero"

mkdir -p third_party/${LIBOSMIUM_DIR}
mkdir -p third_party/${PROTOZERO_DIR}
pushd third_party
wget --quiet -O - ${LIBOSMIUM_URL} | tar --strip-components=1 -xz -C ${LIBOSMIUM_DIR}
wget --quiet -O - ${PROTOZERO_URL} | tar --strip-components=1 -xz -C ${PROTOZERO_DIR}
popd
