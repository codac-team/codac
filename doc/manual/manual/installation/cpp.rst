.. _sec-install-cpp:

.. role:: strike
  :class: strike

C++ installation
================

  Main authors: `Fabrice Le Bars <https://webperso.ensta.fr/lebars/>`_, `Simon Rohou <https://www.simon-rohou.fr>`_

This page describes how to install the C++ library on Linux, Windows, and macOS. Follow the steps outlined for your operating system.


Linux Installation
------------------

.. admonition:: Linux distributions
   
  Ubuntu or Debian-like systems are implied in most of the instructions (especially the ``sudo apt`` commands to install packages).


.. Install from packages (latest release)
.. ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. This installation procedure is valid for Ubuntu (amd64, arm64), Debian (arm64, armhf) and possibly others.
.. A Debian package is available for the last release |version| of the library:

.. .. code-block:: bash

..   sudo sh -c 'echo "deb [trusted=yes] https://webperso.ensta.fr/packages/$(if [ -z "$(. /etc/os-release && echo $UBUNTU_CODENAME)" ]; then echo debian/$(. /etc/os-release && echo $VERSION_CODENAME); else echo ubuntu/$(. /etc/os-release && echo $UBUNTU_CODENAME); fi) ./" > /etc/apt/sources.list.d/ensta-bretagne.list'
..   sudo apt update
..   sudo apt install libcodac-dev

.. Then, check your installation :ref:`with the instructions of this page <sec-start-cpp-project>`.

.. .. admonition:: Uninstall Codac

..   To uninstall Codac, you might want to do the following:

..   .. code-block:: bash

..     sudo apt remove libcodac-dev
..     sudo rm -f /etc/apt/sources.list.d/ensta-bretagne.list
..     sudo apt update

.. .. admonition:: Standalone archives

..   Standalone archives exist also for all the supported configurations, *e.g.* for a Raspberry Pi running Raspberry Pi OS Bookworm 32 bit, download and extract ``codac_standalone_armhf_bookworm.zip`` from `<https://github.com/codac-team/codac/releases/latest/>`_, then in the ``example`` folder run:

..   .. code-block:: bash

..     cmake . ; cmake --build . ; ./my_project

..   and check that the graphical output appears.


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

      sudo apt-get install -y build-essential cmake git

   .. admonition:: The GAOL dependency

     | The intervals of Codac are built upon `GAOL <https://github.com/goualard-f/GAOL>`_, the interval arithmetic library written by `Frédéric Goualard <https://frederic.goualard.net>`_, in its version 5, `GAOL v5 <https://github.com/Jordan08/GAOL>`_, the continuation of GAOL by Jordan Ninin, which computes its elementary functions with `CORE-MATH <https://core-math.gitlabpages.inria.fr>`_, correctly rounded, compiled into its library. You do not have to install it: CMake first looks for a GAOL v5 installed on your system and, when it finds none, downloads GAOL from the master branch of `its repository <https://github.com/Jordan08/GAOL>`_ while Codac is configured, builds it with its CMake build along with Codac, and installs it with Codac. GAOL v5 brings a CMake build, the fixes Codac used to work around, and tests of the bounds it computes; its README lists and explains them. Codac used to reach GAOL through `IBEX <https://github.com/ibex-team/ibex-lib>`_, and no longer depends on it.
     | If you install GAOL yourself, install GAOL v5, version 5.0.2 or later, with its CMake build (``cmake -S . -B build -DCMAKE_INSTALL_PREFIX=<prefix>``, ``cmake --build build --config Release``, then ``cmake --install build --config Release``) or with its autotools or meson builds. CMake looks for its CMake package (``gaolConfig.cmake``) first, then for its ``gaol.pc`` through ``pkg-config``, and Codac is compiled with the flags and linked with the library they give. A ``gaol.pc`` whose flags lack ``-frounding-math`` is not used, and neither is a GAOL older than version 5.0.2: CMake says so, and downloads GAOL v5 instead. To use a GAOL installed in a custom location, add its installation prefix to ``CMAKE_PREFIX_PATH``, or give ``-Dgaol_DIR=<prefix>/lib/cmake/gaol`` for its CMake package, or ``PKG_CONFIG_PATH`` for its ``gaol.pc``. To build the GAOL Codac is tested against even where another one is installed, configure Codac with ``-DENABLE_FIND_PACKAGE_GAOL=OFF``.
     | The interval operations of GAOL are inline in its headers: they are compiled with the flags of interval arithmetic GAOL gives (``-frounding-math`` and its companions, without which the compiler may compute a bound in the wrong rounding direction), and with those of the instructions GAOL was compiled with (such as ``-mfma``). Codac hands these flags over to the projects that use it, through ``CODAC_CXX_FLAGS`` and ``codac.pc``. On a 32-bit x86 processor, GAOL is compiled with ``-msse2 -mfpmath=sse``, except by Visual Studio, which computes in SSE2 already: a processor with SSE2 is therefore required there. GAOL is not built with the compilers that do not compute its intervals right, and its build stops with a message saying why: Clang for 32-bit ARM processors (use GCC), and the compilers that do not honour the rounding direction.
     | The developer option ``-DWITH_IBEX=ON`` builds Codac as it was before GAOL v5, on an installed IBEX and the GAOL 4 it bundles. It is only there to compare the results of both builds.

2. **Install the Codac library**:

   .. code-block:: bash

      # The codac directory can be placed in your home
      git clone https://github.com/codac-team/codac $HOME/codac

      # Configure Codac before installation
      cd $HOME/codac
      mkdir build ; cd build
      cmake -DCMAKE_INSTALL_PREFIX=$HOME/codac/build_install -DCMAKE_BUILD_TYPE=Release ..

      # Building + installing
      make
      make install
      cd ../..

3. **Configure your system to find Codac**:

   In case Codac and its dependencies have been installed locally on your system, you will have to configure your environment variables. This can be done temporarily with:

   .. code-block:: bash

      export CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:$HOME/codac/build_install

   ... or permanently by updating your ``.bashrc`` file by appending the above commands.

4. **Verify the installation** (optional):

   To ensure that the installation has worked properly, the unit tests of the library can be run. For this, you have to configure CMake using the ``-DBUILD_TESTS=ON`` option, before compilation. Then, from the ``$HOME/codac/build`` directory:

   .. code-block:: bash

      make test

5. **Try an example** (optional):

   You may want to try Codac by running one of the proposed examples. After the installation, you can run the following commands:

   .. code-block:: bash

      cd $HOME/codac/examples/01_batman
      mkdir build ; cd build
      cmake ..
      make

   Before executing the example, you will have to launch the `VIBes viewer <../visualization/vibes.html>`_. Then,

   .. code-block:: bash

      ./codac_example

   You should obtain a graphical output corresponding to a curious set inversion.


Windows Installation
--------------------

Install from packages (latest release)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Using MinGW
~~~~~~~~~~~

.. Check https://community.chocolatey.org/packages/codac.

Install `Chocolatey package manager <https://chocolatey.org/install>`_, run `choco install -y cmake make qtcreator` in PowerShell and then download and extract *e.g.* ``codac_standalone_x64_mingw13.zip`` (for MinGW 13) from https://github.com/codac-team/codac/releases/latest, launch Qt Creator and choose Open Project, open ``example\CMakelists.txt``, ensure Desktop is selected and click Configure Project (might be hidden behind notifications at the bottom-right), wait 10 s then click on the big bottom-left green Run button, and finally check that the graphical output appears.

Note that in order to obtain graphical outputs, you will have to download and run https://github.com/ENSTABretagneRobotics/VIBES/releases/latest/download/VIBes-viewer_x86.exe before running the project.


Using Visual Studio
~~~~~~~~~~~~~~~~~~~

Download and extract *e.g.* ``codac_standalone_x64_vc17.zip`` (for Visual Studio 2022) from https://github.com/codac-team/codac/releases/latest, open ``example\CMakelists.txt``, choose ``x64-Release`` configuration in Visual Studio (instead of ``x64-Debug``), double-click on ``main.cpp`` in the Solution Explorer and then click on the green Start button, and finally check that the graphical output appears.

Note that in order to obtain graphical outputs, you will have to download and run https://github.com/ENSTABretagneRobotics/VIBES/releases/latest/download/VIBes-viewer_x86.exe before running the project.


Install from the sources (latest developments)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

You will probably need to install these prerequisites (assuming you already installed `Chocolatey package manager <https://chocolatey.org/install>`_):

.. code-block:: bash

  choco install cmake git make
  choco install eigen
  
Then, install the desired compiler (*e.g.* ``choco install mingw --version=11.2.0.07112021``). 

Optionally, for Python binding (*e.g.* ``choco install python --version=3.10.4``) and documentation:

.. code-block:: bash

  choco install doxygen.install --version=1.16.1
  choco install graphviz
  python -m pip install --upgrade pip
  pip install --upgrade wheel setuptools sphinx sphinx_rtd_theme furo sphinx-math-dollar sphinx_tabs breathe sphinx_togglebutton

The logic to follow will then be similar to Linux. You might want to right-click in your desired folder and choose ``Git Bash Here`` to run the commands related to Git and compilation. Note that for Visual Studio, commands such as ``make`` and ``make install`` need to be replaced with something similar to:

.. code-block:: bash

  cmake --build . --config Release
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

Download and extract *e.g.* ``codac_standalone_arm64_sonoma.zip`` from https://github.com/codac-team/codac/releases/latest, then in ``example`` folder run:

.. code-block:: bash

  cmake . ; cmake --build . ; ./my_project

and check that the graphical output appears.

.. admonition:: Apple silicon vs Intel processors
   
   For *e.g.* macOS 13 Sonoma on a Mac with Apple silicon (arm64 processor), use ``codac_standalone_arm64_sonoma.zip``, while for a Mac with an Intel processor (x86_64 processor) use ``codac_standalone_x86_64_sonoma.zip``, see https://support.apple.com/en-us/116943.


Note that in order to obtain graphical outputs, you will have to download and run https://github.com/ENSTABretagneRobotics/VIBES/releases/latest/download/VIBes-viewer_x86_64.dmg before running the project.



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

