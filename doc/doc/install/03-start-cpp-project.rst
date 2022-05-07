.. _sec-start-cpp-project:

###################
Start a C++ project
###################

.. tip::
   | You are using Python?
   | :ref:`sec-start-py-project`

| Codac is ready to be used on your computer.
| You can now copy-paste the following example code in a file named :file:`main.cpp`:

.. code-block:: c++

  #include <codac.h>
  
  using namespace std;
  using namespace codac;
  
  int main()
  {
    Tube x(Interval(0,10), 0.01, TFunction("cos(t)+abs(t-5)*[-0.1,0.1]"));

    vibes::beginDrawing();
    VIBesFigTube fig("My first tube");
    fig.add_tube(&x, "x");
    fig.show();
    vibes::endDrawing();
  }

For the compilation of your project, you can use CMake with the following file :file:`CMakeLists.txt` that you will add in the same directory:

.. code-block:: cmake

    cmake_minimum_required(VERSION 3.0.2)
    project(my_project LANGUAGES CXX)

    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

  # Adding IBEX

    # In case you installed IBEX in a local directory, you need 
    # to specify its path with the CMAKE_PREFIX_PATH option.
    # set(CMAKE_PREFIX_PATH "~/ibex-lib/build_install")

    find_package(IBEX REQUIRED)
    ibex_init_common() # IBEX should have installed this function
    message(STATUS "Found IBEX version ${IBEX_VERSION}")

  # Adding Eigen3

    # In case you installed Eigen3 in a local directory, you need
    # to specify its path with the CMAKE_PREFIX_PATH option, e.g.
    # set(CMAKE_PREFIX_PATH "~/eigen/build_install")

    find_package(Eigen3 REQUIRED NO_MODULE)
    message(STATUS "Found Eigen3 version ${EIGEN3_VERSION}")

  # Adding Codac

    # In case you installed Codac in a local directory, you need 
    # to specify its path with the CMAKE_PREFIX_PATH option.
    # set(CMAKE_PREFIX_PATH "~/codac/build_install")

    find_package(CODAC REQUIRED)
    message(STATUS "Found Codac version ${CODAC_VERSION}")

  # Compilation

    add_executable(${PROJECT_NAME} main.cpp)
    target_compile_options(${PROJECT_NAME} PUBLIC ${CODAC_CXX_FLAGS})
    target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${CODAC_INCLUDE_DIRS} ${EIGEN3_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PUBLIC ${CODAC_LIBRARIES} Ibex::ibex ${CODAC_LIBRARIES} Ibex::ibex)


The files :file:`main.cpp` and :file:`CMakeLists.txt` appear in the same directory:

.. code-block:: bash

  my_project
  ├── CMakeLists.txt
  └── main.cpp

.. .. note::
.. 
..   `Eigen <http://eigen.tuxfamily.org/index.php?title=Main_Page>`_ is also currently a dependency of the library.


.. admonition:: Custom install directory of IBEX and Codac
  
  If you installed IBEX and/or Codac in a custom directory (instead of the file system such as :file:`/usr/local/` under Linux),
  then you need to specify the ``CMAKE_PREFIX_PATH`` option, as indicated in the above :file:`CMakeLists.txt` file.

  Another way is to export the ``CMAKE_PREFIX_PATH`` environment variable. For instance:

  .. code-block:: bash

    export CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:$HOME/ibex-lib/build_install
    export CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:$HOME/codac/build_install

The compilation of your project is made by the following command line:

.. code-block:: bash

  mkdir build -p ; cd build ; cmake .. ; make ; cd ..

Lastly, the project can be run with:

.. code-block:: bash

  ./build/my_project

| This script will create a simple tube and display it.
| In order to visualize the tube, you need to launch the VIBes viewer independently. On Linux, you can for instance execute:

.. code-block:: bash

  VIBes-viewer

If everything is well installed on your computer, you should see the following window appear:

.. Figure:: img/helloworld.png