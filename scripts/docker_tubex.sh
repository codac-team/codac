#!/bin/bash

git submodule init ; git submodule update # for pybind11 submodule
mkdir build -p
cd build

cmake -DCMAKE_CXX_FLAGS="-fPIC -msse3" -DCMAKE_C_FLAGS="-fPIC -msse3" -DCMAKE_INSTALL_PREFIX=/io/tubex-lib/build_install -DCMAKE_PREFIX_PATH=/io/cache/ibex-lib/build_install -DBUILD_TESTS=ON -DWITH_TUBE_TREE=OFF -DWITH_CAPD=OFF -DPYTHON_VERSION="$py_version" ..
make -j
#make api
#make doc
make install