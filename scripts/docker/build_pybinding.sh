#!/bin/bash

set -e -x

wget https://github.com/lebarsfa/ibex-lib/releases/download/ibex-2.8.9.20260819/ibex_$(uname -m)_manylinux_2_28.zip --no-check-certificate -nv
unzip -q ibex_$(uname -m)_manylinux_2_28.zip
rm -Rf ibex_$(uname -m)_manylinux_2_28.zip
sudo cp -Rf ibex/* /usr/local/

git config --global --add safe.directory /io
cd /io

# Read optional constraints from environment
# MIN_PYTHON and MAX_PYTHON expected as strings like "3.8" or "3.10"
MIN_PYTHON="${MIN_PYTHON:-}"
MAX_PYTHON="${MAX_PYTHON:-}"

# helper function: returns 0 if version is within [min,max], 1 otherwise
# uses Python for robust parsing/comparison
is_version_allowed() {
  local ver="$1"   # e.g. "3.9"
  local min="$2"   # e.g. "3.8" or empty
  local max="$3"   # e.g. "3.12" or empty

  # If neither bound provided, allow
  if [ -z "$min" ] && [ -z "$max" ]; then
    return 0
  fi

  # Use Python to compare versions (handles multi-digit minors)
  "${PYBIN}/python" - <<PYCODE
import sys
from packaging.version import Version, InvalidVersion

ver = Version("$ver")
try:
    if "$min":
        minv = Version("$min")
    else:
        minv = None
    if "$max":
        maxv = Version("$max")
    else:
        maxv = None
except InvalidVersion:
    sys.exit(1)

if minv and ver < minv:
    sys.exit(2)
if maxv and ver > maxv:
    sys.exit(3)
sys.exit(0)
PYCODE

  rc=$?
  if [ $rc -eq 0 ]; then
    return 0
  else
    return 1
  fi
}

for PYBIN in /opt/python/cp3*/bin; do
  
  if [ "${PYBIN}" = "/opt/python/cp36-cp36m/bin" ] || [ "${PYBIN}" = "/opt/python/cp37-cp37m/bin" ]; then
    continue
  fi

  # get the interpreter's major.minor (e.g. "3.9")
  if [ ! -x "${PYBIN}/python" ]; then
    echo "Skipping ${PYBIN} (no python executable)"
    continue
  fi

  PY_VER=$("${PYBIN}/python" -c 'import sys; print("{}.{}".format(sys.version_info[0], sys.version_info[1]))')

  # If MIN_PYTHON or MAX_PYTHON set, check allowed range
  if [ -n "${MIN_PYTHON}" ] || [ -n "${MAX_PYTHON}" ]; then
    if ! is_version_allowed "${PY_VER}" "${MIN_PYTHON}" "${MAX_PYTHON}"; then
      echo "Skipping ${PYBIN} (python ${PY_VER} outside allowed range ${MIN_PYTHON}-${MAX_PYTHON})"
      continue
    fi
  fi

  "${PYBIN}/python" -m pip install --upgrade pip
  "${PYBIN}/python" -m pip install --upgrade wheel setuptools
  mkdir -p build_dir && cd build_dir
  cmake -E env CXXFLAGS="-fPIC" CFLAGS="-fPIC" cmake -DPYTHON_EXECUTABLE=${PYBIN}/python -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON -DWITH_CAPD=OFF -DWITH_PYTHON=ON -DPYBIND11_FINDPYTHON=OFF ..
  make -j4

  make pip_package
  echo "copy wheel and clean build_dir"
  for whl in *.whl; do
    auditwheel repair "$whl" -w /io/wheelhouse/
  done
  
  "${PYBIN}/python" -m pip install codac --no-deps --no-index -f /io/wheelhouse
  "${PYBIN}/python" ../examples/02_centered_form/main.py
  "${PYBIN}/python" -m pip install numpy --prefer-binary
  "${PYBIN}/python" -m pip install sympy
  "${PYBIN}/python" -m unittest discover codac.tests
  
  make test ARGS="-V --output-on-failure"
  echo "start of Testing/Temporary/LastTest.log"
  cat Testing/Temporary/LastTest.log
  echo "end of Testing/Temporary/LastTest.log"

  cd /io
  rm -fr build_dir

done
