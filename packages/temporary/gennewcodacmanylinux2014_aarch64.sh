#!/bin/bash

set -e -x

mkdir -p ~/Downloads/newcodac

cd ~/Downloads/newcodac
rm -Rf codac*
#git clone https://github.com/lebarsfa/codac
git clone https://github.com/codac-team/codac
cd codac
git submodule init
git submodule update
docker pull lebarsfa/manylinux2014_aarch64-for-codac

chmod a+x scripts/docker/build_pybinding.sh
docker run --rm -v `pwd`:/io lebarsfa/manylinux2014_aarch64-for-codac /io/scripts/docker/build_pybinding.sh
ls wheelhouse

cd ..
