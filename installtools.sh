#!/bin/bash

# apt-get update first
sudo apt-get -y update

# CMake
sudo apt-get -y install cmake

sudo apt-get -y install openjdk-8-jdk

sudo apt-get -y install swig

cd ~

git clone --recursive https://github.com/scottslewis/derecho-unified

mkdir Release
cd Release
cmake -DCMAKE_BUILD_TYPE=Release ..
make
