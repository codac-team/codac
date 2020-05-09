.. _sec-manual-dev:

.. warning::
  
  Several changes are currently perfomed on the library.
  This page may not be up to date.

##########################
Information for developers
##########################

| In case you are willing to contribute to Tubex, here are some information that may be useful.
| Linux support.


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

To speed up the compilation with ``make`` and take benefit of, for example, 4 cores of your processor, use the ``-j`` option:

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
  
  # Install Sphinx with
  sudo apt install python3-sphinx
  sudo python3 -m pip install sphinx_rtd_theme sphinx-tabs

  # Build the doc
  cd build
  make doc

The website will be generated in :file:`build/doc/`.


.. rubric:: API technical documentation

Build the API technical documentation using Doxygen:

.. code-block:: bash

  # Install Doxygen with
  sudo apt install doxygen graphviz

  # Build the doc
  cd build
  make api

The API pages will be generated in :file:`build/doc/api/html/`.


.. rubric:: Script :file:`make.sh`

One shortcut to build all the examples (including 3rd parties) is given by:

.. code-block:: bash
  
  # From Tubex root
  ./make examples

Note that the :file:`build` folders of each example is removed before the build.

One shortcut to build the tests is given by:

.. code-block:: bash

  # From Tubex root
  ./make tests

One shortcut to build both the examples and tests is:

.. code-block:: bash

  # From Tubex root
  ./make all