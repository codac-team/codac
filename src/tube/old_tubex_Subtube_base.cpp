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
}