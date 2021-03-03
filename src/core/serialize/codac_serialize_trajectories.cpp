/** 
 *  Serialization tools (trajectories)
 * ----------------------------------------------------------------------------
 *  \date       2016
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac_Vector.h"
#include "codac_Exception.h"
#include "codac_serialize_trajectories.h"

using namespace std;
using namespace ibex;

namespace codac
{
  void serialize_Trajectory(ofstream& bin_file, const Trajectory& traj, int version_number)
  {
    if(!bin_file.is_open())
      throw Exception(__func__, "ofstream& bin_file not open");

    if(traj.definition_type() == TrajDefnType::ANALYTIC_FNC)
      throw Exception(__func__, "Fnc serialization not implemented");

    // Version number for compliance purposes
    bin_file.write((const char*)&version_number, sizeof(short int));

    switch(version_number)
    {
      case 1:
        throw Exception(__func__, "serialization version no more supported");
        break;

      case 2:
      {
        // Points number
        int pts_number = traj.sampled_map().size();
        bin_file.write((const char*)&pts_number, sizeof(int));

        typename map<double,double>::const_iterator it_map;
        for(it_map = traj.sampled_map().begin() ; it_map != traj.sampled_map().end() ; it_map++)
        {
          bin_file.write((const char*)&it_map->first, sizeof(double));
          bin_file.write((const char*)&it_map->second, sizeof(double));
        }

        break;
      }

      default:
        throw Exception(__func__, "unhandled case");
    }
  }

  void deserialize_Trajectory(ifstream& bin_file, Trajectory *&traj)
  {
    if(!bin_file.is_open())
      throw Exception(__func__, "ifstream& bin_file not open");

    // Version number for compliance purposes
    short int version_number;
    bin_file.read((char*)&version_number, sizeof(short int));
    
    switch(version_number)
    {
      case 1:
        throw Exception(__func__, "serialization version no more supported");
        break;

      case 2:
      {
        traj = new Trajectory();

        // Points number
        int pts_number;
        bin_file.read((char*)&pts_number, sizeof(int));

        for(int i = 0 ; i < pts_number ; i++)
        {
          double t, y;
          bin_file.read((char*)&t, sizeof(double));
          bin_file.read((char*)&y, sizeof(double));
          traj->set(y, t);
        }

        break;
      }

      default:
        throw Exception(__func__, "deserialization version number not supported");
    }
  }

  void serialize_TrajectoryVector(ofstream& bin_file, const TrajectoryVector& traj, int version_number)
  {
    if(!bin_file.is_open())
      throw Exception(__func__, "ofstream& bin_file not open");

    short int size = traj.size();
    bin_file.write((const char*)&size, sizeof(short int));
    for(int i = 0 ; i < size ; i++)
      serialize_Trajectory(bin_file, traj[i], version_number);
  }

  void deserialize_TrajectoryVector(ifstream& bin_file, TrajectoryVector *&traj)
  {
    if(!bin_file.is_open())
      throw Exception(__func__, "ifstream& bin_file not open");

    traj = new TrajectoryVector();

    short int size;
    bin_file.read((char*)&size, sizeof(short int));
    traj->m_n = size;
    traj->m_v_trajs = new Trajectory[size];
    
    for(int i = 0 ; i < size ; i++)
    {
      Trajectory *ptr;
      deserialize_Trajectory(bin_file, ptr);
      (*traj)[i] = *ptr;
      delete ptr;
    }
  }
}