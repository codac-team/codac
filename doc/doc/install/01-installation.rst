.. _sec-installation:

############################
Installing the Tubex library
############################

The installation is supported on Linux systems. The current version of Tubex will be soon available for Windows.
Any support is more than welcome.

Note that you can also :ref:`use Tubex online in Python <sec-py-project-online>`, without having to install the library on your machine.


.. contents::


Very fast installation for Python use
-------------------------------------

In case you want to use Tubex only with Python, then the installation procedure is simply:

.. code-block:: bash
  
  pip3 install --upgrade pip # you may have to upgrade pip (19.0.0 required at least)
  pip3 install "pyibex>=1.8.1" tubex-lib

where `pyIbex <http://benensta.github.io/pyIbex/>`_ is the Python wrapper of the `IBEX library <http://www.ibex-lib.org/doc/install.html>`_ on which Tubex is based. Note that Tubex requires pyIbex 1.8.1 at least.

For Python, the ``✓`` configurations are officially supported at the moment:

=================================  =============  =================  =============
Python version                     Linux          Windows            macOS
=================================  =============  =================  =============
Python 3.4                         ✓              soon available ?   ?
Python 3.5                         ✓              soon available ?   ?
Python 3.6                         ✓              soon available ?   ?
Python 3.7                         ✓              soon available ?   ?
Python 3.8                         ✓              soon available ?   ?
=================================  =============  =================  =============

.. note::

  | (5th June) We are currently working on making Tubex available on Windows in Python.
  | Any help is welcome.
  | Meanwhile, you can :ref:`use Tubex online in Python <sec-py-project-online>`.


If your configuration is missing in this table, please contact us. We may be able to make it available for Tubex.

For updating your already installed version of Tubex, the procedure is:

.. code-block:: bash

  pip3 install tubex-lib --upgrade

After the installation, please see the :ref:`sec-installation-graphics` section.


Full installation for C++ use or developers
-------------------------------------------

For C++, the ``✓`` configurations are officially supported at the moment:

=============  =================  =============
Linux          Windows            macOS
=============  =================  =============
✓              soon available ?   ?
=============  =================  =============

.. note::

  | (5th June) We are currently working on making Tubex available on Windows in C++.
  | Any help is welcome.


[Linux] Requirements and dependencies (IBEX)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Tubex uses several features of the `IBEX library <http://www.ibex-lib.org/doc/install.html>`_ that you have to install first. The last version of IBEX is available on `the official development repository <https://github.com/ibex-team/ibex-lib>`_:

.. code-block:: bash

  # Requirements to compile IBEX
  sudo apt-get install -y g++ gcc flex bison cmake git
  
  # Download the sources from Github
  git clone -b develop https://github.com/ibex-team/ibex-lib.git
  
  # Configure IBEX before installation
  cd ibex-lib
  mkdir build && cd build
  cmake -DCMAKE_CXX_FLAGS="-fPIC" -DCMAKE_C_FLAGS="-fPIC" ..
  
  # Building + installing
  make
  sudo make install

For further CMake options, please refer to the documentation. For instance, for a debug version with local installation, one can configure:

.. code-block:: bash

  cmake -DCMAKE_CXX_FLAGS="-fPIC" -DCMAKE_C_FLAGS="-fPIC" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$HOME/ibex-lib/build_install ..

.. admonition:: Debug/development mode
  
  Note that the :code:`-DCMAKE_BUILD_TYPE=Debug` option will slightly slow down your computations, but display useful error messages in case of failure conditions such as access violations. It is highly recommended for your developments.

IBEX uses the `GAOL library <http://frederic.goualard.net/#research-software>`_ by default, but `Filib++ <http://www2.math.uni-wuppertal.de/wrswt/preprints/prep_01_4.pdf>`_ is also supported. Use :code:`-DINTERVAL_LIB=filib` during the CMake configuration to use it.


[Linux] Building the Tubex library
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The last sources are available on `the official Tubex development repository <https://github.com/SimonRohou/tubex-lib>`_. You can download the files and compile the sources with:

.. code-block:: bash

  git clone https://github.com/SimonRohou/tubex-lib # download the sources from GitHub
  cd tubex-lib                                      # move to the Tubex directory
  git submodule init ; git submodule update         # get pybind11 submodule
  mkdir build ; cd build ; cmake .. ; make          # build the sources
  sudo make install                                 # install the library
  cd ../                                            # back to the root of Tubex


.. rst-class:: fit-page

  CMake supports the following options:

    ======================  ======================================================================================
    Option                  Description
    ======================  ======================================================================================
    CMAKE_INSTALL_PREFIX    | By default, the library will be installed in system files (:file:`/usr/local/` under Linux).
                              Use ``CMAKE_INSTALL_PREFIX`` to specify another path.
                            | Example:

                            .. code-block:: bash

                              cmake -DCMAKE_INSTALL_PREFIX=$HOME/tubex-lib/build_install ..
                            
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
                            
                            Once Tubex has been compiled with this option, you should also compile your executable
                            in debug mode.

    CMAKE_PREFIX_PATH       | If IBEX has been installed in a local folder, say :file:`~/ibex-lib/build_install`, you need
                              to indicate this path using the ``CMAKE_PREFIX_PATH`` option.
                            | Example:

                            .. code-block:: bash

                              cmake -DCMAKE_PREFIX_PATH=$HOME/ibex-lib/build_install ..
    ======================  ======================================================================================


.. admonition:: Custom install directory of IBEX and Tubex
  
  A convenient way to refer to custom install directories for IBEX and/or Tubex is to export the ``CMAKE_PREFIX_PATH`` environment variable. For instance:

  .. code-block:: bash

    export CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:$HOME/ibex-lib/build_install
    export CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:$HOME/tubex-lib/build_install


[Linux] Compiling the examples
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To compile one specific example, use CMake in the example directory.
For instance:

.. code-block:: bash
  
  cd examples/basics/ex_01_arithmetic               # moving to the example directory
  mkdir build -p ; cd build ; cmake .. ; make       # cmake compilation
  ./basics_01                                       # running example

Do not forget to launch the VIBes viewer before running your program.


.. _sec-installation-graphics:

Graphical tools
---------------

`VIBes <http://enstabretagnerobotics.github.io/VIBES/>`_ is a visualization system that aims at providing people working with intervals a way to display results (boxes, pavings), without worrying about GUI programming.
Some methods have been implemented in Tubex in order to extend VIBes' features to tubes purposes.

You can `download the VIBes viewer directly from the official page <http://enstabretagnerobotics.github.io/VIBES/>`_ (click on the *Last Release* link).

.. admonition:: (optional) Get the very last version of VIBes from the sources

  You can also install the last version from the sources available on `the GitHub development repository <https://github.com/ENSTABretagneRobotics/VIBES>`_.
  Then, a fast installation can be made using the following command lines:

  .. code-block:: bash
    
    sudo apt-get install qt5-default libqt5svg5-dev cmake git
    git clone https://github.com/ENSTABretagneRobotics/VIBES
    cd VIBES/viewer ; mkdir build ; cd build ; cmake .. ; sudo make install

.. \todo: test sudo make install and executable access

You can click on the icon to launch it, or use a terminal. For instance, on Linux systems:

.. code-block:: bash
  
  VIBes-viewer

This will open the VIBes window, ready to display things:

.. figure:: /manual/07-graphics/img/vibes_window.png

Keep it open!