#!/bin/bash

source args.sh

xhost +

XAUTH_FILE="/tmp/.docker.xauth"

if [ -d "$XAUTH_FILE" ]; then
	echo "Removing xauth directory..."
	sudo rm -rf "$XAUTH_FILE"
fi 

if [ ! -f "$XAUTH_FILE" ]; then
	touch "$XAUTH_FILE"
fi

docker start $CONTAINER --attach --interactive
