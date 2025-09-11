.. _sec-dev-info:

Information for developers
==========================

Building the Manual with Sphinx
-------------------------------

To build this manual using Sphinx, follow these steps:

1. **Requirements**: Install `Sphinx <https://www.sphinx-doc.org/>`_ together with the extensions used by Codac, if they are not already installed.

   .. code-block:: bash

      pip install sphinx sphinx_rtd_theme furo sphinx-math-dollar sphinx_tabs breathe sphinx_togglebutton

2. **Building the manual**: After configuring the CMake project, the manual can be build with:

   .. code-block:: bash

      make manual

   The generated website will be locally available in ``./build/doc/manual``.

To contribute and extend this manual, please consult the Sphinx documentation:  
https://www.sphinx-doc.org

.. _sec-dev-info-binding:

Building a local Python binding for Codac
-----------------------------------------

You can compile Codac's Python binding binaries locally on your machine to take advantage of the library's latest features.
If you simply want to use the latest Codac release in Python, you can download the binaries directly from `PyPi <https://pypi.org/project/codac/>`_ or :ref:`follow the standard installation procedure <sec-install-py>`.

1. **Ensure the following prerequisites are met**:

   - the prerequisites for the :ref:`C++ installation of Codac <sec-install-cpp-prerequisites>`. On Linux systems, you can simply:

   .. code-block:: bash

      sudo apt-get install -y g++ gcc cmake git flex bison

   - a supported version of Python (>=3.6).
   - a recent `Doxygen <https://www.doxygen.nl>`_ version (for instance, release 1.13.0 or newest). On Linux systems, latest releases are not available as Debian packages, so we advice to install Doxygen from the sources:

   .. code-block:: bash

      cd $HOME
      git clone https://github.com/doxygen/doxygen
      cd doxygen
      git checkout Release_1_13_0 # 1.13.0, or any newer release
      mkdir build ; cd build
      cmake -DCMAKE_INSTALL_PREFIX=$HOME/doxygen/build_install ..
      make ; make install

   .. admonition:: About the use of Doxygen
      
      Doxygen software extracts C++ documentation from header files into XML format. We then convert this data into docstring format before embedding it into the binding binaries. In this way, the writing of the documentation is centralized in a single location in the C++ header files.


2. **Configure IBEX prior to compiling Codac**:
   
   We recall that IBEX sources can be obtained with:

   .. code-block:: bash
      
      git clone https://github.com/lebarsfa/ibex-lib.git $HOME/ibex-lib
      cd $HOME/ibex-lib

   You will need to compile both IBEX and Codac using the ``-fPIC`` options. This can be done with the following CMake configuration:

   .. code-block:: bash
      
      mkdir build ; cd build
      cmake -DCMAKE_CXX_FLAGS="-fPIC" -DCMAKE_C_FLAGS="-fPIC" -DCMAKE_INSTALL_PREFIX=$HOME/ibex-lib/build_install -DCMAKE_BUILD_TYPE=Release ..
      make ; make install

3. **Compile Codac with Python binding**:

   We recall that Codac sources can be obtained with:
   
   .. code-block:: bash
      
      git clone https://github.com/codac-team/codac $HOME/codac
      cd $HOME/codac

   .. admonition:: Using Codac v2 simultaneously with Codac v1

      In case you want to use the two versions of Codac in the same Python script, you will have to compile the binaries of Codac v2 under a different name in order to avoid ``import`` conflicts. Things are already prepared in the branch ``codac2_renamed``, you can therefore:
   
      .. code-block:: bash
         
         git checkout codac2_renamed

      Note that you will then have to ``import codac2`` instead of ``import codac`` in your Python scripts.

   In addition to the ``-fPIC`` options, you will have to configure ``WITH_PYTHON=ON`` and ``PYBIND11_FINDPYTHON=OFF``. Note that CMake will automatically get the `pybind11 <https://pybind11.readthedocs.io>`_ files required for the binding.
   
   .. code-block:: bash
      
      mkdir build ; cd build
      cmake -DCMAKE_CXX_FLAGS="-fPIC" -DCMAKE_C_FLAGS="-fPIC" -DWITH_PYTHON=ON -DPYBIND11_FINDPYTHON=OFF -DCMAKE_INSTALL_PREFIX=$HOME/codac/build_install -DCMAKE_PREFIX_PATH="$HOME/ibex-lib/build_install;$HOME/doxygen/build_install" -DCMAKE_BUILD_TYPE=Release ..
      make ; make install

4. **Configure your Python environment**:

   Finally, you need to configure your system so that Python can find access to your Codac binding binaries:

   .. code-block:: bash
      
      cd $HOME/codac/build/python/python_package
      python setup.py develop --user

   And update your configuration file (for instance, ``$HOME/.bashrc`` on Linux systems) with:

   .. code-block:: bash
      
      export PYTHONPATH="${PYTHONPATH}:$HOME/codac/build/python/python_package/"

5. **Verify the installation** (optional):

   To ensure that the installation has worked properly, the unit tests of the library can be run:

   .. code-block:: bash
      
      python -m unittest discover codac.tests

6. **Try an example** (optional):

   You may want to try Codac in Python by running one of the proposed examples. After the installation, you can run the following commands:

   .. code-block:: bash

      cd $HOME/codac/examples/03_sivia
      python main.py

   Note that before executing the example, you will have to launch the VIBes viewer.
   You should obtain a graphical output corresponding to a set inversion.


.. admonition:: For admins
 
   Get all the *wheels* generated by GitHub Actions (the generated release needs to be explicitely set as latest):

   .. code-block:: bash

     sudo apt install curl wget sed
     cd scripts/wheels
     chmod a+x *.sh
     ./getlatestcodacwhl.sh
     rm lk_codac_whl.ver # Optional, contain the version number that was last downloaded.

   .. note::

     | For another version than the latest release, use ``./gencodacwhl.sh`` instead of ``./getlatestcodacwhl.sh`` and enter the desired version when asked.

   The command for uploading the generated wheels on PyPi is:

   .. code-block:: bash
      
      python -m twine upload --repository pypi *

.. note::

  | To use a locally compiled binding in the MATLAB environment, you may need to launch MATLAB from the command line:

  .. code-block:: bash
    
    LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libstdc++.so.6 matlab