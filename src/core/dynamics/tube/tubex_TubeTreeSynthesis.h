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
#include "tubex_TubeDataSynthesis.h"

namespace tubex
{
  class TubeTreeSynthesis
  {
    public:

      TubeTreeSynthesis();
      TubeDataSynthesis_Slicing& slicing_synthesis();
      TubeDataSynthesis_Emptiness& emptiness_synthesis();
      TubeDataSynthesis_Codomain& codomain_synthesis();
      TubeDataSynthesis_Primitive& primitive_synthesis();
      TubeDataSynthesis_Volume& volume_synthesis();

    protected:

      TubeDataSynthesis_Slicing m_syn_slicing;
      TubeDataSynthesis_Emptiness m_syn_emptiness;
      TubeDataSynthesis_Codomain m_syn_codomain;
      TubeDataSynthesis_Primitive m_syn_primitive;
      TubeDataSynthesis_Volume m_syn_volume;

      // Binary tree structure
      TubeTreeSynthesis *m_first_subtree = NULL;
      TubeTreeSynthesis *m_second_subtree = NULL;

      Slice *m_slice_ref = NULL;
  };
}

#endif