/* ============================================================================
 *  tubex-lib - Serialization tools
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2016
 * ---------------------------------------------------------------------------- */

#include "tubex_TrajectorySerialization.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  /*
    TrajectoryVector binary files structure (VERSION 2)
      - minimal storage
      - note: only map valued trajectories are serializable
      - format: [char_version_number]
                [int_nb_points]
                [double_t_pt1]
                [double_y_pt1]
                [double_t_pt2]
                [double_y_pt2]
                [double_t_pt3]
                ...
  */

  void serializeTrajectory(ofstream& bin_file, const TrajectoryVector& traj, int version_number)
  {
    /*if(!bin_file.is_open())
      throw Exception("serializeTrajectory()", "ofstream& bin_file not open");

    if(traj.getFunction() != NULL)
      throw Exception("serializeTrajectory()", "Function serialization not implemented");

    if(traj.getMap().size() == 0)
      throw Exception("serializeTrajectory()", "trajectory not defined");

    // Version number for compliance purposes
    bin_file.write((const char*)&version_number, sizeof(char));

    switch(version_number)
    {
      case 1:
        throw Exception("serializeTrajectory()", "serialization version no more supported");
        break;

      case 2:
      {
        // Points number
        int pts_number = traj.getMap().size();
        bin_file.write((const char*)&pts_number, sizeof(int));

        typename map<double,double>::const_iterator it_map;
        for(it_map = traj.getMap().begin() ; it_map != traj.getMap().end() ; it_map++)
        {
          bin_file.write((const char*)&it_map->first, sizeof(double));
          bin_file.write((const char*)&it_map->second, sizeof(double));
        }

        break;
      }

      default:
        throw Exception("serializeTrajectory()", "unhandled case");
    }*/
  }

  void deserializeTrajectory(ifstream& bin_file, TrajectoryVector& traj)
  {
    /*if(!bin_file.is_open())
      throw Exception("deserializeTrajectory()", "ifstream& bin_file not open");

    if(traj.getMap().size() != 0)
      throw Exception("deserializeTrajectory()", "trajectory already defined");

    // Version number for compliance purposes
    char version_number;
    bin_file.read((char*)&version_number, sizeof(char));
    
    switch(version_number)
    {
      case 1:
        throw Exception("deserializeTrajectory()", "serialization version no more supported");
        break;

      case 2:
      {
        // Points number
        int pts_number;
        bin_file.read((char*)&pts_number, sizeof(int));

        for(int i = 0 ; i < pts_number ; i++)
        {
          double t, y;
          bin_file.read((char*)&t, sizeof(double));
          bin_file.read((char*)&y, sizeof(double));
          traj.set(t, y);
        }

        break;
      }

      default:
        throw Exception("deserializeTrajectory()", "deserialization version number not supported");
    }*/
  }
}