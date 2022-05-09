:orphan:

.. _sec-installation-full-macos:

#####################################
Installing Codac on macOS for C++ use
#####################################


Quick start
-----------

Install Homebrew package manager and build tools:

.. code-block:: bash

  /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
  brew install wget autoconf automake libtool
  brew install --cask cmake

Download and extract e.g. ``codac_standalone_x64_bigsur.zip`` (for macOS 11 Big Sur) from `<https://github.com/codac-team/codac/releases/latest/>`_, then in ``example`` folder run:

.. code-block:: bash

  cmake . ; cmake --build . ; ./my_project

and check that "My first tube:Tube [0, 10]" appears.

Optionally, download and run `<https://github.com/ENSTABretagneRobotics/VIBES/releases/download/0.2.3/VIBes-0.2.2-osx.dmg>`_ before running the project, and check that a tube appears in VIBes window.


Building from sources
---------------------

You will probably need to install those prerequisites (assuming you installed `Homebrew package manager <https://chocolatey.org/install>`_):

.. code-block:: bash

  brew install eigen

The logic to follow will then be similar to `Linux <01-installation-full-linux.html>`_. See also `Information for developers <info_dev.html>`_.
