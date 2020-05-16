#!/bin/bash
# Inspired from pyIbex (Benoît Desrochers)

cd /io/tubex-lib
git submodule init && git submodule update # for pybind11 submodule

export CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:/io/tubex-lib/build_install
export CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:/io/cache/ibex-lib/build_install
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib

for PYBIN in /opt/python/*/bin; do
  mkdir build -p && cd build
  cmake -DPYTHON_EXECUTABLE=${PYBIN}/python -DCMAKE_CXX_FLAGS="-fPIC -msse3" -DCMAKE_INSTALL_PREFIX=/io/tubex-lib/build_install -DCMAKE_PREFIX_PATH=/io/cache/ibex-lib/build_install -DBUILD_TESTS=ON -DWITH_TUBE_TREE=OFF -DWITH_CAPD=OFF -DWITH_PYTHON=ON -DPYTHON_VERSION="$py_version" .. && \
  make -j && make install && make pip_package # && make test ARGS="-V" && make pip_package
  ls
  cd ..
  bash make.sh all
  cd build
  make test
  echo "copy wheel and clean build_dir"
  for whl in *.whl; do
    auditwheel repair "$whl" -w /io/wheelhouse/
  done
  cd ..
  rm -fr build

  break
done

# Install packages and test
for PYBIN in /opt/python/*/bin; do
  "${PYBIN}/python" -m pip install pytubex --no-deps --no-index -f /io/wheelhouse
  (cd "$HOME"; "${PYBIN}/python" -m unittest discover pytubex.tests)

  break
done