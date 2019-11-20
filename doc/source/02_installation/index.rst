Installing the Tubex library
============================

For now, the installation is supported on Linux systems only. Any help for Windows support is more than welcome.


Requirements and dependencies
-----------------------------

Tubex assumes a set of requirements you can meet with the following command line:

.. code-block:: bash

  sudo apt-get install -y cmake g++-5

Tubex also uses several features of the `IBEX library <http://www.ibex-lib.org/doc/install.html>`_ that you have to install first. The last version of IBEX is available on `the official development repository <https://github.com/ibex-team/ibex-lib>`_:

.. code-block:: bash

  git clone https://github.com/ibex-team/ibex-lib   # download the sources from Github
  cd ibex-lib                                       # move to the IBEX directory
  ./waf configure --with-debug --interval-lib=gaol  # configure IBEX before installation
  sudo ./waf install                                # building + installing

Note that the :code:`--with-debug` option will slightly slow down your computations, but display useful error messages in case of failure conditions such as access violations. It is highly recommended for your developments.

Tubex uses the `GAOL library <http://frederic.goualard.net/#research-software>`_ by default, but `Filib++ <http://www2.math.uni-wuppertal.de/~xsc/software/filib.html>`_ is also supported. Use :code:`--interval-lib=filib` during the configuration of IBEX to use it, and change the comments in the ``cmake/modules/FindIbexLib.cmake`` file to support `Filib++` instead of `GAOL`.


Tubex library
-------------

The last sources are available on `the official Tubex development repository <https://github.com/SimonRohou/tubex-lib>`_. You can download the files and compile the sources with:

.. code-block:: bash

  git clone https://github.com/SimonRohou/tubex-lib
  cd tubex-lib ; mkdir make ; cd make ; cmake .. ; make

To build the library, you can also use the :code:`build.sh` script with one of the following command lines:

.. code-block:: bash

  ./build.sh            # to compile the library sources
  ./build.sh examples   # to compile both the library and examples


Graphical tools
---------------

`VIBes <http://enstabretagnerobotics.github.io/VIBES/>`_ is a visualization system that aims at providing people working with interval methods a way to display results (boxes, pavings), without worrying about GUI programming.
Some methods have been implemented in Tubex in order to extend VIBes' features to tubes purposes.

The last sources are available on `the official VIBes development repository <https://github.com/ENSTABretagneRobotics/VIBES>`_. A fast installation can be made using the following command lines:

.. code-block:: bash

  git clone https://github.com/ENSTABretagneRobotics/VIBES
  cd VIBES/viewer ; mkdir make ; cd make ; cmake .. ; sudo make install

.. \todo: test sudo make install and executable access

Then, the VIBes viewer can be launch independently by executing the ``VIBes-viewer``.
You may have to update the :code:`$PATH` environment variable in order to reach the ``VIBes-viewer`` program.


Build the examples
------------------

All the examples can be built using the following script:

.. code-block:: bash

  ./build.sh examples   # to compile both the library and examples

Otherwise, to compile one specific example, use :code:`cmake` in the example directory.
For instance:

.. code-block:: bash
  
  cd examples/basics/ex_01_arithmetic     # moving to example dir
  mkdir make ; cd make ; cmake .. ; make  # cmake compilation
  ./basics_01_arithmetic                  # running example

Do not forget to launch the VIBes viewer before running the example.