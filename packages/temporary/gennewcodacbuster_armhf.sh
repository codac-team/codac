#!/bin/bash

# Need to login in Docker Desktop before...?

mkdir -p ~/Downloads/newcodac

cd ~/Downloads/newcodac
rm -Rf codac*
git clone https://github.com/lebarsfa/codac
cd codac
git submodule init
git submodule update
docker pull lebarsfa/pi:buster-for-codac
docker run --rm -v `pwd`:/io lebarsfa/pi:buster-for-codac /bin/bash -c "uname -a ; cat /etc/os-release ; lsb_release -a ; \
python3 -m pip install --upgrade auditwheel==5.1.2 && \
sudo apt-get -q update --allow-releaseinfo-change ; sudo apt-get -y install libeigen3-dev python3-dev patchelf || true && \
wget https://github.com/lebarsfa/ibex-lib/releases/download/ibex-2.8.9.20240417/ibex_armhf_buster.zip --no-check-certificate -nv && \
unzip -q ibex_armhf_buster.zip && \
rm -Rf ibex_armhf_buster.zip && \
sudo cp -Rf ibex/* /usr/local/ && \
\
git config --global --add safe.directory /io && \
cd /io && \
\
python3 -m pip install --upgrade pip && \
mkdir -p build_dir && cd build_dir && \
cmake -E env CXXFLAGS=\"-fPIC\" CFLAGS=\"-fPIC\" cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON -DWITH_TUBE_TREE=OFF -DWITH_CAPD=OFF -DWITH_PYTHON=ON .. && \
make -j4 && \
\
make test ARGS=\"-V --output-on-failure\" && \
echo \"start of Testing/Temporary/LastTest.log\" && \
cat Testing/Temporary/LastTest.log && \
echo \"end of Testing/Temporary/LastTest.log\" && \
make pip_package && \
echo \"copy wheel and clean build_dir\" && \
for whl in *.whl; do \
auditwheel repair \"\$whl\" -w /io/wheelhouse/ ; \
done && \
\
python3 -m pip install numpy && \
python3 -m pip install codac --no-deps --no-index -f /io/wheelhouse && \
(cd \"\$HOME\"; python3 -m unittest discover codac.tests) && \
cd /io"
