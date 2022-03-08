#!/bin/bash
source ../../args.sh

echo "Build NVIDIA docker image from BASE: $QT_IMAGE"
echo "Building NVIDIA image with name: $NVIDIA_IMAGE"

docker build --build-arg BASE_IMAGE=$QT_IMAGE --build-arg USER_NAME=$USERNAME -f nvidia.Dockerfile -t $NVIDIA_IMAGE .