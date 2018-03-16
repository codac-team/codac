/* ============================================================================
 *  tubex-lib - Subtube class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_Subtube.h"
#include "ibex.h"
#include "exceptions/tubex_Exception.h"
#include "exceptions/tubex_DomainException.h"
#include "exceptions/tubex_EmptyException.h"
#include <iostream>
#include <limits>
#include <iomanip> // for setprecision()
#ifdef _OPENMP
  #include <omp.h> // for multithreading
#endif

using namespace std;
using namespace ibex;

namespace tubex
{
  Subtube::Subtube(const Subtube& tu)
  {
    m_first_subtube = NULL;
    m_second_subtube = NULL;
    *this = tu;
  }

  Subtube::Subtube(const vector<Interval>& vector_dt, const Interval& default_value)
  {
    createFromSlicesVector(vector_dt, default_value);
  }

  void Subtube::createFromSlicesVector(const vector<Interval>& vector_dt, const Interval& default_value)
  {
    m_domain = Interval(vector_dt[0].lb(), vector_dt[vector_dt.size() - 1].ub());
    m_image = default_value;
    m_slices_number = vector_dt.size();
    m_tree_computation_needed = true;

    if(m_slices_number == 1) // if this is a leaf
    {
      m_first_subtube = NULL;
      m_second_subtube = NULL;
    }

    else
    {
      // In the first subtube: [t0,thalf[
      // In the second subtube: [thalf,tf]
      vector<Interval> first_vector_slices, second_vector_slices;
      int i, k = ceil(m_slices_number / 2.);

      for(i = 0 ; i < k ; i++)
        first_vector_slices.push_back(vector_dt[i]);

      for( ; i < m_slices_number ; i++)
        second_vector_slices.push_back(vector_dt[i]);

      #pragma omp parallel num_threads(omp_get_num_procs())
      {
        #pragma omp sections
        {
          #pragma omp section
            m_first_subtube = new Subtube(first_vector_slices, default_value);
          #pragma omp section
            m_second_subtube = new Subtube(second_vector_slices, default_value);
        }
      }
    }

    requestFutureTreeComputation();
  }

  bool Subtube::isPartiallyEmpty() const
  {
    for(int i = 0 ; i < size() ; i++)
      if((*this)[i].is_empty())
        return true;
    return false;
  }

  bool Subtube::isDiscontinuous() const
  {
    Interval previous_slice_y = Interval::ALL_REALS;

    for(int i = 0 ; i < size() ; i++)
    {
      Interval this_intv_y = (*this)[i];
      if((previous_slice_y & this_intv_y).is_empty())
        return true;

      previous_slice_y = this_intv_y;
    }

    return false;
  }

  const Subtube* Subtube::getFirstSubSubtube() const
  {
    return m_first_subtube;
  }

  const Subtube* Subtube::getSecondSubSubtube() const
  {
    return m_second_subtube;
  }

  /*Subtube Subtube::subtube(const Interval& intv_t) const
  {
    checkDomain(*this, intv_t);
    Interval intv_t_inter = intv_t & domain();

    int index_lb = input2index(intv_t_inter.lb());
    int index_ub = input2index(intv_t_inter.ub());
    if(domain(index_ub).lb() == intv_t_inter.ub())
      index_ub = max(0, index_ub - 1);

    vector<Interval> vector_dt;
    for(int i = index_lb ; i <= index_ub ; i++)
      vector_dt.push_back(domain(i));

    Subtube subtube = Subtube(vector_dt);
    #pragma omp parallel num_threads(omp_get_num_procs())
    {
      #pragma omp for
        for(int i = 0 ; i < subtube.size() ; i++)
          subtube.set((*this)[i + index_lb], i);
    }
    
    return subtube;
  }*/

  void Subtube::getSubtubeNodes(vector<const Subtube*> &v_nodes) const
  {
    if(!isSlice())
    {
      m_first_subtube->getSubtubeNodes(v_nodes);
      m_second_subtube->getSubtubeNodes(v_nodes);
    }

    v_nodes.push_back(this);
  }

  void Subtube::getSubtubeNodes(vector<Subtube*> &v_nodes)
  {
    if(!isSlice())
    {
      m_first_subtube->getSubtubeNodes(v_nodes);
      m_second_subtube->getSubtubeNodes(v_nodes);
    }

    v_nodes.push_back(this);
  }
  
  void Subtube::unionWith_localUpdate(const Subtube *x)
  {
    if(m_tree_computation_needed)
      computeTree();
    
    m_image |= x->image();
    pair<Interval,Interval> eb1 = eval();
    pair<Interval,Interval> eb2 = x->eval();
    m_enclosed_bounds = make_pair(Interval(min(eb1.first.lb(), eb2.first.lb()), min(eb1.first.ub(), eb2.first.ub())),
                                  Interval(max(eb1.second.lb(), eb2.second.lb()), max(eb1.second.ub(), eb2.second.ub())));

    requestFuturePrimitiveComputation();
  }

  Subtube& Subtube::operator &=(const Subtube& x)
  {
    if(size() != x.size() || domain() != x.domain())
    {
      for(int i = 0 ; i < size() ; i++)
      {
        Interval this_intv_t = domain(i);
        Interval this_intv_y = (*this)[i];

        Interval x_intv_t = x.domain() & this_intv_t;

        if(!x_intv_t.is_empty())
        {
          Interval x_intv_y = x[x_intv_t];
          set(this_intv_y & x_intv_y, x_intv_t);
        }
      }
    }

    else // fast intersection
    {
      vector<Subtube*> this_nodes;
      vector<const Subtube*> x_nodes;
      getSubtubeNodes(this_nodes);
      x.getSubtubeNodes(x_nodes);

      #pragma omp parallel num_threads(omp_get_num_procs())
      {
        #pragma omp for
        for(int i = 0 ; i < this_nodes.size() ; i++)
          this_nodes[i]->intersectWith_localUpdate(x_nodes[i]);
      }
    }

    requestFutureTreeComputation();
    return *this;
  }

  void Subtube::intersectWith_localUpdate(const Subtube *x)
  {
    if(m_tree_computation_needed)
      computeTree();
    
    m_image &= x->image();
    // Enclosed bounds cannot be computed on this level.
    // Synthesis has to be done from the root (see update() in intersectWith).
    requestFutureTreeComputation();
  }

  /*void Subtube::print(int precision) const
  {
    if(m_tree_computation_needed)
      computeTree();
    
    if(isSlice())
    {
      if(precision != 0)
        cout << setprecision(precision);
      cout << "Subtube: " << m_domain << " \t" << m_image << endl;
    }

    else
    {
      m_first_subtube->print(precision);
      m_second_subtube->print(precision);
    }
  }*/

  std::ostream& operator<<(std::ostream& os, const Subtube& x)
  {
    if(x.m_tree_computation_needed)
      x.computeTree();
    
    cout << "Subtube: t=" << x.domain()
         << ", y=" << x.image() 
         //<< ", " << x.size() << " slices"
         << flush;
    return os;
  }

  void Subtube::requestFutureTreeComputation(int index) const
  {
    m_tree_computation_needed = true;

    if(!isSlice())
    {
      if(index == -1)
      {
        m_first_subtube->requestFutureTreeComputation(-1);
        m_second_subtube->requestFutureTreeComputation(-1);
      }

      else
      {
        checkDomain(*this, index);
        int mid_id = m_first_subtube->size();
        if(index < mid_id) m_first_subtube->requestFutureTreeComputation(index);
        else m_second_subtube->requestFutureTreeComputation(index - mid_id);
      }
    }
    
    requestFuturePrimitiveComputation();
  }

  void Subtube::computeTree() const
  {
    if(m_tree_computation_needed)
    {
      if(isSlice())
      {
        m_volume = m_domain.diam();

        if(m_image.is_empty()) // ibex::diam(EMPTY_SET) is not 0
          m_volume = 0.;

        else if(m_image.is_unbounded())
          m_volume = INFINITY;

        else
          m_volume *= m_image.diam();

        m_enclosed_bounds = make_pair(Interval(m_image.lb()), Interval(m_image.ub()));
      }

      else
      {
        #pragma omp parallel num_threads(omp_get_num_procs())
        {
          #pragma omp sections
          {
            #pragma omp section
              m_first_subtube->computeTree();
            #pragma omp section
              m_second_subtube->computeTree();
          }
        }
        
        m_image = m_first_subtube->image() | m_second_subtube->image();
        m_volume = m_first_subtube->volume() + m_second_subtube->volume();

        // Enclosed bounds
        pair<Interval,Interval> ui_past = m_first_subtube->eval();
        pair<Interval,Interval> ui_future = m_second_subtube->eval();
        m_enclosed_bounds = make_pair(ui_past.first | ui_future.first, ui_past.second | ui_future.second);
      }

      m_tree_computation_needed = false;
    }
  }
}