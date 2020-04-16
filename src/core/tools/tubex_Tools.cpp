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

namespace tubex
{
  void Tools::replace_all(string& input, const string& search, const string& format)
  {cout << "replace" << endl;
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
    cout << "endreplace" << endl;
  }

  string Tools::add_int(const string& name, int id, const string& sep)
  {
    ostringstream o;
    o << name << sep << id;
    return o.str();
  }

  string Tools::add_int(const string& name, const string& sep1, int id, const string& sep2)
  {
    ostringstream o;
    o << name << sep1 << id << sep1;
    return o.str();
  }
}