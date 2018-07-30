#include "ibex.h"
#include "tubex.h"
#include "tubex-robotics.h"

using namespace std;
using namespace ibex;
using namespace tubex;

int main()
{
  TubeVector *x;
  TrajectoryVector *x_truth;
  DataLoader_Redermor data_loader("./data/redermor/gesmi.txt");
  data_loader.load_data(x, x_truth);

  //vibes::beginDrawing();
  //VibesFigure_Tube fig("Redermor", x/*, x_truth*/);
  //fig.set_properties(1550, 50, 600, 300);
  //fig.show();
  //vibes::endDrawing();

  IntervalVector seamark(2);
  map<int,IntervalVector*> m_seamarks;
  seamark[0] = 594.0533723; seamark[1] = 374.7293035;  /*seamark[2] = 19.;*/ m_seamarks[0] = new IntervalVector(seamark);
  seamark[0] = 599.6093723; seamark[1] = 484.5528643;  /*seamark[2] = 19.;*/ m_seamarks[1] = new IntervalVector(seamark);
  seamark[0] = 601.4613723; seamark[1] = 557.3573086;  /*seamark[2] = 19.;*/ m_seamarks[2] = new IntervalVector(seamark);
  seamark[0] = 94.01337232; seamark[1] = -2.868189501; /*seamark[2] = 19.;*/ m_seamarks[3] = new IntervalVector(seamark);
  seamark[0] = 119.9413723; seamark[1] = 84.75155672;  /*seamark[2] = 20.;*/ m_seamarks[4] = new IntervalVector(seamark);
  seamark[0] = 127.3493723; seamark[1] = 156.3280366;  /*seamark[2] = 20.;*/ m_seamarks[5] = new IntervalVector(seamark);

  IntervalVector obs(2);
  map<int,vector<IntervalVector> > m_obs;
  obs[0] = 1748.0; obs[1] = 52.68; m_obs[0].push_back(obs);
  obs[0] = 1054.0; obs[1] = 52.42; m_obs[1].push_back(obs);
  obs[0] = 1374.0; obs[1] = 54.40; m_obs[1].push_back(obs);
  obs[0] = 3038.0; obs[1] = 27.73; m_obs[1].push_back(obs);
  obs[0] = 5688.0; obs[1] = 15.05; m_obs[1].push_back(obs);
  obs[0] = 1092.0; obs[1] = 12.47; m_obs[2].push_back(obs);
  obs[0] = 4817.0; obs[1] = 36.71; m_obs[3].push_back(obs);
  obs[0] = 5172.0; obs[1] = 37.37; m_obs[3].push_back(obs);
  obs[0] = 4024.0; obs[1] = 37.90; m_obs[4].push_back(obs);
  obs[0] = 5232.0; obs[1] = 31.03; m_obs[4].push_back(obs);
  obs[0] = 3688.0; obs[1] = 26.98; m_obs[5].push_back(obs);
  obs[0] = 5279.0; obs[1] = 33.51; m_obs[5].push_back(obs);

  CtcEval ctc_eval(false, true);

  tubex::Function fg("x", "y", "xdot", "ydot", "bx", "by", 
    "(bx ; by ; sqrt((x-bx)^2+(y-by)^2) ; ((sign(x-bx)/sqrt(1+(((y-by)/(x-bx))^2)))*xdot + (sign(y-by)/sqrt(1+(((x-bx)/(y-by))^2)))*ydot))");
  map<int,Tube*> m_g;
  for(int i = 0 ; i < m_seamarks.size() ; i++)
  {
    TubeVector b(*x);
    b.resize(2);
    b.set(*m_seamarks[i]);
    TubeVector temp(*x);
    temp.resize(6);
    temp.put(4, b);
    TubeVector g = fg.eval(temp);

    Tube tube_g(2, g), tube_gdot(3, g);

    for(int k = 0 ; k < m_obs[i].size() ; k++)
    {
      ctc_eval.contract(m_obs[i][k][0], m_obs[i][k][1], tube_g, tube_gdot);
    }
  }

    Variable vx, vy, vxdot, vydot, vbx, vby, vg, vgdot;
    SystemFactory fac;
    fac.add_var(vx);
    fac.add_var(vy);
    fac.add_var(vxdot);
    fac.add_var(vydot);
    fac.add_var(vbx);
    fac.add_var(vby);
    fac.add_var(vg);
    fac.add_var(vgdot);
    //fac.add_ctr(sqr(vx0) + sqr(vx1) = 1);
    //System sys(fac);
    //ibex::CtcHC4 hc4(sys);

  tubex::CtcHC4 ctc_hc4();
  tubex::Function fg_hc4("x", "y", "xdot", "ydot", "bx", "by", "g", "gdot",
      "(g - sqrt((x-bx)^2+(y-by)^2) ; gdot - ((sign(x-bx)/sqrt(1+(((y-by)/(x-bx))^2)))*xdot + (sign(y-by)/sqrt(1+(((x-bx)/(y-by))^2)))*ydot))");
  
  for(int i = 0 ; i < m_seamarks.size() ; i++)
  {


  }

  delete x;
  delete x_truth;
  return EXIT_SUCCESS;
}