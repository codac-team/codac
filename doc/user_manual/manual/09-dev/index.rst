.. _sec-manual-dev:

.. warning::
  
  Several changes are currently perfomed on the library.
  This page may not be up to date.

##########################
Information for developers
##########################

.. rubric:: Definition

.. rst-class:: fit-page

  CMake supports the following options for development:

  ======================  ======================================================================================
  Option                  Description
  ======================  ======================================================================================
  BUILD_TESTS             | By default, the tests are not built.
                          | To enable the compilation of tests:

                          .. code-block:: bash

                            cmake <other_cmake_options> -DBUILD_TESTS=ON ..
  ======================  ======================================================================================



.. rubric:: Tips

To speed up the compilation with ``make`` and take benefit of, for example, 4 cores of your processor, one can use:

.. code-block:: bash
  
  cd build
  make -j4


.. rubric:: Tests

Launch the tests:

.. code-block:: bash

  cd build
  make test


.. rubric:: User manual

Build this user manual using Sphinx:

.. code-block:: bash

  cd build
  make doc


.. rubric:: API technical documentation

Build the API technical documentation using Doxygen:

.. code-block:: bash

  cd build
  make api


.. rubric:: Script :file:`make.sh`

One shortcut to build all the examples (including 3rd party parts) is given by:

.. code-block:: bash
  
  # From Tubex root
  ./make examples

Note that the :file:`build` folders of each example is removed before the build.

One shortcut to build the tests is given by:

.. code-block:: bash

  # From Tubex root
  ./make tests

One shortcut to build both the examples and tests:

.. code-block:: bash

  # From Tubex root
  ./make all