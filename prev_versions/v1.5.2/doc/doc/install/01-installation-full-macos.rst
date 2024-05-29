:orphan:

.. _sec-installation-full-macos:

#####################################
Installing Codac on macOS for C++ use
#####################################


Quick start
-----------

Install `Homebrew package manager <https://brew.sh/>`_ and build tools:

.. code-block:: bash

  /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
  brew install wget autoconf automake libtool
  brew install --cask cmake

Download and extract *e.g.* ``codac_standalone_arm64_monterey.zip`` (for macOS 12 Monterey on a Mac with Apple silicon (arm64 processor), use ``codac_standalone_x86_64_monterey.zip`` for a Mac with an Intel processor (x86_64 processor), see https://support.apple.com/en-us/116943) from `<https://github.com/codac-team/codac/releases/latest/>`_, then in ``example`` folder run:

.. code-block:: bash

  cmake . ; cmake --build . ; ./my_project

and check that "My first tube:Tube [0, 10]" appears.

Optionally, download and run `<https://github.com/ENSTABretagneRobotics/VIBES/releases/download/0.2.3/VIBes-0.2.2-osx.dmg>`_ (see also https://support.apple.com/HT211861) before running the project, and check that a tube appears in :ref:`VIBes viewer <sec-installation-graphics>`.


Building from sources
---------------------

You will probably need to install those prerequisites:

.. code-block:: bash

  brew install eigen

Optionally, for Python and documentation:

.. code-block:: bash

  wget https://github.com/Homebrew/homebrew-core/raw/d2267b9f2ad247bc9c8273eb755b39566a474a70/Formula/doxygen.rb ; brew reinstall ./doxygen.rb ; brew pin doxygen
  brew install graphviz
  python -m pip install --upgrade pip
  pip install --upgrade wheel setuptools sphinx breathe sphinx-issues sphinx-tabs sphinx_rtd_theme sphinx-reredirects

The logic to follow will then be similar to `Linux <01-installation-full-linux.html>`_. See also `Information for developers </dev/info_dev.html>`_.
