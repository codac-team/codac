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

    double TubeComponent::index2input(int slice_id) const
    {
      return getSlice(slice_id)->domain().lb(); // by convention
    }
    
    const IntervalVector TubeComponent::sliceBox(int slice_id) const
    {
      IntervalVector box(2);
      const TubeSlice* slice = getSlice(slice_id);
      box[0] = slice->domain();
      box[1] = slice->codomain();
      return box;
    }
    
    const IntervalVector TubeComponent::sliceBox(double t) const
    {
      return sliceBox(input2index(t));
    }
    
    const Interval& TubeComponent::sliceDomain(int slice_id) const
    {
      return getSlice(slice_id)->domain();
    }
    
    const Interval& TubeComponent::sliceDomain(double t) const
    {
      return getSlice(t)->domain();
    }
    
    TubeNode* TubeComponent::tubeReference() const
    {
      return m_tube_ref;
    }
    
    // Access values
    
    // Tests
    
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

    void TubeComponent::setTubeReference(TubeNode *tube_ref)
    {
      m_tube_ref = tube_ref;
    }

    /*TubeComponent::TubeComponent()
    {

    }*/

    /*// Access values
    void TubeComponent::invert(const Interval& y, vector<Interval> &v_t, const Interval& search_domain, bool concatenate_results) const
    {

    }*/
    
    // Tests

    bool TubeComponent::isEqual(const TubeComponent& x) const
    {
      return domain() == x.domain() && codomain() == x.codomain();
    }

    bool TubeComponent::isDifferent(const TubeComponent& x) const
    {
      return domain() != x.domain() || codomain() != x.codomain();
    }

    // Setting values

    // Operators
    
    // String

    // Integration

}