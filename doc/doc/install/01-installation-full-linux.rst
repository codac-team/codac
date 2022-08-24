:orphan:

.. _sec-installation-full-linux:

#####################################
Installing Codac on Linux for C++ use
#####################################


Install from package (latest release, for Ubuntu amd64)
---------------------------------------------------------

A Debian package is available for the last release |version| of the library:

.. code-block:: bash

  sudo sh -c 'echo "deb [trusted=yes] https://www.ensta-bretagne.fr/packages/`lsb_release --id -s | tr [:upper:] [:lower:]`/`lsb_release -cs` ./" > /etc/apt/sources.list.d/ensta-bretagne.list'
  sudo apt update
  sudo apt install libcodac-dev

Then, check your installation `with the instructions of this page <03-start-cpp-project.html>`_.

.. note::

  For a Raspberry Pi running Raspbian Buster, download and extract ``codac_standalone_armv6hf_buster.zip`` from `<https://github.com/codac-team/codac/releases/latest/>`_, then in the ``example`` folder run:

  .. code-block:: bash

    cmake . ; cmake --build . ; ./my_project

  and check that "My first tube:Tube [0, 10]" appears.


Install from sources (latest development)
-----------------------------------------

In case you prefer the latest development version, Codac can be installed by compiling the sources.

Requirements
^^^^^^^^^^^^

Codac uses several features of the `IBEX library <http://www.ibex-lib.org/doc/install.html>`_ that you have to install first. The last version of IBEX is maintained on `this unofficial development repository <https://github.com/lebarsfa/ibex-lib/tree/actions>`_:

.. code-block:: bash

  # Requirements to compile IBEX and Codac
  sudo apt-get install -y g++ gcc flex bison cmake git libeigen3-dev
  
  # Download IBEX sources from GitHub
  git clone -b actions https://github.com/lebarsfa/ibex-lib.git
  
  # Configure IBEX before installation
  cd ibex-lib
  mkdir build && cd build
  cmake ..
  
  # Building + installing
  make
  sudo make install
  cd ../..

For further CMake options, please refer to the IBEX documentation. 

.. admonition:: Debug/development mode
  
  Note that the :code:`-DCMAKE_BUILD_TYPE=Debug` option will slightly slow down your computations, but display useful error messages in case of failure conditions such as access violations. **It is highly recommended** for your developments. To use it:

  .. code-block:: bash
  
    cmake -DCMAKE_BUILD_TYPE=Debug .. 


Building the Codac library
^^^^^^^^^^^^^^^^^^^^^^^^^^

The last sources are available on `the official Codac development repository <https://github.com/codac-team/codac>`_. You can download the files and compile the sources with:

.. code-block:: bash
  
  # The codac directory can be placed in your home, same level as IBEX
  git clone https://github.com/codac-team/codac   # download the sources from GitHub
  cd codac                                        # move to the Codac directory
  git submodule init ; git submodule update       # get pybind11 submodule
  mkdir build ; cd build ; cmake .. ; make        # build the sources
  sudo make install                               # install the library
  cd ..                                           # back to the root of Codac


Compiling the examples
^^^^^^^^^^^^^^^^^^^^^^

To compile one specific example, use CMake in the example directory.
For instance:

.. code-block:: bash
  
  cd examples/basics/01_arithmetic                # moving to the example directory
  mkdir build -p ; cd build ; cmake .. ; make     # cmake compilation
  ./codac_basics_01                               # running example

Do not forget to launch the VIBes viewer before running your program.


(for experts) Additional installation options
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. _sec-installation-full-linux-cmake:

.. rst-class:: fit-page

  CMake supports the following options:

  ======================  ======================================================================================
  Option                  Description
  ======================  ======================================================================================
  CMAKE_INSTALL_PREFIX    | By default, the library will be installed in system files (:file:`/usr/local/` under Linux).
                            Use ``CMAKE_INSTALL_PREFIX`` to specify another path.
                          | Example:

                          .. code-block:: bash

                            cmake -DCMAKE_INSTALL_PREFIX=$HOME/codac/build_install ..
                          
                          .. warning::
                          
                            The full path of the folder must not contain white space or weird characters like ``'"\()`*[]``.

  CMAKE_BUILD_TYPE        | Set the build mode either to ``Release`` or ``Debug``.
                          | Default value is ``Debug``. Example:

                          .. code-block:: bash

                            cmake -DCMAKE_BUILD_TYPE=Release ..
                
                          The :code:`-DCMAKE_BUILD_TYPE=Debug` option is enabled by default. As for IBEX, it will slightly
                          slow down your computations, but display useful error messages in case of failure conditions such
                          as access violations. It is highly recommended for your developments. You can otherwise use the
                          :code:`-DCMAKE_BUILD_TYPE=Release` option. Note also that O3 optimizations are always activated.
                          
                          Once Codac has been compiled with this option, you should also compile your executable
                          in debug mode.

  CMAKE_PREFIX_PATH       | If IBEX has been installed in a local folder, say :file:`~/ibex-lib/build_install`, you need
                            to indicate this path using the ``CMAKE_PREFIX_PATH`` option.
                          | Example:

                          .. code-block:: bash

                            cmake -DCMAKE_PREFIX_PATH=$HOME/ibex-lib/build_install ..
  ======================  ======================================================================================


.. admonition:: Custom install directory of IBEX and Codac
  
  A convenient way to refer to custom install directories for IBEX and/or Codac is to export the ``CMAKE_PREFIX_PATH`` environment variable. For instance:

  .. code-block:: bash

    export CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:$HOME/ibex-lib/build_install
    export CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:$HOME/codac/build_install

See also `Information for developers <info_dev.html>`_.
