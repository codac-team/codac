.. _sec-extensions-capd-install:

.. warning::
  
  This part of the documentation is deprecated. Several changes are currently perfomed on the library.
  A new stable version of Tubex will be released in the coming weeks.

Installing CAPD
===============

It is now possible to use CAPD to compute a guaranteed integration of an ODE. The result is enclosed in a ``TubeVector`` object of Tubex.

To do so, you need to have CAPD and its DynSys module installed.
You may need to install the following packages in order to compile CAPD:

.. code-block:: bash

    sudo apt-get svn install autoconf libtool

Then you can proceed to the compilation of the sources of CAPD:

.. code-block:: bash

  svn co https://svn.capdnet.ii.uj.edu.pl/capd/ --non-interactive --trust-server-cert-failures=unknown-ca,cn-mismatch,expired,not-yet-valid,other
  cd capd
  svn co https://svn.capdnet.ii.uj.edu.pl/capdDynSys4 --non-interactive --trust-server-cert-failures=unknown-ca,cn-mismatch,expired,not-yet-valid,other
  autoreconf --install
  ./configure --with-filib=check --with-mpfr=check # other options are available on the documentation website
  make
  sudo make install

.. - autoconf
.. - libtool
.. - libgmp10
.. - libgmp-dev
.. - libgmpxx4ldbl
.. - libmpfr6
.. - libmpfr-dev
.. - libmpfrc++-dev
.. - libboost-dev
.. - libboost1.65-dev
.. - liblog4cxx-dev
.. - liblog4cxx10v5

.. Note that packages version may have changed, use

..     sudo apt search <pkg_name> to check if the name as changed due to updated version

.. note::

  For additional options on CAPD compilation and integration please visit the `CAPD official documentation <http://capd.sourceforge.net/capdDynSys/docs/html/>`_.

Finally, you need to compile Tubex again:

.. code-block:: bash

  cmake <other_options> -DWITH_CAPD=ON ..
  make
  sudo make install