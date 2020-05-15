/** 
 *  CtcFunction class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_CtcFunction.h"
#include "tubex_Domain.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  CtcFunction::CtcFunction(const char* x1, const char* f)
    : Ctc3BCid(*new ibex::CtcFwdBwd(*new ibex::Function(x1, CtcFunction::parse_f(f).c_str())))
  {
    // todo: clean delete
  }

  CtcFunction::CtcFunction(const char* x1, const char* x2, const char* f)
    : Ctc3BCid(*new ibex::CtcFwdBwd(*new ibex::Function(x1, x2, CtcFunction::parse_f(f).c_str())))
  {

  }

  CtcFunction::CtcFunction(const char* x1, const char* x2, const char* x3, const char* f)
    : Ctc3BCid(*new ibex::CtcFwdBwd(*new ibex::Function(x1, x2, x3, CtcFunction::parse_f(f).c_str())))
  {

  }

  CtcFunction::CtcFunction(const char* x1, const char* x2, const char* x3, const char* x4, const char* f)
    : Ctc3BCid(*new ibex::CtcFwdBwd(*new ibex::Function(x1, x2, x3, x4, CtcFunction::parse_f(f).c_str())))
  {

  }

  CtcFunction::CtcFunction(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* f)
    : Ctc3BCid(*new ibex::CtcFwdBwd(*new ibex::Function(x1, x2, x3, x4, x5, CtcFunction::parse_f(f).c_str())))
  {

  }

  CtcFunction::CtcFunction(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* f)
    : Ctc3BCid(*new ibex::CtcFwdBwd(*new ibex::Function(x1, x2, x3, x4, x5, x6, CtcFunction::parse_f(f).c_str())))
  {

  }

  CtcFunction::CtcFunction(const ibex::Function& f, const ibex::Domain& y)
    : Ctc3BCid(*new ibex::CtcFwdBwd(*new ibex::Function(f), y))
  {

  }
  
  CtcFunction::CtcFunction(const ibex::Function& f, const ibex::Interval& y)
    : Ctc3BCid(*new ibex::CtcFwdBwd(*new ibex::Function(f), y))
  {

  }

  CtcFunction::CtcFunction(const ibex::Function& f, const ibex::IntervalVector& y)
    : Ctc3BCid(*new ibex::CtcFwdBwd(*new ibex::Function(f), y))
  {

  }

  const string CtcFunction::parse_f(const char* f)
  {
    string str_f(f);
    string str_eq("=");

    if(str_f.find(str_eq) != string::npos)
    {
      str_f.replace(str_f.find(str_eq), str_eq.length(), "-(");
      str_f += ")";
    }

    assert(str_f.find(str_eq) == string::npos && "only one '=' allowed");
    assert(str_f.find(">") == string::npos
        && str_f.find("<") == string::npos
        && "inequalities not supported by this contractor");

    return str_f;
  }
}