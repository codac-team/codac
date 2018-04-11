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
#include "tubex_TrajectorySerialization.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  // Public methods

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
      vector<Trajectory> v_trajs;
      deserialize(binary_file_name, v_trajs);
    }
    
    Tube::Tube(const string& binary_file_name, Trajectory& traj) : TubeTree(Interval(0.))
    {
      vector<Trajectory> v_trajs;
      deserialize(binary_file_name, v_trajs);

      if(v_trajs.size() == 0)
        throw Exception("Tube constructor", "unable to deserialize a Trajectory");

      traj = v_trajs[0];
    }

    Tube::Tube(const string& binary_file_name, vector<Trajectory>& v_trajs) : TubeTree(Interval(0.))
    {
      deserialize(binary_file_name, v_trajs);
      if(v_trajs.size() == 0)
        throw Exception("Tube constructor", "unable to deserialize a Trajectory");
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
      vector<Trajectory> v_trajs;
      serialize(binary_file_name, v_trajs, version_number);
    }

    void Tube::serialize(const string& binary_file_name, const Trajectory& traj, int version_number) const
    {
      vector<Trajectory> v_trajs;
      v_trajs.push_back(traj);
      serialize(binary_file_name, v_trajs, version_number);
    }
    
    void Tube::serialize(const string& binary_file_name, const vector<Trajectory>& v_trajs, int version_number) const
    {
      ofstream bin_file(binary_file_name.c_str(), ios::out | ios::binary);

      if(!bin_file.is_open())
        throw Exception("Tube::serialize()", "error while writing file \"" + binary_file_name + "\"");

      serializeTube(bin_file, *this, version_number);

      int nb_trajs = v_trajs.size();
      bin_file.write((const char*)&nb_trajs, sizeof(int));
      for(int i = 0 ; i < v_trajs.size() ; i++)
        serializeTrajectory(bin_file, v_trajs[i], version_number);

      bin_file.close();
    }

  // Protected methods

    // Serialization

    void Tube::deserialize(const string& binary_file_name, vector<Trajectory>& v_trajs)
    {
      ifstream bin_file(binary_file_name.c_str(), ios::in | ios::binary);

      if(!bin_file.is_open())
        throw Exception("Tube::deserialize()", "error while opening file \"" + binary_file_name + "\"");

      deserializeTube(bin_file, *this);

      int nb_trajs;
      bin_file.read((char*)&nb_trajs, sizeof(int));
      for(int i = 0 ; i < nb_trajs ; i++)
      {
        Trajectory traj;
        deserializeTrajectory(bin_file, traj);
        v_trajs.push_back(traj);
      }

      bin_file.close();
    }
}