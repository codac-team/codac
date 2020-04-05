CAPD Integration
================

It is now possible to use CAPD as a tool to generate a tube which is the result of a guaranteed integration
performed by CAPD.

To do so you need to have CAPD and its DynSys module installed. For the minimum requirements you can use the following
commands

.. code-block:: bash

  sudo apt install

  svn co https://svn.capdnet.ii.uj.edu.pl/capd/ --non-interactive --trust-server-cert-failures=unknown-ca,cn-mismatch,expired,not-yet-valid,other
  cd capd
  svn co https://svn.capdnet.ii.uj.edu.pl/capdDynSys4 --non-interactive --trust-server-cert-failures=unknown-ca,cn-mismatch,expired,not-yet-valid,other
  autoreconf --install
  ./configure --with-filib=check --with-mpfr=check        #other options are available on the documentation website
  make
  sudo make install

You may need to install the additional packages in order to compile and install CAPD:

- autoconf
- libtool
- libgmp10
- libgmp-dev
- libgmpxx4ldbl
- libmpfr6
- libmpfr-dev
- libmpfrc++-dev
- libboost-dev
- libboost1.65-dev
- liblog4cxx-dev
- liblog4cxx10v5

Note that packages version may have changed, use

.. code-block:: bash
    sudo apt search <pkg_name> to check if the name as changed due to updated version

The following command allows you to install everything at once

.. code-block:: bash

    sudo apt-get  install autoconf libtool libgmp10 libgmp-dev libgmpxx4ldbl libmpfr6 libmpfr-dev libmpfrc++-dev
    libboost-dev libboost1.65-dev liblog4cxx-dev liblog4cxx10v5

For additional options on CAPD compilation and integration please visit CAPD official `documentation <http://capd.sourceforge.net/capdDynSys/docs/html/>`_

You then need to compile Tubex again if it as already been done using  first cmake and the option WITH_CAPD .  Then
compile and install the code with make an make install

.. code-block:: bash

    cmake <options> -DWITH_CAPD=ON
    make
    make install



