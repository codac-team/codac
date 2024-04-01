#!/bin/bash

# Need to login in Docker Desktop before...?

mkdir -p ~/Downloads/newcodac

cd ~/Downloads/newcodac
rm -Rf codac*
git clone https://github.com/lebarsfa/codac
cd codac
git submodule init
git submodule update
chmod a+x scripts/docker/build_pybinding.sh
docker pull lebarsfa/manylinux2014_aarch64-for-codac
docker run --rm -v `pwd`:/io lebarsfa/manylinux2014_aarch64-for-codac /io/scripts/docker/build_pybinding.sh
