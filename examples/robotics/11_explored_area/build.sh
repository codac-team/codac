# ==================================================================
#  Codac - build script
# ==================================================================

#!/bin/bash

mkdir build -p
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make
cd ..