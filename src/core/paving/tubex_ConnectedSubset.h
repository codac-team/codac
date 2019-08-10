/** 
 *  \file
 *  ConnectedSubset class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2019 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_CONNECTEDSUBSET_H__
#define __TUBEX_CONNECTEDSUBSET_H__

#include "tubex_Set.h"

namespace tubex
{
  class Paving;

  /**
   * \class ConnectedSubset
   * \brief 
   *
   */
  class ConnectedSubset : public Set
  {
    public:

      ConnectedSubset(const std::vector<const Paving*>& v_subset_items);
      ~ConnectedSubset();

      const ibex::IntervalVector box() const;
      bool is_strictly_included_in_paving() const;
      Paving* get_paving() const;

      const std::vector<const Paving*>& get_items() const; // items of type k
      std::vector<ibex::IntervalVector> get_boxes() const; // items of type k
      std::vector<ibex::IntervalVector> get_boundary(int value_boundary = VALUE_MAYBE, int value_out = VALUE_OUT) const; // items of type k-1

      // Methods related to topological degree

      bool zero_proven(ibex::IntervalVector (*f)(const ibex::IntervalVector& b));
      int zeros_number(ibex::IntervalVector (*f)(const ibex::IntervalVector& b), ibex::IntervalMatrix (*Jf)(const ibex::IntervalVector& b), float precision);

    protected:

      void compute_box();

      // Methods related to topological degree

      int topological_degree(ibex::IntervalVector (*f)(const ibex::IntervalVector& b));
      bool all_positive_signs(const std::vector<int>& v_s) const;
      int orientation(const ibex::IntervalVector& b, const ibex::IntervalVector& parent_coface, int orientation) const;
      int compute_local_degree(ibex::IntervalVector (*f)(const ibex::IntervalVector& b), const ibex::IntervalVector& b, const ibex::IntervalVector& common_cocoface) const;
      std::vector<int> sign_vector(ibex::IntervalVector (*f)(const ibex::IntervalVector& b), const ibex::IntervalVector& b, const ibex::IntervalVector& common_cocoface) const;
      std::vector<ibex::IntervalVector> get_cofaces(const ibex::IntervalVector& b) const;
      std::vector<ibex::IntervalVector> get_cofaces(const ibex::IntervalVector& b, const ibex::IntervalVector& common_cocoface) const;
      int box_dimension(const ibex::IntervalVector& b) const;
      bool non_singular_jacobian(ibex::IntervalMatrix (*Jf)(const ibex::IntervalVector& b), float precision);

    protected:

      std::vector<const Paving*> m_v_subset_items;
  };
}

#endif