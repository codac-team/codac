/** 
 *  Ctc class
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_Ctc.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  Ctc::Ctc(bool intertemporal)
    : m_intertemporal(intertemporal)
  {

  }

  Ctc::~Ctc()
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

  void Ctc::set_restricted_tdomain(const Interval& tdomain)
  {
    m_restricted_tdomain = tdomain;
  }

  bool Ctc::is_intertemporal() const
  {
    return m_intertemporal;
  }
}