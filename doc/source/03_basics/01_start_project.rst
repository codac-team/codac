Start a project
===============

Copy-paste the following example code in a file named ``main.cpp``:

.. code-block:: c++

  #include "tubex.h"
  
  using namespace std;
  using namespace ibex;
  using namespace tubex;
  
  int main()
  {
    Interval domain(0.,10.);
    Tube x(domain);
  
    cout << "My first tube: " << x << endl;
  }

For the compilation of your project, you can use CMake with the following ``CMakeLists.txt``:

.. code-block:: cmake

  cmake_minimum_required(VERSION 3.0)                          # minimum version for CMake
  set(CMAKE_CXX_COMPILER g++-5)                                # C++ compiler
  set(CMAKE_CXX_STANDARD 11)                                   # C++ version
  
  get_filename_component(TUBEX_ROOT "~/tubex-lib" ABSOLUTE)    # used for find_packages modules
  list(APPEND CMAKE_MODULE_PATH "${TUBEX_ROOT}/cmake/modules") # location of find_package modules
  find_package(TubexLib REQUIRED)
  find_package(IbexLib REQUIRED)
  include_directories("${TUBEX_INCLUDE_DIR}" "${IBEX_INCLUDE_DIR}")
  
  add_executable(my_project main.cpp)
  target_link_libraries(my_project ${IBEX_LIBRARY} ${TUBEX_LIBRARY} ${IBEX_LIBRARY})

Here we set the ``TUBEX_ROOT`` variable to ``~/tubex-lib``. Update this value according to your own installation. This variable is used to find the CMake modules located in the Tubex repository. These modules search for the libraries Tubex, IBEX and their dependencies on your system.

The files ``main.cpp`` and ``CMakeLists.txt`` appear in the same directory:

.. code-block:: bash

  my_project
  ├── CMakeLists.txt
  └── main.cpp

The compilation is made by the following command line::

  mkdir make -p ; cd make ; cmake .. ; make ; cd ..

Lastly, the project can be run with::

  ./make/my_project