/** 
 *  VIBesFigTubeVector class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac_VIBesFigTubeVector.h"
#include "codac_Exception.h"

using namespace std;
using namespace ibex;

namespace codac
{
  VIBesFigTubeVector::VIBesFigTubeVector(const std::string& fig_name)
    : Figure(fig_name), m_n(0), m_v_figs(nullptr)
  {

  }

  VIBesFigTubeVector::VIBesFigTubeVector(const std::string& fig_name, int start_index, int end_index)
    : VIBesFigTubeVector(fig_name)
  {
    assert(start_index >= 0 && end_index >= 0);
    assert(start_index <= end_index);

    m_start_index = start_index;
    m_end_index = end_index;
  }

  VIBesFigTubeVector::VIBesFigTubeVector(const std::string& fig_name, const TubeVector *tube, const TrajectoryVector *traj)
    : VIBesFigTubeVector(fig_name)
  {
    assert(tube);
    add_tube(tube, fig_name);
    if(traj)
      add_trajectory(traj, fig_name);
  }

  VIBesFigTubeVector::~VIBesFigTubeVector()
  {
    if(m_v_figs)
      for(int i = 0 ; i < subfigs_number() ; i++)
        delete m_v_figs[i];
    delete[] m_v_figs;
  }

  int VIBesFigTubeVector::size() const
  {
    return m_n;
  }

  int VIBesFigTubeVector::subfigs_number() const
  {
    return (m_v_figs) ? (m_end_index - m_start_index + 1) : -1;
  }

  VIBesFigTube* VIBesFigTubeVector::operator[](int index)
  {
    assert(index >= 0 && index < size());
    return const_cast<VIBesFigTube*>(static_cast<const VIBesFigTubeVector&>(*this).operator[](index));
  }

  const VIBesFigTube* VIBesFigTubeVector::operator[](int index) const
  {
    assert(index >= 0 && index < size());
    return m_v_figs[index];
  }
  
  void VIBesFigTubeVector::set_properties(int x, int y, int width, int height)
  {
    assert(x >= 0 && y >= 0 && width > 0 && height > 0);
    Figure::set_properties(x, y, width, height);
    for(int i = subfigs_number() - 1 ; i >= 0 ; i--) // if subfigures already instantiated, properties are applied
      m_v_figs[i]->set_properties(m_x + (i - m_start_index)*50, m_y + (i - m_start_index)*50, m_width, m_height);
  }

  void VIBesFigTubeVector::show(bool detail_slices)
  {
    for(int i = 0 ; i < subfigs_number() ; i++)
      m_v_figs[i]->show(detail_slices);
  }

  void VIBesFigTubeVector::set_cursor(double t)
  {
    for(int i = 0 ; i < subfigs_number() ; i++)
      m_v_figs[i]->set_cursor(t);
  }

  void VIBesFigTubeVector::show_cursor(bool display)
  {
    for(int i = 0 ; i < subfigs_number() ; i++)
      m_v_figs[i]->show_cursor(display);
  }

  void VIBesFigTubeVector::draw_box(const Interval& tdomain, const IntervalVector& box, const vibes::Params& params)
  {
    if(m_n != 0)
    {
      if(size() != box.size())
        throw Exception(__func__, "box and fig must be of same dimension");
    }
    draw_box(tdomain, box, "", params);
  }

  void VIBesFigTubeVector::draw_box(const Interval& tdomain, const IntervalVector& box, const string& color, const vibes::Params& params)
  {
    if(m_n == 0) // if the figure is still empty
      create_subfigures(box.size());

    if(size() != box.size())
      throw Exception(__func__, "box and fig must be of same dimension");

    for(int i = 0 ; i < subfigs_number() ; i++)
    {
      if(box[i + m_start_index].is_unbounded())
        continue;
      
      IntervalVector proj_box(2); // projected box
      proj_box[0] = tdomain;
      proj_box[1] = box[i + m_start_index];
      m_v_figs[i]->draw_box(proj_box, color, params);
    }
  }

  void VIBesFigTubeVector::add_tube(const TubeVector *tube, const std::string& name, const std::string& color_frgrnd, const std::string& color_bckgrnd)
  {
    assert(tube);

    if(m_n == 0) // if the figure is still empty
      create_subfigures(tube->size());

    assert(size() == tube->size());

    for(int i = 0 ; i < subfigs_number() ; i++)
      m_v_figs[i]->add_tube(&(*tube)[i + m_start_index], name, color_frgrnd, color_bckgrnd);
  }

  void VIBesFigTubeVector::set_tube_name(const TubeVector *tube, const std::string& name)
  {
    assert(tube);
    for(int i = 0 ; i < subfigs_number() ; i++)
      m_v_figs[i]->set_tube_name(&(*tube)[i + m_start_index], name);
  }

  void VIBesFigTubeVector::set_tube_derivative(const TubeVector *tube, const TubeVector *derivative)
  {
    assert(tube && derivative);
    for(int i = 0 ; i < subfigs_number() ; i++)
      m_v_figs[i]->set_tube_derivative(&(*tube)[i + m_start_index], &(*derivative)[i + m_start_index]);
  }

  void VIBesFigTubeVector::set_tube_color(const TubeVector *tube, const std::string& color_frgrnd, const std::string& color_bckgrnd)
  {
    assert(tube);
    for(int i = 0 ; i < subfigs_number() ; i++)
      m_v_figs[i]->set_tube_color(&(*tube)[i + m_start_index], color_frgrnd, color_bckgrnd);
  }

  void VIBesFigTubeVector::set_tube_color(const TubeVector *tube, TubeColorType color_type, const std::string& color)
  {
    assert(tube);
    for(int i = 0 ; i < subfigs_number() ; i++)
      m_v_figs[i]->set_tube_color(&(*tube)[i + m_start_index], color_type, color);
  }

  void VIBesFigTubeVector::reset_tube_background(const TubeVector *tube)
  {
    assert(tube);
    for(int i = 0 ; i < subfigs_number() ; i++)
      m_v_figs[i]->reset_tube_background(&(*tube)[i + m_start_index]);
  }

  void VIBesFigTubeVector::remove_tube(const TubeVector *tube)
  {
    assert(tube);
    for(int i = 0 ; i < subfigs_number() ; i++)
      m_v_figs[i]->remove_tube(&(*tube)[i + m_start_index]);
  }

  void VIBesFigTubeVector::add_trajectory(const TrajectoryVector *traj, const std::string& name, const std::string& color)
  {
    assert(traj);

    if(m_n == 0) // if the figure is still empty
      create_subfigures(traj->size());

    assert(size() == traj->size());

    for(int i = 0 ; i < subfigs_number() ; i++)
      m_v_figs[i]->add_trajectory(&(*traj)[i + m_start_index], name, color);
  }

  void VIBesFigTubeVector::set_trajectory_name(const TrajectoryVector *traj, const std::string& name)
  {
    assert(traj);
    for(int i = 0 ; i < subfigs_number() ; i++)
      m_v_figs[i]->set_trajectory_name(&(*traj)[i + m_start_index], name);
  }

  void VIBesFigTubeVector::set_trajectory_color(const TrajectoryVector *traj, const std::string& color)
  {
    assert(traj);
    for(int i = 0 ; i < subfigs_number() ; i++)
      m_v_figs[i]->set_trajectory_color(&(*traj)[i + m_start_index], color);
  }

  void VIBesFigTubeVector::set_trajectory_points_size(const TrajectoryVector *traj, double points_size)
  {
    assert(traj);
    for(int i = 0 ; i < subfigs_number() ; i++)
      m_v_figs[i]->set_trajectory_points_size(&(*traj)[i + m_start_index], points_size);
  }

  void VIBesFigTubeVector::remove_trajectory(const TrajectoryVector *traj)
  {
    assert(traj);
    for(int i = 0 ; i < subfigs_number() ; i++)
      m_v_figs[i]->remove_trajectory(&(*traj)[i + m_start_index]);
  }

  void VIBesFigTubeVector::create_subfigures(int n)
  {
    assert(n > 0);
    assert(m_v_figs == nullptr && m_n == 0);

    m_n = n;
    m_v_figs = new VIBesFigTube*[m_n];
    if(m_start_index == -1) m_start_index = 0;
    if(m_end_index == -1) m_end_index = n - 1;

    assert(m_start_index <= m_end_index);
    assert(m_start_index >= 0 && m_start_index < size());
    assert(m_end_index >= 0 && m_end_index < size());

    for(int i = m_end_index ; i >= m_start_index ; i--)
    {
      std::ostringstream o;
      o << m_name << " [" << (i + 1) << "/" << m_n << "]";
      m_v_figs[i - m_start_index] = new VIBesFigTube(o.str());
      m_v_figs[i - m_start_index]->set_properties(m_x + (i - m_start_index)*50, m_y + (i - m_start_index)*50, m_width, m_height);
    }
  }
}