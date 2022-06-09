#!/bin/bash

set -e -x

git config --global --add safe.directory /io
cd /io
for PYBIN in /opt/python/cp3*/bin; do
  
  #if [ "${PYBIN}" = "/opt/python/cp310-cp310/bin" ]; then
  #  continue
  #fi

  "${PYBIN}/python" -m pip install --upgrade pip
  mkdir -p build_dir && cd build_dir
  cmake -E env CXXFLAGS="-fPIC" CFLAGS="-fPIC" cmake -DPYTHON_EXECUTABLE=${PYBIN}/python -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON -DWITH_TUBE_TREE=OFF -DWITH_CAPD=OFF -DWITH_PYTHON=ON ..
  make -j2

  make test ARGS="-V"s
  make pip_package
  echo "copy wheel and clean build_dir"
  for whl in *.whl; do
    auditwheel repair "$whl" -w /io/wheelhouse/
  done
  
  "${PYBIN}/python" -m pip install numpy
  "${PYBIN}/python" -m pip install codac --no-deps --no-index -f /io/wheelhouse
  (cd "$HOME"; "${PYBIN}/python" -m unittest discover codac.tests)
  cd /io
  rm -fr build_dir

done
