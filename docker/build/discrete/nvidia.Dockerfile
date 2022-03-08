ARG BASE_IMAGE
FROM $BASE_IMAGE
ARG USER_NAME
ARG FREEZE_IMAGE

RUN echo "USER NAME: $USER_NAME"

MAINTAINER Arindam Roychoudhury <roychoud@cs.uni-bonn.de>

RUN echo "Building docker image for NVIDIA..."

ENV DEBIAN_FRONTEND="noninteractive" 
ENV TZ="Europe/Berlin"

# nvidia-container-runtime
ENV NVIDIA_VISIBLE_DEVICES \
    ${NVIDIA_VISIBLE_DEVICES:-all}
ENV NVIDIA_DRIVER_CAPABILITIES \
    ${NVIDIA_DRIVER_CAPABILITIES:+$NVIDIA_DRIVER_CAPABILITIES,}graphics


# Build failing without the following:
RUN apt-get update && apt-get full-upgrade -y && apt-get install -y --no-install-recommends tzdata

# Dependencies for glvnd and X11.
RUN apt-get update \
  && apt-get install -y -qq --no-install-recommends \
    libglvnd0 \
    libgl1 \
    libglx0 \
    libegl1 \
    libxext6 \
    libx11-6 \
    libclang1 \
    mesa-utils \
    libgl1-mesa-glx \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    mesa-vulkan-drivers \
    mesa-vdpau-drivers \
    mesa-va-drivers \
    mir-platform-graphics-mesa-x16 \
    mir-client-platform-mesa-dev \
    libglx-mesa0 \
    libegl-mesa0 \
    libglapi-mesa \
    freeglut3-dev \
    mesa-common-dev \
    xdg-utils

# Install basic necessities libusb, git, OpenGL, OpenMP, OpenNI2, Eigen, GSL, BLAS, LAPACK, Armadillo, PCL and Qt.
RUN apt-get update && apt-get full-upgrade -y && apt-get install -y --no-install-recommends \
    git \
    ca-certificates \
    locales \
    sudo \
    cmake \
    build-essential \
    gdb \
    pkg-config \
    libxrender1 \
    libfontconfig1 \
    fonts-ubuntu \
    libxcb-xinerama0-dev \
    libomp-dev \
    libopenni2-dev \
    libeigen3-dev \
    libgsl-dev \
    libopenblas-dev \
    liblapack-dev \
    libarpack2-dev \
    libpcl-dev \
    libusb-1.0-0 \
    libusb-1.0-0-dev \
    && apt-get -qq clean

# Install QGLViewer
RUN apt-get -y update && DEBIAN_FRONTEND=noninteractive apt-get -y install \
    libqglviewer-headers \
    libqglviewer-dev-qt5 \
    libqglviewer2-qt5

# Install OpenCV
RUN apt-get update && apt-get full-upgrade -y && apt-get install -y --no-install-recommends libopencv-dev \
    python3-dev \
    python3-numpy \
    python3-opencv \
    libtbb-dev \
    pkg-config \
    && apt-get -qq clean


RUN ln -s /usr/include/opencv4/opencv2 /usr/include/opencv2

# Keep apt active for the development environment
RUN if [[ -z "$FREEZE_IMAGE" ]] ; then echo "Freezing image by disabling apt..." ; fi
RUN if [[ -z "$FREEZE_IMAGE" ]] ; then rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/* ; fi

# Reconfigure locale
RUN locale-gen en_US.UTF-8 && dpkg-reconfigure locales


# Add group & user + sudo
RUN groupadd -r $USER_NAME && useradd --create-home --gid $USER_NAME $USER_NAME 
RUN touch /etc/sudoers.d/$USER_NAME && echo $USER_NAME' ALL=NOPASSWD: ALL' > /etc/sudoers.d/$USER_NAME

RUN mkdir /home/$USER_NAME/workspace

USER $USER_NAME
WORKDIR /home/$USER_NAME/workspace
ENV HOME /home/$USER_NAME

CMD ["bash"]
