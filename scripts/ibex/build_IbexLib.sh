# ==================================================================
#  tube-lib - IBEX-lib installation
# ==================================================================

#!/bin/bash

#set -x # debugging

if [ ! -e "$HOME/ibex/lib/libibex.a" ]; then
  echo 'Installing the IBEX-lib...';
  git clone https://github.com/benEnsta/ibex-lib.git
  cd ibex-lib
  git checkout pyIbex_version_3
  mkdir build
  cd build
  cmake -DBUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX=${HOME}/ibex ../
  make -j4
  make install
fi