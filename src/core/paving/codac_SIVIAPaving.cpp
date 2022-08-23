/** 
 *  SIVIAPaving class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou, Julien Damers
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <list>
#include <iostream>
#include "codac_SIVIAPaving.h"

using namespace std;
using namespace ibex;

namespace codac
{
  SIVIAPaving::SIVIAPaving(const IntervalVector &init_box) : Paving(init_box)
  {

  }

  void SIVIAPaving::compute(const Function &f, const IntervalVector &y, float precision)
  {
    assert(precision > 0.);
    assert(f.nb_var() == box().size());
    assert(f.image_dim() == y.size());

    IntervalVector result = f.eval_vector(box());

    if (result.is_subset(y))
      set_value(SetValue::IN);

    else if(!result.intersects(y))
      set_value(SetValue::OUT);

    else if(box().max_diam() < precision)
      set_value(SetValue::UNKNOWN);

    else
    {
      bisect();
      ((SIVIAPaving*)m_first_subpaving)->compute(f, y, precision);
      ((SIVIAPaving*)m_second_subpaving)->compute(f, y, precision);
    }
  }

  void SIVIAPaving::compute(Ctc &ctc, float precision)
  {
    assert(precision > 0.);
    assert(ctc.nb_var == box().size());

    IntervalVector result = box();
    ctc.contract(result);

    if(result.is_empty())
      set_value(SetValue::OUT);

    else if(result.max_diam() < precision)
      set_value(SetValue::UNKNOWN);

    else
    {
      bisect();
      ((SIVIAPaving*)m_first_subpaving)->compute(ctc, precision);
      ((SIVIAPaving*)m_second_subpaving)->compute(ctc, precision);
    }
  }

  void SIVIAPaving::compute(Sep& sep, float precision)
  {
    assert(precision > 0.);
    IntervalVector box_in = box();
    IntervalVector box_out = box();
    sep.separate(box_in, box_out);

    if(box_in.is_empty())
      set_value(SetValue::IN);

    else if(box_out.is_empty())
      set_value(SetValue::OUT);

    else if(box().max_diam() < precision)
      set_value(SetValue::UNKNOWN);

    else
    {
      bisect();
      ((SIVIAPaving*)m_first_subpaving)->compute(sep, precision);
      ((SIVIAPaving*)m_second_subpaving)->compute(sep, precision);
    }
  }
}