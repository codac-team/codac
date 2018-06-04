/* ============================================================================
 *  tubex-lib - Tube class
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
#include "tubex_TubeSlice.h"
#include "tubex_DomainException.h"
#include "tubex_StructureException.h"
#include "tubex_EmptyException.h"
#include "tubex_CtcDeriv.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  // Public methods

    // Definition

    /*TubeNode::TubeNode(const Interval& domain, const Interval& codomain) : TubeComponent(domain, codomain)
    {
      m_first_tubenode = new TubeSlice(domain, codomain);
      m_first_tubenode->setTubeReference(m_tube_ref);
      m_second_tubenode = NULL;
    }*/

    TubeNode::TubeNode(const TubeNode& x) : TubeComponent(x)
    {
      *this = x;
    }

    TubeNode::TubeNode(const TubeNode& x, const Interval& codomain) : TubeComponent(x)
    {
      *this = x;
      set(codomain);
    }

    TubeNode::~TubeNode()
    {
      if(m_first_tubenode != NULL)
      {
        delete m_first_tubenode;
        m_first_tubenode = NULL;
      }

      if(m_second_tubenode != NULL)
      {
        delete m_second_tubenode;
        m_second_tubenode = NULL;
      }
    }

    TubeNode& TubeNode::operator=(const TubeNode& x)
    {
      // Reset
      {
        if(m_first_tubenode != NULL)
        {
          delete m_first_tubenode;
          m_first_tubenode = NULL;
        }

        if(m_second_tubenode != NULL)
        {
          delete m_second_tubenode;
          m_second_tubenode = NULL;
        }
      }

      TubeComponent::operator=(x);
      m_enclosed_bounds = x.m_enclosed_bounds;

      if(x.getFirstTubeComponent() != NULL)
      {
        if(x.getFirstTubeComponent()->isSlice())
          m_first_tubenode = new TubeSlice(*((TubeSlice*)x.getFirstTubeComponent()));

        else
          m_first_tubenode = new TubeNode(*((TubeNode*)x.getFirstTubeComponent()));
        
        m_first_tubenode->setTubeReference(m_tube_ref);
      }

      if(x.getSecondTubeComponent() != NULL)
      {
        if(x.getSecondTubeComponent()->isSlice())
          m_second_tubenode = new TubeSlice(*((TubeSlice*)x.getSecondTubeComponent()));

        else
          m_second_tubenode = new TubeNode(*((TubeNode*)x.getSecondTubeComponent()));

        m_second_tubenode->setTubeReference(m_tube_ref);
        TubeSlice::chainSlices(m_first_tubenode->getLastSlice(), m_second_tubenode->getFirstSlice());
      }

      return *this;
    }

    // Slices structure
    
    bool TubeNode::isSlice() const
    {
      return false;
    }
    
    TubeComponent* TubeNode::getFirstTubeComponent() const
    {
      return m_first_tubenode;
    }
    
    TubeComponent* TubeNode::getSecondTubeComponent() const
    {
      return m_second_tubenode;
    }

    TubeSlice* TubeNode::getSlice(int slice_id)
    {
      return const_cast<TubeSlice*>(static_cast<const TubeNode*>(this)->getSlice(slice_id));
    }

    TubeSlice* TubeNode::getSlice(double t)
    {
      return const_cast<TubeSlice*>(static_cast<const TubeNode*>(this)->getSlice(t));
    }

    const TubeSlice* TubeNode::getSlice(int slice_id) const
    {
      DomainException::check(*this, slice_id);

      if(slice_id < m_first_tubenode->nbSlices())
        return m_first_tubenode->getSlice(slice_id);

      else
        return m_second_tubenode->getSlice(slice_id - m_first_tubenode->nbSlices());
    }
    
    const TubeSlice* TubeNode::getSlice(double t) const
    {
      DomainException::check(*this, t);
      return getSlice(input2index(t));
    }
    
    void TubeNode::getSlices(vector<const TubeSlice*>& v_slices) const
    {
      m_first_tubenode->getSlices(v_slices);
      m_second_tubenode->getSlices(v_slices);
    }
    
    int TubeNode::input2index(double t) const
    {
      DomainException::check(*this, t);

      if(t == m_domain.ub())
        return nbSlices() - 1;

      else if(t < m_first_tubenode->domain().ub())
        return m_first_tubenode->input2index(t);

      else
        return m_first_tubenode->nbSlices() + m_second_tubenode->input2index(t);
    }
    
    void TubeNode::getTubeComponents(vector<const TubeComponent*> &v_nodes) const
    {
      v_nodes.push_back(static_cast<const TubeComponent*>(this));
      m_first_tubenode->getTubeComponents(v_nodes);
      m_second_tubenode->getTubeComponents(v_nodes);
    }

    // Access values

    const Interval& TubeNode::codomain() const
    {
      checkData();
      return m_codomain;
    }

    double TubeNode::volume() const
    {
      checkData();
      return m_volume;
    }

    const Interval& TubeNode::operator[](int slice_id) const
    {
      // Write access is not allowed for this operator:
      // a further call to checkDataTree() is needed when values change,
      // this call cannot be garanteed with a direct access to m_codomain
      // For write access: use set()
      return getSlice(slice_id)->codomain();
    }

    const Interval TubeNode::operator[](double t) const
    {
      // Write access is not allowed for this operator:
      // a further call to checkDataTree() is needed when values change,
      // this call cannot be garanteed with a direct access to m_codomain
      // For write access: use set()
      return (*getSlice(t))[t];
    }

    const Interval TubeNode::operator[](const ibex::Interval& t) const
    {
      // Write access is not allowed for this operator:
      // a further call to checkDataTree() is needed when values change,
      // this call cannot be garanteed with a direct access to m_codomain
      // For write access: use set()

      Interval intersection = m_domain & t;

      if(intersection.is_empty())
        return Interval::EMPTY_SET;

      else if(t.is_degenerated())
        return (*this)[t.lb()];

      else if(intersection == m_domain)
        return codomain();

      else
      {
        if((m_first_tubenode->domain() & t).is_empty())
          return (*m_second_tubenode)[m_second_tubenode->domain() & t];

        else if((m_second_tubenode->domain() & t).is_empty())
          return (*m_first_tubenode)[m_first_tubenode->domain() & t];

        else
          return (*m_first_tubenode)[m_first_tubenode->domain() & t]
               | (*m_second_tubenode)[m_second_tubenode->domain() & t];
      }
    }
    
    Interval TubeNode::invert(const Interval& y, const Interval& search_domain) const
    {
      if(!domain().intersects(search_domain)) // to apply this function on a tube portion only
        return Interval::EMPTY_SET;

      else if(!codomain().intersects(y))
        return Interval::EMPTY_SET;

      else
      {
        if(isSlice())
          return search_domain & m_domain;

        else
          return m_first_tubenode->invert(y, search_domain) | m_second_tubenode->invert(y, search_domain);
      }
    }

    void TubeNode::invert(const Interval& y, vector<Interval> &v_t, const Interval& search_domain) const
    {
      return invert(y, v_t, search_domain, true);
    }

    const pair<Interval,Interval> TubeNode::eval(const Interval& t) const
    {
      Interval intersection = m_domain & t;

      if(intersection.is_empty())
        return make_pair(Interval::EMPTY_SET, Interval::EMPTY_SET);

      else if(intersection.is_degenerated())
        return make_pair(Interval((*this)[intersection.lb()].lb()), Interval((*this)[intersection.lb()].ub()));

      else if(intersection == m_domain)
      {
        checkData();
        return m_enclosed_bounds; // pre-computed
      }

      else
      {
        Interval inter_firstsubtube = m_first_tubenode->domain() & intersection;
        Interval inter_secondsubtube = m_second_tubenode->domain() & intersection;
        pair<Interval,Interval> ui_past = m_first_tubenode->eval(inter_firstsubtube);
        pair<Interval,Interval> ui_future = m_second_tubenode->eval(inter_secondsubtube);
        return make_pair(ui_past.first | ui_future.first, ui_past.second | ui_future.second);
      }
    }

    // Tests
    
    bool TubeNode::operator==(const TubeNode& x) const
    {
      return isEqual(x);
    }
    
    bool TubeNode::operator!=(const TubeNode& x) const
    {
      return isDifferent(x);
    }

    bool TubeNode::isSubset(const TubeNode& x) const
    {
      StructureException::check(*this, x);
      TubeSlice *slice = getFirstSlice();
      TubeSlice *outer_slice = getFirstSlice();

      while(slice != NULL)
      {
        if(!slice->isSubset(*outer_slice))
          return false;
        slice = slice->nextSlice();
        outer_slice = outer_slice->nextSlice();
      }

      return true;
    }

    bool TubeNode::isStrictSubset(const TubeNode& x) const
    {
      StructureException::check(*this, x);
      return isSubset(x) && (*this) != x;
    }
    
    bool TubeNode::isEmpty() const
    {
      TubeSlice *slice = getFirstSlice();

      while(slice != NULL)
      {
        if(slice->isEmpty())
          return true;
        slice = slice->nextSlice();
      }

      return false;
    }

    bool TubeNode::encloses(const Trajectory& x) const
    {
      TubeSlice *slice = getFirstSlice();

      while(slice != NULL)
      {
        if(!slice->encloses(x))
          return false;
        slice = slice->nextSlice();
      }

      return true;
    }

    // Setting values

    void TubeNode::set(const Interval& y)
    {
      TubeSlice *slice = getFirstSlice();

      while(slice != NULL)
      {
        slice->set(y);
        slice = slice->nextSlice();
      }

      flagFutureDataUpdateFromRoot(); // todo: remove this
    }
    
    void TubeNode::set(const Interval& y, int slice_id)
    {
      getSlice(slice_id)->set(y);
    }
    
    void TubeNode::setEmpty()
    {
      set(Interval::EMPTY_SET);
    }

    TubeNode& TubeNode::inflate(double rad)
    {
      Interval e(-rad,rad);

      TubeSlice *slice = getFirstSlice();
      TubeSlice *first_slice = slice;

      // Setting envelopes before gates' inflation
      while(slice != NULL)
      {
        slice->setEnvelope(slice->codomain() + e);
        slice = slice->nextSlice();
      }

      slice = first_slice;

      while(slice != NULL)
      {
        if(slice == first_slice)
          slice->setInputGate(slice->inputGate() + e);
        slice->setOutputGate(slice->outputGate() + e);
        slice = slice->nextSlice();
      }
    }

    // Integration



  // Protected methods

    // Definition

    // Slices/tree structure
    
    // Access values

    void TubeNode::invert(const Interval& y, vector<ibex::Interval> &v_t, const Interval& search_domain, bool concatenate_results) const
    {
      v_t.clear();
      Interval intv_t_ctc = invert(y, search_domain);

      if(!intv_t_ctc.is_empty())
      {
        pair<Interval,Interval> enc_bounds = eval(intv_t_ctc);

        if(!concatenate_results)
        {
          if(enc_bounds.first.ub() > y.ub() || enc_bounds.second.lb() < y.lb())
          {
            // Bisection is needed
            vector<Interval> v1;
            m_first_tubenode->invert(y, v1, search_domain, false);
            v_t.insert(v_t.end(), v1.begin(), v1.end());
            vector<Interval> v2;
            m_second_tubenode->invert(y, v2, search_domain, false);
            v_t.insert(v_t.end(), v2.begin(), v2.end());
          }

          else
            v_t.push_back(intv_t_ctc);
        }

        else
        {
          vector<Interval> v;
          invert(y, v, search_domain, false);

          // Concatenation (solutions may be adjacent)
          int i = 0;
          while(i < v.size())
          {
            int j = i;
            Interval merge = v[i];

            while(j + 1 < v.size() && v[j].ub() == v[j + 1].lb())
            {
              j ++;
              merge |= v[j];
            }

            v_t.push_back(merge);
            i = j + 1;
          }
        }
      }
    }

    // Tests

    bool TubeNode::nodesAreEqual(const TubeComponent* node1, const TubeComponent* node2)
    {
      if(node1 != NULL || node2 != NULL)
      {
        if(node1 == NULL || node2 == NULL) // one is NULL, the other one is not
          return false;

        else // both are not NULL
        {
          if(node1->isSlice() && node2->isSlice()) // both are slices
          {
            if(!((TubeSlice*)node1)->TubeSlice::isEqual(*(TubeSlice*)node2))
              return false; // slices are not equal
          }

          else if(node1->isSlice() || node2->isSlice())
            return false; // one is a slice, the other one is not

          else if(!((TubeNode*)node1)->TubeNode::isEqual(*(TubeNode*)node2))
            return false; // both are trees, not equal
        }
      }

      return true;
    }

    bool TubeNode::isEqual(const TubeNode& x) const
    {
      StructureException::check(*this, x);

      return TubeComponent::isEqual(x)
        && nodesAreEqual(m_first_tubenode, x.getFirstTubeComponent())
        && nodesAreEqual(m_second_tubenode, x.getSecondTubeComponent());
    }

    bool TubeNode::nodesAreDifferent(const TubeComponent* node1, const TubeComponent* node2)
    {
      if(node1 == NULL && node2 == NULL)
        return false;

      else if(node1 != NULL && node2 != NULL) // both are not NULL
      {
        if((node1->isSlice() && !node2->isSlice()) || (!node1->isSlice() && node2->isSlice()))
          return true;

        else // nodes are of same type
        {
          if(node1->isSlice()) // both are slices
            return ((TubeSlice*)node1)->TubeSlice::isDifferent(*(TubeSlice*)node2);

          else
            return ((TubeNode*)node1)->TubeNode::isDifferent(*(TubeNode*)node2);
        }
      }

      else // one is NULL, the other one is not
        return true;
    }
    
    bool TubeNode::isDifferent(const TubeNode& x) const
    {
      StructureException::check(*this, x);

      return TubeComponent::isDifferent(x)
        || nodesAreDifferent(m_first_tubenode, x.getFirstTubeComponent())
        || nodesAreDifferent(m_second_tubenode, x.getSecondTubeComponent());
    }

    // Setting values

    void TubeNode::checkData() const
    {
      if(!m_data_update_needed)
        return;

      m_first_tubenode->checkData();
      m_codomain = m_first_tubenode->codomain();
      m_volume = m_first_tubenode->volume();

      if(m_second_tubenode != NULL)
      {
        m_second_tubenode->checkData();
        m_codomain |= m_second_tubenode->codomain();
        m_volume += m_second_tubenode->volume();
      }

      // Enclosed bounds
      m_enclosed_bounds = m_first_tubenode->eval();

      if(m_second_tubenode != NULL)
      {
        pair<Interval,Interval> ui_future = m_second_tubenode->eval();
        m_enclosed_bounds.first |= ui_future.first;
        m_enclosed_bounds.second |= ui_future.second;
      }

      m_data_update_needed = false;
    }

    void TubeNode::flagFutureDataUpdateFromRoot(int slice_id) const
    {
      m_data_update_needed = true;
      m_primitive_update_needed = true;

      if(slice_id == -1)
      {
        m_first_tubenode->flagFutureDataUpdateFromRoot(-1);
        if(m_second_tubenode != NULL) m_second_tubenode->flagFutureDataUpdateFromRoot(-1);
      }

      else
      {
        DomainException::check(*this, slice_id);
        int mid_id = m_first_tubenode->nbSlices();

        if(slice_id < mid_id)
          m_first_tubenode->flagFutureDataUpdateFromRoot(slice_id);

        else
          m_second_tubenode->flagFutureDataUpdateFromRoot(slice_id - mid_id);
      }
    }

    // Operators

    // String

    // Contractors

    // Integration

    void TubeNode::checkPartialPrimitive() const
    {
      if(!m_primitive_update_needed)
        return;
      
      m_first_tubenode->checkPartialPrimitive();
      m_partial_primitive = m_first_tubenode->getPartialPrimitiveValue();

      if(m_second_tubenode != NULL)
      {
        m_second_tubenode->checkPartialPrimitive();
        m_partial_primitive.first |= m_second_tubenode->getPartialPrimitiveValue().first;
        m_partial_primitive.second |= m_second_tubenode->getPartialPrimitiveValue().second;
      }

      m_primitive_update_needed = false;
    }

    void TubeNode::flagFuturePrimitiveUpdateFromRoot(int slice_id) const
    {
      m_primitive_update_needed = true;

      if(slice_id == -1)
      {
        m_first_tubenode->flagFuturePrimitiveUpdateFromRoot(-1);
        if(m_second_tubenode != NULL) m_second_tubenode->flagFuturePrimitiveUpdateFromRoot(-1);
      }

      else
      {
        DomainException::check(*this, slice_id);
        int mid_id = m_first_tubenode->nbSlices();

        m_first_tubenode->flagFuturePrimitiveUpdateFromRoot(slice_id < mid_id ? slice_id : -1);

        if(m_second_tubenode != NULL && slice_id >= mid_id)
          m_second_tubenode->flagFuturePrimitiveUpdateFromRoot(slice_id - mid_id);
      }
    }

    const pair<Interval,Interval>& TubeNode::getPartialPrimitiveValue() const
    {
      checkPartialPrimitive();
      return m_partial_primitive;
    }

    pair<Interval,Interval> TubeNode::getPartialPrimitiveValue(const Interval& t) const
    {
      if(t.lb() == t.ub()) // todo: check this:
        return make_pair(Interval((*this)[t.lb()].lb()), Interval((*this)[t.lb()].ub()));

      Interval intersection = m_domain & t;

      if(intersection.is_empty()) // todo: put an Exception?
        return make_pair(Interval::EMPTY_SET, Interval::EMPTY_SET);

      else if(t == m_domain || t.is_unbounded() || t.is_superset(m_domain))
      {
        checkPartialPrimitive();
        return m_partial_primitive;
      }

      else
      {
        Interval inter_firstsubtube = m_first_tubenode->domain() & intersection;
        Interval inter_secondsubtube = m_second_tubenode->domain() & intersection;

        if(inter_firstsubtube.lb() == inter_firstsubtube.ub() && inter_secondsubtube.lb() == inter_secondsubtube.ub())
          return make_pair(m_first_tubenode->getPartialPrimitiveValue().first & m_second_tubenode->getPartialPrimitiveValue().first,
                           m_first_tubenode->getPartialPrimitiveValue().second & m_second_tubenode->getPartialPrimitiveValue().second);

        else if(inter_firstsubtube.is_empty() || inter_firstsubtube.lb() == inter_firstsubtube.ub())
          return m_second_tubenode->getPartialPrimitiveValue(inter_secondsubtube);

        else if(inter_secondsubtube.is_empty() || inter_secondsubtube.lb() == inter_secondsubtube.ub())
          return m_first_tubenode->getPartialPrimitiveValue(inter_firstsubtube);

        else
        {
          pair<Interval,Interval> pp_past = m_first_tubenode->getPartialPrimitiveValue(inter_firstsubtube);
          pair<Interval,Interval> pp_future = m_second_tubenode->getPartialPrimitiveValue(inter_secondsubtube);
          return make_pair(pp_past.first | pp_future.first, pp_past.second | pp_future.second);
        }
      }
    }
}