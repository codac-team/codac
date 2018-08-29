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
  VibesFigure_TubeVector::VibesFigure_TubeVector(const std::string& fig_name)
    : m_name(fig_name), m_n(0), m_v_figs(NULL)
  {

  }

  VibesFigure_TubeVector::VibesFigure_TubeVector(const std::string& fig_name, int start_index, int end_index)
    : VibesFigure_TubeVector(fig_name)
  {
    // todo: check index, not neg, start < end, ...
    m_start_index = start_index;
    m_end_index = end_index;
  }

  VibesFigure_TubeVector::VibesFigure_TubeVector(const std::string& fig_name, const TubeVector *tubevector, const TrajectoryVector *trajvector)
    : VibesFigure_TubeVector(fig_name)
  {
    add_tubevector(tubevector, fig_name);
    if(trajvector != NULL)
      add_trajectoryvector(trajvector, fig_name);
  }

  VibesFigure_TubeVector::~VibesFigure_TubeVector()
  {
    if(m_v_figs != NULL)
      for(int i = 0 ; i < subfigs_number() ; i++)
        delete m_v_figs[i];
    delete[] m_v_figs;
  }

  int VibesFigure_TubeVector::size() const
  {
    return m_n;
  }

  int VibesFigure_TubeVector::subfigs_number() const
  {
    return (m_v_figs != NULL) ? (m_end_index - m_start_index + 1) : -1;
  }
    
  void VibesFigure_TubeVector::set_properties(int x, int y, int width, int height)
  {
    m_x = x;
    m_y = y;
    m_width = width;
    m_height = height;

    for(int i = subfigs_number() - 1 ; i >= 0 ; i--) // if subfigures already instantiated, properties are applied
      m_v_figs[i]->set_properties(m_x + (i - m_start_index)*50, m_y + (i - m_start_index)*50, m_width, m_height);
  }

  void VibesFigure_TubeVector::add_tubevector(const TubeVector *tubevector, const std::string& name, const std::string& color_frgrnd, const std::string& color_bckgrnd)
  {
    // todo: check dim tube

    if(m_n == 0) // if the figure is still empty
      create_subfigures(tubevector->size());

    for(int i = 0 ; i < subfigs_number() ; i++)
      m_v_figs[i]->add_tube(&(*tubevector)[i + m_start_index], name, color_frgrnd, color_bckgrnd);
  }

  void VibesFigure_TubeVector::set_tubevector_name(const TubeVector *tubevector, const std::string& name)
  {
    for(int i = 0 ; i < subfigs_number() ; i++)
      m_v_figs[i]->set_tube_name(&(*tubevector)[i + m_start_index], name);
  }

  void VibesFigure_TubeVector::set_tubevector_derivative(const TubeVector *tubevector, const TubeVector *derivative)
  {
    for(int i = 0 ; i < subfigs_number() ; i++)
      m_v_figs[i]->set_tube_derivative(&(*tubevector)[i + m_start_index], &(*derivative)[i + m_start_index]);
  }

  void VibesFigure_TubeVector::set_tubevector_color(const TubeVector *tubevector, const std::string& color_frgrnd, const std::string& color_bckgrnd)
  {
    for(int i = 0 ; i < subfigs_number() ; i++)
      m_v_figs[i]->set_tube_color(&(*tubevector)[i + m_start_index], color_frgrnd, color_bckgrnd);
  }

  void VibesFigure_TubeVector::set_tubevector_color(const TubeVector *tubevector, int color_type, const std::string& color)
  {
    for(int i = 0 ; i < subfigs_number() ; i++)
      m_v_figs[i]->set_tube_color(&(*tubevector)[i + m_start_index], color_type, color);
  }

  void VibesFigure_TubeVector::remove_tubevector(const TubeVector *tubevector)
  {
    for(int i = 0 ; i < subfigs_number() ; i++)
      m_v_figs[i]->remove_tube(&(*tubevector)[i + m_start_index]);
  }

  void VibesFigure_TubeVector::add_trajectoryvector(const TrajectoryVector *trajvector, const std::string& name, const std::string& color)
  {
    // todo: check dim traj

    if(m_n == 0) // if the figure is still empty
      create_subfigures(trajvector->size());

    for(int i = 0 ; i < subfigs_number() ; i++)
      m_v_figs[i]->add_trajectory(&(*trajvector)[i + m_start_index], name, color);
  }

  void VibesFigure_TubeVector::set_trajectoryvector_name(const TrajectoryVector *trajvector, const std::string& name)
  {
    for(int i = 0 ; i < subfigs_number() ; i++)
      m_v_figs[i]->set_trajectory_name(&(*trajvector)[i + m_start_index], name);
  }

  void VibesFigure_TubeVector::set_trajectoryvector_color(const TrajectoryVector *trajvector, const std::string& color)
  {
    for(int i = 0 ; i < subfigs_number() ; i++)
      m_v_figs[i]->set_trajectory_color(&(*trajvector)[i + m_start_index], color);
  }

  void VibesFigure_TubeVector::remove_trajectoryvector(const TrajectoryVector *trajvector)
  {
    for(int i = 0 ; i < subfigs_number() ; i++)
      m_v_figs[i]->remove_trajectory(&(*trajvector)[i + m_start_index]);
  }

  void VibesFigure_TubeVector::show(bool detail_slices)
  {
    for(int i = 0 ; i < subfigs_number() ; i++)
      m_v_figs[i]->show(detail_slices);
  }

  void VibesFigure_TubeVector::draw_box(const IntervalVector& box, const vibes::Params& params)
  {
    draw_box(box, "", params);
  }

  void VibesFigure_TubeVector::draw_box(const IntervalVector& box, const string& color, const vibes::Params& params)
  {
    if(m_n == 0) // if the figure is still empty
      create_subfigures(box.size() - 1);

    // todo: check dim box = dim vector + 1
    for(int i = 1 ; i < box.size() ; i++)
    {
      if(box[i].is_unbounded())
        continue;
      
      IntervalVector proj_box(2);
      proj_box[0] = box[0];
      proj_box[1] = box[i];
      m_v_figs[i-1]->draw_box(proj_box, color, params);
    }
  }

  void VibesFigure_TubeVector::create_subfigures(int size)
  {
    // todo: check dim
    // todo: check the figure is still empty at this point

    m_n = size;
    m_v_figs = new VibesFigure_Tube*[m_n];
    if(m_start_index == -1) m_start_index = 0;
    if(m_end_index == -1) m_end_index = size - 1;

    // todo: check index with respect to n, now defined

    for(int i = m_end_index ; i >= m_start_index ; i--)
    {
      std::ostringstream o;
      o << m_name << " [" << (i + 1) << "/" << m_n << "]";
      m_v_figs[i - m_start_index] = new VibesFigure_Tube(o.str());
      m_v_figs[i - m_start_index]->set_properties(m_x + (i - m_start_index)*50, m_y + (i - m_start_index)*50, m_width, m_height);
    }
  }
}