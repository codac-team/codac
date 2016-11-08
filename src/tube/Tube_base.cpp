/* ============================================================================
 *  tube-lib - Tube class
 * ============================================================================
 *  Copyright : Copyright 2016 Simon Rohou
 *  License   : This program can be distributed under the terms of
 *              the Apache License, Version 2.0. See the file LICENSE.
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "Tube.h"
#include <iostream>
#include <iomanip> // for setprecision()
#ifdef _OPENMP
  #include <omp.h> // for multithreading
#endif

using namespace std;
using namespace ibex;

Tube::Tube(const Interval &intv_t, double time_step, const Interval& default_value)
{
  if(time_step > 0.)
  {
    double lb, ub = intv_t.lb();
    vector<Interval> vector_dt; // a vector of slices is created only once
    do
    {
      lb = ub; // we guarantee all slices are adjacent
      ub = lb + time_step;
      vector_dt.push_back(Interval(lb, ub));
    } while(ub < intv_t.ub());

    createFromSlicesVector(vector_dt, default_value);
    m_dt = time_step;
  }

  else
    cout << "Error Tube::Tube(...): wrong timestep: " << time_step << endl;
}

Tube::Tube(const vector<Interval>& vector_dt, const Interval& default_value)
{
  createFromSlicesVector(vector_dt, default_value);
}

void Tube::createFromSlicesVector(const vector<Interval>& vector_dt, const Interval& default_value)
{
  m_dt = vector_dt[0].diam(); // all timesteps are identical in the tree
  m_intv_t = Interval(vector_dt[0].lb(), vector_dt[vector_dt.size() - 1].ub());
  m_intv_y = default_value;
  m_slices_number = vector_dt.size();
  m_tree_computation_needed = true;

  if(m_slices_number == 1) // if this is a leaf
  {
    m_first_subtube = NULL;
    m_second_subtube = NULL;
  }

  else
  {
    // In the first subtube: [t0,thalf[
    // In the second subtube: [thalf,tf]
    vector<Interval> first_vector_slices, second_vector_slices;
    int i, k = ceil(m_slices_number / 2.);

    for(i = 0 ; i < k ; i++)
      first_vector_slices.push_back(vector_dt[i]);

    for( ; i < m_slices_number ; i++)
      second_vector_slices.push_back(vector_dt[i]);

    #pragma omp parallel num_threads(omp_get_num_procs())
    {
      #pragma omp sections
      {
        #pragma omp section
          m_first_subtube = new Tube(first_vector_slices, default_value);
        #pragma omp section
          m_second_subtube = new Tube(second_vector_slices, default_value);
      }
    }
  }

  requestFutureTreeComputation();
}

Tube::Tube(const Tube& tu)
{
  m_first_subtube = NULL;
  m_second_subtube = NULL;
  *this = tu;
}

Tube::Tube(const Tube& tu, const Interval& image_value)
{
  *this = tu;
  set(image_value);
}

Tube& Tube::operator=(const Tube& tu)
{
  if(m_first_subtube != NULL)
    delete m_first_subtube;
  if(m_second_subtube != NULL)
    delete m_second_subtube;

  m_dt = tu.dt();
  m_intv_t = tu.domain();
  m_intv_y = tu.image();
  m_slices_number = tu.size();
  m_enclosed_bounds = tu.eval();

  if(tu.isSlice())
  {
    m_first_subtube = NULL;
    m_second_subtube = NULL;
  }

  else
  {
    #pragma omp parallel num_threads(omp_get_num_procs())
    {
      #pragma omp sections
      {
        #pragma omp section
          m_first_subtube = new Tube(*(tu.getFirstSubTube()));
        #pragma omp section
          m_second_subtube = new Tube(*(tu.getSecondSubTube()));
      }
    }
  }

  requestFutureTreeComputation();

  return *this;
}

Tube::~Tube()
{
  delete m_first_subtube;
  delete m_second_subtube;
}

int Tube::size() const
{
  return m_slices_number;
}

double Tube::dt() const
{
  return m_dt;
}

double Tube::volume() const
{
  double volume = 0.;
  for(int i = 0 ; i < m_slices_number ; i++)
  {
    if((*this)[i].is_empty()) // ibex::diam(EMPTY_SET) is not 0
      continue;

    if((*this)[i].is_unbounded())
    {
      volume = INFINITY;
      break;
    }

    volume += m_dt * (*this)[i].diam();
  }

  return volume;
}

double Tube::dist(const Tube& tu) const
{
  return fabs(tu.volume() - volume());
}

bool Tube::isSlice() const
{
  return m_first_subtube == NULL && m_second_subtube == NULL;
}

bool Tube::isEmpty() const
{
  return image().is_empty();
}

bool Tube::isDiscontinuous() const
{
  Interval previous_slice_y = Interval::ALL_REALS;

  for(int i = 0 ; i < size() ; i++)
  {
    Interval this_intv_y = (*this)[i];
    if((previous_slice_y & this_intv_y).is_empty())
      return true;

    previous_slice_y = this_intv_y;
  }

  return false;
}

const Tube* Tube::getSlice(int index) const
{
  if(index < 0 || index >= m_slices_number)
  {
    cout << "Error Tube::getSlice(int): out of range "
         << "for index=" << index << " in [0," << m_slices_number << "[" << endl;
    return NULL;
  }

  else if(isSlice())
    return this;

  else
  {
    int mid_id = ceil(m_slices_number / 2.);

    if(index < mid_id)
      return m_first_subtube->getSlice(index);

    else
      return m_second_subtube->getSlice(index - mid_id);
  }
}

Tube* Tube::getSlice(int index)
{
  return const_cast<Tube*>(static_cast<const Tube*>(this)->getSlice(index));
}

int Tube::input2index(double t) const
{
  if(!m_intv_t.contains(t))
  {
    cout << "Error Tube::input2index(double): no corresponding slice "
         << "for t=" << setprecision(16) << t << " in " << m_intv_t << endl;
    return -1;
  }

  if(t == m_intv_t.ub())
    return m_slices_number - 1;

  if(isSlice())
    return 0;

  if(t < m_first_subtube->domain().ub())
    return m_first_subtube->input2index(t);

  else
    return m_second_subtube->input2index(t) + m_first_subtube->size();
}

double Tube::index2input(int index) const
{
  return getSlice(index)->domain().lb();
}

const Interval& Tube::domain() const
{
  return m_intv_t;
}

const Interval& Tube::domain(int index) const
{
  return getSlice(index)->domain();
}

Interval Tube::domain(double t) const
{
  int index = input2index(t);
  Interval intv_t = getSlice(index)->domain();
  if(t == intv_t.ub() && index < m_slices_number - 1) // on the boundary, between two slices
    return getSlice(index + 1)->domain() | intv_t;
  return intv_t;
}

bool Tube::operator==(const Tube& tu) const
{
  return (isSlice() && tu.isSlice() && image() == tu.image() && domain() == tu.domain())
          || (*getFirstSubTube() == *tu.getFirstSubTube() && *getSecondSubTube() == *tu.getSecondSubTube());
}

const Interval& Tube::operator[](int index) const
{
  // Write access is not allowed for this operator:
  // a further call to computeTree() is needed when values change,
  // this call cannot be garanteed with a direct access to m_intv_y
  // For write access: use set()
  return getSlice(index)->m_intv_y;
}

Interval Tube::operator[](double t) const
{
  // Write access is not allowed for this operator:
  // a further call to computeTree() is needed when values change,
  // this call cannot be garanteed with a direct access to m_intv_y
  // For write access: use set()
  int index = input2index(t);
  Interval intv_t = getSlice(index)->m_intv_t;
  Interval intv_y = (*this)[index];
  if(t == intv_t.lb() && index > 0) // on the boundary, between two slices
    return (*this)[index - 1] & intv_y;
  return intv_y;
}

Interval Tube::operator[](const ibex::Interval& intv_t) const
{
  // Write access is not allowed for this operator:
  // a further call to computeTree() is needed when values change,
  // this call cannot be garanteed with a direct access to m_intv_y
  // For write access: use set()

  if(intv_t.lb() == intv_t.ub())
    return (*this)[intv_t.lb()];

  Interval intersection = m_intv_t & intv_t;

  if(intersection.is_empty())
    return Interval::EMPTY_SET;

  else if(isSlice() || intv_t == m_intv_t || intv_t.is_unbounded() || intv_t.is_superset(m_intv_t))
  {
    if(m_tree_computation_needed)
      computeTree();
    
    return m_intv_y;
  }

  else
  {
    Interval inter_firstsubtube = m_first_subtube->domain() & intersection;
    Interval inter_secondsubtube = m_second_subtube->domain() & intersection;

    if(inter_firstsubtube == inter_secondsubtube)
      return (*m_first_subtube)[inter_firstsubtube.lb()] & (*m_second_subtube)[inter_secondsubtube.lb()];

    else if(inter_firstsubtube.lb() == inter_firstsubtube.ub()
            && inter_secondsubtube.lb() != inter_secondsubtube.ub())
      return (*m_second_subtube)[inter_secondsubtube];

    else if(inter_firstsubtube.lb() != inter_firstsubtube.ub()
            && inter_secondsubtube.lb() == inter_secondsubtube.ub())
      return (*m_first_subtube)[inter_firstsubtube];

    else
      return (*m_first_subtube)[inter_firstsubtube] | (*m_second_subtube)[inter_secondsubtube];
  }
}

const Interval& Tube::image() const
{
  if(m_tree_computation_needed)
    computeTree();
  
  return m_intv_y;
}

void Tube::set(const Interval& intv_y, int index)
{
  getSlice(index)->set(intv_y);
  requestFutureTreeComputation(index);
}

void Tube::set(const Interval& intv_y, double t)
{
  int index = input2index(t);
  set(intv_y, index);
  if(domain(index).lb() == t && index > 0)
    set(intv_y, index - 1);
}

void Tube::set(const Interval& intv_y, const Interval& intv_t)
{
  if(intv_t == ibex::Interval::ALL_REALS ||
     (m_intv_t.intersects(intv_t) && m_intv_t.lb() != intv_t.ub() && m_intv_t.ub() != intv_t.lb()))
  {
    if(isSlice())
      m_intv_y = intv_y;

    else
    {
      m_first_subtube->set(intv_y, intv_t);
      m_second_subtube->set(intv_y, intv_t);
    }

    requestFutureTreeComputation();
  }
}

const Interval Tube::feed(const Interval& intv_y, int index)
{
  set((*this)[index] | intv_y, index);
  return (*this)[index];
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

void Tube::feed(const map<double,Interval>& map_intv_y)
{
  double ta, tb;
  typename map<double,Interval>::const_iterator it_map;
  it_map = map_intv_y.begin();
  tb = it_map->first;
  it_map++;

  for( ; it_map != map_intv_y.end() ; it_map++)
  {
    ta = tb;
    tb = it_map->first;

    for(int i = input2index(ta) ; i <= input2index(tb) ; i++)
    {
      Interval slice_t = domain(i) & Interval(ta,tb);

      if(!slice_t.is_empty())
      {
        double ratio;
        Interval slice_y = Interval::EMPTY_SET;

        ratio = (slice_t.lb() - ta) / (tb - ta);
        slice_y |= (1 - ratio) * map_intv_y.at(ta) + ratio * map_intv_y.at(tb);

        ratio = (slice_t.ub() - ta) / (tb - ta);
        slice_y |= (1 - ratio) * map_intv_y.at(ta) + ratio * map_intv_y.at(tb);

        feed(slice_y, i);
      }
    }
  }
}

void Tube::feed(const map<double,double>& map_y, const Interval& intv_uncertainty)
{
  map<double,Interval> new_map;
  typename map<double,double>::const_iterator it_map;
  for(it_map = map_y.begin() ; it_map != map_y.end() ; it_map++)
    new_map[it_map->first] = it_map->second + intv_uncertainty;
  feed(new_map);
}

void Tube::feed(const map<double,double>& map_y, const Interval& intv_uncertainty, double y_no_uncertainties)
{
  map<double,Interval> new_map;
  typename map<double,double>::const_iterator it_map;
  for(it_map = map_y.begin() ; it_map != map_y.end() ; it_map++)
  {
    Interval intv_y = Interval(it_map->second);
    if(it_map->second != y_no_uncertainties)
      intv_y += intv_uncertainty;
    new_map[it_map->first] = intv_y;
  }
  feed(new_map);
}

const Tube* Tube::getFirstSubTube() const
{
  return m_first_subtube;
}

const Tube* Tube::getSecondSubTube() const
{
  return m_second_subtube;
}

Interval Tube::invert(const Interval& intv_y, const Interval& intv_t) const
{
  if(!domain().intersects(intv_t)) // to apply this function on a tube's portion only
    return Interval::EMPTY_SET;

  else if(!image().intersects(intv_y))
    return Interval::EMPTY_SET;

  else
  {
    if(isSlice())
      return intv_t & m_intv_t;

    else
      return m_first_subtube->invert(intv_y, intv_t) | m_second_subtube->invert(intv_y, intv_t);
  }
}

void Tube::invert(const Interval& intv_y, vector<Interval> &v_intv_t, const Interval& intv_t) const
{
  return invert(intv_y, v_intv_t, intv_t, true);
}

void Tube::invert(const Interval& intv_y, vector<Interval> &v_intv_t, const Interval& intv_t, bool concatenate_results) const
{
  v_intv_t.clear();
  Interval intv_t_ctc = invert(intv_y, intv_t);

  if(!intv_t_ctc.is_empty())
  {
    pair<Interval,Interval> enc_bounds = eval(intv_t_ctc);

    if(!concatenate_results)
    {
      if(enc_bounds.first.ub() > intv_y.ub() || enc_bounds.second.lb() < intv_y.lb())
      {
        // Bisection is needed
        vector<Interval> v1;
        m_first_subtube->invert(intv_y, v1, intv_t, false);
        v_intv_t.insert(v_intv_t.end(), v1.begin(), v1.end());
        vector<Interval> v2;
        m_second_subtube->invert(intv_y, v2, intv_t, false);
        v_intv_t.insert(v_intv_t.end(), v2.begin(), v2.end());
      }

      else
        v_intv_t.push_back(intv_t_ctc);
    }

    else
    {
      vector<Interval> v;
      invert(intv_y, v, intv_t, false);

      // Concatenation (solutions may be adjacent)
      int i = 0;
      while(i < v.size())
      {
        int j = i;
        Interval merge = v[i];

        while(j + 1 < v.size() && v[j].ub() == v[j + 1].lb())
        {
          j ++;
          merge |= v[j];
        }

        v_intv_t.push_back(merge);
        i = j + 1;
      }
    }
  }
}

Tube Tube::subtube(const Interval& intv_t) const
{
  Interval intv_t_inter = intv_t & domain();

  if(intv_t_inter.is_empty())
  {
    cout << "Error Tube::subtube(Interval): wrong domain." << endl;
    return *this;
  }

  int index_lb = input2index(intv_t_inter.lb());
  int index_ub = input2index(intv_t_inter.ub());
  if(domain(index_ub).lb() == intv_t_inter.ub())
    index_ub = max(0, index_ub - 1);

  vector<Interval> vector_dt;
  for(int i = index_lb ; i <= index_ub ; i++)
    vector_dt.push_back(domain(i));

  Tube subtube = Tube(vector_dt);
  #pragma omp parallel num_threads(omp_get_num_procs())
  {
    #pragma omp for
      for(int i = 0 ; i < subtube.size() ; i++)
        subtube.set((*this)[i + index_lb], i);
  }
  
  return subtube;
}

const pair<Interval,Interval> Tube::eval(const Interval& intv_t) const
{  
  if(intv_t.lb() == intv_t.ub())
    return make_pair(Interval((*this)[intv_t.lb()].lb()), Interval((*this)[intv_t.lb()].ub()));

  Interval intersection = m_intv_t & intv_t;

  if(intersection.is_empty())
    return make_pair(Interval::EMPTY_SET, Interval::EMPTY_SET);

  else if(isSlice() || intv_t == m_intv_t || intv_t.is_unbounded() || intv_t.is_superset(m_intv_t))
  {
    if(m_tree_computation_needed)
      computeTree();

    return m_enclosed_bounds; // pre-computed
  }

  else
  {
    Interval inter_firstsubtube = m_first_subtube->domain() & intersection;
    Interval inter_secondsubtube = m_second_subtube->domain() & intersection;

    if(inter_firstsubtube.lb() == inter_firstsubtube.ub() && inter_secondsubtube.lb() == inter_secondsubtube.ub())
      return make_pair((*m_first_subtube)[inter_firstsubtube.lb()] & (*m_second_subtube)[inter_secondsubtube.lb()],
                       (*m_first_subtube)[inter_firstsubtube.ub()] & (*m_second_subtube)[inter_secondsubtube.ub()]);

    else if(inter_firstsubtube.is_empty() || inter_firstsubtube.lb() == inter_firstsubtube.ub())
      return m_second_subtube->eval(inter_secondsubtube);

    else if(inter_secondsubtube.is_empty() || inter_secondsubtube.lb() == inter_secondsubtube.ub())
      return m_first_subtube->eval(inter_firstsubtube);

    else
    {
      pair<Interval,Interval> ui_past = m_first_subtube->eval(inter_firstsubtube);
      pair<Interval,Interval> ui_future = m_second_subtube->eval(inter_secondsubtube);
      return make_pair(ui_past.first | ui_future.first, ui_past.second | ui_future.second);
    }
  }
}

void Tube::getTubeNodes(vector<const Tube*> &v_nodes) const
{
  if(!isSlice())
  {
    m_first_subtube->getTubeNodes(v_nodes);
    m_second_subtube->getTubeNodes(v_nodes);
  }

  v_nodes.push_back(this);
}

void Tube::getTubeNodes(vector<Tube*> &v_nodes)
{
  if(!isSlice())
  {
    m_first_subtube->getTubeNodes(v_nodes);
    m_second_subtube->getTubeNodes(v_nodes);
  }

  v_nodes.push_back(this);
}

Tube& Tube::unionWith(const Tube& x)
{
  if(size() != x.size() || domain() != x.domain())
  {
    for(int i = 0 ; i < size() ; i++)
    {
      Interval this_intv_t = domain(i);
      Interval this_intv_y = (*this)[i];

      Interval x_intv_t = x.domain() & this_intv_t;

      if(!x_intv_t.is_empty())
      {
        Interval x_intv_y = x[x_intv_t];
        set(this_intv_y | x_intv_y, x_intv_t);
      }
    }
  }

  else // fast union
  {
    vector<Tube*> this_nodes;
    vector<const Tube*> x_nodes;
    getTubeNodes(this_nodes);
    x.getTubeNodes(x_nodes);

    #pragma omp parallel num_threads(omp_get_num_procs())
    {
      #pragma omp for
      for(int i = 0 ; i < this_nodes.size() ; i++)
        this_nodes[i]->unionWith_localUpdate(x_nodes[i]);
    }
  }

  requestFutureTreeComputation();
  return *this;
}

void Tube::unionWith_localUpdate(const Tube *x)
{
  if(m_tree_computation_needed)
    computeTree();
  
  m_intv_y |= x->image();
  pair<Interval,Interval> eb1 = eval();
  pair<Interval,Interval> eb2 = x->eval();
  m_enclosed_bounds = make_pair(Interval(min(eb1.first.lb(), eb2.first.lb()), min(eb1.first.ub(), eb2.first.ub())),
                                Interval(max(eb1.second.lb(), eb2.second.lb()), max(eb1.second.ub(), eb2.second.ub())));

  requestFuturePrimitiveComputation();
}

Tube& Tube::intersectWith(const Tube& x)
{
  if(size() != x.size() || domain() != x.domain())
  {
    for(int i = 0 ; i < size() ; i++)
    {
      Interval this_intv_t = domain(i);
      Interval this_intv_y = (*this)[i];

      Interval x_intv_t = x.domain() & this_intv_t;

      if(!x_intv_t.is_empty())
      {
        Interval x_intv_y = x[x_intv_t];
        set(this_intv_y & x_intv_y, x_intv_t);
      }
    }
  }

  else // fast intersection
  {
    vector<Tube*> this_nodes;
    vector<const Tube*> x_nodes;
    getTubeNodes(this_nodes);
    x.getTubeNodes(x_nodes);

    #pragma omp parallel num_threads(omp_get_num_procs())
    {
      #pragma omp for
      for(int i = 0 ; i < this_nodes.size() ; i++)
        this_nodes[i]->intersectWith_localUpdate(x_nodes[i]);
    }
  }

  requestFutureTreeComputation();
  return *this;
}

void Tube::intersectWith_localUpdate(const Tube *x)
{
  if(m_tree_computation_needed)
    computeTree();
  
  m_intv_y &= x->image();
  // Enclosed bounds cannot be computed on this level.
  // Synthesis has to be done from the root (see update() in intersectWith).
  requestFutureTreeComputation();
}

Tube& Tube::operator &=(const Tube& x)
{
  return intersectWith(x);
}

Tube& Tube::operator |=(const Tube& x)
{
  return unionWith(x);
}

void Tube::print(int precision) const
{
  if(m_tree_computation_needed)
    computeTree();
  
  if(isSlice())
  {
    if(precision != 0)
      cout << setprecision(precision);
    cout << "Tube: " << m_intv_t << " \t" << m_intv_y << endl;
  }

  else
  {
    m_first_subtube->print(precision);
    m_second_subtube->print(precision);
  }
}

std::ostream& operator<<(std::ostream& os, const Tube& x)
{
  if(x.m_tree_computation_needed)
    x.computeTree();
  
  cout << "Tube: t=" << x.m_intv_t
       << ", y=" << x.m_intv_y 
       << ", slices=" << x.m_slices_number
       << ", dt=" << x.m_dt
       << flush;
  return os;
}

void Tube::requestFutureTreeComputation(int index) const
{
  m_tree_computation_needed = true;

  if(!isSlice())
  {
    if(index == -1)
    {
      m_first_subtube->requestFutureTreeComputation(-1);
      m_second_subtube->requestFutureTreeComputation(-1);
    }

    else
    {
      if(index < 0 || index >= m_slices_number)
      {
        cout << "Error Tube::requestFutureTreeComputation(int): out of range "
             << "for index=" << index << " in [0," << m_slices_number << "[" << endl;
        return;
      }

      int mid_id = ceil(m_slices_number / 2.);
      if(index < mid_id) m_first_subtube->requestFutureTreeComputation(index);
      else m_second_subtube->requestFutureTreeComputation(index - mid_id);
    }
  }
  
  requestFuturePrimitiveComputation();
}

void Tube::computeTree() const
{
  if(m_tree_computation_needed)
  {
    if(isSlice())
      m_enclosed_bounds = make_pair(Interval(m_intv_y.lb()), Interval(m_intv_y.ub()));

    else
    {
      #pragma omp parallel num_threads(omp_get_num_procs())
      {
        #pragma omp sections
        {
          #pragma omp section
            m_first_subtube->computeTree();
          #pragma omp section
            m_second_subtube->computeTree();
        }
      }
      
      m_intv_y = m_first_subtube->image() | m_second_subtube->image();

      // Enclosed bounds
      pair<Interval,Interval> ui_past = m_first_subtube->eval();
      pair<Interval,Interval> ui_future = m_second_subtube->eval();
      m_enclosed_bounds = make_pair(ui_past.first | ui_future.first, ui_past.second | ui_future.second);
    }

    m_tree_computation_needed = false;
  }
}

Interval Tube::interpol(double t, const Tube& derivative_tube) const
{
  int index = input2index(t);
  Interval dom_ = domain(index);
  Interval deriv = derivative_tube[index];
  return ((*this)[dom_.ub()] - (dom_.ub() - t) * deriv)
       & ((*this)[dom_.lb()] + (t - dom_.lb()) * deriv);
}

Interval Tube::interpol(const Interval& intv_t, const Tube& derivative_tube) const
{
  pair<Interval,Interval> p_interpol = partialInterpol(intv_t, derivative_tube);
  return p_interpol.first | p_interpol.second;
}

pair<Interval,Interval> Tube::partialInterpol(const Interval& intv_t, const Tube& derivative_tube) const
{
  Interval lb, ub;
  lb.set_empty(); ub.set_empty();

  Interval y_tlb = interpol(intv_t.lb(), derivative_tube);
  Interval y_tub = interpol(intv_t.ub(), derivative_tube);

  lb |= y_tlb.lb();
  ub |= y_tlb.ub();

  lb |= y_tub.lb();
  ub |= y_tub.ub();

  Interval y = interpol(intv_t.lb(), derivative_tube) | interpol(intv_t.ub(), derivative_tube);
  for(int i = min(size() - 1, input2index(intv_t.lb()) + 1) ; i < max(0, input2index(intv_t.ub()) - 1) ; i++)
  {
    pair<Interval,Interval> p_i = eval(domain(i));
    lb |= p_i.first;
    ub |= p_i.second;
  }

  return make_pair(lb, ub);
}