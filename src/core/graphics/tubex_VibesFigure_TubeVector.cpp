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

  VibesFigure_TubeVector::VibesFigure_TubeVector(const std::string& fig_name, const TubeVector *tubevector, const TrajectoryVector *trajvector)
    : VibesFigure_TubeVector(fig_name, tubevector->size())
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

  void VibesFigure_TubeVector::add_tubevector(const TubeVector *tubevector, const std::string& name, const std::string& color_frgrnd, const std::string& color_bckgrnd)
  {
    // todo: check dim tube
    for(int i = 0 ; i < size() ; i++)
      m_v_figs[i]->add_tube(&(*tubevector)[i], name, color_frgrnd, color_bckgrnd);
  }

  void VibesFigure_TubeVector::set_tubevector_name(const TubeVector *tubevector, const std::string& name)
  {
    for(int i = 0 ; i < size() ; i++)
      m_v_figs[i]->set_tube_name(&(*tubevector)[i], name);
  }

  void VibesFigure_TubeVector::set_tubevector_derivative(const TubeVector *tubevector, const TubeVector *derivative)
  {
    for(int i = 0 ; i < size() ; i++)
      m_v_figs[i]->set_tube_derivative(&(*tubevector)[i], &(*derivative)[i]);
  }

  void VibesFigure_TubeVector::set_tubevector_color(const TubeVector *tubevector, const std::string& color_frgrnd, const std::string& color_bckgrnd)
  {
    for(int i = 0 ; i < size() ; i++)
      m_v_figs[i]->set_tube_color(&(*tubevector)[i], color_frgrnd, color_bckgrnd);
  }

  void VibesFigure_TubeVector::set_tubevector_color(const TubeVector *tubevector, int color_type, const std::string& color)
  {
    for(int i = 0 ; i < size() ; i++)
      m_v_figs[i]->set_tube_color(&(*tubevector)[i], color_type, color);
  }

  void VibesFigure_TubeVector::remove_tubevector(const TubeVector *tubevector)
  {
    for(int i = 0 ; i < size() ; i++)
      m_v_figs[i]->remove_tube(&(*tubevector)[i]);
  }

  void VibesFigure_TubeVector::add_trajectoryvector(const TrajectoryVector *trajvector, const std::string& name, const std::string& color)
  {
    // todo: check dim traj
    for(int i = 0 ; i < size() ; i++)
      m_v_figs[i]->add_trajectory(&(*trajvector)[i], name, color);
  }

  void VibesFigure_TubeVector::set_trajectoryvector_name(const TrajectoryVector *trajvector, const std::string& name)
  {
    for(int i = 0 ; i < size() ; i++)
      m_v_figs[i]->set_trajectory_name(&(*trajvector)[i], name);
  }

  void VibesFigure_TubeVector::set_trajectoryvector_color(const TrajectoryVector *trajvector, const std::string& color)
  {
    for(int i = 0 ; i < size() ; i++)
      m_v_figs[i]->set_trajectory_color(&(*trajvector)[i], color);
  }

  void VibesFigure_TubeVector::remove_trajectoryvector(const TrajectoryVector *trajvector)
  {
    for(int i = 0 ; i < size() ; i++)
      m_v_figs[i]->remove_trajectory(&(*trajvector)[i]);
  }

  void VibesFigure_TubeVector::show(bool detail_slices)
  {
    for(int i = 0 ; i < size() ; i++)
      m_v_figs[i]->show(detail_slices);
  }
}