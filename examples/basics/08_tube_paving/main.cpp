/** 
 *  Codac - Examples
 *  Tube paving
 * ----------------------------------------------------------------------------
 *
 *  \brief      Tube paving

 *  \date       2021
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <codac.h>

using namespace std;
using namespace codac;

class TemporalPaving : public Paving
{
  public:

    TemporalPaving(const IntervalVector& init_box) : Paving(init_box, SetValue::UNKNOWN)
    {

    }

    void compute(float precision, const Tube& x)
    {
      assert(precision > 0.);
      const Interval& t = box()[0], &y = box()[1];

      if(!x(t).intersects(y))
      {
        assert(!x.invert(y,t).intersects(y)); // used as a test of invert() method
        return set_value(SetValue::OUT);
      }

      pair<Interval,Interval> p = x.eval(t);

      if((p.first == NEG_INFINITY || p.first.ub() < y.lb())
       && (p.second == POS_INFINITY || p.second.lb() > y.ub()))
        return set_value(SetValue::IN);

      if(box().max_diam() < precision)
        return set_value(SetValue::UNKNOWN);

      else
      {
        bisect();
        ((TemporalPaving*)m_first_subpaving)->compute(precision, x);
        ((TemporalPaving*)m_second_subpaving)->compute(precision, x);
      }
    }
};

int main()
{
  float dt = 0.005;
  Interval tdomain(0,6);
  Tube x(tdomain, dt, TFunction("sin(t)+(t+1)*[-0.1,0.1]"));
  x.set(Interval(-1.6,1.6),Interval(3,4));

  clock_t t_start = clock();
  TemporalPaving paving(IntervalVector({{-1,7},{-2,2}}));
  x.enable_synthesis(SynthesisMode::BINARY_TREE);
  paving.compute(dt*2.,x);
  printf("Paving computation time: %.2fs\n", (double)(clock() - t_start)/CLOCKS_PER_SEC);

  vibes::beginDrawing();
  VIBesFigPaving fig_paving("Paving", &paving);
  fig_paving.set_properties(100, 100, 800, 400);
  fig_paving.show();
  vibes::endDrawing();

  return EXIT_SUCCESS;
}