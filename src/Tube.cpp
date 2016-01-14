/************************************************************/
/*    AUTH: Simon Rohou
/*    FILE: Tube.cpp
/*    PRJT: TubeIbex https://github.com/SimonRohou/TubeIbex
/*    DATE: 2015
/************************************************************/

#include "Tube.h"
#include <iostream>
#include <iomanip> // for setprecision()
#include <omp.h>
//#include <thread>

using namespace std;
using namespace ibex;


Tube::Tube(const Interval &intv_t, double time_step, const Interval& default_value)
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
  update(); // recursive update called from the root
  m_dt = time_step;
}

Tube::Tube(vector<Interval> vector_dt, const Interval& default_value)
{
  Tube(vector_dt, default_value, true);
}

Tube::Tube(vector<Interval> vector_dt, const Interval& default_value, bool bool_update)
{
  // Default value feature of C++ is not used because bool_update
  // should not be used outside this class (protected status)
  createFromSlicesVector(vector_dt, default_value);
  if(bool_update)
    update(); // recursive update (faster if only called from the root)
}

void Tube::createFromSlicesVector(vector<Interval> vector_dt, const Interval& default_value)
{
  m_dt = vector_dt[0].diam(); // all timesteps are identical in the tree
  m_intv_t = Interval(vector_dt[0].lb(), vector_dt[vector_dt.size() - 1].ub());
  m_intv_y = default_value;
  m_slices_number = vector_dt.size();

  if(m_slices_number == 1) // if this is a leaf
  {
    m_first_subtube = NULL;
    m_second_subtube = NULL;
  }

  else
  {
    // In the first subtube: [t0,thalf]
    // In the second subtube: ]thalf,tf]
    vector<Interval> first_vector_slices, second_vector_slices;
    int i, k = ceil(m_slices_number / 2.);

    for(i = 0 ; i < k ; i++)
      first_vector_slices.push_back(vector_dt[i]);

    for( ; i < m_slices_number ; i++)
      second_vector_slices.push_back(vector_dt[i]);

    // Subtubes are created without an update (bool_update = false)
    // For reasons of efficiency, the update is called only once from the root
    m_first_subtube = new Tube(first_vector_slices, default_value, false);
    m_second_subtube = new Tube(second_vector_slices, default_value, false);
  }
}

Tube::Tube(const Tube& tu)
{
  m_dt = tu.dt();
  m_intv_t = tu.getT();
  m_intv_y = tu.getY();
  m_slices_number = tu.size();
  m_enclosed_bounds = tu.getEnclosedBounds();

  if(tu.isSlice())
  {
    m_first_subtube = NULL;
    m_second_subtube = NULL;
  }

  else
  {
    m_first_subtube = new Tube(*(tu.getFirstSubTube()));
    m_second_subtube = new Tube(*(tu.getSecondSubTube()));
  }
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
    if((*this)[i].is_empty()) // diam(EMPTY_SET) is not 0
      continue;
    volume += m_dt * (*this)[i].diam();
  }
  return volume;
}

double Tube::dist(const Tube& tube) const
{
  return fabs(tube.volume() - volume());
}

bool Tube::isSlice() const
{
  return m_first_subtube == NULL && m_second_subtube == NULL;
}

Tube* Tube::getSlice(int index)
{
  if(index < 0 || index >= m_slices_number)
  {
    cout << "Error Tube::getSlice(int): out of range "
         << "for index=" << index << " in [0," << m_slices_number << "[" << endl;
    return NULL;
  }

  else if(isSlice())
  {
    if(index != 0)
      cout << "Warning Tube::getX(int): index not null in slice." << endl;

    return this;
  }

  else
  {
    int mid_id = ceil(m_slices_number / 2.);

    if(index < mid_id)
      return m_first_subtube->getSlice(index);

    else
      return m_second_subtube->getSlice(index - mid_id);
  }
}

int Tube::input2index(double t) const
{
  if(!m_intv_t.contains(t))
  {
    cout << "Error Tube::time2index(double): no corresponding slice "
         << "for t=" << t << " in " << setprecision(16) << m_intv_t << endl;
    return -1;
  }

  if(isSlice())
    return 0;

  if(m_first_subtube->getT().contains(t))
    return m_first_subtube->input2index(t);

  else
    return m_second_subtube->input2index(t) + m_first_subtube->size();
}

double Tube::index2input(int index)
{
  return getSlice(index)->getT().lb();
}

const Interval& Tube::getT() const
{
  return m_intv_t;
}

const Interval& Tube::getT(int index)
{
  return getSlice(index)->getT();
}

Interval Tube::getT(double t)
{
  int index = input2index(t);
  Interval intv_t = getSlice(index)->getT();
  if(t == intv_t.ub() && index < m_slices_number - 1) // on the boundary, between two slices
    return getSlice(index + 1)->getT() | intv_t;
  return intv_t;
}

const Interval& Tube::operator[](int index)
{
  // Write access is not allowed for this operator:
  // a call to update() is needed when values change,
  // this call cannot be garanteed with a direct access to m_intv_y
  // For write access: use setY()
  return getSlice(index)->m_intv_y;
}

Interval Tube::operator[](double t)
{
  // Write access is not allowed for this operator:
  // a call to update() is needed when values change,
  // this call cannot be garanteed with a direct access to m_intv_y
  // For write access: use setY()
  int index = input2index(t);
  Interval intv_t = getSlice(index)->m_intv_t;
  Interval intv_y = (*this)[index];
  if(t == intv_t.ub() && index < m_slices_number - 1) // on the boundary, between two slices
    return (*this)[index + 1] | intv_y;
  return intv_y;
}

Interval Tube::operator[](Interval intv_t) const
{
  // Write access is not allowed for this operator:
  // a call to update() is needed when values change,
  // this call cannot be garanteed with a direct access to m_intv_y
  // For write access: use setY()
  if(!m_intv_t.intersects(intv_t))
    return Interval::EMPTY_SET;

  else if(isSlice() || intv_t.is_unbounded() || intv_t.is_superset(m_intv_t))
    return m_intv_y;

  else
    return (*m_first_subtube)[intv_t] | (*m_second_subtube)[intv_t];
}

const Interval& Tube::getY() const
{
  return m_intv_y;
}

void Tube::setY(const Interval& intv_y, int index)
{
  getSlice(index)->setY(intv_y);
  updateFromIndex(index); // an update is needed since some tube's values changed
}

void Tube::setY(const Interval& intv_y, double t)
{
  getSlice(input2index(t))->setY(intv_y);
  updateFromInput(t); // an update is needed since some tube's values changed
}

void Tube::setY(const Interval& intv_y, const Interval& intv_t)
{
  setY(intv_y, intv_t, true);
}

void Tube::setY(const Interval& intv_y, const Interval& intv_t, bool bool_update)
{
  // Default value feature of C++ is not used because bool_update
  // should not be used outside this class (protected status)
  if(m_intv_t.intersects(intv_t))
  {
    if(isSlice())
      m_intv_y = intv_y;

    else
    {
      m_first_subtube->setY(intv_y, intv_t, false);
      m_second_subtube->setY(intv_y, intv_t, false);
    }
    
    if(bool_update)
      update(); // an update is needed since some tube's values changed
  }
}

const Tube* Tube::getFirstSubTube() const
{
  return m_first_subtube;
}

const Tube* Tube::getSecondSubTube() const
{
  return m_second_subtube;
}

const Tube Tube::primitive()
{
  return primitive(Interval(0.));
}

const Tube Tube::primitive(const Interval& initial_value)
{
  Tube primitive(m_intv_t, m_dt);
  Interval sum = initial_value;

  for(int i = 0 ; i < m_slices_number ; i++)
  {
    primitive.setY(sum, i);
    sum += (*this)[i] * getSlice(i)->getT().diam();
  }

  return primitive;
}

Interval Tube::timeIntegration(const Tube& primitive, const Interval& t1, const Interval& t2) const
{
  pair<Interval,Interval> enc_prim = partialTimeIntegration(primitive, t1, t2);
  return Interval(enc_prim.first.lb(), enc_prim.second.ub());
}

pair<Interval,Interval> Tube::partialTimeIntegration(const Tube& primitive, const Interval& t1, const Interval& t2) const
{
  pair<Interval,Interval> enc_prim_t1 = primitive.getEnclosedBounds(t1);
  pair<Interval,Interval> enc_prim_t2 = primitive.getEnclosedBounds(t2);

  Interval integral_min = enc_prim_t2.first - enc_prim_t1.first;
  Interval integral_max = enc_prim_t2.second - enc_prim_t1.second;

  return make_pair(integral_min, integral_max);
}

bool Tube::intersect(const Interval& intv_y, int index)
{
  return intersect(intv_y, index, true);
}

bool Tube::intersect(const Interval& intv_y, int index, bool bool_update)
{
  // Default value feature of C++ is not used because bool_update
  // should not be used outside this class (protected status)
  bool result = getSlice(index)->intersect(intv_y, Interval::ALL_REALS, false);
  if(bool_update)
    updateFromIndex(index);
  return result;
}

bool Tube::intersect(const Interval& intv_y, double t)
{
  return intersect(intv_y, t, true);
}

bool Tube::intersect(const Interval& intv_y, double t, bool bool_update)
{
  // Default value feature of C++ is not used because bool_update
  // should not be used outside this class (protected status)
  bool result = getSlice(input2index(t))->intersect(intv_y, Interval::ALL_REALS, false);
  if(bool_update)
    updateFromInput(t);
  return result;
}

bool Tube::intersect(const Interval& intv_y, const Interval& intv_t)
{
  return intersect(intv_y, intv_t, true);
}

bool Tube::intersect(const Interval& intv_y, const Interval& intv_t, bool bool_update)
{
  // Default value feature of C++ is not used because bool_update
  // should not be used outside this class (protected status)
  if(m_intv_t.intersects(intv_t))
  {
    bool contraction = false;

    if(isSlice())
    {
      if(!m_intv_y.is_empty())
      {
        double diam = m_intv_y.diam();
        m_intv_y &= intv_y;
        contraction = m_intv_y.is_empty() || m_intv_y.diam() < diam; // diam(EMPTY_SET) is not 0
      }
    }

    else
    {
      contraction |= m_first_subtube->intersect(intv_y, intv_t, false);
      contraction |= m_second_subtube->intersect(intv_y, intv_t, false);
    }
    
    // Tube is updated if a contraction has been done or if requested with bool_update
    if(contraction && bool_update)
      update();

    return contraction;
  }

  return false;
}

const pair<Interval,Interval> Tube::getEnclosedBounds(const Interval& intv_t) const
{
  if(intv_t == Interval::ALL_REALS)
    return m_enclosed_bounds; // pre-computed with update()

  else if(!intv_t.is_empty() && m_intv_t.intersects(intv_t))
  {
    if(isSlice() || intv_t.is_superset(m_intv_t))
      return m_enclosed_bounds;

    else
    {
      pair<Interval,Interval> ui_past = m_first_subtube->getEnclosedBounds(intv_t);
      pair<Interval,Interval> ui_future = m_second_subtube->getEnclosedBounds(intv_t);
      return make_pair(ui_past.first | ui_future.first, ui_past.second | ui_future.second);
    }
  }

  else
    return make_pair(Interval::EMPTY_SET, Interval::EMPTY_SET);
}

Tube& Tube::unionWith(const Tube& x)
{
  if(size() != x.size())
    cout << "Warning Tube::unionWith(): cannot make the hull of Tubes of different dimensions: " 
         << "n1=" << size() << " and n2=" << x.size() << endl;

  if(getT() != x.getT())
    cout << "Warning Tube::unionWith(): cannot make the hull of Tubes of different domain: " 
         << "[t1]=" << getT() << " and [t2]=" << x.getT() << endl;

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

  return *this;
}

void Tube::unionWith_localUpdate(const Tube *x)
{
  m_intv_y |= x->getY();
  pair<Interval,Interval> eb1 = getEnclosedBounds();
  pair<Interval,Interval> eb2 = x->getEnclosedBounds();
  m_enclosed_bounds = make_pair(Interval(min(eb1.first.lb(), eb2.first.lb()), min(eb1.first.ub(), eb2.first.ub())),
                                Interval(max(eb1.second.lb(), eb2.second.lb()), max(eb1.second.ub(), eb2.second.ub())));
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

void Tube::getTubeNodes(vector<const Tube*> &v_nodes) const
{
  if(!isSlice())
  {
    m_first_subtube->getTubeNodes(v_nodes);
    m_second_subtube->getTubeNodes(v_nodes);
  }

  v_nodes.push_back(this);
}

Tube& Tube::intersectWith(const Tube& x, bool bool_update)
{
  if(size() != x.size())
    cout << "Warning Tube::intersectWith(): cannot make the intersection of Tubes of different dimensions: " 
         << "n1=" << size() << " and n2=" << x.size() << endl;

  if(getT() != x.getT())
    cout << "Warning Tube::intersectWith(): cannot make the intersection of Tubes of different domain: " 
         << "[t1]=" << getT() << " and [t2]=" << x.getT() << endl;

  if(isSlice())
    m_intv_y &= x.getY();

  else
  {
    m_first_subtube->intersectWith(*(x.getFirstSubTube()), false);
    m_second_subtube->intersectWith(*(x.getSecondSubTube()), false);
  }

  if(bool_update)
    update();

  return *this;
}

Tube& Tube::operator &=(const Tube& x)
{
  return intersectWith(x, true);
}

Tube& Tube::operator |=(const Tube& x)
{
  return unionWith(x);
}

void Tube::print(int precision) const
{
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
  cout << "Tube: t=" << x.m_intv_t
       << ", y=" << x.m_intv_y 
       << ", slices=" << x.m_slices_number
       << ", dt=" << x.m_dt
       << flush;
  return os;
}

bool Tube::ctcFwd(Tube* derivative_tube)
{
  if(size() != derivative_tube->size())
    cout << "Warning ctcFwd(Tube* derivative_tube): tube of different size: "
         << derivative_tube->size() << "/" << size() << endl;

  bool contraction = false;

  for(int i = 1 ; i < size() ; i++) // from the past to the future
  {
    Interval y_new = (*this)[i-1] + (*derivative_tube)[i-1] * derivative_tube->getSlice(i-1)->getT().diam();
    contraction |= getSlice(i)->intersect(y_new, 0, false);
  }

  update();
  return contraction;
}

bool Tube::ctcBwd(Tube* derivative_tube)
{
  if(size() != derivative_tube->size())
    cout << "Warning ctcBwd(Tube* derivative_tube): tube of different size: "
         << derivative_tube->size() << "/" << size() << endl;

  bool contraction = false;

  for(int i = size() - 2 ; i >= 0 ; i--) // from the future to the past
  {
    Interval y_new = (*this)[i+1] - (*derivative_tube)[i+1] * derivative_tube->getSlice(i+1)->getT().diam();
    contraction |= getSlice(i)->intersect(y_new, 0, false);
  }

  update();
  return contraction;
}

bool Tube::ctcFwdBwd(Tube* derivative_tube)
{
  bool contraction = false;
  contraction |= ctcFwd(derivative_tube);
  contraction |= ctcBwd(derivative_tube);
  return contraction;
}

bool Tube::ctcIn(Tube *derivative_tube, Interval& y, Interval& t)
{
  bool t_contraction = false;
  bool y_contraction = false;
  bool tube_contraction = false;

  // Trying to contract [t]
  {
    double t_diam = t.diam();

    // In forward
    int i = input2index(t.lb());
    while(!(*this)[i].intersects(y) && i < input2index(t.ub()))
    {
      i++;
      t = Interval(getT(i).lb(), t.ub()); // updating lower-bound (contraction)
    }

    // In backward
    i = input2index(t.ub());
    while(!(*this)[i].intersects(y) && i > input2index(t.lb()))
    {
      i--;
      t = Interval(t.lb(), getT(i).ub()); // updating upper-bound (contraction)
    }

    t_contraction = t.diam() < t_diam;
  }

  // Trying to contract [y]
  {
    double y_diam = y.diam();
    y &= (*this)[t];
    y_contraction = y.diam() < y_diam;
  }

  // Trying to contract this
  {
    Tube tube_temp1(*this);

    // Raw contraction without derivative information
    for(int i = input2index(t.lb()) ; i <= input2index(t.ub()) ; i++)
    {
      Interval old_y = (*this)[i];
      Interval new_y = old_y & y;
      tube_temp1.setY(new_y, i);
    }

    // Taking derivative information into account
    Tube tube_temp2(tube_temp1);
    for(int i = input2index(t.lb()) ; i <= input2index(t.ub()) ; i++)
    {
      Tube tube_temp3(tube_temp1);
      Interval new_y = tube_temp3[i];

      // In forward
      for(int j = i ; j <= input2index(t.ub()) ; j++)
      {
        new_y += Interval((*derivative_tube)[j].lb() * derivative_tube->getT(j).diam(),
                          (*derivative_tube)[j].ub() * derivative_tube->getT(j).diam());
        tube_temp3.setY(new_y, j);
      }
      new_y = tube_temp1[i];

      // In backward
      for(int j = i ; j >= input2index(t.lb()) ; j--)
      {
        Interval deriv = ((*derivative_tube)[j]);
        new_y -= Interval(deriv.lb() * derivative_tube->getT(j).diam(),
                          deriv.ub() * derivative_tube->getT(j).diam());
        tube_temp3.setY(new_y | tube_temp3[j], j); // an union is made to keep forward's update
      }

      tube_temp2 |= tube_temp3;
    }

    double vol = volume();
    (*this) &= tube_temp2;
    tube_contraction = volume() < vol;
  }

  return t_contraction | tube_contraction | y_contraction;
}

void Tube::update()
{
  updateFromIndex(-1);
}

void Tube::updateFromInput(double t_focus)
{
  updateFromIndex(input2index(t_focus));
}

void Tube::updateFromIndex(int index_focus)
{
  if(index_focus == -1 || index_focus < m_slices_number)
  {
    if(isSlice())
      m_enclosed_bounds = make_pair(Interval(m_intv_y.lb()), Interval(m_intv_y.ub()));

    else
    {
      if(index_focus == -1)
      {
        m_first_subtube->updateFromIndex(-1);
        m_second_subtube->updateFromIndex(-1);
      }

      else
      {
        int mid_id = ceil(m_slices_number / 2.);

        if(index_focus < mid_id)
          m_first_subtube->updateFromIndex(index_focus);
        
        else
          m_second_subtube->updateFromIndex(index_focus == -1 ? -1 : index_focus - mid_id);
      }
      
      m_intv_y = m_first_subtube->getY() | m_second_subtube->getY();
      pair<Interval,Interval> ui_past = m_first_subtube->getEnclosedBounds();
      pair<Interval,Interval> ui_future = m_second_subtube->getEnclosedBounds();
      m_enclosed_bounds = make_pair(ui_past.first | ui_future.first, ui_past.second | ui_future.second);
    }
  }
}