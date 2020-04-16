.. _sec-start-project-label:

Start a project
===============

| Tubex is ready to be used on your computer.
| You can now copy-paste the following example code in a file named :file:`main.cpp`:

.. code-block:: c++

  #include <tubex.h>
  
  using namespace std;
  using namespace tubex;
  
  int main()
  {
    Interval domain(0.,10.);
    Tube x(domain);
  
    cout << "My first tube: " << x << endl;
  }

For the compilation of your project, you can use CMake with the following file :file:`CMakeLists.txt` that you will add in the same directory:

.. code-block:: cmake

  cmake_minimum_required(VERSION 3.0.2)
  project(my_project LANGUAGES CXX)

  # Compilation options
  set(CMAKE_CXX_STANDARD 11)
  add_compile_options(-O3 -DNDEBUG) # comment for debug mode
  if(WIN32)
    add_definitions(-U__STRICT_ANSI__)
  endif()

  # Dependencies
  include(FindPkgConfig)
  pkg_search_module(TUBEX REQUIRED tubex)
  message(STATUS "Found Tubex version ${TUBEX_VERSION}")

  # Compilation
  add_executable(${PROJECT_NAME} main.cpp)
  target_compile_options(${PROJECT_NAME} PUBLIC ${TUBEX_CFLAGS})
  target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${TUBEX_INCLUDE_DIRS})
  target_link_libraries(${PROJECT_NAME} PUBLIC ${TUBEX_LDFLAGS})

The files :file:`main.cpp` and :file:`CMakeLists.txt` appear in the same directory:

.. code-block:: bash

  my_project
  ├── CMakeLists.txt
  └── main.cpp

.. admonition:: Custom install directory of IBEX and Tubex
  
  If you installed IBEX and/or Tubex in a custom directory (instead of file system such as :file:`/usr/local/` under Linux),
  then you need to specify the path to the `pkg-config` files. This can be done either by setting environment variables, for instance:

  .. code-block:: bash

    export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$HOME/tubex-lib/build_install/share/pkgconfig
    export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$HOME/ibex-lib/build_install/share/pkgconfig

  Or by adding the following lines in your :file:`CMakeLists.txt` file. For instance:

  .. code-block:: cmake

    set(PKG_CONFIG_PATH ${PKG_CONFIG_PATH} "${HOME}/tubex-lib/build_install/share/pkgconfig")
    set(PKG_CONFIG_PATH ${PKG_CONFIG_PATH} "${HOME}/ibex-lib/build_install/share/pkgconfig")

The compilation is made by the following command line:

.. code-block:: bash

  mkdir build -p ; cd build ; cmake .. ; make ; cd ..

Lastly, the project can be run with:

.. code-block:: bash

  ./build/my_project