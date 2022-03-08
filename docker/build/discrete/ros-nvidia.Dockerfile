ARG BASE_IMAGE
FROM $BASE_IMAGE
ARG UBUNTU_VERSION
ARG ROS_VERSION

MAINTAINER Arindam Roychoudhury <roychoud@cs.uni-bonn.de>

ENV DEBIAN_FRONTEND="noninteractive" 
ENV TZ="Europe/Berlin"

# Set up the ROS repository

# Add repo to sources.list
RUN sudo su -c "echo 'deb http://packages.ros.org/ros/ubuntu $UBUNTU_VERSION main' > /etc/apt/sources.list.d/ros-$ROS_VERSION.list"
# Set up keys
RUN sudo su -c "apt-key adv --keyserver 'hkp://keyserver.ubuntu.com:80' --recv-key C1CF6E31E6BADE8868B172B4F42ED6FBAB17C654"


# Install ROS packages
RUN sudo su -c "apt-get update && apt-get install -y ros-$ROS_VERSION-desktop-full"
