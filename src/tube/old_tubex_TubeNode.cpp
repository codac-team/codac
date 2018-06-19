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

    TubeNode::TubeNode(const TubeSlice& slice, double t) : TubeComponent(slice.domain(), slice.codomain())
    {
      if(t <= slice.domain().lb() || t >= slice.domain().ub())
        throw Exception("TubeNode constructor", "invalid time bisection");

      m_first_component = new TubeSlice(Interval(slice.domain().lb(), t), slice.codomain());
      m_second_component = new TubeSlice(Interval(t, slice.domain().ub()), slice.codomain());
      setTubeReference(slice.tubeReference());
      
      if(slice.prevSlice() != NULL)
        TubeSlice::chainSlices(slice.prevSlice(), (TubeSlice*)m_first_component);

      else
        ((TubeSlice*)m_first_component)->setInputGate(slice.inputGate());

      TubeSlice::chainSlices((TubeSlice*)m_first_component, (TubeSlice*)m_second_component);
      
      if(slice.nextSlice() != NULL)
        TubeSlice::chainSlices((TubeSlice*)m_second_component, slice.nextSlice());

      else
        ((TubeSlice*)m_second_component)->setOutputGate(slice.outputGate());

      m_slices_number = 2;
    }

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
      if(m_first_component == NULL || m_second_component == NULL)
        throw Exception("Tube destructor", "uninitialized components");
      
      delete m_first_component;
      delete m_second_component;
    }

    TubeNode& TubeNode::operator=(const TubeNode& x)
    {
      delete m_first_component;
      delete m_second_component;

      TubeComponent::operator=(x);
      m_enclosed_bounds = x.m_enclosed_bounds;

      if(typeid(*(x.getFirstTubeComponent())) == typeid(TubeSlice))
        m_first_component = new TubeSlice(*((TubeSlice*)x.getFirstTubeComponent()));

      else
        m_first_component = new TubeNode(*((TubeNode*)x.getFirstTubeComponent()));

      if(typeid(*(x.getSecondTubeComponent())) == typeid(TubeSlice))
        m_second_component = new TubeSlice(*((TubeSlice*)x.getSecondTubeComponent()));

      else
        m_second_component = new TubeNode(*((TubeNode*)x.getSecondTubeComponent()));
      
      TubeSlice::chainSlices(m_first_component->getLastSlice(), m_second_component->getFirstSlice());
      
      return *this;
    }

    // Slices structure
    
    bool TubeNode::isSlice() const
    {
      return false;
    }
    
    TubeComponent* TubeNode::getFirstTubeComponent() const
    {
      return m_first_component;
    }
    
    TubeComponent* TubeNode::getSecondTubeComponent() const
    {
      return m_second_component;
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

      if(slice_id < m_first_component->nbSlices())
        return m_first_component->getSlice(slice_id);

      else
        return m_second_component->getSlice(slice_id - m_first_component->nbSlices());
    }
    
    const TubeSlice* TubeNode::getSlice(double t) const
    {
      DomainException::check(*this, t);
      return getSlice(input2index(t));
    }
    
    void TubeNode::getSlices(vector<const TubeSlice*>& v_slices) const
    {
      m_first_component->getSlices(v_slices);
      m_second_component->getSlices(v_slices);
    }
    
    int TubeNode::input2index(double t) const
    {
      DomainException::check(*this, t);

      if(t == m_domain.ub())
        return nbSlices() - 1;

      else if(t < m_first_component->domain().ub())
        return m_first_component->input2index(t);

      else
        return m_first_component->nbSlices() + m_second_component->input2index(t);
    }
    
    TubeNode* TubeNode::getParentOf(TubeComponent* component)
    {
      if(m_first_component == component || m_second_component == component)
        return this;

      else
      {
        TubeNode *parent = m_first_component->getParentOf(component);

        if(parent != NULL)
          return parent;

        else
          return m_second_component->getParentOf(component);
      }
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

    const Interval TubeNode::operator[](int slice_id) const
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
        if((m_first_component->domain() & t).is_empty())
          return (*m_second_component)[m_second_component->domain() & t];

        else if((m_second_component->domain() & t).is_empty())
          return (*m_first_component)[m_first_component->domain() & t];

        else
          return (*m_first_component)[m_first_component->domain() & t]
               | (*m_second_component)[m_second_component->domain() & t];
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
        if(isSlice()) // todo: remove this?
          return search_domain & m_domain;

        else
          return m_first_component->invert(y, search_domain) | m_second_component->invert(y, search_domain);
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
        Interval inter_firstsubtube = m_first_component->domain() & intersection;
        Interval inter_secondsubtube = m_second_component->domain() & intersection;
        pair<Interval,Interval> ui_past = m_first_component->eval(inter_firstsubtube);
        pair<Interval,Interval> ui_future = m_second_component->eval(inter_secondsubtube);
        return make_pair(ui_past.first | ui_future.first, ui_past.second | ui_future.second);
      }
    }

    // Tests
    
    bool TubeNode::operator==(const TubeNode& x) const
    {
      return TubeComponent::operator==(x)
        && nodesAreEqual(m_first_component, x.getFirstTubeComponent())
        && nodesAreEqual(m_second_component, x.getSecondTubeComponent());
    }
    
    bool TubeNode::operator!=(const TubeNode& x) const
    {
      return TubeComponent::operator!=(x)
        || nodesAreDifferent(m_first_component, x.getFirstTubeComponent())
        || nodesAreDifferent(m_second_component, x.getSecondTubeComponent());
    }

    bool TubeNode::isSubset(const TubeNode& x) const // todo: test this
    {
      StructureException::check(*this, x);
      TubeSlice *slice = getFirstSlice();
      TubeSlice *outer_slice = x.getFirstSlice();

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

    void TubeNode::setTubeReference(Tube *tube_ref)
    {
      m_tube_ref = tube_ref;
      m_first_component->setTubeReference(tube_ref);
      m_second_component->setTubeReference(tube_ref);
    }

    // Slices/tree structure

    void TubeNode::updateSlicesNumber()
    {
      m_first_component->updateSlicesNumber();
      m_second_component->updateSlicesNumber();
      m_slices_number = m_first_component->nbSlices() + m_second_component->nbSlices();
    }
    
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
            m_first_component->invert(y, v1, search_domain, false);
            v_t.insert(v_t.end(), v1.begin(), v1.end());
            vector<Interval> v2;
            m_second_component->invert(y, v2, search_domain, false);
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
      if(node1 == NULL || node2 == NULL)
        throw Exception("TubeNode::nodesAreEqual", "NULL pointer");

      if(typeid(*node1) != typeid(*node2))
        return false;

      if(typeid(*node1) == typeid(TubeSlice))
        return ((TubeSlice*)node1)->TubeSlice::operator==(*((TubeSlice*)node2));

      else
        return ((TubeNode*)node1)->TubeNode::operator==(*((TubeNode*)node2));
    }

    bool TubeNode::nodesAreDifferent(const TubeComponent* node1, const TubeComponent* node2)
    {
      if(node1 == NULL || node2 == NULL)
        throw Exception("TubeNode::nodesAreEqual", "NULL pointer");

      if(typeid(*node1) != typeid(*node2))
        return true;

      if(typeid(*node1) == typeid(TubeSlice))
        return ((TubeSlice*)node1)->TubeSlice::operator!=(*((TubeSlice*)node2));

      else
        return ((TubeNode*)node1)->TubeNode::operator!=(*((TubeNode*)node2));
    }

    // Setting values

    void TubeNode::checkData() const
    {
      if(!m_data_update_needed)
        return;

      m_first_component->checkData();
      m_codomain = m_first_component->codomain();
      m_volume = m_first_component->volume();
      m_second_component->checkData();
      m_codomain |= m_second_component->codomain();
      m_volume += m_second_component->volume();

      // Enclosed bounds
      m_enclosed_bounds = m_first_component->eval();
      pair<Interval,Interval> ui_future = m_second_component->eval();
      m_enclosed_bounds.first |= ui_future.first;
      m_enclosed_bounds.second |= ui_future.second;

      m_data_update_needed = false;
    }

    void TubeNode::flagFutureDataUpdateFromRoot(int slice_id) const
    {
      m_data_update_needed = true;
      m_primitive_update_needed = true;

      if(slice_id == -1)
      {
        m_first_component->flagFutureDataUpdateFromRoot(-1);
        m_second_component->flagFutureDataUpdateFromRoot(-1);
      }

      else
      {
        DomainException::check(*this, slice_id);
        int mid_id = m_first_component->nbSlices();

        if(slice_id < mid_id)
          m_first_component->flagFutureDataUpdateFromRoot(slice_id);

        else
          m_second_component->flagFutureDataUpdateFromRoot(slice_id - mid_id);
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
      
      m_first_component->checkPartialPrimitive();
      m_second_component->checkPartialPrimitive();
      m_partial_primitive = m_first_component->getPartialPrimitiveValue();
      m_partial_primitive.first |= m_second_component->getPartialPrimitiveValue().first;
      m_partial_primitive.second |= m_second_component->getPartialPrimitiveValue().second;

      m_primitive_update_needed = false;
    }

    void TubeNode::flagFuturePrimitiveUpdateFromRoot(int slice_id) const
    {
      m_primitive_update_needed = true;

      if(slice_id == -1)
      {
        m_first_component->flagFuturePrimitiveUpdateFromRoot(-1);
        m_second_component->flagFuturePrimitiveUpdateFromRoot(-1);
      }

      else
      {
        DomainException::check(*this, slice_id);
        int mid_id = m_first_component->nbSlices();

        m_first_component->flagFuturePrimitiveUpdateFromRoot(slice_id < mid_id ? slice_id : -1);

        if(slice_id >= mid_id)
          m_second_component->flagFuturePrimitiveUpdateFromRoot(slice_id - mid_id);
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
        Interval inter_firstsubtube = m_first_component->domain() & intersection;
        Interval inter_secondsubtube = m_second_component->domain() & intersection;

        if(inter_firstsubtube.lb() == inter_firstsubtube.ub() && inter_secondsubtube.lb() == inter_secondsubtube.ub())
          return make_pair(m_first_component->getPartialPrimitiveValue().first & m_second_component->getPartialPrimitiveValue().first,
                           m_first_component->getPartialPrimitiveValue().second & m_second_component->getPartialPrimitiveValue().second);

        else if(inter_firstsubtube.is_empty() || inter_firstsubtube.lb() == inter_firstsubtube.ub())
          return m_second_component->getPartialPrimitiveValue(inter_secondsubtube);

        else if(inter_secondsubtube.is_empty() || inter_secondsubtube.lb() == inter_secondsubtube.ub())
          return m_first_component->getPartialPrimitiveValue(inter_firstsubtube);

        else
        {
          pair<Interval,Interval> pp_past = m_first_component->getPartialPrimitiveValue(inter_firstsubtube);
          pair<Interval,Interval> pp_future = m_second_component->getPartialPrimitiveValue(inter_secondsubtube);
          return make_pair(pp_past.first | pp_future.first, pp_past.second | pp_future.second);
        }
      }
    }
}