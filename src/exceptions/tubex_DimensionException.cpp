/* ============================================================================
 *  tubex-lib - DimensionException class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include <string>
#include <sstream>
#include "tubex_DimensionException.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  DimensionException::DimensionException(int dim)
  {
    ostringstream os;
    os << "invalid dimension: " << dim << endl;
    m_what_msg = os.str();
  }

  DimensionException::DimensionException(const Vector& x1, const Vector& x2)
  {
    ostringstream os;
    os << "objects of different dimension: ";
    os << "x1(" << x1.size() << "), x2(" << x2.size() << ")" << endl;
    m_what_msg = os.str();
  }

  DimensionException::DimensionException(const Trajectory& traj, const IntervalVector& box)
  {
    ostringstream os;
    os << "objects of different dimension: ";
    os << "traj(" << traj.dim() << "), box(" << box.size() << ")" << endl;
    m_what_msg = os.str();
  }

  DimensionException::DimensionException(const Trajectory& traj, const Function& f)
  {
    ostringstream os;
    os << "objects of different dimension: ";
    os << "traj(" << traj.dim() << "), f(" << f.image_dim() << ")" << endl;
    m_what_msg = os.str();
  }

  DimensionException::DimensionException(const Trajectory& traj1, const Trajectory& traj2)
  {
    ostringstream os;
    os << "objects of different dimension: ";
    os << "traj1(" << traj1.dim() << "), traj2(" << traj2.dim() << ")" << endl;
    m_what_msg = os.str();
  }

  DimensionException::DimensionException(const TubeSlice& x, int i)
  {
    ostringstream os;
    os << "wrong dimension access: ";
    os << "slice(" << x.dim() << "), i(" << i << ")" << endl;
    m_what_msg = os.str();
  }

  DimensionException::DimensionException(const TubeSlice& x, const IntervalVector& box)
  {
    ostringstream os;
    os << "objects of different dimension: ";
    os << "slice(" << x.dim() << "), box(" << box.size() << ")" << endl;
    m_what_msg = os.str();
  }

  DimensionException::DimensionException(const TubeSlice& x, const Trajectory& traj)
  {
    ostringstream os;
    os << "objects of different dimension: ";
    os << "slice(" << x.dim() << "), traj(" << traj.dim() << ")" << endl;
    m_what_msg = os.str();
  }

  DimensionException::DimensionException(const TubeSlice& x, const Function& f)
  {
    ostringstream os;
    os << "objects of different dimension: ";
    os << "slice(" << x.dim() << "), f(" << f.image_dim() << ")" << endl;
    m_what_msg = os.str();
  }

  DimensionException::DimensionException(const TubeSlice& x1, const TubeSlice& x2)
  {
    ostringstream os;
    os << "objects of different dimension: ";
    os << "slice1(" << x1.dim() << "), slice2(" << x2.dim() << ")" << endl;
    m_what_msg = os.str();
  }

  DimensionException::DimensionException(const TubeVector& x, const IntervalVector& box)
  {
    ostringstream os;
    os << "objects of different dimension: ";
    os << "tube(" << x.dim() << "), box(" << box.size() << ")" << endl;
    m_what_msg = os.str();
  }

  DimensionException::DimensionException(const TubeVector& x, const Trajectory& traj)
  {
    ostringstream os;
    os << "objects of different dimension: ";
    os << "tube(" << x.dim() << "), traj(" << traj.dim() << ")" << endl;
    m_what_msg = os.str();
  }

  DimensionException::DimensionException(const TubeVector& x, const Function& f)
  {
    ostringstream os;
    os << "objects of different dimension: ";
    os << "tube(" << x.dim() << "), f(" << f.image_dim() << ")" << endl;
    m_what_msg = os.str();
  }

  DimensionException::DimensionException(const TubeVector& x1, const TubeVector& x2)
  {
    ostringstream os;
    os << "objects of different dimension: ";
    os << "tube1(" << x1.dim() << "), tube2(" << x2.dim() << ")" << endl;
    m_what_msg = os.str();
  }

  void DimensionException::check(int dim)
  {
    if(dim < 1)
      throw DimensionException(dim);
  }

  void DimensionException::check(const Vector& x1, const Vector& x2)
  {
    if(x1.size() != x2.size())
      throw DimensionException(x1, x2);
  }

  void DimensionException::check(const Trajectory& traj, const Function& f)
  {
    if(traj.dim() != f.image_dim())
      throw DimensionException(traj, f);
  }

  void DimensionException::check(const Trajectory& traj, const IntervalVector& box)
  {
    if(traj.dim() != box.size())
      throw DimensionException(traj, box);
  }

  void DimensionException::check(const Trajectory& traj1, const Trajectory& traj2)
  {
    if(traj1.dim() != traj2.dim())
      throw DimensionException(traj1, traj2);
  }

  void DimensionException::check(const TubeSlice& x, int i)
  {
    if(i < 0 || i >= x.dim())
      throw DimensionException(x, i);
  }

  void DimensionException::check(const TubeSlice& x, const IntervalVector& box)
  {
    if(x.dim() != box.size())
      throw DimensionException(x, box);
  }

  void DimensionException::check(const TubeSlice& x, const Trajectory& traj)
  {
    if(x.dim() != traj.dim())
      throw DimensionException(x, traj);
  }

  void DimensionException::check(const TubeSlice& x, const Function& f)
  {
    if(x.dim() != f.image_dim())
      throw DimensionException(x, f);
  }

  void DimensionException::check(const TubeSlice& x1, const TubeSlice& x2)
  {
    if(x1.dim() != x2.dim())
      throw DimensionException(x1, x2);
  }

  void DimensionException::check(const TubeVector& x, const IntervalVector& box)
  {
    if(x.dim() != box.size())
      throw DimensionException(x, box);
  }

  void DimensionException::check(const TubeVector& x, const Trajectory& traj)
  {
    if(x.dim() != traj.dim())
      throw DimensionException(x, traj);
  }

  void DimensionException::check(const TubeVector& x, const Function& f)
  {
    if(x.dim() != f.image_dim())
      throw DimensionException(x, f);
  }

  void DimensionException::check(const TubeVector& x1, const TubeVector& x2)
  {
    if(x1.dim() != x2.dim())
      throw DimensionException(x1, x2);
  }
}