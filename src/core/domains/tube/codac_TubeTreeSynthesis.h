/** 
 *  TubeTreeSynthesis class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_TUBETREESYNTHESIS_H__
#define __CODAC_TUBETREESYNTHESIS_H__

#include "codac_Slice.h"

namespace codac
{
  class TubeTreeSynthesis
  {
    public:

      TubeTreeSynthesis(const Tube* tube, int k0, int kf, const std::vector<const Slice*>& v_tube_slices);
      ~TubeTreeSynthesis();

      const Interval tdomain() const;
      int nb_slices() const;
      const Interval operator()(const Interval& t);
      const Interval invert(const Interval& y, const Interval& search_tdomain);
      const Interval codomain();
      const std::pair<Interval,Interval> codomain_bounds();
      const std::pair<Interval,Interval> eval(const Interval& t = Interval::ALL_REALS);
      
      int time_to_index(double t) const;
      Slice* slice(int slice_id);
      const Slice* slice(int slice_id) const;
      Slice* slice(double t);
      const Slice* slice(double t) const;

      bool is_leaf() const;
      bool is_root() const;
      TubeTreeSynthesis* root();

      void request_values_update();
      void request_integrals_update(bool propagate_to_other_slices = true);
      void update_values();
      void update_integrals();
      std::pair<Interval,Interval> partial_integral(const Interval& t);
      const std::pair<Interval,Interval> partial_primitive_bounds(const Interval& t = Interval::ALL_REALS);

    protected:

      // Slices connections
      const Slice *m_slice_ref = nullptr;
      const Tube *m_tube_ref = nullptr;

      // Binary tree structure
      TubeTreeSynthesis *m_parent = nullptr;
      TubeTreeSynthesis *m_first_subtree = nullptr, *m_second_subtree = nullptr;

      int m_nb_slices = 1;
      Interval m_tdomain, m_codomain;
      std::pair<Interval,Interval> m_codomain_bounds;
      std::pair<Interval,Interval> m_partial_primitive;

      bool m_integrals_update_needed = true;
      bool m_values_update_needed = true;
  };
}

#endif