# Installation

## Conda (Best way to install)

 **[conda](http://conda.pydata.org/docs/)**
Builds for osx-64,linux-64,linux-32,win-64,win-32 for Python 2.7, Python 3.4, and Python 3.5 are all available.

``` bash
conda install -c BenEnsta pyibex
```

## Using pip
Best way to get pyibex is to install it using
``` python
pip install pyibex
```
On linux system, the pip version must be greater than 9.0 in order to support
pre-compiled binaries (.whl)

<!-- ## Using apt-get (Ubuntu 16.04 / 14.04)
```bash
sudo add-apt-repository ppa:ben-ensta/pyibex
sudo apt-get update
sudo apt install python3-pyibex
``` -->

## Build from sources

### Dependancies
+ [CMake](http://www.cmake.org "CMake project page") (>= 2.8.3)
+ [Python](http://www.python.org "Python home page") (tested with 3.4, but should work with >= 2.7)
+ [pybind11](https://github.com/wjakob/pybind11) required C++11 support
+ [ibex-lib](http://www.ibex-lib.org/) custom version from [github](https://github.com/benEnsta/ibex-lib/tree/pyIbex_version_3)
+ a C++ compiler for your platform with c++11 support, e.g. [GCC](http://gcc.gnu.org "GCC home") (>= 4.8), [MSVS 13](https://www.visualstudio.com "Visual Studio 2014"), [llvm](http://llvm.org "llvm")


### Linux/Mac OSX


To build pyIbex from source with python 3.4 run the following commands:

```bash
git clone https://github.com/benEnsta/pyIbex.git
cd pyIbex
install.sh  --python=3.4
```
You can change the install directory with `--prefix=${PREFIX}` option.
and add `/home/${PREFIX}/lib/python3/dist-packages` to PYTHONPATH



### For Windows Users (Win64 version)

+ Build Ibex
  Open the developer command prompt and compile ibex-lib
```bash
git clone https://github.com/benEnsta/ibex-lib.git
cd ibex-lib
git checkout with_cmake
mkdir build
cd build
cmake -G "Visual Studio 12 2013 Win64" -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=OFF ../
msbuild /P:Configuration=Release /p:RuntimeLibrary=MT_StaticRelease PACKAGE.vcxproj
```
The last command generates a installer for ibex-lib

+ build pyIbex with pyzo2015a
```bash
cd pyIbex
mkdir build
cd build
cmake -G "Visual Studio 12 2013 Win64" -DBOOST_ROOT=$$$$$$$$ -DIBEX_ROOT=$$$$$$$$ -DPYTHON_EXECUTABLE=$$$$$$$$ ../
msbuild /P:Configuration=Release /p:RuntimeLibrary=MT_StaticRelease INSTALL.vcxproj
```
