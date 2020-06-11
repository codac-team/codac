/** 
 *  Tools
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <iostream>
#include <sstream>
#include "tubex_Tools.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  void Tools::replace_all(string& input, const string& search, const string& format)
  {
    size_t index = 0;

    while(index != string::npos)
    {
      index = input.find(search, index);
      if(index != string::npos)
      {
        input.replace(index, search.length(), format);
        index += format.length();
      }
    }
  }

  string Tools::add_int(const string& str, int id, const string& sep)
  {
    ostringstream o;
    o << str << sep << id;
    return o.str();
  }

  string Tools::add_int(const string& str, const string& sep1, int id, const string& sep2)
  {
    ostringstream o;
    o << str << sep1 << id << sep2;
    return o.str();
  }

  double Tools::rand_in_bounds(const Interval& itv)
  {
    // The seed of the pseudo-random number generator is voluntarily initialized
    // outside this function, on demand.
    return max(itv.lb(),min(itv.ub(),rand()/double(RAND_MAX)*itv.diam()+itv.lb()));
  }
}