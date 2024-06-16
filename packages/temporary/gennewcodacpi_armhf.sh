#!/bin/bash

set -e -x

mkdir -p ~/Downloads/newcodac

cd ~/Downloads/newcodac
sudo rm -Rf codac*
#git clone https://github.com/lebarsfa/codac
git clone https://github.com/codac-team/codac
cd codac
git submodule init
git submodule update

for DIST in bookworm bullseye buster; do

docker pull lebarsfa/pi:$DIST-for-codac
docker run --rm -v `pwd`:/io lebarsfa/pi:$DIST-for-codac /bin/bash -c "uname -a ; cat /etc/os-release ; lsb_release -a ; \
if [ \"\$(lsb_release -cs)\" = \"bookworm\" ]; then \
 PIP_OPTIONS=--break-system-packages ; \
 #python3 -m pip install \$PIP_OPTIONS --upgrade \"auditwheel\" ; \\
else \
 PIP_OPTIONS= ; \
 #python3 -m pip install \$PIP_OPTIONS --upgrade \"auditwheel==5.1.2\" ; \\
fi && \
sudo apt-get -q update --allow-releaseinfo-change ; sudo apt-get -y install libeigen3-dev python3-dev patchelf || true && \
python3 -m pip install \$PIP_OPTIONS --upgrade patchelf --prefer-binary --extra-index-url https://www.piwheels.org/simple && \
python3 -m pip install \$PIP_OPTIONS --upgrade auditwheel --prefer-binary --extra-index-url https://www.piwheels.org/simple && \
# wget https://github.com/lebarsfa/ibex-lib/releases/download/ibex-2.8.9.20240417/ibex_armhf_\$(lsb_release -cs).zip --no-check-certificate -nv is causing illegal instruction on a Mac M1... \\
curl -L -O https://github.com/lebarsfa/ibex-lib/releases/download/ibex-2.8.9.20240417/ibex_armhf_\$(lsb_release -cs).zip --insecure && \
unzip -q ibex_armhf_\$(lsb_release -cs).zip && \
rm -Rf ibex_armhf_\$(lsb_release -cs).zip && \
sudo cp -Rf ibex/* /usr/local/ && \
\
git config --global --add safe.directory /io && \
cd /io && \
\
python3 -m pip install \$PIP_OPTIONS --upgrade pip && \
mkdir -p build_dir_\$(lsb_release -cs) && cd build_dir_\$(lsb_release -cs) && \
cmake -E env CXXFLAGS=\"-fPIC\" CFLAGS=\"-fPIC\" cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON -DWITH_CAPD=OFF -DWITH_PYTHON=ON .. && \
make -j4 && \
\
make pip_package && \
echo \"copy wheel and clean build_dir_\$(lsb_release -cs)\" && \
for whl in *.whl; do \
 # auditwheel might fail, so instead just copy... \\
 auditwheel repair \"\$whl\" -w /io/wheelhouse/ || cp -f \"\$whl\" /io/wheelhouse/\"\$whl\" ; \
done ; \
#for file in /io/wheelhouse/*armv6l.whl; do \\
# cp \"\$file\" \"\${file/armv6l/armv7l}\" ; \\
#done ; \\
#for file in /io/wheelhouse/*armv7l.whl; do \\
# cp \"\$file\" \"\${file/armv7l/armv6l}\" ; \\
#done ; \\
\
python3 -m pip install \$PIP_OPTIONS codac --no-deps --no-index -f /io/wheelhouse && \
python3 -m ../examples/02_centered_form/main.py && \
# Prerequisites for numpy. \\
sudo apt-get -y install libatlas3-base || true && \
sudo apt-get -y install libopenblas0-pthread || true && \
sudo apt-get -y install libgfortran5 || true && \
python3 -m pip install \$PIP_OPTIONS numpy --prefer-binary --extra-index-url https://www.piwheels.org/simple && \
#(cd \"\$HOME\"; python3 -m unittest discover codac.tests) && \\
if [ \"\$(lsb_release -cs)\" = \"buster\" ]; then \
 echo \"TESTS DISABLED FOR BUSTER DUE TO CATCH2\" ; \
else \
 make test ARGS=\"-V --output-on-failure\" && \
 echo \"start of Testing/Temporary/LastTest.log\" && \
 cat Testing/Temporary/LastTest.log && \
 echo \"end of Testing/Temporary/LastTest.log\" ; \
fi && \
\
cd /io && \
rm -fr build_dir_\$(lsb_release -cs)"
ls wheelhouse

done

cd ..
