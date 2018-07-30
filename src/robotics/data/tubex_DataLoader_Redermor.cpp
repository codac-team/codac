/* ============================================================================
 *  tubex-lib - DataLoader_Redermor class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2018
 * ---------------------------------------------------------------------------- */

#include <time.h>
#include "tubex_Exception.h"
#include "tubex_DataLoader_Redermor.h"
#include "tubex_Function.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  DataLoader_Redermor::DataLoader_Redermor(const string& file_path)
    : DataLoader(file_path)
  {

  }

  void DataLoader_Redermor::load_data(TubeVector *&x, TrajectoryVector *&truth, const Interval& domain)
  {
    clock_t t_start = clock();

    // todo: truncate domain
    if(!m_datafile->is_open())
      throw Exception("DataLoader_Redermor::load_data", "data file not already opened");

    int i = 0;
    string line;
    TrajectoryVector traj_data_x, traj_data_dx;
    truth = new TrajectoryVector();

    while(getline(*m_datafile, line))
    {
      i ++;
      if(i < 46) continue; // accessing data
      if(i == 60000) break; // end of data

      double t;
      Vector y(10), dy(10);

      istringstream iss(line);
      if(!(iss >> t
               >> y[0] >> dy[0]   // phi
               >> y[1] >> dy[1]   // theta
               >> y[2] >> dy[2]   // psi
               >> y[3] >> dy[3]   // vx
               >> y[4] >> dy[4]   // vy
               >> y[5] >> dy[5]   // vz
               >> y[6] >> dy[6]   // depth
               >> y[7] >> dy[7]   // alt
               >> y[8] >> dy[8]   // x
               >> y[9] >> dy[9])) // y
        throw Exception("DataLoader_Redermor::load_data", "fail loading data");

      traj_data_x.set(t, y.subvector(0,5));
      traj_data_dx.set(t, dy.subvector(0,5));
      truth->set(t, y.subvector(8,9));
    }

    // Data from sensors with uncertainties
    TubeVector data_x(traj_data_x, 1.);
    data_x.inflate(traj_data_dx);

    // Computing robot's velocities
    tubex::Function f("phi", "theta", "psi", "vxr", "vyr", "vzr", 
                      "(vxr * cos(theta) * cos(psi) \
                        - vyr * (cos(phi) * sin(psi) - sin(theta) * cos(psi) * sin(phi)) \
                        + vzr * (sin(phi) * sin(psi) + sin(theta) * cos(psi) * cos(phi)) ; \
                        vxr * cos(theta) * sin(psi) \
                        + vyr * (cos(psi) * cos(phi) + sin(theta) * sin(psi) * sin(phi)) \
                        - vzr * (cos(psi) * sin(phi) - sin(theta) * cos(phi) * sin(psi)))");

    x = new TubeVector(data_x);
    x->resize(4);
    x->put(0, f.eval(data_x).primitive());
    x->put(2, f.eval(data_x));
  }
}