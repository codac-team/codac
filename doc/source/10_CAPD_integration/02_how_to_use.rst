How to use
==========
The module works with reading a json configuration file that gives the parameters of the integration for CAPD
All the different options are given in the travis_test.json file in the folder make/tests/3rd/Capd2Tubex/parameters


To use the module you need to add at the beginning of your code:

.. code-block:: c++

    #include "tubex-3rd.h"
    // or
    #include "tubex_Capd2tTubex.h"
    /*
    *
    * other include define...
    *
    */

    int main()
    {
        ifstream i("parameters/travis_test.json");
        json j;
        i >> j;
        vector<IntervalVector> ibex_curve = capd2ibex(j); // list of IBEX format IntervalVector
        // or
        TubeVector output = capd2tubex(j); // one tube of Tubex format

    }
    You can then use any Tubex operator on this new generated tube