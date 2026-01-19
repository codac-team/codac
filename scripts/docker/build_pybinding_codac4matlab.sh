#!/bin/bash

set -e -x

wget https://github.com/lebarsfa/ibex-lib/releases/download/ibex-2.8.9.20250626/ibex_$(uname -m)_manylinux_2_28.zip --no-check-certificate -nv
unzip -q ibex_$(uname -m)_manylinux_2_28.zip
rm -Rf ibex_$(uname -m)_manylinux_2_28.zip
sudo cp -Rf ibex/* /usr/local/

git config --global --add safe.directory /io
cd /io
for PYBIN in /opt/python/cp3*/bin; do
  
  if [ "${PYBIN}" = "/opt/python/cp36-cp36m/bin" ] || [ "${PYBIN}" = "/opt/python/cp37-cp37m/bin" ]; then
    continue
  fi

  "${PYBIN}/python" -m pip install --upgrade pip
  "${PYBIN}/python" -m pip install --upgrade wheel setuptools
  mkdir -p build_dir && cd build_dir
  cmake -E env CXXFLAGS="-fPIC" CFLAGS="-fPIC" cmake -DPYTHON_EXECUTABLE=${PYBIN}/python -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON -DWITH_CAPD=OFF -DWITH_PYTHON=ON -DPYBIND11_FINDPYTHON=OFF ..
  make -j4

  make pip_package
  echo "copy wheel and clean build_dir"
  for whl in *.whl; do
    auditwheel repair "$whl" -w /io/wheelhouse/
  done
  
  "${PYBIN}/python" -m pip install codac4matlab --no-deps --no-index -f /io/wheelhouse
  "${PYBIN}/python" -c "import sys; print(sys.version); import codac4matlab; print(codac4matlab.__version__); from codac4matlab import *; print(IntervalVector([[-0.1],[0],[0.2]]))"
  #"${PYBIN}/python" ../examples/02_centered_form/main.py
  #"${PYBIN}/python" -m pip install numpy --prefer-binary
  #"${PYBIN}/python" -m unittest discover codac.tests
  
  #make test ARGS="-V --output-on-failure"
  #echo "start of Testing/Temporary/LastTest.log"
  #cat Testing/Temporary/LastTest.log
  #echo "end of Testing/Temporary/LastTest.log"

  cd /io
  rm -fr build_dir

done
