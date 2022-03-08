#!/bin/bash
source ../../args.sh

echo "Build ROS NVIDIA docker image..."

if [[ -z "$NVIDIA_IMAGE" ]] 
then 
	echo "Base image not found: exiting..." 
	exit 1
else
	echo "Base image name: $NVIDIA_IMAGE"
fi

if [[ -z "$UBUNTU_VERSION" ]] 
then 
	echo "Ubuntu version not found: exiting..." 
	exit 1 
else
	echo "Ubuntu version: $UBUNTU_VERSION"
fi

if [[ -z "$ROS_VERSION" ]] 
then 
	echo "ROS version not found: exiting..." 
	exit 1
else
	echo "ROS version: $ROS_VERSION"
fi

docker build --build-arg BASE_IMAGE=$NVIDIA_IMAGE --build-arg UBUNTU_VERSION=$UBUNTU_VERSION --build-arg ROS_VERSION=$ROS_VERSION -t $IMAGE -f ros-nvidia.Dockerfile .