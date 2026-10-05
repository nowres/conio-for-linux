#!/bin/sh
# Build deb and rpm packages into $OUT (default: out/). Requires nfpm and a built dist/ (make -f build.mk).
set -eu
cd "$(dirname "$0")/.."
VERSION=${VERSION:-1.0.0}
ARCH=${ARCH:-amd64}
OUT=${OUT:-out}
mkdir -p "$OUT"
TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT

# build <template> <format> <pkg name> <libdir>
build() {
    sed "s|\${PKG_NAME}|$3|g; s|\${LIBDIR}|$4|g; s|\${VERSION}|$VERSION|g; s|\${ARCH}|$ARCH|g" \
        "packaging/$1" > "$TMP/$1"
    nfpm package -f "$TMP/$1" -p "$2" -t "$OUT/"
}

DEB_LIBDIR=/usr/lib/x86_64-linux-gnu
RPM_LIBDIR=/usr/lib64
build nfpm-runtime.yaml deb libconio1 "$DEB_LIBDIR"
build nfpm-dev.yaml deb libconio-dev "$DEB_LIBDIR"
build nfpm-runtime.yaml rpm libconio "$RPM_LIBDIR"
build nfpm-dev.yaml rpm libconio-devel "$RPM_LIBDIR"
