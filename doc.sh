# ==================================================================
#  tubex-lib - documentation script
# ==================================================================

#!/bin/bash

# install :
#  sudo apt-get install python3-sphinx doxygen graphviz
#  sudo pip install sphinx_rtd_theme

cd doc
make html
cd doxygen
doxygen Doxyfile
cd ../../


# Lines 489, 497
# HIDE_UNDOC_MEMBERS     = YES
# HIDE_UNDOC_CLASSES     = YES