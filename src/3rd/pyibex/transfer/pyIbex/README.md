[![Build Status](https://travis-ci.org/benEnsta/pyIbex.svg?branch=master)](https://travis-ci.org/benEnsta/pyIbex)
[![Build status](https://ci.appveyor.com/api/projects/status/ughoj13u2rve6mw0?svg=true)](https://ci.appveyor.com/project/benEnsta/pyibex)
[![Conda_Statux](https://anaconda.org/benensta/pyibex/badges/installer/conda.svg)](https://ci.appveyor.com/project/benEnsta/pyibex)
==========================
Ibex python binding
==========================

--------------------------
Introduction
--------------------------
pyIbex is a python binding of [ibex-lib](http://www.ibex-lib.org/). It aims at providing a basic interface
of Ibex types (Interval, IntervalVector, Function, Ctc, Sep, ...),  and high level functionalities such as
contractors and separators programming.

It uses [pybind11](https://github.com/wjakob/pybind11) to link C++ code to python.
It contains core functionalities of ibex and additional modules:
  + **geometry** to deal with geometrical constraints
    + Polygon separator "SepPolygon" [article](http://www.sciencedirect.com/science/article/pii/S0952197614000864)
    + Separator transformation "SepTransform"
    + Sepatator fix point "SepFixPpoint"
    + Polar contractor and separators "CtcPolar, SepPolarXY" [article](http://www.sciencedirect.com/science/article/pii/S0952197616301129)


See [pyibex website](http://www.ensta-bretagne.fr/desrochers/pyibex/) for more informations.


# Install with a precompiled version using pip

## With pip
```python
>>> python -m pip install pyibex
```

## try it.
A set of example can be found in the pyibex/example directory. to run them enter:
```python
>>> python -m pyibex.example.main
```


# Build From Source



## Dependancies

+ [CMake](http://www.cmake.org "CMake project page") (>= 2.8.12)
+ [Python](http://www.python.org "Python home page") (tested with 3.4, 3.5, 3.6, but should work with >= 2.7)
+ [pybind11](https://github.com/wjakob/pybind11) required C++11 support
+ [ibex-lib](http://www.ibex-lib.org/) custom version from [github](https://github.com/ibex-team/ibex-lib/tree/with_cmake)
+ a C++ compiler for your platform , e.g. [GCC](http://gcc.gnu.org "GCC home") (>= 4.8), [MSVS 13](https://www.visualstudio.com "Visual Studio 2014"), [llvm](http://llvm.org "llvm")


###Building
--------------------------------------

The build process is entirely based on cmake with the following options:

	- use -DCMAKE_INSTALL_PREFIX= to change the install destination
	- use -DCMAKE_BUILD_TYPE=DEBUG | RELEASE to change the compilation mode
	- use -DIBEX_ROOT= if ibex is installed in a non-standard directory
  - use -DPYTHON_VERSION= to set the target python version.

Sometime, you will have to manually specify python executable, include and libs directory using:

  -using -DPYTHON_EXECUTABLE to specify python executable path.

Alternatively, run the provided `script/install.sh` script.
remark : If Ibex isn't installed on the current machine, it will be downloaded, build and installed.

remark : Ibex must be compuled with the -fPIC option


### Linux/Mac OS X
--------------------------------------

If ibex-lib is not installed
```bash
git clone -b develop https://github.com/ibex-team/ibex-lib.git
cd ibex-lib
mkdir build && cd build
cmake -DCMAKE_CXX_FLAGS="-fPIC" -DCMAKE_C_FLAGS="-fPIC" ..
make && make check && make install
```

To install pyIbex run the following commands:
```bash
git clone https://github.com/benEnsta/pyIbex.git
cd pyIbex
git submodule init
mkdir -p build && cd build
make && make test
# generate the python package
make pip_package
# or go to the package directory and install in dev mode
cd ./src/python_package
python ./setup.py develop
```

###For Windows Users (Win64 version) (Not ready Yet)
--------------------------------------
+ Build Ibex
  Open the developer command prompt and compile ibex-lib
```bash
git clone -b with_cmake https://github.com/benEnsta/ibex-lib.git
cd ibex-lib
mkdir build
cd build
cmake -G "Visual Studio 12 2013 Win64" -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=OFF ../
msbuild /P:Configuration=Release /p:RuntimeLibrary=MT_StaticRelease PACKAGE.vcxproj
```

The last command generates a installer for ibex-lib
```bash
cd pyIbex
mkdir build
cd build
cmake -G "Visual Studio 12 2013 Win64" -DIBEX_ROOT=$$$$$$$$ -DPYTHON_EXECUTABLE=$$$$$$$$ ../
msbuild /P:Configuration=Release /p:RuntimeLibrary=MT_StaticRelease INSTALL.vcxproj
```

--------------------------
Tutorial
--------------------------
###Basic types manipulation

```python
# Load pyIbex lib
from pyIbex import *

# Create new Intervals
a = Interval.EMPTY_SET
a = Interval.ALL_REALS
a  = Interval(-2, 3)

# Create IntervalVector
b = IntervalVector( 2, a)
c = IntervalVector([1,2,3])
d = IntervalVector([[-1,3], [3,10], [-3, -1]])

e = IntervalVector( (a, Interval(-1,0), Interval(0)) )

# Operations
e = c & d
e = c+d
e = a * c
```

###Functions and contractors manipulation
```python
# Define a Function from an equation
f = Function("x", "y", "x^2 + y^2 - 3")

# FwdBwd Contractor
ctc1 = CtcFwdBwd(f, CmpOp.LEQ, FwdMode.AFFINE_MODE)

# CtcIn/CtcOut contractors :math:`$f \in [-2.5, 3.5]$`
ctcIn = CtcIn(f, Interval(3).inflate(0.5))
ctcOut = CtcNotIn(f, Interval(3).inflate(0.5))

# Operations on Contractors
# composition of two contractor
ctc = ctcIn & ctcOut
# union of a python list of contractors
ctc1 = CtcUnion([ctcIn, ctcOut, ctc1])

```
