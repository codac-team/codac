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

Documentation
^^^^^^^^^^^^^

The documentation is generated thanks to `Sphinx <http://www.sphinx-doc.org/en/stable/>`_.

.. code-block:: none

  pip install Sphinx

Then, run:

.. code-block:: none

  cd doc ; make html