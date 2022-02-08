#!/bin/bash
#
# <container> is a running <image>
# <image>     akin to program
# <container> akin to process
#
# IMAGE
# 
# export DOCKER_BUILDKIT=1                        ==> Use Docker's BuildKit on Linux Shell
# $env:DOCKER_BUILDKIT=1                          ==> Use Docker's BuildKit on Windows PowerShell
#
# docker build -t <image> <path>                  ==> create <image> based on Dockerfile located at <path>
# docker commit <container> <image>               ==> save changes from <container> into <image>
# docker tag <image> <repo>/<image>               ==> add <repo>/<image> as an alias to <image>
# docker rmi <image>                              ==> remove <image>
# docker save -o <file> <image>                   ==> save <image> to a <file>
# docker load -i <file>                           ==> load images from <file>
# docker images                                   ==> show the list of images available
#
# CONTAINER
# 
# docker run --name <container> <image>           ==> create <container> based on <image>
# docker run --name <container> -ti <image> <prog>==> create <container> based on <image>, then run <prog> into <container>
# docker ps                                       ==> show running images (aka containers)
# docker exec -ti <container> <prog>              ==> executes <prog> into <conainer>
# docker attach <container>                       ==> link another terminal to an existing <conainer> stdio
# docker kill <container>                         ==> stop <container>
# docker rm <container>                           ==> remove <container>
# docker logs <container>                         ==> show logs from stopped <container>
# docker port <container>                         ==> show ports used by <container>
# docker inspect <container>                      ==> show information about <container>
#
# REGISTRY
# 
# docker login                                    ==> login to registry (Docker Hub) so we can push and pull images
# docker push <repo>/<image>                      ==> send <image> to <repo> registry (Docker Hub)
# docker pull <repo>/<image>                      ==> download <image> from <repo> registry (Docker Hub)
# 
# NETWORK
# 
# docker network ls                               ==> list all networks
# docker network inspect bridge                   ==> inspect bridge network
# docker network inspect host                     ==> inspect host network
#

IMG_TAG=cpp-img
IMG_VER=latest

HOST_DIR=${PWD}
CONT_DIR=/usr/src/cpp-test

HOST_PORT=8080
CONT_PORT=8080

# Build image based on Dockerfile and add a name-tag
docker build -t ${IMG_TAG} .

# Run image with terminal interactive (-ti)
# docker run -ti ${IMG_TAG}:${IMG_VER} bash

# Run image with interactive terminal and volume (-v)
# docker run -v ${HOST_DIR}:${CONT_DIR} -ti ${IMG_TAG}:${IMG_VER} bash

# Run image with interactive terminal, volume, port (-p), and export variable (-e)
# docker run -v ${HOST_DIR}:${CONT_DIR} -p ${HOST_PORT}:${CONT_PORT} -e PORT=${CONT_PORT} -ti ${IMG_TAG}:${IMG_VER} bash

# Run Web Interface
# docker run --net=host --env="DISPLAY" --port=8080:8080 -ti ${IMG_TAG}:${IMG_VER} bash

# Run GUI Interface
# docker run --user=$(id -u $USER):$(id -g $USER) \
#            --env="DISPLAY" \
#            --volume="/tmp/.X11-unix:/tmp/.X11-unix:rw" \
#            --volume="/etc/group:/etc/group:ro" \
#            --volume="/etc/passwd:/etc/passwd:ro" \
#            --volume="/etc/shadow:/etc/shadow:ro" \
#            --volume="/etc/sudoers.d:/etc/sudoers.d:ro" \
#            -it -ti ${IMG_TAG}:${IMG_VER} bash
#
# Another way is to give root access to xhost:
#   xhost +local:root
#   docker build -t ${IMG_TAG} .
#   xhost -local:root
#
exit 0
