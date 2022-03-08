#!/bin/bash

source args.sh

XAUTH=/tmp/.docker.xauth
if [ ! -f $XAUTH ]
then
    xauth_list=$(xauth nlist :0 | sed -e 's/^..../ffff/')
    if [ ! -z "$xauth_list" ]
    then
        echo $xauth_list | xauth -f $XAUTH nmerge -
    else
        touch $XAUTH
    fi
    chmod a+r $XAUTH
fi

xhost +
docker run -it \
    --env="DISPLAY=$DISPLAY" \
    --env="QT_X11_NO_MITSHM=1" \
    --volume="/tmp/.X11-unix:/tmp/.X11-unix:rw" \
    --env="XAUTHORITY=$XAUTH" \
    --volume="$XAUTH:$XAUTH:rw" \
    --volume="/dev:/dev" \
    --device-cgroup-rule='c 81:* rmw' \
    --runtime=nvidia \
    --net=host \
    --user="$USER:$USER" \
    --env="LANG=C.UTF-8" \
    --env="LC_ALL=C.UTF-8" \
    --env="QT_PLUGIN_PATH=/opt/Qt/$QT_VERSION/gcc_64/plugins" \
    --env="PATH=/opt/Qt/$QT_VERSION/gcc_64/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin" \
    --env="QML_IMPORT_PATH=/opt/Qt/$QT_VERSION/gcc_64/qml" \
    --env="QML2_IMPORT_PATH=/opt/Qt/$QT_VERSION/gcc_64/qml" \
    --env="PATH=/opt/Qt/Tools/QtCreator/bin/:/opt/Qt/$QT_VERSION/gcc_64/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin" \
    --privileged \
    --volume="$SOURCELOC:/home/$USER/workspace" \
    --volume="$DATA:/home/$USER/data" \
    --name $CONTAINER \
    $IMAGE \
    /bin/bash
