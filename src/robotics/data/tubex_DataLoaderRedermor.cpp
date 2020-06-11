/** 
 *  DataLoaderRedermor class
 * ----------------------------------------------------------------------------
 *  \date       2016
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <time.h>
#include "tubex_DataLoaderRedermor.h"
#include "tubex_TFunction.h"
#include "tubex_Exception.h"
#include "tubex_Tube.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  DataLoaderRedermor::DataLoaderRedermor(const string& file_path)
    : DataLoader(file_path)
  {

  }

  void DataLoaderRedermor::load_data(TubeVector *&x, TrajectoryVector *&truth, float timestep, const Interval& tdomain)
  {
    assert(tdomain == Interval::ALL_REALS || DynamicalItem::valid_tdomain(tdomain));
    clock_t t_start = clock();
    cout << "Loading data... " << flush;

    if(serialized_data_available())
      deserialize_data(x, truth);
    
    else // loading data from file
    {
      if(!m_datafile->is_open())
        throw Exception("DataLoaderRedermor::load_data", "data file not already open");

      int i = 0;
      string line;
      TrajectoryVector traj_data_x(10), traj_data_dx(10);
      truth = new TrajectoryVector(6);

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
          throw Exception("DataLoaderRedermor::load_data", "fail loading data");

        // Trajectory used for velocities evaluations:
        traj_data_x.set(y, t);
        traj_data_dx.set(dy, t);

        // Trajectory used as ground truth:
        Vector truth_vector(6);
        truth_vector.put(0, y.subvector(8,9)); // position
        truth_vector[2] = y[6]; // depth
        truth_vector.put(3, Vector(3, 0.)); // unknown velocities
        truth->set(truth_vector, t);
      }

      // Data from sensors with uncertainties:
      x = new TubeVector(traj_data_x, timestep); // state vector
      x->inflate(traj_data_dx);
      x->resize(6);

      // Computing robot's velocities:
      TFunction f("phi", "theta", "psi", "vxr", "vyr", "vzr", 
       "(vxr * cos(theta) * cos(psi) \
         - vyr * (cos(phi) * sin(psi) - sin(theta) * cos(psi) * sin(phi)) \
         + vzr * (sin(phi) * sin(psi) + sin(theta) * cos(psi) * cos(phi)) \
         ; \
         vxr * cos(theta) * sin(psi) \
         + vyr * (cos(psi) * cos(phi) + sin(theta) * sin(psi) * sin(phi)) \
         - vzr * (cos(psi) * sin(phi) - sin(theta) * cos(phi) * sin(psi)) ; \
         - vxr * sin(theta) + vyr * cos(theta)*sin(phi) + vzr * cos(theta) * cos(phi))");
      TubeVector velocities = f.eval_vector(*x);

      // Horizontal position
      (*x)[0] = velocities[0].primitive(traj_data_x[8](traj_data_x.tdomain().lb())); // datafile: robot starts at (0.06,0.)
      (*x)[1] = velocities[1].primitive(traj_data_x[9](traj_data_x.tdomain().lb()));

      // Case of the depth, directly sensed:
      (*x)[2].set_empty();
      (*x)[2] |= traj_data_x[6]; // envelope of the trajectory
      (*x)[2].inflate(traj_data_dx[6]);

      // 3d velocities
      (*x)[3] = velocities[0];
      (*x)[4] = velocities[1];
      (*x)[5] = velocities[2];

      serialize_data(*x, *truth);
    }

    printf(" %.2fs\n", (double)(clock() - t_start)/CLOCKS_PER_SEC);
  }

  vector<Beacon> DataLoaderRedermor::get_beacons() const
  {
    vector<Beacon> v_beacons;
    v_beacons.push_back(Beacon(594.0533723, 374.72930350, 19.));
    v_beacons.push_back(Beacon(599.6093723, 484.55286430, 19.));
    v_beacons.push_back(Beacon(601.4613723, 557.35730860, 19.));
    v_beacons.push_back(Beacon(94.01337232, -2.868189501, 19.));
    v_beacons.push_back(Beacon(119.9413723, 84.751556720, 20.));
    v_beacons.push_back(Beacon(127.3493723, 156.32803660, 20.));
    return v_beacons;
  }
  
  map<int,vector<IntervalVector> > DataLoaderRedermor::get_observations() const
  {
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

    // Inflations
    for(map<int,vector<IntervalVector> >::iterator it = m_obs.begin(); it != m_obs.end(); it++)
      for(size_t i = 0 ; i < it->second.size() ; i++)
        it->second[i][1].inflate(8.);

    return m_obs;
  }
}