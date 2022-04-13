#!/bin/bash

# Usage : 
#sh ./genlibcodac-dev.sh ubuntu bionic amd64 0.1 0 0

OS=$1
DIST=$2
ARCH=$3
VER=$4
DREV=$5
REV=$6

#sudo apt-get -y install dpkg-dev

cd $DIST/$ARCH
mkdir -p libcodac-dev/usr
cp -Rf ../../../codac/* libcodac-dev/usr/
sed_param=s/Version:\ .*/Version:\ ${VER}/  
sed -i "$sed_param" libcodac-dev/DEBIAN/control
sed_param=s/Architecture:\ .*/Architecture:\ ${ARCH}/
sed -i "$sed_param" libcodac-dev/DEBIAN/control
chmod 775 libcodac-dev/DEBIAN
dpkg-deb --build libcodac-dev
mv libcodac-dev.deb ../../../libcodac-dev-$VER-$DREV$DIST$REV\_$ARCH.deb
cd ../..
