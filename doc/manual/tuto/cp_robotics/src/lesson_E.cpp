// [E-q1-beg]
#include <codac>
using namespace std;
using namespace codac2;
// [E-q1-end]

// [E-q3-beg]
vector<double> tile_times(const std::string& file)
{
  ifstream f(file);
  if(!f)
    throw runtime_error("Unable to open joints detections file.");

  vector<double> t;
  string token;

  while(getline(f, token, '\n'))
    if(!token.empty())
      t.push_back(stod(token));

  return t;
}
// [E-q3-end]


// [E-q4-beg]
void draw_map(const IntervalVector& X, double L)
{
  for(int d = 0; d < 2; d++)
  {
    int kmin = std::min(0, (int)(std::floor(X[d].lb()/L))+1);
    int kmax = std::max(0, (int)(std::ceil (X[d].ub()/L))-1);

    for(int k = kmin; k <= kmax; k++)
    {
      double a = k*L;
      if(d == 0)
        DefaultFigure::draw_line({{a, X[1].lb()}, {a, X[1].ub()}}, Color::light_gray());
      else
        DefaultFigure::draw_line({{X[0].lb(), a}, {X[0].ub(), a}}, Color::light_gray());
    }
  }

  for(const auto& v : { Vector({0,0}), Vector({-8,8})})
    DefaultFigure::draw_box(
      IntervalVector(L*v) + IntervalVector({{0,L},{0,L}}),
      {Color::dark_green(),Color::green(0.1)});
}
// [E-q4-end]


int main()
{
  // [E-q2-beg]
  // Loading trajectories
  SampledTraj<Vector> pos;
  {
    std::ifstream in("../data/herdeudedac_tiles_pos.cdc", std::ios::binary);
    deserialize(in, pos);
    in.close();
  }
  SampledTraj<Vector> u;
  {
    std::ifstream in("../data/herdeudedac_tiles_odo.cdc", std::ios::binary);
    deserialize(in, u);
    in.close();
    u = u.sampled_as(pos);
  }
  // [E-q2-end]



  // [E-q3b-beg]
  // Observations (times of tiles detections)
  vector<double> T = tile_times("../data/herdeudedac_tiles_detections.txt");
  // [E-q3b-end]




  // [E-q2b-beg]

  // [E-q2b-end]


  // [E-q4b-beg]
  // Map parameters
  double L = 0.3; // tile width
  double L_eps = 1e-2; // tile thickness
  // [E-q4b-end]


  // [E-q5-beg]
  auto f = [L,L_eps](Index i)
  {
    VectorVar x(2);
    return AnalyticFunction({x}, mod(x[i]+L_eps/2,L));
  };

  CtcInverse c_verti(f(0), {0,L_eps});
  CtcInverse c_horiz(f(1), {0,L_eps});

  auto ctc_map = c_verti | c_horiz;
  // [E-q5-end]

  auto your_contractor = ctc_map;


  // [E-q6-beg]
  Figure2D fig_ctcmap("Testing tile-map contractor", GraphicOutput::VIBES);
  fig_ctcmap.set_axes(axis(0,{-1,1}), axis(1,{-1,1})).auto_scale();
  fig_ctcmap.pave({{-1,1},{-1,1}}, your_contractor, 1e-2);
  // [E-q6-end]


  // [E-q6b-beg]

  // [E-q6b-end]


  // [E-q7-beg]
  // Robot parameters
  double R1 = 0.033, R2 = 0.16, R3 = 8e-2;
  Vector x0 { L/2, L/2, 0.018, 0 }; // initial state

  SampledTraj traj_spd = R1*(u[1].derivative()+u[0].derivative())/2;
  SampledTraj traj_hdg = (R1*(u[1].derivative()-u[0].derivative())/R2).primitive() + x0[2];
  traj_hdg = continuous_traj(pos[2]); // better using the measured heading (avoiding errors due to drift)
  // [E-q7-end]


  // [E-q8-beg]
  auto tdomain = create_tdomain(u.tdomain(), 5e-2); // last argument is time discretization

  SlicedTube tube_hdg(tdomain, traj_hdg);
  tube_hdg.inflate(2e-3);
  SlicedTube tube_spd(tdomain, traj_spd);
  tube_spd.inflate(9e-3);
  // [E-q8-end]


  // [E-q9-beg]
  // Computing tube of velocities
  ScalarVar v_hdg, v_spd;
  AnalyticFunction f_evol({v_hdg,v_spd}, { 
    v_spd*cos(v_hdg),
    v_spd*sin(v_hdg)
  });
  SlicedTube tube_v12 = f_evol.tube_eval(tube_hdg,tube_spd);
  // [E-q9-end]


  // [E-q10-beg]
  // Computing initial tube of positions
  IntervalVector ix0(x0.subvector(0,1));
  ix0.inflate(L/3);
  SlicedTube tube_x12 = tube_v12.primitive(ix0);
  // [E-q10-end]


  // [E-q11-beg]
  IntervalVector X({{-5,2},{-2,5}});
  DefaultFigure::set_window_properties({50,50},{1000,1000});
  DefaultFigure::set_axes(axis(0,X[0]), axis(1,X[1])).auto_scale();

  draw_map(X,L);
  DefaultFigure::draw_tube(tube_x12, {Color::light_gray(),Color::light_gray()});
  // [E-q11-end]


  // [E-q12-beg]
  CtcDeriv ctc_deriv;
  // ctc_map is already defined from E.5
  // [E-q12-end]


  // [E-q13-beg]
  fixpoint([&]()
  {
    for(const auto& ti : T)
    {
      double tj = ti-0.01;
      IntervalVector pi = tube_x12(tj);
      Interval hdgi = tube_hdg(tj);
      IntervalVector bi { pi[0]-R3*cos(hdgi), pi[1]-R3*sin(hdgi) };
      ctc_map.contract(bi);
      pi[0] &= bi[0]+R3*cos(hdgi);
      pi[1] &= bi[1]+R3*sin(hdgi);
      tube_x12.set(pi,tj);
    }

    ctc_deriv.contract(tube_x12,tube_v12);
  }, tube_x12);
  // [E-q13-end]


  // [E-q14-beg]
  DefaultFigure::draw_tube(tube_x12, ColorMap::blue_tube());
  draw_map(X,L); // drawing again the map over the tube
  // [E-q14-end]


  // [E-q15-beg]
  auto draw_estim = [R3,&traj_hdg,&T](const SampledTraj<Vector>& x, const Color& traj_col, const Color& detec_col)
  {
    DefaultFigure::draw_trajectory(x, {traj_col,"--"});
    for(const auto& ti : T)
    {
      Vector pi = cart_prod(x(ti).subvector(0,1),traj_hdg(ti));
      Vector bi { pi[0]-R3*cos(pi[2]), pi[1]-R3*sin(pi[2]) };
      DefaultFigure::draw_tank(pi.subvector(0,2), .15, detec_col);
      DefaultFigure::draw_circle(bi, 5e-3, detec_col);
    }
  };

  draw_estim(tube_x12.mid<Vector>(), Color::white(), Color::dark_blue());
  // [E-q15-end]


  // [E-q16-beg]

  // [E-q16-end]
}