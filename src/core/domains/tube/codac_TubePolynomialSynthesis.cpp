/** 
 *  TubePolynomialTreeSynthesis class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <Eigen/Dense>
#include "codac_TubePolynomialSynthesis.h"
#include "ibex_LargestFirst.h"

using namespace std;
using namespace ibex;

namespace codac
{
  TubePolynomialSynthesis::TubePolynomialSynthesis(const Tube& x, double eps) :
    m_fig(new VIBesFigTube("TubePolynomialSynthesis", &x)),
    m_trees(make_pair(TubePolynomialTreeSynthesis(x, x.tdomain(), false, eps, *m_fig),TubePolynomialTreeSynthesis(x, x.tdomain(), true, eps, *m_fig)))
  {
    m_fig->set_properties(100, 100, 600, 300);
    m_fig->show();
  }

  TubePolynomialSynthesis::~TubePolynomialSynthesis()
  {
    delete m_fig;
  }

  Interval TubePolynomialSynthesis::operator()(const Interval& t) const
  {
    return m_trees.first.operator()(t) | m_trees.second.operator()(t);
  }

  TubePolynomialTreeSynthesis::TubePolynomialTreeSynthesis(const Tube& x, const Interval& tdomain, bool upper_bound, double eps, VIBesFigTube& fig) :
    m_tube_ref(x),
    m_tdomain(tdomain),
    m_upper_bound(upper_bound),
    m_p(polynomial_traj(upper_bound)),
    m_fig(fig)
  {
    if(fabs(m_p.offset) > eps)
    {
      LargestFirst bisector;
      pair<IntervalVector,IntervalVector> t =
        bisector.bisect(IntervalVector(1,tdomain));
      m_left = new TubePolynomialTreeSynthesis(x, t.first[0], upper_bound, eps, m_fig);
      m_right = new TubePolynomialTreeSynthesis(x, t.second[0], upper_bound, eps, m_fig);
    }

    else
    {
      Trajectory *traj = new Trajectory(traj_from_polynom(m_p));
      ostringstream ostr;
      ostr << m_upper_bound << m_tdomain;
      m_fig.add_trajectory(traj, ostr.str(), m_upper_bound ? "blue" : "red");
      m_fig.draw_box(IntervalVector({m_tdomain, m_p.codomain}), m_upper_bound ? "blue" : "red");
      // todo: manage delete
    }
  }

  TubePolynomialTreeSynthesis::~TubePolynomialTreeSynthesis()
  {
    if(m_left != nullptr && m_right != nullptr)
    {
      // todo: delete m_left;
      // todo: delete m_right;
    }
  }

  Interval TubePolynomialTreeSynthesis::operator()(const Interval& t) const
  {
    if(!t.intersects(m_tdomain))
      return Interval::EMPTY_SET;

    else if(m_left)
      return m_left->operator()(t) | m_right->operator()(t);

    else if(t.is_superset(m_tdomain))
      return m_p.codomain;
    
    else
      return f(m_p, t & m_tdomain);
  }

  Interval TubePolynomialTreeSynthesis::f(const PolynomialSubtraj& p, const Interval& t) const
  {
    if(p.factor_form)
      return p.c*sqr(t-p.a)+p.b+p.offset;

    Interval x = p.offset;
    for(int i = 0 ; i < POLYNOMIAL_ORDER + 1 ; i++)
      x += p.coeff[i]*pow(t,i);
    return x;
  }

  void TubePolynomialTreeSynthesis::polyfit(const vector<double> &t, const vector<double> &v, array<double,POLYNOMIAL_ORDER+1> &coeff) const
  {
    // Create a matrix placeholder of size (n x k),
    // n = nb of datapoints,
    // k = polynomial order, e.g. k = 3 for cubic polynomial
    Eigen::MatrixXd T(t.size(), POLYNOMIAL_ORDER + 1);
    Eigen::VectorXd V = Eigen::VectorXd::Map(&v.front(), v.size());
    Eigen::VectorXd result;

    assert(t.size() == v.size());

    for(size_t i = 0 ; i < t.size(); ++i)
      for(size_t j = 0; j < POLYNOMIAL_ORDER + 1; ++j)
        T(i, j) = std::pow(t.at(i), j);

    // Linear least square fit
    result = T.householderQr().solve(V);
    for(int k = 0 ; k < POLYNOMIAL_ORDER + 1 ; k++)
      coeff[k] = result[k];


  }

  void TubePolynomialTreeSynthesis::get_bounds(const Interval& tdomain, bool upper_bound, vector<double>& t, vector<double>& v) const
  {
    map<double,double> map_values;
    for(const Slice *s = m_tube_ref.slice(tdomain.lb()) ; s != NULL ; s = s->next_slice())
    {
      Interval t_ = s->tdomain() & tdomain;
      if(t_ == Interval::EMPTY_SET)
        break;

      t.push_back(t_.lb());
      v.push_back(upper_bound ? s->codomain().ub() : s->codomain().lb());

      t.push_back(t_.ub());
      v.push_back(upper_bound ? s->codomain().ub() : s->codomain().lb());
    }

    assert(t.size() > 3);
  }

  Trajectory TubePolynomialTreeSynthesis::traj_from_polynom(const PolynomialSubtraj& p) const
  {
    // todo: analytical traj?
    Trajectory traj;
    double dt = m_tdomain.diam()/1000.;
    for(double t = m_tdomain.lb() ; t < m_tdomain.ub() + dt ; t += dt)
    {
      double t_ = std::min(t, m_tdomain.ub());
      traj.set(f(p,t_).mid(), t_);
    }
    return traj;
  }

  PolynomialSubtraj TubePolynomialTreeSynthesis::polynomial_traj(bool upper_bound) const
  {
    PolynomialSubtraj p;

    vector<double> t,v;
    get_bounds(m_tdomain, upper_bound, t, v);
    polyfit(t, v, p.coeff);

    double offset = 0.;
    for(size_t i = 0 ; i < t.size() ; i++)
    {
      Interval diff = v[i]-f(p, t[i]);
      if(upper_bound)
        offset = std::max(offset, diff.ub());
      else
        offset = std::min(offset, diff.lb());
    }

    if(p.coeff[2] != 0.)
    {
      p.a = -p.coeff[1]/(2*p.coeff[2]); 
      p.b = f(p, p.a).mid();
      if(m_tdomain.mid() != p.a)
      {
        p.c = (f(p, m_tdomain.mid()).mid()-p.b)/std::pow(m_tdomain.mid()-p.a,2);
        p.factor_form = true;
      }
    }

    p.offset = offset;
    p.codomain = f(p,m_tdomain);
    return p;
  }
}