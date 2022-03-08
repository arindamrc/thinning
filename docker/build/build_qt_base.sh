#!/bin/bash

# Build base image
source ../args.sh

docker build --build-arg UBUNTU_VERSION=$UBUNTU_VERSION -t qt-base -f qt.Dockerfile .