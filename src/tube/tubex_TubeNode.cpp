/* ============================================================================
 *  tubex-lib - TubeNode class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_TubeNode.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  TubeNode::TubeNode(const Interval& domain, const Interval& codomain) : m_domain(domain), m_codomain(codomain)
  {
    
  }

  const ibex::Interval& TubeNode::domain() const
  {
    return m_domain;
  }

  const ibex::Interval& TubeNode::codomain() const
  {
    return m_codomain;
  }

  const double TubeNode::volume() const
  {
    return m_volume;
  }
}