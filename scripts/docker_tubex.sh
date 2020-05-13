#!/bin/bash

cd /io/tubex-lib
git submodule init && git submodule update # for pybind11 submodule

for PYBIN in /opt/python/*/bin; do
  mkdir build -p && cd build
  cmake -DPYTHON_EXECUTABLE=${PYBIN}/python -DCMAKE_CXX_FLAGS="-fPIC -msse3" -DCMAKE_INSTALL_PREFIX=/io/tubex-lib/build_install -DCMAKE_PREFIX_PATH=/io/cache/ibex-lib/build_install -DBUILD_TESTS=ON -DWITH_TUBE_TREE=OFF -DWITH_CAPD=OFF -DWITH_PYTHON=ON -DPYTHON_VERSION="$py_version" .. && \
  make -j && make install # && make test ARGS="-V" && make pip_package
  #echo "copy wheel and clean build_dir"
  #for whl in *.whl; do
  #    auditwheel repair "$whl" -w /io/wheelhouse/
  #done
  cd ..
  rm -fr build
done

#RUN cd /io/tubex-lib && \
#    pwd && \
#    ls . && \
##    bash ./scripts/docker_tubex.sh
#    git submodule init && git submodule update && \
#    mkdir build -p && cd build && \
#    cmake -DCMAKE_CXX_FLAGS="-fPIC -msse3" -DCMAKE_INSTALL_PREFIX=/io/tubex-lib/build_install -DCMAKE_PREFIX_PATH=/io/cache/ibex-lib/build_install -DBUILD_TESTS=ON -DWITH_TUBE_TREE=OFF -DWITH_CAPD=OFF -DWITH_PYTHON=ON -DPYTHON_VERSION="$py_version" .. && \
#    make -j && \
#    make install
#
#git submodule init ; git submodule update # for pybind11 submodule
#mkdir build -p
#cd build
#
#cmake -DCMAKE_CXX_FLAGS="-fPIC -msse3" -DCMAKE_INSTALL_PREFIX=/io/tubex-lib/build_install -DCMAKE_PREFIX_PATH=/io/cache/ibex-lib/build_install -DBUILD_TESTS=ON -DWITH_TUBE_TREE=OFF -DWITH_CAPD=OFF -DWITH_PYTHON=ON -DPYTHON_VERSION="$py_version" ..
#make -j
##make api
##make doc
#make install