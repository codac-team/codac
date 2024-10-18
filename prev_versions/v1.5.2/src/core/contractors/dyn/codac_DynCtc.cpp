/** 
 *  DynCtc class
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac_DynCtc.h"

using namespace std;
using namespace ibex;

namespace codac
{
  DynCtc::DynCtc(bool intertemporal)
    : m_intertemporal(intertemporal)
  {

  }

  DynCtc::~DynCtc()
  {

  }

  void DynCtc::preserve_slicing(bool preserve)
  {
    m_preserve_slicing = preserve;
  }

  void DynCtc::set_fast_mode(bool fast_mode)
  {
    m_fast_mode = fast_mode;
  }

  void DynCtc::restrict_tdomain(const Interval& tdomain)
  {
    m_restricted_tdomain = tdomain;
  }

  bool DynCtc::is_intertemporal() const
  {
    return m_intertemporal;
  }
}