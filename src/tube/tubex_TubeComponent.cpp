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

    Interval TubeComponent::interpol(double t, const TubeComponent& derivative) const
    {
      return getSlice(t)->interpol(t, *derivative.getSlice(t));
    }

    Interval TubeComponent::interpol(const Interval& t, const TubeComponent& derivative) const
    {
      pair<Interval,Interval> p_interpol;// = partialInterpol(t, derivative);
      return p_interpol.first | p_interpol.second;
    }
    
    /*
    pair<Interval,Interval> TubeComponent::partialInterpol(const Interval& t, const TubeComponent& derivative) const
    {
      // todo: change interpol to operator() with derivative as second argument
      // todo: gates
      Interval y_tlb = interpol(t.lb(), derivative);
      Interval y_tub = interpol(t.ub(), derivative);

      // Dealing with infinity...
      //
      // In IBEX, the following is defined:
      //    Interval(NEG_INFINITY) = EMPTY_SET
      //    Interval(POS_INFINITY) = EMPTY_SET
      //    Interval(NEG_INFINITY,POS_INFINITY) = EMPTY_SET
      //
      // So, we have to detect infinite bounds
      // and work with dbl_max (max double).
      //
      // This is only a temporary solution.
      // [todo]

        double dbl_max = std::numeric_limits<double>::max();

        Interval lb, ub;
        lb.set_empty(); ub.set_empty();

        lb |= y_tlb.lb() == NEG_INFINITY ? Interval(-dbl_max) : y_tlb.lb();
        ub |= y_tlb.ub() == POS_INFINITY ? Interval(dbl_max) : y_tlb.ub();

        lb |= y_tub.lb() == NEG_INFINITY ? Interval(-dbl_max) : y_tub.lb();
        ub |= y_tub.ub() == POS_INFINITY ? Interval(dbl_max) : y_tub.ub();

        // todo: check the following simplification:
        // lb &= dbl_max*Interval(-1.,1.);
        // ub &= dbl_max*Interval(-1.,1.);

      for(int i = min(nbSlices() - 1, input2index(t.lb()) + 1) ; i < max(0, input2index(t.ub()) - 1) ; i++)
      {
        pair<Interval,Interval> p_i = eval(sliceDomain(i));
        lb |= p_i.first;
        ub |= p_i.second;
      }

      return make_pair(lb, ub);
    }*/
    
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