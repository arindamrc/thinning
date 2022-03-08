#!/bin/bash

# Build qt base image (with the downloaded installer)
source ../args.sh


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


docker build --build-arg UBUNTU_VERSION=$UBUNTU_VERSION -t qt-base -f qt.Dockerfile .

# Build the final qt target image (by executing the downloaded installer)
echo "Creating QT image: $QT_IMAGE"

xhost +
docker run -it \
	--name qt_install \
	--privileged \
	--net=host \
    -u root:root \
    -e DISPLAY=unix$DISPLAY \
    -e LANG=C.UTF-8 \
    -e LC_ALL=C.UTF-8 \
	-e QT_X11_NO_MITSHM=1 \
	--device=/dev/dri:/dev/dri \
    --volume="/tmp/.X11-unix:/tmp/.X11-unix:rw" \
    qt-base \
    /bin/bash


docker commit qt_install $QT_IMAGE

docker rm qt_install

# Exiting from the above container will create a new qt image.


# Build NVIDIA image...

echo "Build NVIDIA docker image from BASE: $QT_IMAGE"
echo "Building NVIDIA image with name: $NVIDIA_IMAGE"

docker build --build-arg BASE_IMAGE=$QT_IMAGE --build-arg USER_NAME=$USERNAME -f discrete/nvidia.Dockerfile -t $NVIDIA_IMAGE ./discrete

# Add ROS and build the ros-nvidia image...

echo "Build ROS NVIDIA docker image..."

docker build --build-arg BASE_IMAGE=$NVIDIA_IMAGE --build-arg UBUNTU_VERSION=$UBUNTU_VERSION --build-arg ROS_VERSION=$ROS_VERSION -t $IMAGE -f discrete/ros-nvidia.Dockerfile ./discrete