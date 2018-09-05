/* ============================================================================
 *  tubex-lib - TubeDataSynthesis class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_TUBEDATASYNTHESIS_H__
#define __TUBEX_TUBEDATASYNTHESIS_H__

namespace tubex
{
  class TubeTreeSynthesis;

  class TubeDataSynthesis
  {
    public:

      TubeDataSynthesis();
      bool is_up_to_date() const;
      void update(const TubeTreeSynthesis& tree_node);

    protected:

      bool m_up_to_date = false;
  };

  class TubeDataSynthesis_Slicing : public TubeDataSynthesis
  {
    public:

      TubeDataSynthesis_Slicing();
      int slices_number() const;
      const ibex::Interval& domain() const;

    protected:

      int m_slices_number;
      ibex::Interval m_domain;
  };

  class TubeDataSynthesis_Emptiness : public TubeDataSynthesis
  {
    public:

      TubeDataSynthesis_Emptiness();
      bool emptiness() const;

    protected:

      bool m_is_empty;
  };

  class TubeDataSynthesis_Codomain : public TubeDataSynthesis
  {
    public:

      TubeDataSynthesis_Codomain();
      const ibex::Interval& codomain() const;
      const std::pair<ibex::Interval,ibex::Interval>& codomain_bounds() const;

    protected:

      ibex::Interval m_codomain;
      std::pair<ibex::Interval,ibex::Interval> m_enclosed_bounds;
  };

  class TubeDataSynthesis_Primitive : public TubeDataSynthesis
  {
    public:

      TubeDataSynthesis_Primitive();
      const std::pair<ibex::Interval,ibex::Interval>& primitive_bounds() const;

    protected:

      std::pair<ibex::Interval,ibex::Interval> m_partial_primitive;
  };

  class TubeDataSynthesis_Volume : public TubeDataSynthesis
  {
    public:

      TubeDataSynthesis_Volume();
      double volume() const;

    protected:

      double m_volume;
  };
}

#endif