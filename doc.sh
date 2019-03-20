# ==================================================================
#  tubex-lib - documentation script
# ==================================================================

#!/bin/bash

# install :
#  sudo pip install sphinx_rtd_theme
#  sudo apt-get install doxygen graphviz

cd doc
make html
cd doxygen
doxygen Doxyfile
cd ../../


# Lines 489, 497
# HIDE_UNDOC_MEMBERS     = YES
# HIDE_UNDOC_CLASSES     = YES