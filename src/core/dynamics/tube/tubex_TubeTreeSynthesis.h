/** 
 *  TubeTreeSynthesis class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_TUBETREESYNTHESIS_H__
#define __TUBEX_TUBETREESYNTHESIS_H__

#include "tubex_Slice.h"

namespace tubex
{
  class TubeTreeSynthesis
  {
    public:

      TubeTreeSynthesis(const Tube* tube, int k0, int kf, const std::vector<const Slice*>& v_tube_slices);
      ~TubeTreeSynthesis();

      const ibex::Interval tdomain() const;
      int nb_slices() const;
      const ibex::Interval operator()(const ibex::Interval& t);
      const ibex::Interval invert(const ibex::Interval& y, const ibex::Interval& search_tdomain);
      const ibex::Interval codomain();
      const std::pair<ibex::Interval,ibex::Interval> codomain_bounds();
      const std::pair<ibex::Interval,ibex::Interval> eval(const ibex::Interval& t = ibex::Interval::ALL_REALS);
      
      int time_to_index(double t) const;
      Slice* slice(int slice_id);
      const Slice* slice(int slice_id) const;

      bool is_leaf() const;
      bool is_root() const;
      TubeTreeSynthesis* root();

      void request_values_update();
      void request_integrals_update(bool propagate_to_other_slices = true);
      void update_values();
      void update_integrals();
      std::pair<ibex::Interval,ibex::Interval> partial_integral(const ibex::Interval& t);
      const std::pair<ibex::Interval,ibex::Interval> partial_primitive_bounds(const ibex::Interval& t = ibex::Interval::ALL_REALS);

    protected:

      // Slices connections
      const Slice *m_slice_ref = NULL;
      const Tube *m_tube_ref = NULL;

      // Binary tree structure
      TubeTreeSynthesis *m_parent = NULL;
      TubeTreeSynthesis *m_first_subtree = NULL, *m_second_subtree = NULL;

      int m_nb_slices = 1;
      ibex::Interval m_tdomain, m_codomain;
      std::pair<ibex::Interval,ibex::Interval> m_codomain_bounds;
      std::pair<ibex::Interval,ibex::Interval> m_partial_primitive;

      bool m_integrals_update_needed = true;
      bool m_values_update_needed = true;
  };
}

#endif