.. _sec-start-cpp-project-label:

###################
Start a C++ project
###################

.. tip::
   | You are using Python?
   | :ref:`sec-start-py-project-label`

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
    add_compile_options(-O3 -Wall)
    #add_compile_options(-DNDEBUG) # comment for debug mode

  # Adding IBEX

    # In case you installed IBEX in a local directory, you need 
    # to specify its path with the CMAKE_PREFIX_PATH option.
    # set(CMAKE_PREFIX_PATH "~/ibex-lib/build_install")

    find_package(IBEX REQUIRED)
    ibex_init_common() # IBEX should have installed this function
    message(STATUS "Found IBEX version ${IBEX_VERSION}")

  # Adding Tubex

    # In case you installed Tubex in a local directory, you need 
    # to specify its path with the CMAKE_PREFIX_PATH option.
    # set(CMAKE_PREFIX_PATH "~/tubex-lib/build_install")

    find_package(TUBEX REQUIRED)
    message(STATUS "Found Tubex version ${TUBEX_VERSION}")

  # Compilation

    add_executable(${PROJECT_NAME} main.cpp)
    target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${TUBEX_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PUBLIC ${TUBEX_LIBRARIES} Ibex::ibex ${TUBEX_LIBRARIES})


The files :file:`main.cpp` and :file:`CMakeLists.txt` appear in the same directory:

.. code-block:: bash

  my_project
  ├── CMakeLists.txt
  └── main.cpp

.. admonition:: Custom install directory of IBEX and Tubex
  
  If you installed IBEX and/or Tubex in a custom directory (instead of file system such as :file:`/usr/local/` under Linux),
  then you need to specify the ``CMAKE_PREFIX_PATH`` option, as indicated in the above :file:`CMakeLists.txt` file.

  Another way is to export the ``CMAKE_PREFIX_PATH`` environment variable. For instance:

  .. code-block:: bash

    export CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:$HOME/ibex-lib/build_install
    export CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:$HOME/tubex-lib/build_install

The compilation is made by the following command line:

.. code-block:: bash

  mkdir build -p ; cd build ; cmake .. ; make ; cd ..

Lastly, the project can be run with:

.. code-block:: bash

  ./build/my_project