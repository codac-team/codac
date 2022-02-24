/** 
 *  DataLoaderLissajous class
 * ----------------------------------------------------------------------------
 *  \date       2016
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <time.h>
#include "codac_DataLoaderLissajous.h"
#include "codac_TFunction.h"
#include "codac_Tube.h"

using namespace std;
using namespace ibex;

namespace codac
{
  DataLoaderLissajous::DataLoaderLissajous()
    : DataLoader()
  {

  }

  void DataLoaderLissajous::load_data(TubeVector *&x, TubeVector *&xdot, TrajectoryVector *&truth, float timestep, const Interval& tdomain)
  {
    assert(DynamicalItem::valid_tdomain(tdomain));

    clock_t t_start = clock();
    cout << "Loading data... " << flush;

    TFunction f_lissajous("(120.*2.*cos(t) ; \
                               120.*sin(2.*t) ; \
                               atan2(240.*cos(2.*t),-120.*2.*sin(t)))");
    TFunction f_lissajous_dot("(-120.*2.*sin(t) ; \
                                   240.*cos(2.*t))");
    // todo: derivative for heading

    truth = new TrajectoryVector(tdomain, f_lissajous);
    x = new TubeVector(tdomain, 0.01, f_lissajous);
    xdot = new TubeVector(tdomain, 0.01, f_lissajous_dot);

    xdot->inflate(0.1); // uncertainties on velocities

    printf(" %.2fs\n", (double)(clock() - t_start)/CLOCKS_PER_SEC);
  }

  vector<Beacon> DataLoaderLissajous::get_beacons(const IntervalVector& map_box, int nb_beacons) const
  {
    assert(map_box.size() == 2);
    assert(nb_beacons >= 0);

    srand(time(nullptr));
    vector<Beacon> v_beacons;
    for(int i = 0 ; i < nb_beacons ; i++)
    {
      double x = map_box[0].lb() + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * map_box[0].diam();
      double y = map_box[1].lb() + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * map_box[1].diam();
      v_beacons.push_back(Beacon(x, y));
    }

    return v_beacons;
  }
  
  vector<IntervalVector> DataLoaderLissajous::get_observations(const TrajectoryVector& x, const vector<Beacon>& map, int nb_obs, const Interval& visi_range, const Interval& visi_angle, const Interval& tdomain) const
  {
    assert(x.size() >= 2);
    assert(nb_obs >= 0);
    assert(map.size() > 0 || nb_obs == 0); // if no observation, no beacon needed

    float dt = 0.001;
    vector<IntervalVector> v_obs;

    if(nb_obs == 0)
      return v_obs;

    Interval tdomain_ = x.tdomain() & tdomain;
    for(double t = tdomain_.lb() ; t < tdomain_.ub()-dt ; t+= tdomain_.diam() / nb_obs)
    {
      for(size_t i = 0 ; i < map.size() ; i++)
      {
        Interval t_(t);
        Interval r = sqrt(pow(x[0](t_) - map[i].pos()[0], 2) + pow(x[1](t_) - map[i].pos()[1], 2));
        Interval heading = atan2(x[1](t+dt) - x[1](t_), x[0](t+dt) - x[0](t_));
        Interval a = atan2(map[i].pos()[1] - x[1](t_), map[i].pos()[0] - x[0](t_)) - heading;
        
        if(visi_range.intersects(r) && visi_angle.intersects(a)) // if the beacon is seen by the robot
        {
          IntervalVector obs(3);
          obs[0] = t; obs[1] = r; obs[2] = a;
          v_obs.push_back(obs);
        }
      }
    }
    
    return v_obs;
  }
}