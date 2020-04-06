.. _sec-installation-label:

Installing the Tubex library
============================

The installation is supported on Linux systems. The current version of Tubex has not been tested on Windows.
Any support is more than welcome.


Requirements and dependencies
-----------------------------

Tubex uses several features of the `IBEX library <http://www.ibex-lib.org/doc/install.html>`_ that you have to install first. The last version of IBEX is available on `the official development repository <https://github.com/ibex-team/ibex-lib>`_:

.. code-block:: bash

  sudo apt-get install -y cmake g++-5 flex bison    # requirements to compile IBEX
  git clone https://github.com/ibex-team/ibex-lib   # download the sources from Github
  cd ibex-lib                                       # move to the IBEX directory
  ./waf configure --with-debug --interval-lib=gaol  # configure IBEX before installation
  sudo ./waf install                                # building + installing

Note that a CMake installation procedure will be soon available for IBEX.

Note also that the :code:`--with-debug` option will slightly slow down your computations, but display useful error messages in case of failure conditions such as access violations. It is highly recommended for your developments.

Tubex uses the `GAOL library <http://frederic.goualard.net/#research-software>`_ by default, but `Filib++ <http://www2.math.uni-wuppertal.de/wrswt/preprints/prep_01_4.pdf>`_ is also supported. Use :code:`--interval-lib=filib` during the configuration of IBEX to use it.


Tubex library
-------------

The last sources are available on `the official Tubex development repository <https://github.com/SimonRohou/tubex-lib>`_. You can download the files and compile the sources with:

.. code-block:: bash

  git clone https://github.com/SimonRohou/tubex-lib
  cd tubex-lib ; mkdir build ; cd build ; cmake .. ; make
  sudo make install

If you want to compile without debug mode, then you can configure CMake with the following option:

.. code-block:: bash

  cmake -DCMAKE_BUILD_TYPE=Release ..


Graphical tools
---------------

`VIBes <http://enstabretagnerobotics.github.io/VIBES/>`_ is a visualization system that aims at providing people working with intervals a way to display results (boxes, pavings), without worrying about GUI programming.
Some methods have been implemented in Tubex in order to extend VIBes' features to tubes purposes.

You can `download the VIBes viewer directly from the website <http://enstabretagnerobotics.github.io/VIBES/>`_, or install the last version from the sources available on `the GitHub development repository <https://github.com/ENSTABretagneRobotics/VIBES>`_. In this case, a fast installation can be made using the following command lines:

.. code-block:: bash

  git clone https://github.com/ENSTABretagneRobotics/VIBES
  cd VIBES/viewer ; mkdir build ; cd build ; cmake .. ; sudo make install

.. \todo: test sudo make install and executable access

Then, the VIBes viewer can be launch independently by executing:

.. code-block:: bash

  VIBes-viewer


Build the examples
------------------

To compile one specific example, use CMake in the example directory.
For instance:

.. code-block:: bash
  
  cd examples/basics/ex_01_arithmetic               # moving to the example directory
  mkdir build -p ; cd build ; cmake .. ; make       # cmake compilation
  ./basics_01                                       # running example

Do not forget to launch the VIBes viewer before running the example.