#!/bin/bash

set -x
# check to see if protobuf folder is empty
if [ ! -e "$HOME/ibex/lib/libibex.a" ]; then
	git clone -b develop https://github.com/ibex-team/ibex-lib.git
	cd ibex-lib
	mkdir build && cd build
	cmake -DCMAKE_INSTALL_PREFIX=${HOME}/ibex cmake -DCMAKE_CXX_FLAGS="-fPIC" -DCMAKE_C_FLAGS="-fPIC" ..
	make -j2 && make install
else
  echo 'Using cached directory.';
fi
