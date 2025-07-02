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

    void contract(IntervalVector& a) const
    {
      IntervalVector u = IntervalVector::empty(2);
      for(const auto& mi : _M)
        u |= a & mi;
      a = u;
    }

  protected:

    const std::vector<IntervalVector> _M;
};


// [C-q4-beg]
std::vector<IntervalVector> g(double t, const Vector& x, const std::vector<IntervalVector>& M)
{
  std::vector<IntervalVector> obs; // several landmarks can be seen at some ti
  Interval scope_range(0,10);
  Interval scope_angle(-PI/4,PI/4);

  for(const auto& mi : M)
  {
    Interval r = sqrt(sqr(mi[0]-x[0]) + sqr(mi[1]-x[1]));
    Interval a = atan2(mi[1]-x[1], mi[0]-x[0]) - x[2];

    // If the landmark is seen by the robot:
    if(scope_range.is_superset(r) && scope_angle.is_superset(a))
      obs.push_back(IntervalVector({t,r,a}));
  }
  
  return obs;
}
// [C-q4-end]


int main()
{
  // [C-q2-beg]
  srand(time(NULL)); // initialize the random seed (from C++)
  int N = 50; // number of landmarks
  IntervalVector X({{-40,40},{-40,40}}); // landmarks distribution zone

  std::vector<IntervalVector> M; // creating the landmarks
  for(int i = 0 ; i < N ; i++)
    M.push_back(IntervalVector(X.rand()).inflate(0.2));

  Figure2D fig("Robot simulation", GraphicOutput::VIBES);
  fig.set_axes(
    axis(0, X[0].inflate(10), "x_1"),
    axis(1, X[1].inflate(10), "x_2")
  ).auto_scale();

  for(const auto& mi : M) // displaying the landmarks
    fig.draw_box(mi, {Color::dark_green(),Color::green()});
  // [C-q2-end]


  // [C-q3-beg]
  std::list<Vector> wpts; // creating random waypoints for simulating the robot trajectory
  X = IntervalVector({{-35,35},{-35,35}}); // robot evolution zone
  for(int i = 0 ; i < 5 ; i++) // 5 waypoints
    wpts.push_back(X.rand());

  RobotSimulator s;
  s.w_max = 0.2; // maximum turning speed
  auto u = SampledTraj<Vector>(); // the simulator will return the inputs (not used)
  auto x_truth = s.simulate(
    {0,0,0,0}, // initial state (will be supposed unknown)
    1e-2, // simulation time step
    wpts, u);
  // [C-q3-end]


  // [C-q5-beg]
  double prev_t = 0.;
  double time_between_obs = 3.;
  std::vector<IntervalVector> obs;

  for(double t = x_truth.tdomain().lb() ; t < x_truth.tdomain().ub() ; t += 1e-2)
  {
    if(t-prev_t > time_between_obs)
    {
      auto obs_ti = g(t,x_truth(t),M); // computing the observation vector

      for(const auto& yi : obs_ti)
      {
        prev_t = yi[0].mid();
        fig.draw_pie(x_truth(t).subvector(0,1), yi[1]|0., yi[2]+x_truth(t)[2], Color::light_gray());
        fig.draw_pie(x_truth(t).subvector(0,1), yi[1],    yi[2]+x_truth(t)[2], Color::red());
      }

      obs.insert(obs.end(),obs_ti.begin(),obs_ti.end());
    }
  }
  // [C-q5-end]


  // [C-q6-beg]
  VectorVar _x(4);
  AnalyticFunction h({_x},
    {
      // Positions are not known..
      Interval(-oo,oo), 
      Interval(-oo,oo),
      // But headings (x3) and velocities (x4) are bounded..
      _x[2] + 2e-2*Interval(-1,1),
      _x[3] + 2e-2*Interval(-1,1)
    });

  auto tdomain = create_tdomain(x_truth.tdomain(), 5e-2, true);
  // The tube x is created from the interval evaluation of the actual trajectory
  SlicedTube x = h.tube_eval(SlicedTube(tdomain,x_truth));
  // The tube v is created as a four-dimensional tube of infinite values
  SlicedTube v(tdomain, IntervalVector({{-oo,oo},{-oo,oo},{-oo,oo},{-oo,oo}}));
  // [C-q6-end]


  VectorVar v1(2), v2(4), v3(2);
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


  // [C-q7-beg]
  CtcDeriv ctc_deriv;

  VectorVar x_(4), v_(4);
  AnalyticFunction f({x_,v_},
    {
      v_[0]-x_[3]*cos(x_[2]),
      v_[1]-x_[3]*sin(x_[2])
    });

  CtcInverse<
    IntervalVector, // f output type
    IntervalVector,IntervalVector // f inputs (i.e. contractor inputs)
    > ctc_f(f, {0,0});
  // + other contractors from previous lessons:
  // ctc_plus, ctc_minus, ctc_polar, ctc_constell
  // [C-q7-end]

  CtcPolar ctc_polar;
  MyCtc ctc_constell(M);


  // [C-q8-beg]
  fixpoint([&]() {

    for(auto& yi : obs)
    {
      IntervalVector xi = x(yi[0]); // evalution of the tube at time ti=yi[0]
      Interval ai;
      IntervalVector si(2);
      IntervalVector mi(2); // the identity (position) of the landmark is not known

      fixpoint([&]() {

        ctc_minus.contract(mi,xi,si);
        ctc_plus.contract(xi[2],yi[2],ai);
        ctc_polar.contract(si[0],si[1],yi[1],ai);
        ctc_constell.contract(mi);

      }, xi,yi,mi);

      x.set(xi,yi[0]); // restriction on the tube x at time ti=yi[0]
    }

    ctc_f.contract_tube(x,v);
    ctc_deriv.contract(x,v);

  }, x);
  // [C-q8-end]

  std::cout << x << std::endl;

  // [C-q9-beg]
  fig.draw_tube(x);
  fig.draw_trajectory(x_truth);
  auto xf = x_truth(x_truth.tdomain().ub());
  fig.draw_tank(xf, 2., {Color::black(),Color::yellow()});
  fig.draw_pie(xf, {0,10}, xf[2]+Interval(-PI/4,PI/4), Color::dark_gray());
  for(const auto& mi : M)
     fig.draw_box(mi, {Color::dark_green(),Color::green()});
  // [C-q9-end]
}