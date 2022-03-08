#!/bin/bash

USER="arc"
USERNAME="arc"
REPONAME="thinning"
UBUNTU_VERSION="focal"
ROS_VERSION="noetic"
IMAGE="$UBUNTU_VERSION-$ROS_VERSION-nvidia"
CONTAINER="shell-$REPONAME"
SOURCELOC="/home/arc/Sources/$REPONAME"
DATA="/home/arc/Datasets"
QT_VERSION="5.15.2"
NVIDIA_IMAGE="$UBUNTU_VERSION-nvidia"
QT_IMAGE_PREFIX="qt"
QT_IMAGE_SUFFIX=`echo $QT_VERSION | sed 's/\.//g'`
QT_IMAGE=$QT_IMAGE_PREFIX$QT_IMAGE_SUFFIX

echo "args imported"
