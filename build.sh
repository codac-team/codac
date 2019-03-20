# ==================================================================
#  tubex-lib - build script
# ==================================================================

#!/bin/bash

TUBEX_DIR="$(pwd)"

if [ $# -ne 0 ] && ([ $1 = "clean" ] || [ $2 = "clean" ]) # cleaning before build
then
  find . -name make | xargs rm -fr
fi

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

cd examples
if [ $# -ne 0 ] && ([ $1 = "examples" ] || [ $1 = "all" ]) # build examples
then
  find . -name "ex\_*" | xargs -L 1 bash -c 'cd "$0" && ./build.sh && cd "$TUBEX_DIR"/'
fi
cd ..