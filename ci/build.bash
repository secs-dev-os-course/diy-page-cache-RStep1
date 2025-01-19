#!/bin/bash

set -e

cd "$(dirname "$0")"/..

echo "Building the project..."

(cd build && make)

echo "Build completed successfully!"