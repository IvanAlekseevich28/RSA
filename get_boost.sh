#!/bin/bash

cd /tmp
wget https://boostorg.jfrog.io/artifactory/main/release/1.76.0/source/boost_1_76_0.tar.gz
tar xzvf boost_1_76_0.tar.gz
cd boost_1_76_0/
./bootstrap.sh --prefix=/usr/
./b2
sudo ./b2 install
