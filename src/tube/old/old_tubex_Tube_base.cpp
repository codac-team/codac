/* ============================================================================
 *  tubex-lib - Tube class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_Tube.h"
#include "exceptions/tubex_Exception.h"
#include "exceptions/tubex_DomainException.h"
#include "exceptions/tubex_EmptyException.h"
#include "ibex.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  Tube::Tube(const Interval& domain_, double timestep, const Interval& default_value)
  {
    createFromSpecifications(domain_, timestep, default_value);
  }

  void Tube::createTube(const vector<Interval>& vector_dt, const Interval& default_value)
  {
    setName(""); // an automatic name will be given
    createFromSlicesVector(vector_dt, default_value);

    m_v_slices.clear();
    //void Tube::getSlices(std::vector<Subtube*>& v_slices) const
    // todo: procedure to read the tree
  }

  void Tube::createFromSpecifications(const Interval& domain_, double timestep, const Interval& default_value)
  {
    if(timestep > 0.)
    {
      double lb, ub = domain_.lb();
      vector<Interval> vector_dt; // a vector of slices is created only once
      do
      {
        lb = ub; // we guarantee all slices are adjacent
        ub = lb + timestep;
        vector_dt.push_back(Interval(lb, ub));
      } while(ub < domain_.ub());

      createTube(vector_dt, default_value);
    }

    else
      throw Exception("Tube constructor", "wrong timestep");
  }

  Tube::Tube(const Interval& domain_, double timestep, const Function& fmin, const Function& fmax)
  {
    if(fmin.nb_var() > 1 || fmax.nb_var() > 1)
      throw Exception("Tube constructor", "too many variables in function definition");

    createFromSpecifications(domain_, timestep);
    for(int i = 0 ; i < size() ; i++)
    {
      IntervalVector dom(1);
      dom[0] = domain(i);
      set(Interval(fmin.eval_vector(dom)[0].lb(), fmax.eval_vector(dom)[0].ub()), i);
    }
  }

  Tube::Tube(const Interval& domain_, double timestep, const Function& f, const Interval& thickness)
  {
    if(f.nb_var() > 1)
      throw Exception("Tube constructor", "too many variables in function definition");

    Interval tube_thickness = thickness == Interval::EMPTY_SET ? 0. : thickness; // default value: [0.]
    createFromSpecifications(domain_, timestep);
    for(int i = 0 ; i < size() ; i++)
    {
      IntervalVector dom(1);
      dom[0] = domain(i);
      set(Interval(f.eval_vector(dom)[0]) + tube_thickness, i);
    }
  }

  const Subtube* Tube::getSlice(int index) const
  {
    vector<Subtube*> v_slices;
    getSlices(v_slices);

    checkDomain(*this, index);
    return m_v_slices[index];
  }

  Subtube* Tube::getSlice(int index)
  {
    return const_cast<Subtube*>(getSlice(index));
  }

  Interval Tube::domain(double t) const
  {
    int index = input2index(t);
    Interval intv_t = getSlice(index)->domain();
    if(t == intv_t.ub() && index < size() - 1) // on the boundary, between two slices
      return getSlice(index + 1)->domain() | intv_t;
    return intv_t;
  }

  IntervalVector Tube::sliceBox(int index) const
  {
    const Subtube *slice = getSlice(index);
    IntervalVector slice_box(2);
    slice_box[0] = slice->domain();
    slice_box[1] = slice->image();
    return slice_box;
  }


  const Interval Tube::feed(const Interval& intv_y, int index)
  {
    Interval new_y = (*this)[index] | intv_y;
    set(new_y, index);
    return new_y;
  }

  const Interval Tube::feed(const Interval& intv_y, double t)
  {
    int index = input2index(t);
    Interval previous_y = (*this)[index];
    feed(intv_y, index);
    if(domain(index).lb() == t && index > 0)
      feed(intv_y, index - 1);
    return (*this)[t];
  }

  void Tube::feed(const map<double,Interval>& map_values)
  {
    double ta, tb;
    typename map<double,Interval>::const_iterator it_map;
    it_map = map_values.begin();
    tb = it_map->first;
    it_map++;

    for( ; it_map != map_values.end() ; it_map++)
    {
      ta = tb;
      tb = it_map->first;

      if(ta < domain().lb())
        continue;

      if(tb > domain().ub())
        break;

      for(int i = input2index(ta) ; i <= input2index(tb) ; i++)
      {
        Interval slice_t = domain(i) & Interval(ta,tb);

        if(!slice_t.is_empty())
        {
          double ratio;
          Interval slice_y = Interval::EMPTY_SET;

          ratio = (slice_t.lb() - ta) / (tb - ta);
          slice_y |= (1 - ratio) * map_values.at(ta) + ratio * map_values.at(tb);

          ratio = (slice_t.ub() - ta) / (tb - ta);
          slice_y |= (1 - ratio) * map_values.at(ta) + ratio * map_values.at(tb);

          feed(slice_y, i);
        }
      }
    }
  }

  void Tube::feed(const map<double,double>& map_values, const map<double,double>& map_stddev)
  {
    map<double,Interval> new_map;
    typename map<double,double>::const_iterator it_map;
    for(it_map = map_values.begin() ; it_map != map_values.end() ; it_map++)
    {
      if(std::isnan(it_map->second))
        new_map[it_map->first] = Interval::ALL_REALS;
      else
      {
        double sigma = map_stddev.at(it_map->first);
        new_map[it_map->first] = Interval(it_map->second - 2*sigma, it_map->second + 2*sigma);
      }
    }
    feed(new_map);
  }

  void Tube::feed(const map<double,double>& map_values, const Interval& intv_uncertainty)
  {
    map<double,Interval> new_map;
    typename map<double,double>::const_iterator it_map;
    for(it_map = map_values.begin() ; it_map != map_values.end() ; it_map++)
    {
      if(std::isnan(it_map->second))
        new_map[it_map->first] = Interval::ALL_REALS;
      else
        new_map[it_map->first] = it_map->second + intv_uncertainty;
    }
    feed(new_map);
  }

  Subtube& Tube::inflate(double rad)
  {
    for(int i = 0 ; i < size() ; i++)
    {
      Interval old_slice = (*this)[i];
      set(old_slice.inflate(rad), i);
    }
    return *this;
  }

  pair<Tube,Tube> Tube::bisect(const Tube& derivative, double t, float ratio)
  {
    Tube x1 = *this, x2 = *this;
    IntervalVector x_obs(1, (*this)[t]);

    LargestFirst bisector(0., ratio);
    pair<IntervalVector,IntervalVector> p_xobs = bisector.bisect(x_obs);
    x1.ctcEval(derivative, t, p_xobs.first[0]);
    x2.ctcEval(derivative, t, p_xobs.second[0]);

    return make_pair(x1, x2);
  }

  Tube Tube::primitive(const Interval& initial_value) const
  {
    Tube primitive(*this, Interval::ALL_REALS);
    primitive.ctcFwd(*this, initial_value);
    return primitive;
  }


  int Tube::nb_tubes = 0;
  vector<string> Tube::v_tubes_names;

  void Tube::setName(const string& name)
  {
    if(name == "")
    {
      Tube::nb_tubes ++;
      ostringstream o;
      o << "x" << std::hex << Tube::nb_tubes;
      m_name = o.str();
    }

    else
    {
      for(int i = 0 ; i < Tube::v_tubes_names.size() ; i++)
        if(Tube::v_tubes_names[i] == name)
          cout << "Tube::setName(): warning, tube \"" << name << "\" already exists" << endl;
      m_name = name;
    }
    
    Tube::v_tubes_names.push_back(m_name);
  }

  const string& Tube::name() const
  {
    return m_name;
  }



  void Subtube::sample(int index)
  {
    // todo

    /*if(isSlice())
    {
      LargestFirst bisector(0., 0.5);
      IntervalVector slice_domain(1, domain());
      pair<IntervalVector,IntervalVector> p_domain = bisector.bisect(slice_domain);
      vector<Interval> vector_dt;
      vector_dt.push_back(p_domain.first[0]);
      vector_dt.push_back(p_domain.second[0]);
      createFromSlicesVector(vector_dt, image());
    }

    else
    {
      if(index < m_first_subtube->size())
        m_first_subtube->sample(index);
      else
        m_second_subtube->sample(index - m_first_subtube->size());

      m_v_slices.clear();

      for(int i = 0 ; i < m_first_subtube->size() ; i++)
        m_v_slices.push_back(m_first_subtube->slices()[i]);
      
      for(int i = 0 ; i < m_second_subtube->size() ; i++)
        m_v_slices.push_back(m_second_subtube->slices()[i]);
    }
    */
  }
}