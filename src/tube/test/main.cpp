/* ============================================================================
 *  tube-lib - Tube class tests
 * ============================================================================
 *  Copyright : Copyright 2016 Simon Rohou
 *  License   : This program can be distributed under the terms of
 *              the Apache License, Version 2.0. See the file LICENSE.
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "../Tube.h"
#include <iostream>
#include <string>
#include <iomanip> // for setprecision()

using namespace std;
using namespace ibex;

bool test(bool success, const string& test_msg)
{
  cout << "[\033[1;" << (success ? "32mOK" : "31mNOK") << "\033[1;0m] \t" << test_msg << endl;
  return success;
}

bool compare(const Interval& intv1, const Interval& intv2)
{
  return intv1 == intv2 ||
         fabs(intv1.lb() - intv2.lb()) < 1.0e-10 && fabs(intv1.ub() - intv2.ub()) < 1.0e-10;
}

bool compare(const pair<Interval,Interval>& p_intv1, const pair<Interval,Interval>& p_intv2)
{
  return compare(p_intv1.first, p_intv2.first) && compare(p_intv1.second, p_intv2.second);
}

#include "tubes_examples.cpp"
#include "tests_base.cpp"
#include "tests_integration.cpp"
#include "tests_ctc.cpp"

int main(int argc, char *argv[])
{
  bool success = true;

  // Functions from tests_base.cpp
  success &= test(test_getY(), "test_getY");
  success &= test(test_input2index(), "test_input2index");
  success &= test(test_index2input(), "test_index2input");
  success &= test(test_volume(), "test_volume");
  success &= test(test_union(), "test_union");
  success &= test(test_intersection(), "test_intersection");
  success &= test(test_enclosed_bounds(), "test_enclosed_bounds");
  success &= test(test_set_inversion(), "test_set_inversion");
  success &= test(test_set_inversion_vector(), "test_set_inversion_vector");

  // Functions from tests_integration.cpp
  success &= test(test_primitive(), "test_primitive");
  success &= test(test_from_0_integration_double(), "test_from_0_integration_double");
  success &= test(test_from_0_integration_interval(), "test_from_0_integration_interval");
  success &= test(test_from_0_partial_integration(), "test_from_0_partial_integration");
  success &= test(test_integration(), "test_integration");
  success &= test(test_partial_integration(), "test_partial_integration");

  // Functions from tests_ctc.cpp
  //success &= test(test_ctcFwd(), "test_ctcFwd");

  if(success)
    return EXIT_SUCCESS;

  else
    return EXIT_FAILURE;
}