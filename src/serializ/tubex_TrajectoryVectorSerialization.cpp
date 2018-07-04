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

#include "ibex_Vector.h"
#include "tubex_Exception.h"
#include "tubex_TrajectoryVectorSerialization.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  /*
    TrajectoryVector binary files structure (VERSION 2)
      - minimal storage
      - note: only map valued trajectories are serializable
      - format: [char_version_number]
                [int_dim]
                [int_nb_points]
                [double_t_pt1]
                [double_y_pt1]
                [double_t_pt2]
                [double_y_pt2]
                [double_t_pt3]
                ...
  */

  void serializeTrajectoryVector(ofstream& bin_file, const TrajectoryVector& traj, int version_number)
  {
    if(!bin_file.is_open())
      throw Exception("serializeTrajectoryVector()", "ofstream& bin_file not open");

    if(traj.getFunction() != NULL)
      throw Exception("serializeTrajectoryVector()", "Fnc serialization not implemented");

    if(traj.getMap().size() == 0)
      throw Exception("serializeTrajectoryVector()", "trajectory not defined");

    // Version number for compliance purposes
    bin_file.write((const char*)&version_number, sizeof(char));

    switch(version_number)
    {
      case 1:
        throw Exception("serializeTrajectoryVector()", "serialization version no more supported");
        break;

      case 2:
      {
        // Dimension
        char dim = traj.dim();
        bin_file.write((const char*)&dim, sizeof(char));

        // Points number
        char pts_number = traj.getMap().size();
        bin_file.write((const char*)&pts_number, sizeof(char));

        typename map<double,Vector>::const_iterator it_map;
        for(it_map = traj.getMap().begin() ; it_map != traj.getMap().end() ; it_map++)
        {
          bin_file.write((const char*)&it_map->first, sizeof(double));
          for(int i = 0 ; i < dim ; i++)
            bin_file.write((const char*)&it_map->second[i], sizeof(double));
        }

        break;
      }

      default:
        throw Exception("serializeTrajectoryVector()", "unhandled case");
    }
  }

  void deserializeTrajectoryVector(ifstream& bin_file, TrajectoryVector& traj)
  {
    if(!bin_file.is_open())
      throw Exception("deserializeTrajectoryVector()", "ifstream& bin_file not open");

    if(traj.getMap().size() != 0)
      throw Exception("deserializeTrajectoryVector()", "trajectory already defined");

    // Version number for compliance purposes
    char version_number;
    bin_file.read((char*)&version_number, sizeof(char));
    
    switch(version_number)
    {
      case 1:
        throw Exception("deserializeTrajectoryVector()", "serialization version no more supported");
        break;

      case 2:
      {
        // Dimension
        char dim;
        bin_file.read((char*)&dim, sizeof(char));

        // Points number
        char pts_number;
        bin_file.read((char*)&pts_number, sizeof(char));

        for(int i = 0 ; i < pts_number ; i++)
        {
          double t;
          Vector y(dim);
          bin_file.read((char*)&t, sizeof(double));
          for(int j = 0 ; j < dim ; j++)
            bin_file.read((char*)&y[j], sizeof(double));
          traj.set(t, y);
        }

        break;
      }

      default:
        throw Exception("deserializeTrajectoryVector()", "deserialization version number not supported");
    }
  }
}