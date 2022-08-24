.. _sec-manual-dev:

.. warning::
  
  Several changes are currently performed on the library.
  This page may not be up to date.

##########################
Information for developers
##########################

In case you are willing to contribute to Codac, here are some information that may be useful.


.. rubric:: Definition

.. rst-class:: fit-page

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
  WITH_CAPD=ON            :ref:`See more <sec-extensions-ode>` (not fully supported yet)
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

                            cmake <other_cmake_options> -DWITH_PYTHON=ON -DCMAKE_CXX_FLAGS="-fPIC" -DCMAKE_C_FLAGS="-fPIC" ..

                          This configuration generates header files containing docstrings for Python, based on
                          the content of XML files made by Doxygen. The documentation of any C++/Python function
                          is then located in the C++ header files of the :file:`/src` directory.

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
  pip3 install -U sphinx breathe sphinx_rtd_theme sphinx-tabs sphinx-issues sphinx-reredirects

  # Build the doc
  cd build
  make doc

The website will be generated in :file:`build/doc/`.


.. rubric:: API technical documentation

For building the API technical documentation using Doxygen:

.. code-block:: bash

  # Install Doxygen with
  sudo apt install doxygen graphviz

The API pages will be generated in :file:`build/doc/api/html/`.


.. rubric:: Script :file:`make.sh`

One shortcut to build all the examples (including 3rd parties) is given by:

.. code-block:: bash
  
  # From Codac root
  ./make.sh examples

Note that the :file:`build` folders of each example is removed before the build.

One shortcut to build the tests is given by:

.. code-block:: bash

  # From Codac root
  ./make.sh tests

One shortcut to build both the examples and tests is:

.. code-block:: bash

  # From Codac root
  ./make.sh all


.. rubric:: Visualization of graphs of CN


.. code-block:: bash

  sudo apt install dot2tex


.. rubric:: Install IBEX for Python binding (example of local install)

.. code-block:: bash
  
  cmake -DCMAKE_CXX_FLAGS="-fPIC" -DCMAKE_C_FLAGS="-fPIC" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$HOME/ibex-lib/build_install ..

IBEX uses the `GAOL library <http://frederic.goualard.net/#research-software>`_ by default, but `Filib++ <http://www2.math.uni-wuppertal.de/wrswt/preprints/prep_01_4.pdf>`_ is also supported. Use :code:`-DINTERVAL_LIB=filib` during the CMake configuration to use it.


.. rubric:: Installing local Python binding

.. warning::

  The binding is not working with G++7 for the moment. 

Get Pybind11 as submodule:

.. code-block:: bash
  
  git submodule init
  git submodule update

Then, configure ``cmake`` with custom options and ``-DWITH_PYTHON=ON``:

.. code-block:: bash
  
  cmake <...> -DWITH_PYTHON=ON -DCMAKE_CXX_FLAGS="-fPIC" -DCMAKE_C_FLAGS="-fPIC" ..

This configuration generates header files containing docstrings for Python, based on
the content of XML files made by Doxygen. The documentation of any C++/Python function
is then located in the C++ header files of the :file:`/src` directory.

Finally, after the compilation:

.. code-block:: bash
  
  cd build/python/python_package
  python3 setup.py develop --user


.. rubric:: Script to create a first draft of binding

Copy the :file:`pybind11gen.py` script in the sources folder. Then, for instance:

.. code-block:: bash
  
  python3 pybind11gen.py module_name=RandTrajectory codac_RandTrajectory.h
  # get the files in /tmp


.. rubric:: [For admins] Upload Python binding on PyPi (`see Codac repo <https://pypi.org/project/codac/>`_)

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

  python3 -m twine upload --repository pypi *