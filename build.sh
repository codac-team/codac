# ==================================================================
#  tubex-lib - build script
# ==================================================================

#!/bin/bash

mkdir make -p
cd make

if [ $# -ne 0 ] && ([ $1 = "tests" ] || [ $1 = "all" ]) # build tests
then
  cmake -DBUILD_TESTS=ON ..
else
  cmake -DBUILD_TESTS=OFF ..
fi

make
cd ..