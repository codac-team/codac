# ==================================================================
#  tubex-lib - IBEX-lib installation
# ==================================================================

#!/bin/bash

echo 'Installing the IBEX-lib...';
git clone https://github.com/ibex-team/ibex-lib
cd ibex-lib
./waf configure --with-debug --interval-lib=filib
sudo ./waf install
cd ..

##set -x # debugging
#
#if [ ! -e "$HOME/ibex/lib/libibex.a" ]; then
#  echo 'Installing the IBEX-lib...';
#  git clone https://github.com/benEnsta/ibex-lib.git
#  cd ibex-lib
#  git checkout pyIbex_version_3
#  mkdir build
#  cd build
#  cmake -DBUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX=${HOME}/ibex ../
#  make -j4
#  make install
#fi