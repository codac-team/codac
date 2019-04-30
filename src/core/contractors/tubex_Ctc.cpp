/** 
 *  Ctc class
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Simon Rohou
 *  \copyright  Copyright 2019 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_Ctc.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  Ctc::Ctc()
  {

  }

  void Ctc::preserve_slicing(bool preserve)
  {
    m_preserve_slicing = preserve;
  }

  void Ctc::set_fast_mode(bool fast_mode)
  {
    m_fast_mode = fast_mode;
  }

  void Ctc::set_restricted_domain(const Interval& domain)
  {
    m_restricted_domain = domain;
  }
}