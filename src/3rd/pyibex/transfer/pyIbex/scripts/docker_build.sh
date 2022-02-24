#!/bin/bash

set -e -x

cd /io
for PYBIN in /opt/python/*3*/bin; do
    "${PYBIN}/python" -m pip install vibes

    mkdir -p build_dir && cd build_dir
    cmake3 -DPYTHON_EXECUTABLE=${PYBIN}/python ..
    make -j2 && make test ARGS="-V" && make pip_package
    echo "copy wheel and clean build_dir"
    for whl in *.whl; do
        auditwheel repair "$whl" -w /io/wheelhouse/
    done
    cd ..
    rm -fr build_dir
done



# Install packages and test
for PYBIN in /opt/python/*3*/bin; do
    "${PYBIN}/python" -m pip   install pyibex --no-deps --no-index -f /io/wheelhouse
    (cd "$HOME"; "${PYBIN}/python" -m unittest discover pyibex.tests)
done
