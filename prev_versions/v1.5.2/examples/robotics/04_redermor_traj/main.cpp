/** 
 *  Codac - Examples
 *  Localization of the Autonomous Underwater Vehicle (AUV) Redermor
 * ----------------------------------------------------------------------------
 *
 *  \date       2016
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <codac.h>
#include <codac-rob.h>

using namespace std;
using namespace codac;

int main(int argc, char** argv)
{
  /* =========== LOADING DATA =========== */

    TubeVector *x;
    TrajectoryVector *x_truth;

    string file_path = "./data/redermor/gesmi.txt";
    if(argc > 1) file_path = argv[1];
    
    DataLoaderRedermor data_loader(file_path);
    data_loader.load_data(x, x_truth);
    vector<Beacon> v_seamarks = data_loader.get_beacons();
    map<int,vector<IntervalVector> > m_obs = data_loader.get_observations();

  /* =========== GRAPHICS =========== */

    vibes::beginDrawing();
    VIBesFigTubeVector fig_x("Redermor's state", 0, 1); // first two components
    fig_x.set_properties(2000, 50, 500, 250);
    fig_x.add_tube(x, "x");
    fig_x.add_trajectory(x_truth, "x*");
    fig_x.show();

    VIBesFigMap fig_map("Map");
    fig_map.set_properties(1450, 50, 500, 500);
    fig_map.add_tube(x, "x", 0, 1);
    fig_map.add_trajectory(x_truth, "x*", 0, 1, "white");
    fig_map.add_beacons(v_seamarks);
    fig_map.show();

  /* =========== CREATING CONTRACTORS =========== */

    const char* xdyn[11];
    xdyn[0] = "x"; xdyn[1] = "y"; xdyn[2] = "z";
    xdyn[3] = "xdot"; xdyn[4] = "ydot"; xdyn[5] = "zdot";
    xdyn[6] = "bx"; xdyn[7] = "by"; xdyn[8] = "bz";
    xdyn[9] = "g"; xdyn[10] = "gdot";
    Function f_obs(11, xdyn, "(sqrt((x-bx)^2+(y-by)^2+(z-bz)^2) - g ; \
                                     (xdot*sign(x-bx)/sqrt(1+(((y-by)^2+(z-bz)^2)/((x-bx)^2)))) \
                                   + (ydot*sign(y-by)/sqrt(1+(((x-bx)^2+(z-bz)^2)/((y-by)^2)))) \
                                   + (zdot*sign(z-bz)/sqrt(1+(((y-by)^2+(x-bx)^2)/((z-bz)^2)))) - gdot)");
    CtcFunction ctc_fwdbwd(f_obs, Vector(2,0.));
    CtcDeriv ctc_deriv;
    CtcEval ctc_eval;
    ctc_eval.enable_time_propag(false); // faster use

  /* =========== STATE ESTIMATION =========== */

    map<int,TubeVector*> m_x;
    for(size_t i = 0 ; i < v_seamarks.size() ; i++)
    {
      m_x[i] = new TubeVector(*x);
      m_x[i]->resize(11);

      (*m_x[i])[6].set(v_seamarks[i].pos_box()[0]);
      (*m_x[i])[7].set(v_seamarks[i].pos_box()[1]);
      (*m_x[i])[8].set(v_seamarks[i].pos_box()[2]);

      for(size_t k = 0 ; k < m_obs[i].size() ; k++)
        ctc_eval.contract(m_obs[i][k][0], m_obs[i][k][1], (*m_x[i])[9], (*m_x[i])[10]);
    }

    for(size_t i = 0 ; i < v_seamarks.size() ; i++)
    {
      cout << "Seamark " << (i+1) << endl;

      // Algebraic constraints
      ctc_fwdbwd.contract(*m_x[i]);

      // Differential constraints
      ctc_deriv.contract((*m_x[i])[0], (*m_x[i])[3]);
      ctc_deriv.contract((*m_x[i])[1], (*m_x[i])[4]);
      ctc_deriv.contract((*m_x[i])[9], (*m_x[i])[10]);

      for(int j = 0 ; j < 6 ; j++)
        (*x)[j] &= (*m_x[i])[j];
    }

    fig_x.show();
    fig_map.show();

  /* =========== ENDING =========== */

    vibes::endDrawing();

    bool success = x->subvector(0,1).contains(x_truth->subvector(0,1)) != BoolInterval::NO;
    
    for(size_t i = 0 ; i < v_seamarks.size() ; i++)
      delete m_x[i];
    delete x;
    delete x_truth;

    return success ? EXIT_SUCCESS : EXIT_FAILURE;
}