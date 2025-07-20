:orphan:

.. _sec-installation-full-windows:

##########################################
Installing Codac v1 on Windows for C++ use
##########################################


Quick start
-----------

.. rubric:: Using MinGW

Check https://community.chocolatey.org/packages/codac/1.5.7.20250626.


.. rubric:: Using Visual Studio

Download and extract *e.g.* ``codac_standalone_x64_vc17.zip`` (for Visual Studio 2022) from `<https://github.com/codac-team/codac/releases/tag/v1.5.7>`_, open ``example\CMakelists.txt``, choose ``x64-Release`` configuration in Visual Studio (instead of ``x64-Debug``), double-click on ``main.cpp`` in the Solution Explorer and then click on the green Start button, finally check that "My first tube:Tube [0, 10]" appears.

Optionally, download and run `<https://github.com/ENSTABretagneRobotics/VIBES/releases/download/0.2.3/VIBes-0.2.3-win32.exe>`_ before running the project, and check that a tube appears in :ref:`VIBes viewer <sec-installation-graphics>`.


Building from sources
---------------------

You will probably need to install these prerequisites (assuming you installed `Chocolatey package manager <https://chocolatey.org/install>`_):

.. code-block:: bat

  choco install -y cmake.install --installargs 'ADD_CMAKE_TO_PATH=System'
  choco install -y git wget make patch winflexbison
  choco install -y eigen

.. note::

  | All the commands that install packages need to be run from an `elevated administrator terminal <https://www.elevenforum.com/t/open-elevated-windows-terminal-as-administrator-in-windows-11.556/>`_. See also `sudo command availability <https://www.xda-developers.com/how-use-sudo-windows/>`_.

Then, install the desired compiler (*e.g.* ``choco install -y mingw --version=11.2.0.07112021``). 

Optionally, for Python (*e.g.* ``choco install -y python --version=3.10.4``) and documentation:

.. code-block:: bat

  choco install -y doxygen.install --version=1.9.6
  choco install -y graphviz
  python -m pip install --upgrade pip
  pip install --upgrade wheel setuptools sphinx breathe sphinx-issues sphinx-tabs sphinx_rtd_theme sphinx-reredirects

The logic to follow will then be similar to :ref:`Linux <sec-installation-full-linux>`. You might want to right-click in your desired folder and choose ``Git Bash Here`` to run the commands related to Git and compilation. Note that if using Visual Studio build tools, commands such as ``make`` and ``make install`` need to be replaced with something similar to:

.. code-block:: bash

  cmake --build . --config Release
  cmake --build . --config Release --target install

or

.. code-block:: bash

  cmake --build . -j 4 --config Release
  cmake --build . -j 4 --config Release --target install

to enable parallel compilation on 4 cores.

.. warning::

  | Dependencies of Codac need to built with the same compiler as Codac itself. First, you will need to recompile IBEX (from https://github.com/lebarsfa/ibex-lib) and its dependencies GAOL (https://github.com/lebarsfa/GAOL) and mathlib (https://github.com/lebarsfa/mathlib), which need to be installed manually via CMake with the options ``-D GAOL_ENABLE_PRESERVE_ROUNDING=OFF -D GAOL_ENABLE_OPTIMIZE=ON -D GAOL_ENABLE_VERBOSE_MODE=OFF``. During the compilation of IBEX, specify where they are installed if necessary using the CMake options, e.g., ``-D INTERVAL_LIB=gaol -D MATHLIB_DIR=../mathlib -D GAOL_DIR=../gaol``).

.. warning::

  | You might need to replace all occurences of :literal:`PATH_SUFFIXES \ ` with something similar to :literal:`PATHS ${CMAKE_CURRENT_LIST_FILE}/../../../../ PATH_SUFFIXES \ ` in all ``.cmake`` in ``codac/share/codac/cmake/`` (where Codac was installed) if a CMake project that tries to use Codac appears to find its installation location but fails to configure the project properly.

.. warning::

  | If using the MinGW version installed using ``choco install -y mingw --version=11.2.0.07112021``, you might need to install the following patch for debugging to work correctly:

  .. code-block:: bat

    wget.exe https://packages.ensta-bretagne.fr/choco/mingw-patch.11.2.0.20230603.nupkg --no-check-certificate -nv
    choco install -y -s . mingw-patch --version=11.2.0.20230603

.. note::

  | If using CLion IDE, it might be difficult to set CMake options such as ``-DBUILD_TESTS=ON`` from inside the IDE (it seems related to Ninja support). As a workaround, try to set these options, assuming the MinGW version suggested above was installed:

  .. image:: img/clion_1.png
           :width: 800px

  |

  .. image:: img/clion_2.png
           :width: 800px

  |

  .. image:: img/clion_3.png
           :width: 800px

.. tip::

  | If using Visual Studio IDE or build tools, you might want to add the ``/MP`` compiler flag to enable more parallelization possibilities, in *e.g.*

  .. code-block:: bash

    cmake -G "Visual Studio 17 2022" -A x64 -D CMAKE_PREFIX_PATH="../ibex" -D CMAKE_INSTALL_PREFIX="../codac" -D CMAKE_C_FLAGS="/MP4" -D CMAKE_CXX_FLAGS="/MP4" ..

  Then, opening the generated ``.sln`` file and building the ``INSTALL`` project from inside Visual Studio or from the command-line (*e.g.* ``cmake --build . -j 4 --config Debug --target install``) should be faster (change ``4`` in ``-j 4`` and ``/MP4`` to another number of cores if needed).

.. tip::

  | To run the tests, the generic command is ``ctest -C Debug -V --output-on-failure`` or ``ctest -C Release -V --output-on-failure`` (assuming the CMake option ``-DBUILD_TESTS=ON`` was specified).

See also :ref:`Information for developers <sec-manual-dev>`.
