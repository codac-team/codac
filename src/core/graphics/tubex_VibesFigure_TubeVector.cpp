/* ============================================================================
 *  tubex-lib - VibesFigure_TubeVector class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_VibesFigure_TubeVector.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  VibesFigure_TubeVector::VibesFigure_TubeVector(const std::string& fig_name, int n)
    : m_n(n), m_v_figs(new VibesFigure_Tube*[n])
  {
    for(int i = 0 ; i < n ; i++)
    {
      std::ostringstream o;
      o << fig_name << " (dim " << (i + 1) << "/" << n << ")";
      m_v_figs[i] = new VibesFigure_Tube(o.str());
    }
  }

  VibesFigure_TubeVector::VibesFigure_TubeVector(const std::string& fig_name, const TubeVector *tube, const TrajectoryVector *traj)
    : VibesFigure_TubeVector(fig_name, tube->size())
  {
    // todo: check dim tube, traj
  }

  VibesFigure_TubeVector::~VibesFigure_TubeVector()
  {
    for(int i = 0 ; i < m_n ; i++)
      delete m_v_figs[i];
    delete[] m_v_figs;
  }

  int VibesFigure_TubeVector::size() const
  {
    return m_n;
  }
    
  void VibesFigure_TubeVector::set_properties(int x, int y, int width, int height)
  {
    for(int i = 0 ; i < size() ; i++)
      m_v_figs[i]->set_properties(x + i*200, y + i*200, width, height);
  }

  void VibesFigure_TubeVector::add_tube(const TubeVector *tube, const std::string& name, const std::string& color_frgrnd, const std::string& color_bckgrnd)
  {
    // todo: check dim tube
    for(int i = 0 ; i < size() ; i++)
      m_v_figs[i]->add_tube(&(*tube)[i], name, color_frgrnd, color_bckgrnd);
  }

  void VibesFigure_TubeVector::set_tube_name(const TubeVector *tube, const std::string& name)
  {
    for(int i = 0 ; i < size() ; i++)
      m_v_figs[i]->set_tube_name(&(*tube)[i], name);
  }

  void VibesFigure_TubeVector::set_tube_derivative(const TubeVector *tube, const TubeVector *derivative)
  {
    for(int i = 0 ; i < size() ; i++)
      m_v_figs[i]->set_tube_derivative(&(*tube)[i], &(*derivative)[i]);
  }

  void VibesFigure_TubeVector::set_tube_color(const TubeVector *tube, const std::string& color_frgrnd, const std::string& color_bckgrnd)
  {
    for(int i = 0 ; i < size() ; i++)
      m_v_figs[i]->set_tube_color(&(*tube)[i], color_frgrnd, color_bckgrnd);
  }

  void VibesFigure_TubeVector::set_tube_color(const TubeVector *tube, int color_type, const std::string& color)
  {
    for(int i = 0 ; i < size() ; i++)
      m_v_figs[i]->set_tube_color(&(*tube)[i], color_type, color);
  }

  void VibesFigure_TubeVector::remove_tube(const TubeVector *tube)
  {
    for(int i = 0 ; i < size() ; i++)
      m_v_figs[i]->remove_tube(&(*tube)[i]);
  }

  void VibesFigure_TubeVector::add_trajectory(const TrajectoryVector *traj, const std::string& name, const std::string& color)
  {
    // todo: check dim traj
    for(int i = 0 ; i < size() ; i++)
      m_v_figs[i]->add_trajectory(&(*traj)[i], name, color);
  }

  void VibesFigure_TubeVector::set_trajectory_name(const TrajectoryVector *traj, const std::string& name)
  {
    for(int i = 0 ; i < size() ; i++)
      m_v_figs[i]->set_trajectory_name(&(*traj)[i], name);
  }

  void VibesFigure_TubeVector::set_trajectory_color(const TrajectoryVector *traj, const std::string& color)
  {
    for(int i = 0 ; i < size() ; i++)
      m_v_figs[i]->set_trajectory_color(&(*traj)[i], color);
  }

  void VibesFigure_TubeVector::remove_trajectory(const TrajectoryVector *traj)
  {
    for(int i = 0 ; i < size() ; i++)
      m_v_figs[i]->remove_trajectory(&(*traj)[i]);
  }

  void VibesFigure_TubeVector::show(bool detail_slices)
  {
    for(int i = 0 ; i < size() ; i++)
      m_v_figs[i]->show(detail_slices);
  }
}