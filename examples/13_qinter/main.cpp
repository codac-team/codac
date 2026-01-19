// Example from:
//   Robust Localisation Using Separators
//   Luc Jaulin and Benoı̂t Desrochers

#include <codac>
using namespace codac2;

int main()
{
  auto create_sep = [](const Vector& b, const Interval& d)
  {
    DefaultFigure::draw_circle(b, 0.1, {{Color::white(),Color::white()}, "z:5"});
    DefaultFigure::draw_ring(b, d, {Color::dark_gray(), "z:4"});

    VectorVar v(2);
    AnalyticFunction f({v},sqr(v[0]-b[0])+sqr(v[1]-b[1]));
    return SepInverse(f, sqr(d));
  };

  auto s1 = create_sep({1,3},{1,2});
  auto s2 = create_sep({3,1},{2,3});
  auto s3 = create_sep({-1,-1},{3,4});

  DefaultFigure::set_axes(axis(0,{-6,6}), axis(1,{-6,6}));

  int q = 2;
  DefaultFigure::pave(
    {{-6,6},{-6,6}},
    SepQInter(q, s1,s2,s3),
    5e-2
  );
}