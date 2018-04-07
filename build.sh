# ==================================================================
#  tubex-lib - build script
# ==================================================================

#!/bin/bash

TUBELIB_DIR="$(pwd)"

cd ..
sh $TUBELIB_DIR/scripts/ibex/build_IbexLib.sh
cd $TUBELIB_DIR

mkdir make -p
cd make
cmake -DCMAKE_INSTALL_PREFIX=~/ -DIBEX_ROOT=~/ibex -DCMAKE_CXX_COMPILER=g++-5 ..
make
cd ..