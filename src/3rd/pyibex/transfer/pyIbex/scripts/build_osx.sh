#!/bin/bash
set -e -x
eval "$(pyenv init -)"
mkdir -p wheelhouse
for PY_VERSION in "2.7.14" "3.4.9" "3.5.6" "3.6.8" "3.7.2"; do
  echo "BUILD FOR PYIBEX FOR PYTHON ${PY_VERSION}";
  mkdir -p build$PY_VERSION;
  cd build$PY_VERSION;
  # export CXX=clang++ CC=clang;
  # if [ $PY_VERSION != "system" ]; then
  pyenv install -s $PY_VERSION;
  # fi;
  pyenv shell $PY_VERSION;
  python -m pip install --upgrade pip
  python -m pip install wheel;
  cmake -DIBEX_ROOT=$HOME/ibex ..;
  make -j2 && make test && make pip_package;
  mv *.whl ../wheelhouse
  cd ../
done;
