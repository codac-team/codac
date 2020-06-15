.. _sec-manual-dev:

.. warning::
  
  Several changes are currently performed on the library.
  This page may not be up to date.

##########################
Information for developers
##########################

In case you are willing to contribute to Tubex, here are some information that may be useful.


.. rubric:: Definition

CMake supports the following options for development:

======================  ======================================================================================
Option                  Description
======================  ======================================================================================
CMAKE_INSTALL_PREFIX    :ref:`See more <sec-installation-full-linux-cmake>`
----------------------  --------------------------------------------------------------------------------------
CMAKE_BUILD_TYPE        :ref:`See more <sec-installation-full-linux-cmake>`
----------------------  --------------------------------------------------------------------------------------
CMAKE_PREFIX_PATH       :ref:`See more <sec-installation-full-linux-cmake>`
----------------------  --------------------------------------------------------------------------------------
WITH_CAPD=ON            :ref:`See more <sec-extensions-ode>`
----------------------  --------------------------------------------------------------------------------------
BUILD_TESTS             | By default, the tests are not built.
                        | To enable the compilation of tests:

                        .. code-block:: bash

                          cmake <other_cmake_options> -DBUILD_TESTS=ON ..
----------------------  --------------------------------------------------------------------------------------
TEST_EXAMPLES           This will add the execution of the examples in the tests list.

                        .. code-block:: bash

                          cmake <other_cmake_options> -DTEST_EXAMPLES=ON ..
----------------------  --------------------------------------------------------------------------------------
WITH_PYTHON             Note: you need to have ``doxygen`` and Python3 installed on your computer.

                        To enable the compilation of Python binding:

                        .. code-block:: bash

                          cmake <other_cmake_options> -DWITH_PYTHON=ON ..

                        You will then have to generate the Doxygen documentation (XML files):

                        .. code-block:: bash

                          make api

                        And finally run again the ``cmake`` command:

                        .. code-block:: bash

                          cmake <other_cmake_options> -DWITH_PYTHON=ON ..

                        This last step will generate header files containing docstrings for Python, based on
                        the content of XML files made by Doxygen. The documentation of any C++/Python function
                        is then located in the C++ header files of the /src directory.

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

.. sudo apt install python3-sphinx
.. sudo python3 -m pip install sphinx_rtd_theme sphinx-tabs

.. code-block:: bash
  
  # Install Sphinx with
  pip3 install -U sphinx breathe sphinx_rtd_theme sphinx-tabs

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


.. rubric:: Visualization of graphs of CN


.. code-block:: bash

  sudo apt install dot2tex


.. rubric:: Installing local Python binding

After having configured ``-DWITH_PYTHON=ON`` and compiled:

.. code-block:: bash
  
  cd build/python/python_package
  python3 setup.py develop --user


.. rubric:: Script to create a first draft of binding

Copy the :file:`pybind11gen.py` script in the sources folder. Then, for instance:

.. code-block:: bash
  
  python3 pybind11gen.py module_name=RandTrajectory tubex_RandTrajectory.h
  # get the files in /tmp


.. rubric:: [For admins] Upload Python binding on PyPi (`see Tubex repo <https://pypi.org/project/tubex_lib/>`_)

Tag the current version:

.. code-block:: bash

  git tag -a v3.0.0-beta1
  git push origin v3.0.0-beta1

Create the *wheels* with a Docker:

.. code-block:: bash

  docker pull benensta/pyibex-docker
  docker run --rm -v `pwd`:/io benensta/pyibex-docker /io/scripts/docker/build_pybinding.sh

Upload the *wheels* on PyPi:

.. code-block:: bash

  python3 -m twine upload --repository pypi wheelhouse/*