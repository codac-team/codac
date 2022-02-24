#!/bin/bash

set -x
# check to see if protobuf folder is empty
if [ ! -e "$HOME/ibex/lib/libibex-robotics-polar.a" ]; then
	git clone https://github.com/benEnsta/ibex-robotics.git
	cd ibex-robotics
	git checkout build_process
	mkdir build
	cd build
	cmake -DBUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX=${HOME}/ibex ../
	make -j2
	make install
else
  echo 'Using cached directory.';
fi
