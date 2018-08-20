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
  vector<Beacon> v_seamarks = data_loader.get_beacons();
  map<int,vector<IntervalVector> > m_obs = data_loader.get_observations();

  //vibes::beginDrawing();
  //VibesFigure_Tube fig_x("Redermor state", 2);
  //fig_x.add_tube(x, "x");
  //fig_x.add_trajectory(x_truth, "x_truth");
  //fig_x.set_properties(2000, 50, 500, 250);

  //VibesFigure_Map fig_map("Redermor map", x, x_truth);
  //fig_map.add_beacons(v_seamarks);
  //fig_map.set_properties(1450, 50, 500, 500);

  for(int i = 0 ; i < v_seamarks.size() ; i++)
    for(int k = 0 ; k < m_obs[i].size() ; k++)
      x->sample(m_obs[i][k][0].mid());

  CtcEval ctc_eval(false, false);

  const char* xdyn[11];
  xdyn[0] = "x"; xdyn[1] = "y"; xdyn[2] = "z";
  xdyn[3] = "xdot"; xdyn[4] = "ydot"; xdyn[5] = "zdot";
  xdyn[6] = "bx"; xdyn[7] = "by"; xdyn[8] = "bz";
  xdyn[9] = "g"; xdyn[10] = "gdot";
  tubex::Function fg(11, xdyn, "(sqrt((x-bx)^2+(y-by)^2+(z-bz)^2) ; \
                                (xdot*sign(x-bx)/sqrt(1+(((y-by)^2+(z-bz)^2)/((x-bx)^2)))) \
                              + (ydot*sign(y-by)/sqrt(1+(((x-bx)^2+(z-bz)^2)/((y-by)^2)))) \
                              + (zdot*sign(z-bz)/sqrt(1+(((y-by)^2+(x-bx)^2)/((z-bz)^2)))))");

  map<int,TubeVector*> m_x;
  for(int i = 0 ; i < v_seamarks.size() ; i++)
  {
    m_x[i] = new TubeVector(*x);
    m_x[i]->resize(11);

    (*m_x[i])[6].set(v_seamarks[i].pos_box()[0]);
    (*m_x[i])[7].set(v_seamarks[i].pos_box()[1]);
    (*m_x[i])[8].set(v_seamarks[i].pos_box()[2]);

    m_x[i]->put(9, fg.eval(*m_x[i]));

    for(int k = 0 ; k < m_obs[i].size() ; k++)
      ctc_eval.contract(m_obs[i][k][0], m_obs[i][k][1], (*m_x[i])[9], (*m_x[i])[10]);
    (*m_x[i])[9].ctc_deriv((*m_x[i])[10]);
  }

  Variable vt, vx, vy, vz, vxdot, vydot, vzdot, vbx, vby, vbz, vg, vgdot;
  SystemFactory fac;
  fac.add_var(vt);
  fac.add_var(vx); fac.add_var(vy); fac.add_var(vz);
  fac.add_var(vxdot); fac.add_var(vydot); fac.add_var(vzdot);
  fac.add_var(vbx); fac.add_var(vby); fac.add_var(vbz);
  fac.add_var(vg); fac.add_var(vgdot);
  fac.add_ctr(vg = sqrt(sqr(vx-vbx)+sqr(vy-vby)+sqr(vz-vbz)));
  fac.add_ctr(vgdot = (vxdot*sign(vx-vbx)/sqrt(1+((sqr(vy-vby)+sqr(vz-vbz))/(sqr(vx-vbx))))) 
                    + (vydot*sign(vy-vby)/sqrt(1+((sqr(vx-vbx)+sqr(vz-vbz))/(sqr(vy-vby))))) 
                    + (vzdot*sign(vz-vbz)/sqrt(1+((sqr(vy-vby)+sqr(vx-vbx))/(sqr(vz-vbz))))));
  System sys(fac);
  ibex::CtcHC4 hc4(sys);
  tubex::CtcHC4 ctc_hc4;

  for(int i = 0 ; i < v_seamarks.size() ; i++)
  {
    cout << "Seamark " << (i+1) << endl;
    //fig_x.show();
    //fig_map.show();

    ctc_hc4.contract(hc4, *m_x[i]);

    (*m_x[i])[0].ctc_deriv((*m_x[i])[1]);
    (*m_x[i])[1].ctc_deriv((*m_x[i])[4]);
    (*m_x[i])[9].ctc_deriv((*m_x[i])[10]);

    for(int j = 0 ; j < 6 ; j++)
      (*x)[j] &= (*m_x[i])[j];
  }

  //fig_x.show();
  //fig_map.show();

  for(int i = 0 ; i < v_seamarks.size() ; i++)
  {
    delete m_x[i];
  }

  //vibes::endDrawing();

  delete x;
  delete x_truth;
  return EXIT_SUCCESS;
}