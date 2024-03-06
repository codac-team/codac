:orphan:

.. _sec-installation-full-windows:

#######################################
Installing Codac on Windows for C++ use
#######################################


Quick start
-----------

.. rubric:: Using MinGW

Check https://community.chocolatey.org/packages/codac.


.. rubric:: Using Visual Studio

Download and extract *e.g.* ``codac_standalone_x64_vc17.zip`` (for Visual Studio 2022) from `<https://github.com/codac-team/codac/releases/latest/>`_, open ``example\CMakelists.txt``, choose ``x64-Release`` configuration in Visual Studio (instead of ``x64-Debug``), double-click on ``main.cpp`` in the Solution Explorer and then click on the green Start button, finally check that "My first tube:Tube [0, 10]" appears.

Optionally, download and run `<https://github.com/ENSTABretagneRobotics/VIBES/releases/download/0.2.3/VIBes-0.2.3-win32.exe>`_ before running the project, and check that a tube appears in :ref:`VIBes viewer <sec-installation-graphics>`.


Building from sources
---------------------

You will probably need to install these prerequisites (assuming you installed `Chocolatey package manager <https://chocolatey.org/install>`_):

.. code-block:: bat

  choco install cmake git make patch winflexbison
  choco install eigen
  
Then, install the desired compiler (*e.g.* ``choco install mingw --version=11.2.0.07112021``). 

Optionally, for Python (*e.g.* ``choco install python --version=3.10.4``) and documentation:

.. code-block:: bat

  choco install doxygen.install --version=1.9.6
  choco install graphviz
  python -m pip install --upgrade pip
  pip install --upgrade wheel setuptools sphinx breathe sphinx-issues sphinx-tabs sphinx_rtd_theme sphinx-reredirects

The logic to follow will then be similar to `Linux <01-installation-full-linux.html>`_ (note that for Visual Studio, commands such as ``make install`` need to be replaced with something similar to:

.. code-block:: bash

  cmake --build . --config Release --target install

See also `Information for developers </dev/info_dev.html>`_.
