#!/bin/bash

cd $HOME
echo 'Installing IBEX in ' $HOME '...';
if [ ! -e "ibex-lib/README.md" ]; then
  git clone -b master https://github.com/lebarsfa/ibex-lib.git ;
  cd ibex-lib ;
  mkdir build && cd build ;
  cmake -E env CXXFLAGS="-fPIC" CFLAGS="-fPIC" cmake -DCMAKE_INSTALL_PREFIX=$HOME/ibex-lib/build_install -DCMAKE_BUILD_TYPE=Debug .. ;
  make -j 4 ;
else
  echo 'Using cached directory.' ;
fi
cd $HOME/ibex-lib/build
make install
cd $HOME