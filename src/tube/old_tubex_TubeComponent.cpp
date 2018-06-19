/* ============================================================================
 *  tubex-lib - TubeComponent class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_TubeComponent.h"
#include "tubex_Exception.h"
#include "tubex_DomainException.h"
#include "tubex_StructureException.h"
#include "tubex_EmptyException.h"
#include "tubex_TubeSlice.h"
#include "tubex_CtcDeriv.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  // Public methods

    // Definition

    TubeComponent::TubeComponent(const Interval& domain, const Interval& codomain) : m_domain(domain), m_codomain(codomain)
    {
      if(domain.is_unbounded() || domain.is_empty())
        throw Exception("TubeComponent constructor", "empty or unbounded domain");
    }

    TubeComponent::TubeComponent(const TubeComponent& x)
    {
      *this = x;
    }

    TubeComponent::~TubeComponent()
    {

    }

    TubeComponent& TubeComponent::operator=(const TubeComponent& x)
    {
      m_domain = x.m_domain;
      m_codomain = x.m_codomain;
      m_slices_number = x.m_slices_number;
      m_volume = x.m_volume;
      m_partial_primitive = x.m_partial_primitive;
      m_data_update_needed = x.m_data_update_needed;
      m_primitive_update_needed = x.m_primitive_update_needed;
      return *this;
    }
    
    const Interval& TubeComponent::domain() const
    {
      return m_domain;
    }
    
    // Slices structure

    int TubeComponent::nbSlices() const
    {
      return m_slices_number;
    }
    
    TubeSlice* TubeComponent::getFirstSlice() const
    {
      return const_cast<TubeSlice*>(getSlice(0));
    }

    TubeSlice* TubeComponent::getLastSlice() const
    {
      return const_cast<TubeSlice*>(getSlice(nbSlices() - 1));
    }

    Tube* TubeComponent::tubeReference() const
    {
      return m_tube_ref;
    }
    
    // Access values
    
    // Tests

    bool TubeComponent::operator==(const TubeComponent& x) const
    {
      return domain() == x.domain() && codomain() == x.codomain();
    }

    bool TubeComponent::operator!=(const TubeComponent& x) const
    {
      return domain() != x.domain() || codomain() != x.codomain();
    }

    bool TubeComponent::isSubset(const TubeComponent& x) const
    {
      StructureException::check(*this, x);
      return codomain().is_subset(x.codomain());
    }

    bool TubeComponent::isStrictSubset(const TubeComponent& x) const
    {
      return isSubset(x) && (*this) != x;
    }

    bool TubeComponent::isEmpty() const
    {
      return codomain().is_empty();
    }

    bool TubeComponent::encloses(const Trajectory& x) const
    {
      return x[m_domain].is_subset(codomain());
    }
    
    // Setting values

    // Operators

    // String

    ostream& operator<<(ostream& str, const TubeComponent& x)
    {
      str << "TubeComponent " << x.domain() << "↦" << x.codomain() << flush;
      return str;
    }

  // Protected methods

    // Definition

    /*TubeComponent::TubeComponent()
    {

    }*/

    /*// Access values
    void TubeComponent::invert(const Interval& y, vector<Interval> &v_t, const Interval& search_domain, bool concatenate_results) const
    {

    }*/
    
    // Tests

    // Setting values

    // Operators
    
    // String

    // Integration

}