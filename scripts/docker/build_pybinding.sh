#!/bin/bash

set -e -x

# Quick fix for CentOS EOL since 2020-11-30...
#ls -l /etc/yum.repos.d
sed -i 's/mirrorlist=/#mirrorlist=/g' /etc/yum.repos.d/CentOS-Base.repo
sed -i 's/#baseurl=http:\x2F\x2Fmirror.centos.org\x2Fcentos\x2F$releasever/baseurl=http:\x2F\x2Fvault.centos.org\x2Fcentos\x2F6.10/g' /etc/yum.repos.d/CentOS-Base.repo
sed -i 's/mirrorlist=http:\x2F\x2Fmirrorlist.centos.org?arch=$basearch&release=6&repo=sclo-rh/baseurl=http:\x2F\x2Fvault.centos.org\x2Fcentos\x2F6.10\x2Fsclo\x2FSource\x2Frh\x2F/g' /etc/yum.repos.d/CentOS-SCLo-scl-rh.repo
sed -i 's/mirrorlist=http:\x2F\x2Fmirrorlist.centos.org?arch=$basearch&release=6&repo=sclo-sclo/baseurl=http:\x2F\x2Fvault.centos.org\x2Fcentos\x2F6.10\x2Fsclo\x2FSource\x2Fsclo\x2F/g' /etc/yum.repos.d/CentOS-SCLo-scl.repo
#cat /etc/yum.repos.d/CentOS-Base.repo
#cat /etc/yum.repos.d/CentOS-SCLo-scl-rh.repo
#cat /etc/yum.repos.d/CentOS-SCLo-scl.repo
#yum -y update

#yum -y install doxygen eigen3-devel
yum -y install doxygen wget

wget https://gitlab.com/libeigen/eigen/-/archive/3.3.4/eigen-3.3.4.zip -nv
unzip -q -o eigen-3.3.4.zip
cd eigen-3.3.4
mkdir build
cd build
cmake3 ..
cmake3 --build . --target install
cd ../..
rm -Rf eigen-3.3.4.zip
rm -Rf eigen-3.3.4

cd /io
for PYBIN in /opt/python/cp3*/bin; do
  
  #if [ "${PYBIN}" != "/opt/python/cp36-cp36m/bin" ]; then
  #  continue
  #fi

  "${PYBIN}/python" -m pip install --upgrade pip
  #"${PYBIN}/python" -m pip install --upgrade pyibex
  "${PYBIN}/python" -m pip install pyibex==1.9.2
  mkdir -p build_dir && cd build_dir
  cmake3 -DPYTHON_EXECUTABLE=${PYBIN}/python -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON -DWITH_TUBE_TREE=OFF -DWITH_CAPD=OFF -DWITH_PYTHON=ON -DCMAKE_CXX_FLAGS="-fPIC" ..
  make api
  # Again for doxygen2docstring:
  cmake3 -DPYTHON_EXECUTABLE=${PYBIN}/python -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON -DWITH_TUBE_TREE=OFF -DWITH_CAPD=OFF -DWITH_PYTHON=ON -DCMAKE_CXX_FLAGS="-fPIC" ..
  make -j2

  make test ARGS="-V"s
  make pip_package
  echo "copy wheel and clean build_dir"
  for whl in *.whl; do
    auditwheel repair "$whl" -w /io/wheelhouse/
  done
  
  "${PYBIN}/python" -m pip install codac --no-deps --no-index -f /io/wheelhouse
  (cd "$HOME"; "${PYBIN}/python" -m unittest discover codac.tests)
  cd /io
  rm -fr build_dir

done