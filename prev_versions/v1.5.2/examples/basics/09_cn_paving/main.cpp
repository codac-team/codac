/** 
 *  Codac - Examples
 *  Using CN for paving
 * ----------------------------------------------------------------------------
 *
 *  \brief      Using CN for paving

 *  \date       2021
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <codac.h>

using namespace std;
using namespace codac;

class FunctionPaving : public Paving
{
  public:

    FunctionPaving(const IntervalVector& init_box) : Paving(init_box, SetValue::UNKNOWN)
    {

    }

    void compute(ContractorNetwork& cn, IntervalVectorVar& var_x, IntervalVar& var_y, float precision)
    {
      assert(precision > 0.);

      Interval y;
      // Abstract variables replaced by actual domains
      // related to the current paving box 
      cn.contract({
        {var_x, box()},
        {var_y, y}
      });

      Interval zero(-0.5,0.5);

      if(y.is_subset(zero))
        set_value(SetValue::IN);

      else if(!y.intersects(zero))
        set_value(SetValue::OUT);

      else if(box().max_diam() < precision)
        set_value(SetValue::UNKNOWN);

      else
      {
        bisect();
        ((FunctionPaving*)m_first_subpaving)->compute(cn, var_x, var_y, precision);
        ((FunctionPaving*)m_second_subpaving)->compute(cn, var_x, var_y, precision);
      }
    }   
};

int main()
{
  CtcFunction ctc_f(Function("x[2]", "y", "x[0]*cos(x[0]-x[1])*sin(x[0])+x[1]-y"));

  ContractorNetwork cn;

  // Abstract variables are created,
  // they will be replaced afterwards by the values
  // corresponding to each box of the paving
  IntervalVar y;
  IntervalVectorVar x(2);

  // The CN is built only once, outside the Paving class
  cn.add(ctc_f, {x, y});

  FunctionPaving paving(IntervalVector({{-3,3},{-3,3}}));
  paving.compute(cn, x, y, 0.05);

  vibes::beginDrawing();
  VIBesFigPaving fig_paving("Paving", &paving);
  fig_paving.set_properties(100, 100, 400, 400);
  fig_paving.show();
  vibes::endDrawing();

  return EXIT_SUCCESS;
}