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

    TubeNode::TubeNode(const Interval& domain, const Interval& codomain) : m_domain(domain), m_codomain(codomain)
    {
      if(domain.is_unbounded() || domain.is_empty())
        throw Exception("TubeNode constructor", "empty or unbounded domain");
    }

    TubeNode::TubeNode(const TubeNode& x)
    {
      *this = x;
    }

    TubeNode::~TubeNode()
    {

    }

    TubeNode& TubeNode::operator=(const TubeNode& x)
    {
      m_domain = x.m_domain;
      m_codomain = x.m_codomain;
      m_slices_number = x.m_slices_number;
      m_volume = x.m_volume;
      return *this;
    }
    
    const Interval& TubeNode::domain() const
    {
      return m_domain;
    }
    
    // Slices structure

    int TubeNode::nbSlices() const
    {
      return m_slices_number;
    }
    
    TubeSlice* TubeNode::getFirstSlice() const
    {
      return const_cast<TubeSlice*>(getSlice(0));
    }

    TubeSlice* TubeNode::getLastSlice() const
    {
      return const_cast<TubeSlice*>(getSlice(nbSlices() - 1));
    }

    double TubeNode::index2input(int slice_id) const
    {
      return getSlice(slice_id)->domain().lb(); // by convention
    }
    
    const IntervalVector TubeNode::sliceBox(int slice_id) const
    {
      IntervalVector box(2);
      const TubeSlice* slice = getSlice(slice_id);
      box[0] = slice->domain();
      box[1] = slice->codomain();
      return box;
    }
    
    const IntervalVector TubeNode::sliceBox(double t) const
    {
      return sliceBox(input2index(t));
    }
    
    const Interval& TubeNode::sliceDomain(int slice_id) const
    {
      return getSlice(slice_id)->domain();
    }
    
    const Interval& TubeNode::sliceDomain(double t) const
    {
      return getSlice(t)->domain();
    }
    
    // Access values

    /*
    Interval TubeNode::interpol(double t, const TubeNode& derivative) const
    {
      // todo: change interpol to operator() with derivative as second argument
      return getSlice(t)->interpol(t, *derivative.getSlice(t));
    }
    
    Interval TubeNode::interpol(const Interval& t, const TubeNode& derivative) const
    {
      // todo: change interpol to operator() with derivative as second argument
      pair<Interval,Interval> p_interpol = partialInterpol(t, derivative);
      return p_interpol.first | p_interpol.second;
    }
    
    pair<Interval,Interval> TubeNode::partialInterpol(const Interval& t, const TubeNode& derivative) const
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
    }
    
    // Tests

    bool TubeNode::isEmpty() const
    {
      return codomain().is_empty();
    }*/
    
    bool TubeNode::operator==(const TubeNode& x) const
    {
      
    }
    
    bool TubeNode::operator!=(const TubeNode& x) const
    {
      
    }
    /*
    // Setting values

    bool TubeNode::contract(const Interval& y, int slice_id)
    {

    }
    
    TubeNode& TubeNode::inflate(const Interval& y, int slice_id)
    {

    }
    
    TubeNode& TubeNode::inflate(const Interval& y, double t)
    {

    }
    
    TubeNode& TubeNode::inflate(double rad)
    {

    }
    
    // Operators
    
    TubeNode& TubeNode::operator|=(const TubeNode& x)
    {

    }
    
    TubeNode& TubeNode::operator|=(const Trajectory& x)
    {

    }
    
    TubeNode& TubeNode::operator&=(const TubeNode& x)
    {

    }

    // String

    ostream& operator<<(ostream& str, const TubeNode& x)
    {

    }
    
    // Integration

    Interval TubeNode::integral(double t) const
    {

    }
    
    Interval TubeNode::integral(const Interval& t) const
    {

    }
    
    Interval TubeNode::integral(const Interval& t1, const Interval& t2) const
    {

    }
    
    pair<Interval,Interval> TubeNode::partialIntegral(const Interval& t) const
    {

    }
    
    pair<Interval,Interval> TubeNode::partialIntegral(const Interval& t1, const Interval& t2) const
    {

    }*/
  
  // Protected methods

    // Definition

    /*TubeNode::TubeNode()
    {

    }*/

    /*// Access values
    void TubeNode::invert(const Interval& y, vector<Interval> &v_t, const Interval& search_domain, bool concatenate_results) const
    {

    }
    
    // Tests

    // Setting values

    // Operators

    void TubeNode::unionWith_localUpdate(const TubeNode *x)
    {

    }
    
    void TubeNode::intersectWith_localUpdate(const TubeNode *x)
    {

    }
    
    // String

    // Integration

    pair<Interval,Interval> TubeNode::getPartialPrimitiveValue(const Interval& t) const
    {

    }*/
}