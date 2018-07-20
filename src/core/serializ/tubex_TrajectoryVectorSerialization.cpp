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

  void serialize_trajvector(ofstream& bin_file, const TrajectoryVector& traj, int version_number)
  {
    if(!bin_file.is_open())
      throw Exception("serialize_trajvector()", "ofstream& bin_file not open");

    if(traj.get_function() != NULL)
      throw Exception("serialize_trajvector()", "Fnc serialization not implemented");

    if(traj.get_map().size() == 0)
      throw Exception("serialize_trajvector()", "trajectory not defined");

    // Version number for compliance purposes
    bin_file.write((const char*)&version_number, sizeof(char));

    switch(version_number)
    {
      case 1:
        throw Exception("serialize_trajvector()", "serialization version no more supported");
        break;

      case 2:
      {
        // Dimension
        char dim = traj.dim();
        bin_file.write((const char*)&dim, sizeof(char));

        // Points number
        char pts_number = traj.get_map().size();
        bin_file.write((const char*)&pts_number, sizeof(char));

        typename map<double,Vector>::const_iterator it_map;
        for(it_map = traj.get_map().begin() ; it_map != traj.get_map().end() ; it_map++)
        {
          bin_file.write((const char*)&it_map->first, sizeof(double));
          for(int i = 0 ; i < dim ; i++)
            bin_file.write((const char*)&it_map->second[i], sizeof(double));
        }

        break;
      }

      default:
        throw Exception("serialize_trajvector()", "unhandled case");
    }
  }

  void deserialize_trajvector(ifstream& bin_file, TrajectoryVector& traj)
  {
    if(!bin_file.is_open())
      throw Exception("deserialize_trajvector()", "ifstream& bin_file not open");

    if(traj.get_map().size() != 0)
      throw Exception("deserialize_trajvector()", "trajectory already defined");

    // Version number for compliance purposes
    char version_number;
    bin_file.read((char*)&version_number, sizeof(char));
    
    switch(version_number)
    {
      case 1:
        throw Exception("deserialize_trajvector()", "serialization version no more supported");
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
        throw Exception("deserialize_trajvector()", "deserialization version number not supported");
    }
  }
}