#!/bin/bash
# Inspired from pyIbex (Benoît Desrochers)

mkdir -p /io/cache
cd /io/cache
if [ ! -d "/io/cache/cmake-3.13.4" ]; then
  curl -L https://github.com/Kitware/CMake/releases/download/v3.13.4/cmake-3.13.4.tar.gz --output cmake-3.13.4.tar.gz
  tar -zxf cmake-3.13.4.tar.gz
  cd cmake-3.13.4
  ./bootstrap && make -j && make install
else
  cd /io/cache/cmake-3.13.4
  make install
fi