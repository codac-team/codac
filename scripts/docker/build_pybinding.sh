#!/bin/bash

set -e -x

cd /io
for PYBIN in /opt/python/cp3*/bin; do
  "${PYBIN}/python" -m pip install --upgrade pip
  "${PYBIN}/python" -m pip install --upgrade pyibex
  mkdir -p build_dir && cd build_dir
  cmake3 -DPYTHON_EXECUTABLE=${PYBIN}/python -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON -DWITH_TUBE_TREE=OFF -DWITH_CAPD=OFF -DWITH_PYTHON=ON -DCMAKE_CXX_FLAGS="-fPIC" ..
  make -j
  make test ARGS="-V"s
  make pip_package
  echo "copy wheel and clean build_dir"
  for whl in *.whl; do
    auditwheel repair "$whl" -w /io/wheelhouse/
  done
  
  "${PYBIN}/python" -m pip  install pytubex --no-deps --no-index -f /io/wheelhouse
  (cd "$HOME"; "${PYBIN}/python" -m unittest discover pytubex.tests)
  cd /io
  rm -fr build_dir
done