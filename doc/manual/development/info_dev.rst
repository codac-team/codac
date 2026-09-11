.. _sec-dev-info:

Information for developers
==========================

Building the Manual with Sphinx
-------------------------------

To build this manual using Sphinx, follow these steps:

1. **Requirements**: Install `Sphinx <https://www.sphinx-doc.org/>`_ together with the extensions used by Codac, if they are not already installed. You will need also the ``dot`` command from `Graphviz <https://graphviz.org/>`_.

   .. code-block:: bash

      sudo apt-get install graphviz
      pip install sphinx breathe furo sphinx-rtd-theme sphinx-math-dollar sphinx-tabs sphinx-togglebutton

   That list is where the ``extensions`` declared in ``doc/manual/conf.py.in``
   come from -- ``breathe``, ``sphinx_math_dollar``, ``sphinx_tabs``,
   ``sphinx_togglebutton`` and ``sphinx_rtd_theme``, the sixth one,
   ``sphinx.ext.mathjax``, shipping with Sphinx itself -- plus ``furo``, which is
   the theme the manual is actually rendered with (``html_theme``). Adding an
   extension to ``conf.py.in`` means adding its package here as well. Sphinx
   itself comes from ``pip`` rather than from the distribution's
   ``sphinx-common`` package, so that Sphinx and its extensions are installed by
   the same tool and cannot end up in two incompatible versions.

   Building the manual asks for a more recent Python than building the binding
   does. The current Sphinx requires **Python 3.12 or newer**, and the current
   ``sphinx-math-dollar``, ``sphinx-tabs`` and ``breathe`` require 3.11, 3.10 and
   3.9 respectively. Nothing fails on an older interpreter, and that is precisely
   what makes it worth knowing: ``pip`` quietly falls back to whatever release
   still supports it, so on Python 3.8 the command line above installs Sphinx
   7.1.2 and ``furo`` 2024.8.6 rather than the current ones. When the manual
   renders differently on two machines, comparing ``pip list`` is the first thing
   to do.

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

   - a supported version of Python (>=3.8).
   - `Doxygen <https://www.doxygen.nl>`_. **1.17.0 is the recommended version, and 1.16.1 the minimum.** Avoid 1.18.0: it segfaults while parsing Codac's headers on macOS with an Intel processor, which is why ``.github/workflows/macosmatrix.yml`` pins 1.17.0 on those runners. On Linux systems, recent releases are not available as Debian packages, so we advise to install Doxygen from the sources:

   .. code-block:: bash

      cd $HOME
      git clone https://github.com/doxygen/doxygen
      cd doxygen
      git checkout Release_1_17_0 # 1.16.1 is the oldest that works; 1.18.0 is known to break on MacOS intel
      mkdir build ; cd build
      cmake -DCMAKE_INSTALL_PREFIX=$HOME/doxygen/build_install ..
      make ; make install

   .. admonition:: About the use of Doxygen
      
      Doxygen software extracts C++ documentation from header files into XML format. We then convert this data into docstring format before embedding it into the binding binaries. In this way, the writing of the documentation is centralized in a single location in the C++ header files.

      This is also why the Doxygen version is worth pinning rather than taking whatever is at hand. The names of the generated docstring macros are derived from the text Doxygen produces, so a wording change between two releases renames them, and the binding sources that spell those names out then stop compiling. ``scripts/doxygen/doxygen2docstring.py`` normalizes that text rather than trusting it, which absorbs small differences; staying within the recommended range is what covers the rest.


2. **Configure IBEX prior to compiling Codac**:
   
   We recall that IBEX sources can be obtained with:

   .. code-block:: bash
      
      git clone -b ibex-2.8.9.20260819 https://github.com/lebarsfa/ibex-lib.git $HOME/ibex-lib
      cd $HOME/ibex-lib

   ``ibex-2.8.9.20260819`` is the version every continuous-integration workflow
   builds and links against; it is the single place it is written down,
   ``scripts/dependencies/install_ibex.sh``, that those workflows read. Cloning
   the default branch instead gives whatever that fork has reached since, which
   may or may not still match Codac.

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


   In addition to the ``-fPIC`` options, you will have to configure ``WITH_PYTHON=ON`` and ``PYBIND11_FINDPYTHON=OFF``. Note that CMake will automatically get the `pybind11 <https://pybind11.readthedocs.io>`_ files required for the binding. Also, you will have to configure ``BUILD_TESTS=ON`` if you want to run the unit tests.
   
   .. code-block:: bash
      
      mkdir build ; cd build
      cmake -DCMAKE_CXX_FLAGS="-fPIC" -DCMAKE_C_FLAGS="-fPIC" -DWITH_PYTHON=ON -DPYBIND11_FINDPYTHON=OFF -DBUILD_TESTS=ON -DCMAKE_INSTALL_PREFIX=$HOME/codac/build_install -DCMAKE_PREFIX_PATH="$HOME/ibex-lib/build_install;$HOME/doxygen/build_install" -DCMAKE_BUILD_TYPE=Release ..
      make ; make install

4. **Configure your Python environment**:

   Finally, you need to configure your system so that Python can find access to your Codac binding binaries:

   .. code-block:: bash
      
      cd $HOME/codac/build/python/python_package
      pip install --user -e .

   (Earlier versions of this page used ``python setup.py develop --user``. That
   command has been removed from setuptools; ``pip install -e`` is its
   replacement and does the same thing.)

   Alternatively, or in addition, update your configuration file (for instance, ``$HOME/.bashrc`` on Linux systems) with:

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

   Note that before executing the example, you will have to launch the `VIBes viewer <../visualization/vibes.html>`_.
   You should obtain a graphical output corresponding to a set inversion.


.. _sec-dev-info-tests:

Running the tests
-----------------

The test suite is built when the project is configured with ``-D BUILD_TESTS=ON``
and run through `CTest <https://cmake.org/cmake/help/latest/manual/ctest.1.html>`_
from the build directory:

.. code-block:: bash

   cd $HOME/codac/build
   ctest --output-on-failure

Two ``make`` targets wrap it, and they are not interchangeable:

.. code-block:: bash

   make test    # what CMake provides: runs ctest on the build tree as it stands
   make check   # rebuilds the suite first, and prints the output of what failed

``make check`` is the one to use after editing a source file. ``make test`` runs
whatever executables are already there, so it happily reports a pass on the
previous build of a test you have just changed. Both accept ``ARGS`` to forward
options to ctest, with the Makefile generator:

.. code-block:: bash

   make check ARGS="-R AffineVector"

Every entry of the suite exists in up to two flavours, one per language: a C++
test, and -- when the build has ``WITH_PYTHON=ON`` and a Python file of the same
name sits next to the C++ one -- its Python counterpart. The CTest name of each
is built from the path of the source under ``tests/``, with the slashes turned
into underscores and ``_cpp`` or ``_py`` appended, so
``tests/core/domains/affine/codac2_tests_AffineVector.cpp`` becomes
``codac2_tests_core_domains_affine_AffineVector_cpp``. ``-R`` takes a regular
expression matched anywhere in that name, which is what makes the long names
painless:

.. code-block:: bash

   ctest -N                          # list the tests without running them
   ctest -R AffineVector             # both flavours of one test
   ctest -R "_py"                    # the whole Python half of the suite
   ctest -R "_cpp" -j 8              # the C++ half, eight at a time
   ctest -V -R AffineVector_cpp      # verbose: everything the test prints

``ctest -V`` matters more than it looks, for the reason given in the next
section: a sanitizer can report a problem without failing the test, and without
``-V`` CTest discards the output of every test that passes.

.. _sec-dev-info-adding-a-test:

Adding a test
-------------

1. Write the test as ``tests/<path>/codac2_tests_<name>.cpp``, using
   `Catch2 <https://github.com/catchorg/Catch2>`_ (version 3), next to the
   existing tests of the same part of the library.

2. **Register it**, by adding its path -- relative to ``tests/``, without the
   ``.cpp`` extension -- to the ``SRC_TESTS`` list of ``tests/CMakeLists.txt``:

   .. code-block:: cmake

      list(APPEND SRC_TESTS

        # ----------------------------------------------------------------
        # Core / Affine
        # ----------------------------------------------------------------

        core/domains/affine/codac2_tests_AffineForm_base
        core/domains/affine/codac2_tests_MyNewTest      # <-- the new one
        ...

   This step is not optional, and nothing will remind you of it: a test file that
   is not listed there is never compiled and never run. The suite stays green
   while the new test does nothing at all, which is the one failure mode worth
   guarding against by hand.

3. Optionally, write the Python counterpart as
   ``tests/<path>/codac2_tests_<name>.py`` -- same name, same directory. It needs
   no registration: it is picked up automatically and added as
   ``codac2_tests_<name>_py`` whenever the build has ``WITH_PYTHON=ON``.

4. Re-run CMake, and check that what you expect is actually there:

   .. code-block:: bash

      cmake .. && ctest -N -R MyNewTest

.. _sec-dev-info-debug:

Debug builds: AddressSanitizer and UndefinedBehaviorSanitizer
-------------------------------------------------------------

Configuring with ``-D CMAKE_BUILD_TYPE=Debug`` does more than turn the
optimizations off: the top-level ``CMakeLists.txt`` also compiles and links
everything with ``-fsanitize=address,undefined``. A Debug build therefore reports
out-of-bounds accesses, use-after-free, memory leaks and undefined behaviour as it
runs, and running the test suite in that configuration is the cheapest way to find
them.

.. code-block:: bash

   mkdir build_debug ; cd build_debug
   cmake -D CMAKE_BUILD_TYPE=Debug -D BUILD_TESTS=ON -D TEST_EXAMPLES=ON ..
   make 
   ctest -V --output-on-failure

Two things are worth knowing about it.

- The sanitizers are enabled only if the compiler in use actually ships their
  runtime. CMake probes for it and says which way it went, at configure time:
  either ``ASan/UBSan will be enabled for Debug-configuration builds.`` or ``No
  ASan/UBSan runtime found ...: Debug-configuration builds will not be
  sanitized.`` Falling back to an unsanitized Debug build is deliberate -- it
  beats a configuration that fails to link on every target -- but it does mean
  that line is worth reading before concluding that a build found nothing.

- ASan and UBSan do not agree on what a diagnostic costs. A buffer overflow or a
  leak aborts the process and fails the test; a UBSan ``runtime error:`` only
  prints and lets the run continue. A test can therefore pass having reported
  dozens of undefined behaviours, and ``ctest -V`` is what makes them visible.

This is the configuration ``.github/workflows/unixdebug.yml`` and
``.github/workflows/macdebug.yml`` run on every push, across four Linux and four
macOS jobs (GCC and Clang, x86_64 and arm64).

.. _sec-dev-info-examples:

Building and running the examples
---------------------------------

``-D TEST_EXAMPLES=ON``, together with ``BUILD_TESTS=ON``, compiles every example
of ``examples/`` against the library being built and registers each one as a CTest
integration test:

.. code-block:: bash

   cmake -D BUILD_TESTS=ON -D TEST_EXAMPLES=ON -D WITH_PYTHON=ON ..
   make 
   ctest -R codac2_examples

The names follow the same rule as the tests, from the path under ``examples/``:
``examples/03_sivia/main.cpp`` becomes ``codac2_examples_03_sivia_main_cpp``, and
its Python counterpart ``..._py``. Examples that need an optional dependency --
CAPD, or SymPy -- are registered only when it is available.

They are worth running for more than the fact that they still produce a result.
The examples are what a user compiles against the *installed* headers, and only
the public headers are made visible to them here, so an example that stops
compiling in this build is an example a user could no longer build either.

.. _sec-dev-info-coverage:

Measuring the code coverage
---------------------------

Locally: ``make coverage``
^^^^^^^^^^^^^^^^^^^^^^^^^^

Configuring with ``-D WITH_COVERAGE=ON`` instruments the build and adds two
targets. It needs `gcovr <https://gcovr.com>`_ (``pip install gcovr``) and a
GCC or Clang toolchain -- MSVC provides no ``gcov`` and the option refuses to
configure there.

.. code-block:: bash

   mkdir build_coverage ; cd build_coverage
   cmake -D CMAKE_BUILD_TYPE=Release \
         -D WITH_COVERAGE=ON \
         -D BUILD_TESTS=ON -D TEST_EXAMPLES=ON -D WITH_PYTHON=ON ..
   make 
   make coverage

``make coverage`` runs the whole CTest suite and turns the counters ``gcov``
leaves behind into a report: a summary printed on the console, ``coverage.html``
to open in a browser (with one page per source file next to it), and
``coverage.xml`` in the Cobertura format that most editors and code-review tools
read.

``make coverage-report`` is the second half on its own. It re-reads whatever the
last ``ctest`` invocation happened to touch, without running anything, which is
what to use after running part of the suite by hand:

.. code-block:: bash

   ctest -R codac2_tests_AffineForm
   make coverage-report

Turning ``BUILD_TESTS`` and ``TEST_EXAMPLES`` on is what makes the figure mean
anything -- coverage answers "what do the tests reach", and the examples are
integration tests that reach code the unit tests do not.

Everything else the measurement needs is decided by the top-level
``CMakeLists.txt`` rather than left to the caller: the instrumentation flags, the
``gcov`` matching the compiler (Clang's counters are read by ``llvm-cov gcov``,
not by GCC's ``gcov``, and pointing ``gcovr`` at the wrong one yields an empty
report rather than an error), the exclusion of the dependencies CMake fetches into
the build tree, and ``CMAKE_INTERPROCEDURAL_OPTIMIZATION=OFF`` -- which is not
optional, pybind11 adding ``-flto`` to the modules it builds and GCC being unable
to combine that with ``gcov`` instrumentation, the link then failing on an
undefined vtable.

From GitHub Actions
^^^^^^^^^^^^^^^^^^^

Every push runs the same measurement in CI, in the job named **Ubuntu 26.04 GCC
x86_64 Release coverage** of ``.github/workflows/unixdebug.yml``. It configures
with ``-D WITH_COVERAGE=ON``, runs the whole suite -- the C++ and Python unit
tests, plus every example -- and calls the very same ``coverage-report`` target,
so what CI measures and what ``make coverage`` measures cannot drift apart. The
summary appears in the job log, and the report is uploaded as an artifact named
``coverage-report``, kept for 14 days.

1. **Download the report**: with the `GitHub CLI <https://cli.github.com/>`_,
   from a clone of the repository:

   .. code-block:: bash

      # the latest run of the workflow on the current branch
      gh run list --workflow unixdebug.yml --limit 1
      gh run download <run-id> -n coverage-report -D coverage

   The directory then holds ``coverage.xml`` and the ``coverage.*.html`` files,
   ``coverage.html`` being the index to open in a browser.

2. **Read the summary without downloading anything**: the three totals are printed
   at the end of the *Coverage report* step of the job, and can be pulled from the
   log directly:

   .. code-block:: bash

      gh run view <run-id> --log | grep -E "^(lines|functions|branches):"

.. admonition:: Reading the figures

   Two things make the raw percentages narrower than they look.

   A header compiled into several translation units appears once per include
   path in gcovr's output -- typically once under ``src/`` and once under
   ``build/include/`` -- so summing those entries counts each of its lines twice.
   A line reached through either path is covered, and the two entries have to be
   merged line by line to get a per-file figure.

   And on heavily templated headers, gcov counts every instantiated line: the
   denominators are inflated, and the percentages are best read as orders of
   magnitude rather than to the decimal.


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