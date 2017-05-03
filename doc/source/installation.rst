Installing the Tubex library
============================

Prerequisites
-------------

The installation assumes you have already installed the `IBEX library <http://www.ibex-lib.org/doc/install.html>`_.


Installation
------------

Compiling the Tube class
^^^^^^^^^^^^^^^^^^^^^^^^

**Linux:**

.. code-block:: none

  git clone https://github.com/SimonRohou/tubex-lib
  cd tubex-lib
  ./build.sh


**Windows:**

The code has not been tested on Windows yet.

Graphics
^^^^^^^^

Graphical tools are provided in this library. These require the installation of the `Vibes viewer <http://enstabretagnerobotics.github.io/VIBES/>`_. See the instalaltion requirements.

Multi-threading
^^^^^^^^^^^^^^^

The Tube class provides multi-threaded features implemented with the `OpenMP library <http://www.openmp.org/>`_.
**Note:** several functions running with OpenMP have not be fully tested yet.

Development
-----------

Documentation
^^^^^^^^^^^^^

The documentation is generated thanks to `Sphinx <http://www.sphinx-doc.org/en/stable/>`_.

.. code-block:: none

  pip install Sphinx

Then, run:

.. code-block:: none

  cd doc ; make html

Tests
^^^^^

For now, we are using `Catch <https://github.com/philsquared/Catch>`_ to implement tests.

Note that the *cmake* project file may need to be configured in order to set :code:`BUILD_TESTS`.

To launch tests:

.. code-block:: none

  cd build
  cmake -DBUILD_TESTS=ON ..
  make
  make test