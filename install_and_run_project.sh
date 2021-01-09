#!/usr/bin/env bash

sudo apt-get -y install make
sudo apt-get -y install cmake
sudo apt-get -y install libncurses-dev

sudo cmake "CMakeLists.txt"
sudo make
sudo chmod +x schiffe_versenken
./schiffe_versenken

