/* ============================================================================
 *  tubex-lib - TubeTreeSynthesis class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_TUBETREESYNTHESIS_H__
#define __TUBEX_TUBETREESYNTHESIS_H__

#include "tubex_Slice.h"
//#include "tubex_TubeDataSynthesis.h"

namespace tubex
{
  class TubeTreeSynthesis
  {
    public:

      TubeTreeSynthesis();
      const ibex::Interval domain();

      bool is_leaf() const;
      bool is_root() const;
      TubeTreeSynthesis* get_root();
      Slice* get_first_slice();

      //TubeDataSynthesis_Slicing& slicing_synthesis();
      //TubeDataSynthesis_Emptiness& emptiness_synthesis();
      //TubeDataSynthesis_Codomain& codomain_synthesis();
      //TubeDataSynthesis_Primitive& primitive_synthesis();
      //TubeDataSynthesis_Volume& volume_synthesis();

      void request_updates();
      void update_values();
      std::pair<ibex::Interval,ibex::Interval> partial_integral(const ibex::Interval& t);
      const std::pair<ibex::Interval,ibex::Interval> partial_primitive_bounds(const ibex::Interval& t = ibex::Interval::ALL_REALS);

    protected:

      //TubeDataSynthesis_Slicing m_syn_slicing;
      //TubeDataSynthesis_Emptiness m_syn_emptiness;
      //TubeDataSynthesis_Codomain m_syn_codomain;
      //TubeDataSynthesis_Primitive m_syn_primitive;
      //TubeDataSynthesis_Volume m_syn_volume;

      // Binary tree structure
      TubeTreeSynthesis *m_parent = NULL;
      TubeTreeSynthesis *m_first_subtree = NULL, *m_second_subtree = NULL;

      // Slices connections
      Slice *m_slice_ref = NULL;
      Tube *m_tube_ref = NULL;

      int m_slices_number;
      ibex::Interval m_domain;
      std::pair<ibex::Interval,ibex::Interval> m_partial_primitive;

      bool m_update_needed;

  };
}

#endif