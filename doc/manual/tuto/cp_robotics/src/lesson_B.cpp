#include <codac>
using namespace codac2;

class MyCtc : public Ctc<MyCtc,IntervalVector>
{
  public:

    MyCtc(const std::vector<IntervalVector>& M)
      : Ctc<MyCtc,IntervalVector>(2), // the contractor acts on 2d boxes
      _M(M) // attribute needed later on for the contraction
    {

    }

    // [B-q2-beg]
    void contract(IntervalVector& a) const
    {
      IntervalVector u = IntervalVector::empty(2);
      for(const auto& mi : _M)
        u |= a & mi;
      a = u;
    }
    // [B-q2-end]

  protected:

    const std::vector<IntervalVector> _M;
};

// [B-q6-beg]
IntervalVector g(const IntervalVector& x, const IntervalVector& mi)
{
  Interval r = sqrt(sqr(mi[0]-x[0]) + sqr(mi[1]-x[1]));
  Interval b = atan2(mi[1]-x[1], mi[0]-x[0]) - x[2];
  return IntervalVector({r,b}).inflate(0.02);
}
// [B-q6-end]

int main()
{
  // [B-q3-beg]
  std::vector M = {
    IntervalVector({1.5,2.5}),
    IntervalVector({3,1}),
    IntervalVector({2,2}),
    IntervalVector({2.5,3}),
    IntervalVector({3.5,2}),
    IntervalVector({4,1}),
    IntervalVector({1.5,0.5})
  };

  for(auto& Mi : M)
    Mi.inflate(0.05);

  IntervalVector a1({{1.25,3},{1.6,2.75}});
  IntervalVector a2({{2,3.5},{0.6,1.2}});
  IntervalVector a3({{1.1,3.25},{0.2,1.4}});

  MyCtc ctc_constell(M);

  ctc_constell.contract(a1);
  ctc_constell.contract(a2);
  ctc_constell.contract(a3);

  std::cout << a1 << std::endl << a2 << std::endl << a3 << std::endl;
  // [B-q3-end]

  // [B-q4-beg]
  Vector x_truth({2,1,PI/6});
  // [B-q4-end]

  // [B-q5-beg]
  DefaultFigure::set_axes(axis(0,{1,4.5}), axis(1,{0,3.5}));
  DefaultFigure::draw_tank(x_truth, 0.4, {Color::black(),Color::yellow()});
  for(const auto& mi : M)
    DefaultFigure::draw_box(mi, {Color::dark_green(),Color::green()});
  // [B-q5-end]

  // [B-q7-beg]
  std::vector<IntervalVector> obs;
  for(const auto& mi : M)
  {
    obs.push_back(cart_prod(g(x_truth,mi),mi));
    // We append the position of the landmark to the measurement
    // yi = {range}×{bearing}×{2d position}
  }

  for(const auto& yi : obs)
  {
    DefaultFigure::draw_pie(x_truth.subvector(0,1), yi[0],     x_truth[2]+yi[1], Color::red());
    DefaultFigure::draw_pie(x_truth.subvector(0,1), yi[0] | 0, x_truth[2]+yi[1], Color::light_gray());
  }
  // [B-q7-end]

  CtcPolar ctc_polar;

  VectorVar v1(2), v2(3), v3(2);
  AnalyticFunction f_minus({v1,v2,v3}, {
      v1[0]-v2[0]-v3[0],
      v1[1]-v2[1]-v3[1]
  });
  CtcInverse<IntervalVector, // f_minus output type
    IntervalVector,IntervalVector,IntervalVector // f_minus inputs (i.e. contractor inputs)
    > ctc_minus(f_minus, {0,0});

  ScalarVar s1, s2, s3;
  AnalyticFunction f_plus({s1,s2,s3}, s1+s2-s3);
  CtcInverse<
    Interval, // f_plus output type
    Interval,Interval,Interval // f_plus inputs (i.e. contractor inputs)
    > ctc_plus(f_plus, Interval(0.));

  // [B-q7b-beg]
  IntervalVector x = cart_prod(Interval(-oo,oo),Interval(-oo,oo),x_truth[2]);

  fixpoint([&]() {

    for(auto& yi : obs)
    {
      // Intermediate variables for each observation;
      Interval ai;
      IntervalVector di(2);
      IntervalVector mi = yi.subvector(2,3); // the identity (position) of the landmark is known

      // Running a fixpoint for each observation
      fixpoint([&]() {

        ctc_minus.contract(mi,x,di);
        ctc_plus.contract(x[2],yi[1],ai);
        ctc_polar.contract(di[0],di[1],yi[0],ai);

      }, x,yi,mi,ai,di);
    }

  }, x);
  // [B-q7b-end]

  // [B-q8-beg]
  std::cout << x << std::endl;
  DefaultFigure::draw_box(x); // does not display anything if unbounded
  // [B-q8-end]

  // [B-q9-beg]
  x = cart_prod(Interval(-oo,oo),Interval(-oo,oo),x_truth[2]); // reset for testing

  fixpoint([&]() {

    for(auto& yi : obs)
    {
      Interval ai;
      IntervalVector di(2);
      // ==== Changed domain ====
      IntervalVector mi(2); // the identity (position) of the landmark is not known
      // ========================

      fixpoint([&]() {

        ctc_minus.contract(mi,x,di);
        ctc_plus.contract(x[2],yi[1],ai);
        ctc_polar.contract(di[0],di[1],yi[0],ai);
        // ==== Added contractor ====
        ctc_constell.contract(mi); // new contractor for data association
        // ==========================

      }, x,yi,mi,ai,di);
    }

  }, x);
  // [B-q9-end]

  fixpoint([&]() {

    for(auto& yi : obs)
    {
      Interval ai;
      IntervalVector di(2);
      // ==== Changed domain ====
      IntervalVector mi(2); // the identity (position) of the landmark is not known
      // ========================

      fixpoint([&]() {

        ctc_minus.contract(mi,x,di);
        ctc_plus.contract(x[2],yi[1],ai);
        ctc_polar.contract(di[0],di[1],yi[0],ai);
        // ==== Added contractor ====
        ctc_constell.contract(mi); // new contractor for data association
        // ==========================

      }, x,yi,mi,ai,di);

      // [B-q10-beg]
      if(mi.max_diam() <= 1)
        DefaultFigure::draw_point(mi.mid());
      // [B-q10-end]
    }

  }, x);
}