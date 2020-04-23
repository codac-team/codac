.. _sec-extensions-capd-howto:

.. warning::
  
  This part of the documentation is deprecated. Several changes are currently perfomed on the library.
  A new stable version of Tubex will be released in the coming weeks.

How to use
==========

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