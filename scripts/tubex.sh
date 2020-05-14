#!/bin/bash

git submodule init ; git submodule update # for pybind11 submodule
mkdir build -p
cd build

cmake -DCMAKE_INSTALL_PREFIX=$HOME/tubex-lib/build_install -DCMAKE_PREFIX_PATH=$HOME/ibex-lib/build_install -DBUILD_TESTS=ON -DWITH_TUBE_TREE=OFF -DWITH_CAPD=OFF -DPYTHON_VERSION="$py_version" ..
make
#make api
#make doc
#make install