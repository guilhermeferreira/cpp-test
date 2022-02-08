#!/bin/bash

# Uninstall old versions. Older versions of Docker were called docker,
# docker.io, or docker-engine
sudo apt-get remove docker docker-engine docker.io containerd runc

# Update the apt package index and install packages to allow apt to use a
# repository over HTTPS
sudo apt-get update
sudo apt-get install \
     apt-transport-https \
     ca-certificates \
     curl \
     gnupg-agent \
     software-properties-common

# Add Docker’s official GPG key
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -

# Verify that you now have the key with the fingerprint
# 9DC8 5822 9FC7 DD38 854A  E2D8 8D81 803C 0EBF CD88
sudo apt-key fingerprint 0EBFCD88

# Set up the stable repository
sudo add-apt-repository \
    "deb [arch=amd64] https://download.docker.com/linux/ubuntu \
    $(lsb_release -cs) \
    stable"

# Update the apt package index, and install the latest version of Docker Engine
# and containerd
sudo apt-get update
sudo apt-get install docker-ce docker-ce-cli containerd.io

# Verify that Docker Engine is installed correctly
sudo docker run hello-world

# Create the docker group, add current user (to avoid sudo tu run docker), and
# activate the changes to groups
sudo groupadd docker
sudo usermod -aG docker $USER
newgrp docker

# Verify that Docker Engine runs without sudo
docker run hello-world

exit 0
