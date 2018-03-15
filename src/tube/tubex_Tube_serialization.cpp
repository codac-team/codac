/* ============================================================================
 *  tubex-lib - Tube class (serialization methods)
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2016
 * ---------------------------------------------------------------------------- */

#include "tubex_Serialization.h"
#include "tubex_Tube.h"
#include "exceptions/tubex_Exception.h"
#include <iomanip> // for setprecision()
#ifdef _OPENMP
  #include <omp.h> // for multithreading
#endif

using namespace std;
using namespace ibex;

namespace tubex
{
  // Public methods

    bool Tube::serialize(const string& binary_file_name, int version_number) const
    {

    }

    bool Tube::serialize(const string& binary_file_name, const Trajectory& traj, int version_number) const
    {

    }
    
    bool Tube::serialize(const string& binary_file_name, const vector<const Trajectory&>& v_trajs, int version_number) const
    {

    }
    
    void Tube::deserialize(const string& binary_file_name, Trajectory& traj)
    {

    }
    
    void Tube::deserialize(const string& binary_file_name, vector<Trajectory>& v_trajs)
    {

    }
}