Development
===========

Documentation
^^^^^^^^^^^^^

The documentation is generated thanks to `Sphinx <http://www.sphinx-doc.org/en/stable/>`_.

.. code-block:: none

  pip install Sphinx
  pip install sphinx_rtd_theme

Then, run:

.. code-block:: none

  cd doc ; make html

Tests
^^^^^

For now, we are using `Catch <https://github.com/philsquared/Catch>`_ to implement tests.

Note that the *cmake* project file may need to be configured in order to set :code:`BUILD_TESTS`.

To launch tests:

.. code-block:: none

  cd build
  cmake -DBUILD_TESTS=ON ..
  make
  make test

These tests include the run of C++ examples.