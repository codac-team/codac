/* ============================================================================
 *  tubex-lib - DataLoader_Lissajous class
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
#include "tubex_DataLoader_Lissajous.h"
#include "tubex_Function.h"
#include "tubex_Exception.h"
#include "tubex_Tube.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  DataLoader_Lissajous::DataLoader_Lissajous()
    : DataLoader()
  {

  }

  void DataLoader_Lissajous::load_data(TubeVector *&x, TrajectoryVector *&truth, const Interval& domain)
  {
    clock_t t_start = clock();
    cout << "Loading data... " << flush;

    Interval t_domain = domain;
    if(t_domain.is_unbounded())
      t_domain = Interval(0., 6.);

    Function f_lissajous("(120.*2.*cos(t) - 240. ; 120.*sin(2.*t) ; -120.*2.*sin(t) ; 240.*cos(2.*t))");
    truth = new TrajectoryVector(t_domain, f_lissajous);

    //x = new TubeVector(t_domain, 0.01, f_lissajous);
    x = new TubeVector(*truth, 0.01);
    (*x)[2].inflate(0.1);
    (*x)[3].inflate(0.1);
    (*x)[0] = (*x)[2].primitive();
    (*x)[1] = (*x)[3].primitive();

    printf(" %.2fs\n", (double)(clock() - t_start)/CLOCKS_PER_SEC);
  }

  vector<Beacon> DataLoader_Lissajous::get_beacons() const
  {
    vector<Beacon> v_beacons;

    return v_beacons;
  }
  
  vector<IntervalVector> DataLoader_Lissajous::get_observations() const
  {
    vector<IntervalVector> v_obs;
    
    return v_obs;
  }
}