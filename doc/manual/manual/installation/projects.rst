.. _sec-start-cpp-project:

Using Codac in my own C++ or Python project
===========================================

The previous pages explain how to *install* Codac. This one explains how to *use*
the installation you have just made from a project of your own: which files CMake
or ``pkg-config`` look for, which paths have to be given to them, and what a
minimal project looks like in C++ and in Python.

Everything below assumes an installation prefix, referred to as
``$CODAC_PREFIX``. If you followed :ref:`the installation from sources
<sec-install-cpp>` that is the directory you passed to
``-DCMAKE_INSTALL_PREFIX``, typically ``$HOME/codac/build_install``; if you
installed a Debian package or a standalone archive it is ``/usr`` or the
directory you extracted the archive into. Its layout is:

.. code-block:: text

   $CODAC_PREFIX/
   ├── include/
   │   ├── codac                     <- the umbrella header, for #include <codac>
   │   ├── codac-core/               <- one directory per module
   │   ├── codac-graphics/
   │   ├── codac-unsupported/
   │   └── eigen3/                   <- Eigen, part of Codac's public interface
   ├── lib/
   │   └── libcodac-core.a, libcodac-graphics.a, libcodac-unsupported.a
   └── share/
       ├── codac/cmake/
       │   ├── codac-config.cmake    <- what find_package(CODAC) reads
       │   └── codac-targets.cmake   <- the exported Codac:: targets
       └── pkgconfig/
           └── codac.pc              <- what pkg-config reads


.. _sec-start-cpp-project-cmake:

A C++ project with CMake
------------------------

This is the recommended route, and the one every project under ``examples/``
takes. Consider a project made of two files:

.. code-block:: text

   my_project/
   ├── CMakeLists.txt
   └── main.cpp

``main.cpp`` includes the single umbrella header and uses the ``codac2``
namespace:

.. code-block:: c++

  #include <codac>

  using namespace codac2;

  int main()
  {
    VectorVar x(2);
    AnalyticFunction f({x}, x[0]*cos(x[0]-x[1])+x[1]);
    SepInverse sep(f, {-oo,0});
    DefaultFigure::pave({{-10,10},{-10,10}}, sep, 0.1);
  }

and ``CMakeLists.txt`` is:

.. code-block:: cmake

  cmake_minimum_required(VERSION 3.14)
  project(my_project LANGUAGES CXX)

  # Codac is a C++20 library, so a project using it is compiled as C++20 too.
  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)

  # Where to look for Codac and for IBEX. Both are needed: codac-config.cmake
  # itself calls find_package(IBEX). Leave this out if they are installed in a
  # system directory, and prefer passing it on the command line (see below) to
  # hard-coding it here.
  # set(CMAKE_PREFIX_PATH "$ENV{HOME}/codac/build_install;$ENV{HOME}/ibex-lib/build_install")

  find_package(CODAC REQUIRED)
  message(STATUS "Found Codac version ${CODAC_VERSION}")

  # Applies the interval-arithmetic compilation flags of IBEX (-frounding-math
  # and the rest). Without them the rounding modes Codac relies on are not
  # guaranteed, and the results lose the very property the library is for.
  ibex_init_common()

  add_executable(${PROJECT_NAME} main.cpp)
  target_compile_options(${PROJECT_NAME} PUBLIC ${CODAC_CXX_FLAGS})
  target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${CODAC_INCLUDE_DIRS})
  target_link_libraries(${PROJECT_NAME} PUBLIC ${CODAC_LIBRARIES})

Which paths to give to CMake
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

``find_package(CODAC REQUIRED)`` looks for ``codac-config.cmake`` under the
prefixes listed in ``CMAKE_PREFIX_PATH``, in ``share/codac/cmake/`` — which is
exactly where Codac installs it. **IBEX has to be reachable the same way**, since
``codac-config.cmake`` calls ``find_package(IBEX REQUIRED)`` itself. Two prefixes
are therefore usually needed, and there are three ways to give them:

.. code-block:: bash

  # 1. On the configuration command line (recommended: nothing in the sources
  #    then depends on where a given machine happens to keep its libraries)
  cmake -DCMAKE_PREFIX_PATH="$HOME/codac/build_install;$HOME/ibex-lib/build_install" ..

  # 2. Through the environment, once and for all, e.g. in your .bashrc
  export CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:$HOME/ibex-lib/build_install
  export CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:$HOME/codac/build_install

  # 3. In the CMakeLists.txt itself, as in the commented line above
  #    (note the ";" separator: CMAKE_PREFIX_PATH is a CMake list, not a PATH)

.. admonition:: ``CODAC_DIR`` rather than ``CMAKE_PREFIX_PATH``

  ``-DCODAC_DIR=$CODAC_PREFIX/share/codac/cmake`` points CMake straight at the
  configuration file, bypassing the prefix search. It is useful to disambiguate
  between two installed versions, but it says nothing about IBEX, which still has
  to be found through ``CMAKE_PREFIX_PATH`` or ``IBEX_DIR``.

What ``find_package(CODAC)`` defines
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. list-table::
  :header-rows: 1
  :widths: 30 70

  * - Variable
    - Contents

  * - ``CODAC_VERSION``
    - The version of the installation that was found, *e.g.* ``2.1.1``.

  * - ``CODAC_INCLUDE_DIRS``
    - The include directories: the header root (through which ``<codac>`` is
      reached), ``include/eigen3``, and one directory per module.

  * - ``CODAC_LIBRARIES``
    - The libraries to link against. These are *imported targets*
      (``Codac::codac-core``, ``Codac::codac-graphics``,
      ``Codac::codac-unsupported``, plus ``Ibex::ibex`` and
      ``Threads::Threads``) rather than library paths, which is what lets CMake
      work out the link order for itself.

  * - ``CODAC_CXX_FLAGS``
    - The architecture flags Codac was compiled with (FMA, and so on). They have
      to be applied to your own translation units too: Eigen's headers are
      compiled by your project, and compiling them under different alignment and
      instruction-set assumptions than the installed archives were built with is
      an ODR/ABI mismatch.

  * - ``CODAC_CAPD_LIBRARY``, ``CODAC_SYMPY_LIBRARY``
    - Defined only when the corresponding optional module was built. They are
      already part of ``CODAC_LIBRARIES``; use them to opt into one module
      explicitly.

Building it
^^^^^^^^^^^

.. code-block:: bash

  cd my_project
  mkdir build ; cd build
  cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="$HOME/codac/build_install;$HOME/ibex-lib/build_install" ..
  cmake --build .
  ./my_project

On Visual Studio, replace the last two commands with
``cmake --build . --config Release``. If your program produces a graphical
output, remember to launch :ref:`the VIBes viewer <sec-graphics-vibes>` first.


.. _sec-start-cpp-project-pkgconfig:

A C++ project with pkg-config
-----------------------------

Codac also installs ``share/pkgconfig/codac.pc``, for projects built with a
plain ``Makefile``, with Autotools, or compiled by hand. Since it is installed in
``share/pkgconfig`` rather than ``lib/pkgconfig``, and since an installation in
your home directory is not on the default search path either, ``pkg-config`` has
to be told where to look:

.. code-block:: bash

  export PKG_CONFIG_PATH=$CODAC_PREFIX/share/pkgconfig:$PKG_CONFIG_PATH

  # IBEX answers for its own flags through the "Requires: ibex" line of
  # codac.pc, so its .pc file has to be reachable as well:
  export PKG_CONFIG_PATH=$HOME/ibex-lib/build_install/share/pkgconfig:$PKG_CONFIG_PATH

Check that it is found, and look at what it answers:

.. code-block:: bash

  pkg-config --modversion codac      # 2.1.1
  pkg-config --cflags codac
  pkg-config --libs codac

Compiling with it
^^^^^^^^^^^^^^^^^

.. code-block:: bash

  g++ -std=c++20 -O3 -DNDEBUG $(pkg-config --cflags codac) \
      main.cpp -o my_project $(pkg-config --libs codac)

The language standard and the optimisation level are deliberately **not** part of
``codac.pc``: they are your project's business, exactly as ``CMAKE_CXX_STANDARD``
and ``CMAKE_BUILD_TYPE`` are on the CMake side. Everything else is carried by the
file — the include directories (Codac's, Eigen's and IBEX's), the architecture
flags, the Codac and IBEX libraries in a working link order, and the thread
library where one is needed.

The same thing in a ``Makefile``:

.. code-block:: make

  CXXFLAGS := -std=c++20 -O3 -DNDEBUG $(shell pkg-config --cflags codac)
  LDLIBS   := $(shell pkg-config --libs codac)

  my_project: main.cpp
  	$(CXX) $(CXXFLAGS) $< -o $@ $(LDLIBS)

.. admonition:: ``-frounding-math``, and why it is in ``codac.pc``

  A CMake consumer gets the interval-arithmetic flags by calling
  ``ibex_init_common()``. A ``pkg-config`` consumer has no equivalent, so
  ``codac.pc`` carries those flags in its ``Cflags:`` line itself. This matters:
  compiling Codac's headers without ``-frounding-math`` silently gives up the
  guarantee the whole library rests on, and nothing in the build would report it.
  Use the flags ``pkg-config`` gives you rather than a subset of them.

.. warning::

  ``pkg-config`` speaks ``-I``/``-L``/``-l``, which ``cl.exe`` does not
  understand. **On Visual Studio, use the CMake route**; ``codac.pc`` is of no
  use there. It works as expected with MinGW.


.. _sec-start-py-project:

A Python project
----------------

If Codac was :ref:`installed with pip <sec-install-py>`, there is nothing to
configure: the package is in your interpreter's ``site-packages`` and a plain
``import`` finds it. A complete script is then:

.. code-block:: py

  from codac import *

  x = VectorVar(2)
  f = AnalyticFunction([x], x[0]*cos(x[0]-x[1])+x[1])
  sep = SepInverse(f, [-oo,0])
  DefaultFigure.pave([[-10,10],[-10,10]], sep, 0.1)

Pointing Python at a local build
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If instead you :ref:`built the binding yourself <sec-dev-info-binding>`, the
package is left in the build tree, at
``$HOME/codac/build/python/python_package/codac``. Python has to be told about
the directory *containing* the package — that is, ``python_package``, not
``python_package/codac``. Three ways, in decreasing order of preference:

.. code-block:: bash

  # 1. Register the build tree with your interpreter, once. The installation
  #    stays editable: rebuilding Codac is enough, there is nothing to reinstall.
  cd $HOME/codac/build/python/python_package
  pip install --user -e .

  # 2. Through the environment, e.g. in your .bashrc
  export PYTHONPATH="${PYTHONPATH}:$HOME/codac/build/python/python_package/"

The third way is from the script itself, which is useful for a one-off test or
when several builds of Codac coexist on the same machine. The path has to be
added **before** the import:

.. code-block:: py

  import sys
  # The directory that contains the "codac" package, not the package itself:
  sys.path.insert(0, "/home/me/codac/build/python/python_package")

  from codac import *

  x = VectorVar(2)
  f = AnalyticFunction([x], x[0]*cos(x[0]-x[1])+x[1])
  sep = SepInverse(f, [-oo,0])
  DefaultFigure.pave([[-10,10],[-10,10]], sep, 0.1)

Checking which Codac you are actually using
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

When a pip-installed Codac and a locally built one are both present, this is the
quickest way to see which one won:

.. code-block:: bash

  python -c "import codac ; print(codac.__version__) ; print(codac.__file__)"

``__file__`` is the path of the package that was imported, so it tells you
whether you are running the release from PyPI or your own build.

.. admonition:: MATLAB

  The MATLAB interface is the same Python package under another name,
  ``codac4matlab``, so the paths above apply to it as well. See
  :ref:`sec-install-matlab`.
