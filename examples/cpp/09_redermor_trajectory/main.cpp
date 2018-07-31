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

  vibes::beginDrawing();
  VibesFigure_Tube fig("Redermor", x, x_truth);
  fig.set_properties(1550, 50, 600, 300);

  IntervalVector seamark(3);
  map<int,IntervalVector*> m_seamarks;
  seamark[0] = 594.0533723; seamark[1] = 374.7293035;  seamark[2] = 19.; m_seamarks[0] = new IntervalVector(seamark);
  seamark[0] = 599.6093723; seamark[1] = 484.5528643;  seamark[2] = 19.; m_seamarks[1] = new IntervalVector(seamark);
  seamark[0] = 601.4613723; seamark[1] = 557.3573086;  seamark[2] = 19.; m_seamarks[2] = new IntervalVector(seamark);
  seamark[0] = 94.01337232; seamark[1] = -2.868189501; seamark[2] = 19.; m_seamarks[3] = new IntervalVector(seamark);
  seamark[0] = 119.9413723; seamark[1] = 84.75155672;  seamark[2] = 20.; m_seamarks[4] = new IntervalVector(seamark);
  seamark[0] = 127.3493723; seamark[1] = 156.3280366;  seamark[2] = 20.; m_seamarks[5] = new IntervalVector(seamark);

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

  for(int i = 0 ; i < m_seamarks.size() ; i++)
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
  for(int i = 0 ; i < m_seamarks.size() ; i++)
  {
    m_x[i] = new TubeVector(*x);
    m_x[i]->resize(11);

    // todo: optimize this block:
    TubeVector b(*x);
    b.resize(3);
    b.set(*m_seamarks[i]);
    m_x[i]->put(6, b);
    m_x[i]->put(9, fg.eval(*m_x[i]));

    Tube temp_g(9, *m_x[i]), temp_gdot(10, *m_x[i]);
    for(int k = 0 ; k < m_obs[i].size() ; k++)
      ctc_eval.contract(m_obs[i][k][0], m_obs[i][k][1], temp_g, temp_gdot);
    temp_g.ctc_deriv(temp_gdot);

    m_x[i]->put(9, temp_g);
    m_x[i]->put(10, temp_gdot);
  }

  Variable vt, vx, vy, vz, vxdot, vydot, vzdot, vbx, vby, vbz, vg, vgdot;
  SystemFactory fac;
  fac.add_var(vt);
  fac.add_var(vx);
  fac.add_var(vy);
  fac.add_var(vz);
  fac.add_var(vxdot);
  fac.add_var(vydot);
  fac.add_var(vzdot);
  fac.add_var(vbx);
  fac.add_var(vby);
  fac.add_var(vbz);
  fac.add_var(vg);
  fac.add_var(vgdot);
  fac.add_ctr(vg = sqrt(sqr(vx-vbx)+sqr(vy-vby)+sqr(vz-vbz)));
  fac.add_ctr(vgdot = (vxdot*sign(vx-vbx)/sqrt(1+((sqr(vy-vby)+sqr(vz-vbz))/(sqr(vx-vbx))))) 
                    + (vydot*sign(vy-vby)/sqrt(1+((sqr(vx-vbx)+sqr(vz-vbz))/(sqr(vy-vby))))) 
                    + (vzdot*sign(vz-vbz)/sqrt(1+((sqr(vy-vby)+sqr(vx-vbx))/(sqr(vz-vbz))))));
  System sys(fac);
  ibex::CtcHC4 hc4(sys);
  tubex::CtcHC4 ctc_hc4;

  for(int i = 0 ; i < m_seamarks.size() ; i++)
  {
    cout << "Seamark " << (i+1) << endl;
    fig.show();

    ctc_hc4.contract(hc4, *m_x[i]);

    Tube x_temp(0, *m_x[i]);
    Tube xdot_temp(3, *m_x[i]);
    x_temp.ctc_deriv(xdot_temp);
    m_x[i]->put(0, x_temp);

    Tube y_temp(1, *m_x[i]);
    Tube ydot_temp(4, *m_x[i]);
    y_temp.ctc_deriv(ydot_temp);
    m_x[i]->put(1, y_temp);

    //Tube z_temp(2, *m_x[i]);
    //Tube zdot_temp(5, *m_x[i]);
    //z_temp.ctc_deriv(zdot_temp);
    //m_x[i]->put(2, z_temp);

    Tube g_temp(9, *m_x[i]);
    Tube gdot_temp(10, *m_x[i]);
    g_temp.ctc_deriv(gdot_temp);
    m_x[i]->put(9, g_temp);

    TubeVector temp(*m_x[i]);
    temp.resize(6);
    *x &= temp;
    fig.show();
  }

  for(int i = 0 ; i < m_seamarks.size() ; i++)
  {
    delete m_x[i];
  }

  vibes::endDrawing();

  delete x;
  delete x_truth;
  return EXIT_SUCCESS;
}