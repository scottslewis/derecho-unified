#!/bin/bash

# apt-get update first
sudo apt-get -y update

# librdmacm-dev
sudo apt-get -y install librdmacm-dev
# ipverbs
sudo apt-get -y install libibverbs-dev
# libboost-dev
sudo apt-get -y install libboost-dev
# libboost-system-dev
sudo apt-get -y install libboost-system-dev

# CMake
sudo apt-get -y install cmake

