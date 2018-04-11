/* ============================================================================
 *  tubex-lib - Tube class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_Tube.h"
#include "tubex_Exception.h"
#include "tubex_DomainException.h"
#include "tubex_TubeSerialization.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  // Definition

  Tube::Tube(const Interval& domain, const Interval& codomain) : TubeTree(domain, codomain)
  {

  }
  
  Tube::Tube(const Interval& domain, double timestep, const Interval& codomain) : TubeTree(domain, codomain)
  {
    if(timestep < 0.)
      throw Exception("Tube constructor", "invalid timestep");

    else if(timestep == 0.)
    {
      // then the tube is defined as one single slice
    }

    else if(timestep < domain.diam())
    {
      double lb, ub = domain.lb();
      vector<double> v_bounds; // a vector of slices is created only once
      do
      {
        lb = ub; // we guarantee all slices are adjacent
        ub = lb + timestep;
        if(ub < domain.ub()) v_bounds.push_back(ub);
      } while(ub < domain.ub());

      sample(v_bounds);
    }
  }

  Tube::Tube(const Tube& x, const Interval& codomain) : TubeTree(x, codomain)
  {

  }

  Tube::Tube(const Trajectory& traj, double thickness, double timestep) : Tube(traj.domain(), timestep, Interval::EMPTY_SET)
  {
    *this |= traj;
    inflate(thickness);
  }

  Tube::Tube(const Trajectory& lb, const Trajectory& ub, double timestep) : Tube(lb.domain(), timestep, Interval::EMPTY_SET)
  {
    *this |= lb;
    *this |= ub;
  }

  Tube::Tube(const string& binary_file_name) : TubeTree(Interval(0.))
  {
    ifstream bin_file(binary_file_name.c_str(), ios::in | ios::binary);

    if(!bin_file.is_open())
      throw Exception("Tube constructor", "error while opening file \"" + binary_file_name + "\"");

    deserializeTube(bin_file, *this);
  }

  // String

  ostream& operator<<(ostream& str, const Tube& x)
  {
    cout << "Tube " << x.domain() << "↦" << x.codomain() << flush;
    return str;
  }

  // Serialization

  void Tube::serialize(const string& binary_file_name, int version_number) const
  {
    ofstream bin_file(binary_file_name.c_str(), ios::out | ios::binary);

    if(!bin_file.is_open())
      throw Exception("Tube::serialize()", "error while writing file \"" + binary_file_name + "\"");

    serializeTube(bin_file, *this, version_number);
  }
}