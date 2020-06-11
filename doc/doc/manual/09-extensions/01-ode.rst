.. _sec-extensions-ode:

.. warning::
  
  This part of the documentation is deprecated. Several changes are currently performed on the library.
  A new stable version of Tubex will be released in the coming weeks.

*******************************************
Using external integration methods for ODEs
*******************************************

.. contents::


Installing CAPD
---------------

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


How to use
----------

The module works with reading a JSON configuration that gives the parameters of the integration for CAPD.


To use the module you need to add at the beginning of your code:

.. code-block:: c++

  #include <tubex-capd.h>


Here is an example of use:

.. code-block:: c++

  #include <tubex.h>
  #include <tubex-capd.h>

  int main()
  {
    auto j = R"(
    {
      "config":
      {
        "function":"var:x,y;fun: x^3+x*y^2-x+y, y^3+x^2*y-x-y;",
        "fixed_step": true,
        "integration_time": 5,
        "integration_step": 0.001,
        "nb_fixed_parameters":0,
        "parameters_name":[],
        "parameters_value":[],
        "dimensions": 2,
        "initial_coordinates": [[0.5,0.5],[0,0]],

        "output_file": "reference_output.txt",
        "tubex_output": "tube.tube"
      }
    }
    )"_json;

    vector<IntervalVector> ibex_curve = capd2ibex(j); // list of IBEX format IntervalVector
    // or
    TubeVector output = capd2tubex(j); // one tube of Tubex format
  }


All the different options are given in the above example.

Note that you can also define the configuration in a separated file with:

.. code-block:: c++

    ifstream i("parameters/travis_test.json");
    json j;
    i >> j;

You can then use any Tubex operator on this new generated tube.