.. _sec-install-cpp:

.. role:: strike
  :class: strike

C++ installation
================

  Main authors: `Fabrice Le Bars <https://www.ensta-bretagne.fr/lebars/>`_, `Simon Rohou <https://www.simon-rohou.fr/research/>`_

This page describes how to install the C++ library on Linux, Windows, and macOS. Follow the steps outlined for your operating system.


Linux Installation
------------------


Install from packages (latest release)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This installation procedure is valid for Ubuntu (amd64, arm64), Debian (arm64, armhf) and possibly others.
A Debian package is available for the last release |version| of the library:

.. code-block:: bash

  sudo sh -c 'echo "deb [trusted=yes] https://packages.ensta-bretagne.fr/$(if [ -z "$(. /etc/os-release && echo $UBUNTU_CODENAME)" ]; then echo debian/$(. /etc/os-release && echo $VERSION_CODENAME); else echo ubuntu/$(. /etc/os-release && echo $UBUNTU_CODENAME); fi) ./" > /etc/apt/sources.list.d/ensta-bretagne.list'
  sudo apt update
  sudo apt install libcodac-dev

Then, check your installation :ref:`with the instructions of this page <sec-start-cpp-project>`.

.. warning::

  | :strike:`URL changed: Please uninstall before.`

.. admonition:: Uninstall Codac

  To uninstall Codac, you might want to do the following:

  .. code-block:: bash

    sudo apt remove libcodac-dev libibex-dev
    sudo rm -f /etc/apt/sources.list.d/ensta-bretagne.list
    sudo apt update

  :strike:`Note also that ``libeigen3-dev`` ` :strike:`might have been installed as a dependency of Codac but might be also used by other software. You might want to keep it.`

.. admonition:: Standalone archives

  Standalone archives exist also for all the supported configurations, *e.g.* for a Raspberry Pi running Raspberry Pi OS Bookworm 32 bit, download and extract ``codac_standalone_armhf_bookworm.zip`` from `<https://github.com/codac-team/codac/releases/latest/>`_, then in the ``example`` folder run:

  .. code-block:: bash

    cmake . ; cmake --build . ; ./my_project

  and check that the graphical output appears.


Install from the sources (latest developments)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If you prefer to use the latest development version, you can install Codac by compiling it from the source code.

Steps
~~~~~

.. _sec-install-cpp-prerequisites:

1. **Ensure the following prerequisites are met**:

   - A C++ compiler supporting C++20 or later (*e.g.*, GCC 11.0+, Clang 13.0+).
   - CMake 3.14 or higher.
   - Git (to clone the repository).

   These prerequisites can be installed directly with the command::

      sudo apt-get install -y g++ gcc cmake git

2. **Install the IBEX dependency**:
   
   Codac still uses some features of the `IBEX library <https://ibex-team.github.io/ibex-lib/install-cmake.html>`_ that you have to install first (currently, the only thing Codac uses from IBEX is a wrapper of the `GAOL library <https://github.com/goualard-f/GAOL>`_). The last version of IBEX is maintained on `this unofficial development repository <https://github.com/lebarsfa/ibex-lib/tree/master>`_:

   .. code-block:: bash

      # Requirements to compile IBEX
      sudo apt-get install -y flex bison

      # Download IBEX sources from GitHub
      git clone -b master https://github.com/lebarsfa/ibex-lib.git $HOME/ibex-lib

      # Configure IBEX before installation
      cd $HOME/ibex-lib
      mkdir build ; cd build
      cmake -DCMAKE_INSTALL_PREFIX=$HOME/ibex-lib/build_install -DCMAKE_BUILD_TYPE=Release ..

      # Building + installing
      make
      make install
      cd ../..

   For further CMake options, please refer to the IBEX documentation.

   .. warning::

     **GAOL prerequisite:** On some platforms, you might need to install manually `MathLib <https://github.com/lebarsfa/mathlib>`_ and `GAOL <https://github.com/lebarsfa/GAOL>`_ with CMake and `specify where they are <https://ibex-team.github.io/ibex-lib/install-cmake.html#configuration-options>`_ in order to build IBEX successfully and have accurate computations.

3. **Install the Codac library**:

   .. code-block:: bash

      # The codac directory can be placed in your home, same level as IBEX
      git clone https://github.com/codac-team/codac $HOME/codac

      # Configure Codac before installation
      cd $HOME/codac
      # Get automatically pybind11 and eigen submodules:
      git submodule init ; git submodule update 
      mkdir build ; cd build
      cmake -DCMAKE_INSTALL_PREFIX=$HOME/codac/build_install -DCMAKE_PREFIX_PATH=$HOME/ibex-lib/build_install -DCMAKE_BUILD_TYPE=Release ..

      # Building + installing
      make
      make install
      cd ../..

4. **Configure your system to find Codac**:

   In case Codac and its dependencies have been installed locally on your system, you will have to configure your environment variables. This can be done temporarily with:

   .. code-block:: bash

      export CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:$HOME/ibex-lib/build_install
      export CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:$HOME/codac/build_install

   ... or permanently by updating your ``.bashrc`` file by appending the above commands.

5. **Verify the installation** (optional):

   To ensure that the installation has worked properly, the unit tests of the library can be run. For this, you have to configure CMake using the ``-DBUILD_TESTS=ON`` option, before compilation. Then, from the ``$HOME/codac/build`` directory:

   .. code-block:: bash

      make test

6. **Try an example** (optional):

   You may want to try Codac by running one of the proposed examples. After the installation, you can run the following commands:

   .. code-block:: bash

      cd $HOME/codac/examples/01_batman
      mkdir build ; cd build
      cmake ..
      make

   Before executing the example, you will have to launch the VIBes viewer. Then,

   .. code-block:: bash

      ./codac_example

   You should obtain a graphical output corresponding to a curious set inversion.


Windows Installation
--------------------

Install from packages (latest release)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Using MinGW
~~~~~~~~~~~

Check https://community.chocolatey.org/packages/codac.


Using Visual Studio
~~~~~~~~~~~~~~~~~~~

Download and extract *e.g.* ``codac_standalone_x64_vc17.zip`` (for Visual Studio 2022) from https://github.com/codac-team/codac/releases/latest, open ``example\CMakelists.txt``, choose ``x64-Release`` configuration in Visual Studio (instead of ``x64-Debug``), double-click on ``main.cpp`` in the Solution Explorer and then click on the green Start button, and finally check that the graphical output appears.

Note that in order to obtain graphical outputs, you will have to download and run https://github.com/ENSTABretagneRobotics/VIBES/releases/download/0.2.3/VIBes-0.2.3-win32.exe before running the project.


Install from the sources (latest developments)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

You will probably need to install these prerequisites (assuming you already installed `Chocolatey package manager <https://chocolatey.org/install>`_):

.. code-block:: bash

  choco install cmake git make patch winflexbison
  choco install eigen
  
Then, install the desired compiler (*e.g.* ``choco install mingw --version=11.2.0.07112021``). 

Optionally, for Python binding (*e.g.* ``choco install python --version=3.10.4``) and documentation:

.. code-block:: bash

  choco install doxygen.install --version=1.13
  choco install graphviz
  python -m pip install --upgrade pip
  pip install --upgrade wheel setuptools sphinx sphinx_rtd_theme furo sphinx-math-dollar sphinx_tabs breathe sphinx_togglebutton

The logic to follow will then be similar to Linux (note that for Visual Studio, commands such as ``make install`` need to be replaced with something similar to:

.. code-block:: bash

  cmake --build . --config Release --target install

.. warning::

  | You might need to replace all occurences of :literal:`PATH_SUFFIXES \ ` with something similar to :literal:`PATHS ${CMAKE_CURRENT_LIST_FILE}/../../../../ PATH_SUFFIXES \ ` in all ``.cmake`` in ``codac/share/codac/cmake/`` (where Codac was installed) if a CMake project that tries to use Codac appears to find its installation location but fails to configure the project properly.



macOS Installation
-------------------

Install from packages (latest release)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Install `Homebrew package manager <https://brew.sh/>`_ and then build tools:

.. code-block:: bash

  /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
  brew install wget autoconf automake libtool
  brew install --cask cmake

Download and extract *e.g.* ``codac_standalone_arm64_monterey.zip`` from `<https://github.com/codac-team/codac/releases/tag/v1.5.6/>`_, then in ``example`` folder run:

.. code-block:: bash

  cmake . ; cmake --build . ; ./my_project

and check that the graphical output appears.

.. admonition:: For macOS 12 Monterey
   
   For macOS 12 Monterey on a Mac with Apple silicon (arm64 processor), use ``codac_standalone_x86_64_monterey.zip`` for a Mac with an Intel processor (x86_64 processor), see https://support.apple.com/en-us/116943.


Note that in order to obtain graphical outputs, you will have to download and run https://github.com/ENSTABretagneRobotics/VIBES/releases/download/0.2.3/VIBes-0.2.2-osx.dmg before running the project.



Install from the sources (latest developments)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The logic to follow will then be similar to Linux.

Optionally, for Python binding and documentation:

.. code-block:: bash

  wget https://github.com/Homebrew/homebrew-core/raw/d2267b9f2ad247bc9c8273eb755b39566a474a70/Formula/doxygen.rb ; brew reinstall ./doxygen.rb ; brew pin doxygen
  brew install graphviz
  python -m pip install --upgrade pip
  pip install --upgrade wheel setuptools sphinx sphinx_rtd_theme furo sphinx-math-dollar sphinx_tabs sphinx_togglebutton


Troubleshooting
----------------
If you encounter issues during the installation process, consider the following:

- ensure all prerequisites are installed and up to date.
- check `the GitHub issues <https://github.com/codac-team/codac/issues>`_ page for known problems.

If you need further assistance, reach out to the library maintainers via the GitHub repository's issue tracker or email support at `simon.rohou [at] ensta.fr`.

