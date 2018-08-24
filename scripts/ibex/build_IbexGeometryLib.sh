# ==================================================================
#  tubex-lib - IBEX-geometry installation
# ==================================================================

#!/bin/bash

echo 'Installing the IBEX-geometry...';
git clone https://github.com/benEnsta/ibex-geometry
cd ibex-geometry
mkdir make
cd make
cmake -DWITH_PYTHON=OFF -DBUILD_TESTS=OFF -DIBEX_INCLUDE_DIR=/usr/local/include/ibex/ -DFILIB_INCLUDE_DIR=/usr/local/include/ibex/3rd/ -DPC_IBEX_LIBRARY_DIRS=/usr/local/lib/ibex/3rd/ ..
sudo make install
cd ..
cd ..