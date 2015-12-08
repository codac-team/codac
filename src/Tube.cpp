/************************************************************/
/*    AUTH: Simon Rohou
/*    FILE: Tube.cpp
/*    PRJT: TubeIbex https://github.com/SimonRohou/TubeIbex
/*    DATE: 2015
/************************************************************/

#include "Tube.h"
#include <iomanip> // for setprecision()

using namespace std;
using namespace ibex;


Tube::Tube(const Interval &intv_t, double dt)
{
  double lb, ub = intv_t.lb();
  vector<Interval> vector_dt;
  do
  {
    lb = ub;
    ub = lb + dt;
    vector_dt.push_back(Interval(lb, ub));
  } while(ub < intv_t.ub());

  initFromSlicesVector(vector_dt);
  update();
}

Tube::Tube(vector<Interval> vector_dt, bool bool_update)
{
  initFromSlicesVector(vector_dt);
  if(bool_update)
    update();
}

void Tube::initFromSlicesVector(vector<Interval> vector_dt)
{
  m_dt = vector_dt[0].diam();
  m_intv_t = new Interval(vector_dt[0].lb(), vector_dt[vector_dt.size() - 1].ub());
  m_intv_y = Interval::EMPTY_SET;
  m_slices_number = vector_dt.size();

  if(m_slices_number == 1)
  {
    m_first_subtube = NULL;
    m_second_subtube = NULL;
  }

  else
  {
    vector<Interval> first_vector_slices, second_vector_slices;
    int i, k = ceil(m_slices_number / 2.);
    for(i = 0 ; i < k ; i++)
      first_vector_slices.push_back(vector_dt[i]);

    for( ; i < m_slices_number ; i++)
      second_vector_slices.push_back(vector_dt[i]);

    m_first_subtube = new Tube(first_vector_slices, false);
    m_second_subtube = new Tube(second_vector_slices, false);
  }
}

Tube::Tube(const Tube& tu)
{
  m_dt = tu.dt();
  m_intv_t = new Interval(tu.getT());
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

const Interval& Tube::operator[](int index)
{
  // Write access is not allowed for this operator:
  // a call to update() is needed when values change,
  // this call cannot be garanteed with a direct access to m_intv_y
  // For write access: use setY()
  return getSlice(index)->m_intv_y;
}

const Interval& Tube::operator[](double t)
{
  // Write access is not allowed for this operator:
  // a call to update() is needed when values change,
  // this call cannot be garanteed with a direct access to m_intv_y
  // For write access: use setY()
  return getSlice(input2index(t))->m_intv_y;
}

Interval Tube::operator[](Interval intv_t) const
{
  // Write access is not allowed for this operator:
  // a call to update() is needed when values change,
  // this call cannot be garanteed with a direct access to m_intv_y
  // For write access: use setY()
  if(!m_intv_t->intersects(intv_t))
    return Interval::EMPTY_SET;

  else if(isSlice() || intv_t.is_unbounded() || intv_t.is_superset(*m_intv_t))
    return m_intv_y;

  else
    return (*m_first_subtube)[intv_t] | (*m_second_subtube)[intv_t];
}

int Tube::input2index(double t) const
{
  if(!m_intv_t->contains(t))
  {
    cout << "Error Tube::time2index(double): no corresponding slice "
         << "for t=" << t << " in " << setprecision(16) << *m_intv_t << endl;
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
  return *m_intv_t;
}

const Interval& Tube::getT(int index)
{
  return getSlice(index)->getT();
}

const Interval& Tube::getT(double t)
{
  return getSlice(input2index(t))->getT();
}

const Interval& Tube::getY() const
{
  return m_intv_y;
}

void Tube::setY(const Interval& intv_y, int index)
{
  getSlice(index)->setY(intv_y);
  updateFromIndex(index);
}

void Tube::setY(const Interval& intv_y, double t)
{
  getSlice(input2index(t))->setY(intv_y);
  updateFromInput(t);
}

void Tube::setY(const Interval& intv_y, const Interval& intv_t)
{
  if(m_intv_t->intersects(intv_t))
  {
    if(isSlice())
      m_intv_y = intv_y;

    else
    {
      m_first_subtube->setY(intv_y, intv_t);
      m_second_subtube->setY(intv_y, intv_t);
    }
    
    update();
  }
}

const Tube Tube::primitive()
{
  return primitive(Interval(0.));
}

const Tube Tube::primitive(const Interval& initial_value)
{
  Tube primitive(*m_intv_t, m_dt);

  Interval sum = initial_value;
  for(int i = 0 ; i < m_slices_number ; i++)
  {
    primitive.setY(sum, i);
    sum += (*this)[i] * m_dt;
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
  bool result = getSlice(index)->intersect(intv_y, Interval::ALL_REALS, false);
  updateFromIndex(index);
  return result;
}

bool Tube::intersect(const Interval& intv_y, double t)
{
  bool result = getSlice(input2index(t))->intersect(intv_y, Interval::ALL_REALS, false);
  updateFromInput(t);
  return result;
}

bool Tube::intersect(const Interval& intv_y, const Interval& intv_t, bool allow_update)
{
  if(m_intv_t->intersects(intv_t))
  {
    bool contraction = false;

    if(isSlice())
    {
      if(!m_intv_y.is_empty())
      {
        double diam = m_intv_y.diam();
        m_intv_y &= intv_y;
        contraction = m_intv_y.is_empty() || m_intv_y.diam() < diam;
      }
    }

    else
    {
      contraction |= m_first_subtube->intersect(intv_y, intv_t, false);
      contraction |= m_second_subtube->intersect(intv_y, intv_t, false);
    }
    
    if(contraction && allow_update)
      update();

    return contraction;
  }

  return false;
}

const pair<const Interval&,const Interval&> Tube::getEnclosedBounds() const
{
  return m_enclosed_bounds;
}

const pair<Interval,Interval> Tube::getEnclosedBounds(const Interval& intv_t) const
{
  if(!intv_t.is_empty() && m_intv_t->intersects(intv_t))
  {
    if(isSlice() || intv_t.is_superset(*m_intv_t))
      return m_enclosed_bounds;

    else
    {
      pair<Interval,Interval> ui_past = m_first_subtube->getEnclosedBounds(intv_t);
      pair<Interval,Interval> ui_future = m_second_subtube->getEnclosedBounds(intv_t);
      return make_pair(ui_past.first | ui_future.first, ui_past.second | ui_future.second);
    }
  }

  return make_pair(Interval::EMPTY_SET, Interval::EMPTY_SET);
}

const pair<Interval,Interval> Tube::getEnclosedBounds(const Interval& t1, const Interval& t2) const
{
  pair<Interval,Interval> uy1 = getEnclosedBounds(t1);
  pair<Interval,Interval> uy2 = getEnclosedBounds(t2);
  return make_pair(Interval(uy1.first.lb() - uy2.second.ub(), uy1.first.ub() - uy2.second.lb()),
                   Interval(uy1.second.lb() - uy2.first.ub(), uy1.second.ub() - uy2.first.lb()));
}

void Tube::print() const
{
  if(isSlice())
    cout << "Tube: " << *m_intv_t << " \t" << m_intv_y << endl;

  else
  {
    m_first_subtube->print();
    m_second_subtube->print();
  }
}

std::ostream& operator<<(std::ostream& os, const Tube& x)
{ 
  cout << "Tube: t=" << *(x.m_intv_t)
       << ", y=" << x.m_intv_y 
       << ", slices=" << x.m_slices_number
       << ", dt=" << x.m_dt
       << flush;
  return os;
}

Tube& Tube::operator |=(const Tube& x)
{
  if(size() != x.size())
    cout << "Warning Tube::operator |=: cannot make the hull of Tubes of different dimensions: " 
         << "n1=" << size() << " and n2=" << x.size() << endl;

  if(getT() != x.getT())
    cout << "Warning Tube::operator |=: cannot make the hull of Tubes of different domain: " 
         << "[t1]=" << getT() << " and [t2]=" << x.getT() << endl;

  m_intv_y |= x.getY();

  if(!isSlice())
  {
    *m_first_subtube |= *(x.getFirstSubTube());
    *m_second_subtube |= *(x.getSecondSubTube());
  }

  update();
  return *this;
}

Tube& Tube::operator &=(const Tube& x)
{
  if(size() != x.size())
    cout << "Warning Tube::operator &=: cannot make the intersection of Tubes of different dimensions: " 
         << "n1=" << size() << " and n2=" << x.size() << endl;

  if(getT() != x.getT())
    cout << "Warning Tube::operator &=: cannot make the intersection of Tubes of different domain: " 
         << "[t1]=" << getT() << " and [t2]=" << x.getT() << endl;

  m_intv_y &= x.getY();

  if(!isSlice())
  {
    *m_first_subtube &= *(x.getFirstSubTube());
    *m_second_subtube &= *(x.getSecondSubTube());
  }

  update();
  return *this;
}

const Tube* Tube::getFirstSubTube() const
{
  return m_first_subtube;
}

const Tube* Tube::getSecondSubTube() const
{
  return m_second_subtube;
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
    {
      m_enclosed_bounds = make_pair(Interval(m_intv_y.lb()), Interval(m_intv_y.ub()));
    }

    else
    {
      int mid_id = ceil(m_slices_number / 2.);

      if(index_focus < mid_id)
        m_first_subtube->updateFromIndex(index_focus);
      
      else
        m_second_subtube->updateFromIndex(index_focus == -1 ? -1 : index_focus - mid_id);
      
      m_intv_y = m_first_subtube->getY() | m_second_subtube->getY();

      pair<Interval,Interval> ui_past = m_first_subtube->getEnclosedBounds();
      pair<Interval,Interval> ui_future = m_second_subtube->getEnclosedBounds();
      m_enclosed_bounds = make_pair(ui_past.first | ui_future.first, ui_past.second | ui_future.second);
    }
  }
}

bool Tube::ctcFwd(const Tube& derivative_tube)
{
  if(size() != derivative_tube.size())
    cout << "Warning ctcFwd(Tube derivative_tube): tube of different size." << endl;

  bool contraction = false;

  for(int i = 1 ; i < size() ; i++)
  {
    ibex::Interval y_new = (*this)[i - 1] + derivative_tube[i - 1] * derivative_tube.dt();
    contraction |= getSlice(i)->intersect(y_new);
  }

  return contraction;
}

bool Tube::ctcBwd(const Tube& derivative_tube)
{
  if(size() != derivative_tube.size())
    cout << "Warning ctcBwd(Tube derivative_tube): tube of different size." << endl;

  bool contraction = false;

  for(int i = size() - 2 ; i >= 0 ; i--)
  {
    ibex::Interval y_new = (*this)[i + 1] - derivative_tube[i + 1] * derivative_tube.dt();
    contraction |= getSlice(i)->intersect(y_new);
  }

  return contraction;
}

bool Tube::ctcFwdBwd(const Tube& derivative_tube)
{
  bool contraction = false;
  contraction |= ctcFwd(derivative_tube);
  contraction |= ctcBwd(derivative_tube);
  return contraction;
}