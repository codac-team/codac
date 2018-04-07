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

#include "tubex_Tube.h"
#include "tubex_TubeSlice.h"
#include "tubex_DomainException.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  // Public methods

    // Definition

    TubeTree::TubeTree(const Interval& domain, const Interval& codomain) : TubeNode(domain, codomain)
    {
      m_first_tubenode = new TubeSlice(domain, codomain);
      m_second_tubenode = NULL;
    }

    TubeTree::TubeTree(const TubeTree& x) : TubeNode(x)
    {
      *this = x;
    }

    TubeTree::~TubeTree()
    {
      // Deleting gate between the two nodes
      TubeSlice *last_slice_first_node = getSlice(m_first_tubenode->nbSlices() - 1);
      if(((TubeSlice*)last_slice_first_node)->m_output_gate != NULL)
        delete ((TubeSlice*)last_slice_first_node)->m_output_gate;

      // Deleting nodes
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

    TubeTree& TubeTree::operator=(const TubeTree& x)
    {
      TubeNode::operator=(x);

      m_enclosed_bounds = x.m_enclosed_bounds;
      m_partial_primitive = x.m_partial_primitive;
      m_tree_update_needed = x.m_tree_update_needed;
      m_primitive_update_needed = x.m_primitive_update_needed;

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

      if(x.getFirstTubeNode() != NULL)
      {
        if(x.getFirstTubeNode()->isSlice())
          m_first_tubenode = new TubeSlice(*((TubeSlice*)x.getFirstTubeNode()));

        else
          m_first_tubenode = new TubeTree(*((TubeTree*)x.getFirstTubeNode()));
      }

      if(x.getSecondTubeNode() != NULL)
      {
        if(x.getSecondTubeNode()->isSlice())
          m_second_tubenode = new TubeSlice(*((TubeSlice*)x.getSecondTubeNode()));

        else
          m_second_tubenode = new TubeTree(*((TubeTree*)x.getSecondTubeNode()));

        // Chaining slices (+ gate in between)

        Interval *gate = NULL;

        if(x.m_first_tubenode->getLastSlice()->m_output_gate != NULL)
          gate = new Interval(*x.m_first_tubenode->getLastSlice()->m_output_gate);

        TubeSlice::chainSlices(m_first_tubenode->getLastSlice(), m_second_tubenode->getFirstSlice(), gate);
      }

      return *this;
    }

    // Slices structure
    
    bool TubeTree::isSlice() const
    {
      return false;
    }
    
    TubeNode* TubeTree::getFirstTubeNode() const
    {
      return m_first_tubenode;
    }
    
    TubeNode* TubeTree::getSecondTubeNode() const
    {
      return m_second_tubenode;
    }
    
    int TubeTree::sample(double t, const Interval& gate)
    {
      DomainException::check(*this, t);
      int new_slices_nb = 1;

      if(m_second_tubenode == NULL)
      {
        if(m_first_tubenode->domain().lb() == t || m_first_tubenode->domain().ub() == t)
          return 0; // no degenerate slice

        TubeSlice slice(*(TubeSlice*)m_first_tubenode);
        TubeSlice *prev_slice = ((TubeSlice*)m_first_tubenode)->prevSlice();
        TubeSlice *next_slice = ((TubeSlice*)m_first_tubenode)->nextSlice();
        delete m_first_tubenode;

        m_first_tubenode = new TubeSlice(Interval(slice.domain().lb(), t), codomain());
        m_second_tubenode = new TubeSlice(Interval(t, slice.domain().ub()), codomain());

        TubeSlice::chainSlices(prev_slice, (TubeSlice*)m_first_tubenode);
        TubeSlice::chainSlices((TubeSlice*)m_first_tubenode, (TubeSlice*)m_second_tubenode, new Interval(gate));
        TubeSlice::chainSlices((TubeSlice*)m_second_tubenode, next_slice);
      }

      else
      {
        TubeNode **slice_ptr = NULL;

        if(m_first_tubenode->domain().contains(t))
        {
          if(m_first_tubenode->nbSlices() >= 2)
            new_slices_nb = ((TubeTree*)m_first_tubenode)->sample(t, gate);

          else
            slice_ptr = &m_first_tubenode;
        }

        else
        {
          if(m_second_tubenode->nbSlices() >= 2)
            new_slices_nb = ((TubeTree*)m_second_tubenode)->sample(t, gate);

          else if(m_second_tubenode != NULL)
            slice_ptr = &m_second_tubenode;
        }

        if(slice_ptr != NULL)
        {
          if((*slice_ptr)->domain().lb() == t || (*slice_ptr)->domain().ub() == t)
            return 0; // no degenerate slice
          
          TubeSlice slice(*(TubeSlice*)(*slice_ptr));
          TubeSlice *prev_slice = ((TubeSlice*)(*slice_ptr))->prevSlice();
          TubeSlice *next_slice = ((TubeSlice*)(*slice_ptr))->nextSlice();
          delete (*slice_ptr);

          *slice_ptr = new TubeTree(slice.domain(), slice.codomain());
          TubeSlice **first_slice = (TubeSlice**)&((TubeTree*)(*slice_ptr))->m_first_tubenode;
          TubeSlice **second_slice = (TubeSlice**)&((TubeTree*)(*slice_ptr))->m_second_tubenode;

          *first_slice = new TubeSlice(Interval(slice.domain().lb(), t), slice.codomain());
          *second_slice = new TubeSlice(Interval(t, slice.domain().ub()), slice.codomain());
          
          TubeSlice::chainSlices(prev_slice, *first_slice);
          TubeSlice::chainSlices(*first_slice, *second_slice, new Interval(gate));
          TubeSlice::chainSlices(*second_slice, next_slice);

          (*slice_ptr)->m_slices_number = 2;
        }
      }

      m_slices_number += new_slices_nb;
      return new_slices_nb;
    }

    TubeSlice* TubeTree::getSlice(int slice_id)
    {
      return const_cast<TubeSlice*>(static_cast<const TubeTree*>(this)->getSlice(slice_id));
    }

    const TubeSlice* TubeTree::getSlice(int slice_id) const
    {
      DomainException::check(*this, slice_id);

      if(slice_id < m_first_tubenode->nbSlices())
        return m_first_tubenode->getSlice(slice_id);

      else
        return m_second_tubenode->getSlice(slice_id - m_first_tubenode->nbSlices());
    }

    TubeSlice* TubeTree::getSlice(double t)
    {
      return const_cast<TubeSlice*>(static_cast<const TubeTree*>(this)->getSlice(t));
    }
    
    const TubeSlice* TubeTree::getSlice(double t) const
    {
      DomainException::check(*this, t);
      return getSlice(input2index(t));
    }
    
    void TubeTree::getSlices(vector<const TubeSlice*>& v_slices) const
    {
      m_first_tubenode->getSlices(v_slices);
      m_second_tubenode->getSlices(v_slices);
    }
    
    int TubeTree::input2index(double t) const
    {
      DomainException::check(*this, t);

      if(t == m_domain.ub())
        return nbSlices() - 1;

      else if(t < m_first_tubenode->domain().ub())
        return m_first_tubenode->input2index(t);

      else
        return m_second_tubenode->input2index(t) + m_first_tubenode->nbSlices();
    }
    
    void TubeTree::getTubeNodes(vector<const TubeNode*> &v_nodes) const
    {
      v_nodes.push_back(static_cast<const TubeNode*>(this));
      m_first_tubenode->getTubeNodes(v_nodes);
      m_second_tubenode->getTubeNodes(v_nodes);
    }

    // Access values

    const Interval& TubeTree::codomain() const
    {
      return m_codomain;
    }

    // Tests


  // Protected methods

    // Definition

    // Slices/tree structure

    // Tests

    // Setting values

    // Operators

    // String

    // Contractors

    // Integration





















    /*bool TubeTree::isSlice() const
    {
      return false;
    }
  /*Tube::Tube(const Interval& domain) : TubeNode(domain, Interval::ALL_REALS)
  {
    m_first_tubenode = new TubeSlice(domain, Interval::ALL_REALS);
  }

  Interval Tube::operator()(double t) const
  {
    return Interval::ALL_REALS;
  }

  void Tube::sample(double t, const Interval& gate)
  {

  }*/
    
  /*  Interval TubeTree::invert(const Interval& y, const Interval& t) const
    {
      if(!m_domain.intersects(t) || !codomain().intersects(y))
        return Interval::EMPTY_SET;

      else
        return m_first_tubenode->invert(y, t) | m_second_tubenode->invert(y, t);
    }*/
}