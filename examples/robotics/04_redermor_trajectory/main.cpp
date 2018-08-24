#include "ibex.h"
#include "tubex.h"
#include "tubex-robotics.h"

using namespace std;
using namespace ibex;
using namespace tubex;

int main()
{
  /* =========== LOADING DATA =========== */

    TubeVector *x;
    TrajectoryVector *x_truth;
    DataLoader_Redermor data_loader("./data/redermor/gesmi.txt");
    data_loader.load_data(x, x_truth);
    vector<Beacon> v_seamarks = data_loader.get_beacons();
    map<int,vector<IntervalVector> > m_obs = data_loader.get_observations();

  /* =========== GRAPHICS =========== */

    vibes::beginDrawing();
    VibesFigure_TubeVector fig_x("Redermor's state", 3, 4); // first two components
    fig_x.set_properties(2000, 50, 500, 250);
    fig_x.add_tubevector(x, "x");
    fig_x.add_trajectoryvector(x_truth, "x*");
    fig_x.show();

    VibesFigure_Map fig_map("Redermor map");
    fig_map.set_properties(1450, 50, 500, 500);
    fig_map.add_tube(x, "x", 0, 1);
    fig_map.add_trajectory(x_truth, "x*", 0, 1);
    fig_map.add_beacons(v_seamarks);
    fig_map.show();

  /* =========== STATE ESTIMATION =========== */

    for(int i = 0 ; i < v_seamarks.size() ; i++)
      for(int k = 0 ; k < m_obs[i].size() ; k++)
        x->sample(m_obs[i][k][0].mid());

    CtcEval ctc_eval(false, false);

    map<int,TubeVector*> m_x;
    for(int i = 0 ; i < v_seamarks.size() ; i++)
    {
      m_x[i] = new TubeVector(*x);
      m_x[i]->resize(11);

      (*m_x[i])[6].set(v_seamarks[i].pos_box()[0]);
      (*m_x[i])[7].set(v_seamarks[i].pos_box()[1]);
      (*m_x[i])[8].set(v_seamarks[i].pos_box()[2]);

      for(int k = 0 ; k < m_obs[i].size() ; k++)
        ctc_eval.contract(m_obs[i][k][0], m_obs[i][k][1], (*m_x[i])[9], (*m_x[i])[10]);
    }

    Variable vt, vx, vy, vz, vxdot, vydot, vzdot, vbx, vby, vbz, vg, vgdot;
    SystemFactory fac;
    fac.add_var(vt);
    fac.add_var(vx); fac.add_var(vy); fac.add_var(vz);
    fac.add_var(vxdot); fac.add_var(vydot); fac.add_var(vzdot);
    fac.add_var(vbx); fac.add_var(vby); fac.add_var(vbz);
    fac.add_var(vg); fac.add_var(vgdot);
    fac.add_ctr(vg = sqrt(sqr(vx-vbx)+sqr(vy-vby)+sqr(vz-vbz)));
    fac.add_ctr(vgdot = (vxdot*sign(vx-vbx)/sqrt(1+((sqr(vy-vby)+sqr(vz-vbz))/sqr(vx-vbx)))) 
                      + (vydot*sign(vy-vby)/sqrt(1+((sqr(vx-vbx)+sqr(vz-vbz))/sqr(vy-vby)))) 
                      + (vzdot*sign(vz-vbz)/sqrt(1+((sqr(vy-vby)+sqr(vx-vbx))/sqr(vz-vbz)))));
    System sys(fac);
    ibex::CtcHC4 hc4(sys);
    tubex::CtcHC4 ctc_hc4;

    for(int i = 0 ; i < v_seamarks.size() ; i++)
    {
      cout << "Seamark " << (i+1) << endl;

      ctc_hc4.contract(hc4, *m_x[i]);

      (*m_x[i])[0].ctc_deriv((*m_x[i])[3]);
      (*m_x[i])[1].ctc_deriv((*m_x[i])[4]);
      (*m_x[i])[9].ctc_deriv((*m_x[i])[10]);

      for(int j = 0 ; j < 6 ; j++)
        (*x)[j] &= (*m_x[i])[j];
    }

    fig_x.show();
    fig_map.show();

  /* =========== ENDING =========== */

    vibes::endDrawing();

    for(int i = 0 ; i < v_seamarks.size() ; i++)
      delete m_x[i];
    delete x;
    delete x_truth;
    return EXIT_SUCCESS;
}