/** 
 *  TFunction class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_TFunction.h"
#include "tubex_Tube.h"
#include "tubex_TubeVector.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  TFunction::TFunction(int n, const char** x, const char* y)
  {
    construct_from_array(n, x, y);
  }

  TFunction::TFunction(const char* y)
  {
    construct_from_array(0, NULL, y);
  }

  TFunction::TFunction(const char* x1, const char* y)
  {
    const char* xdyn[1];
    xdyn[0] = x1;
    construct_from_array(1, xdyn, y);
  }

  TFunction::TFunction(const char* x1, const char* x2, const char* y)
  {
    const char* xdyn[2];
    xdyn[0] = x1;
    xdyn[1] = x2;
    construct_from_array(2, xdyn, y);
  }

  TFunction::TFunction(const char* x1, const char* x2, const char* x3, const char* y)
  {
    const char* xdyn[3];
    xdyn[0] = x1;
    xdyn[1] = x2;
    xdyn[2] = x3;
    construct_from_array(3, xdyn, y);
  }

  TFunction::TFunction(const char* x1, const char* x2, const char* x3, const char* x4, const char* y)
  {
    const char* xdyn[4];
    xdyn[0] = x1;
    xdyn[1] = x2;
    xdyn[2] = x3;
    xdyn[3] = x4;
    construct_from_array(4, xdyn, y);
  }

  TFunction::TFunction(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* y)
  {
    const char* xdyn[5];
    xdyn[0] = x1;
    xdyn[1] = x2;
    xdyn[2] = x3;
    xdyn[3] = x4;
    xdyn[4] = x5;
    construct_from_array(5, xdyn, y);
  }

  TFunction::TFunction(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* y)
  {
    const char* xdyn[6];
    xdyn[0] = x1;
    xdyn[1] = x2;
    xdyn[2] = x3;
    xdyn[3] = x4;
    xdyn[4] = x5;
    xdyn[5] = x6;
    construct_from_array(6, xdyn, y);
  }

  TFunction::TFunction(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* x7, const char* y)
  {
    const char* xdyn[7];
    xdyn[0] = x1;
    xdyn[1] = x2;
    xdyn[2] = x3;
    xdyn[3] = x4;
    xdyn[4] = x5;
    xdyn[5] = x6;
    xdyn[6] = x7;
    construct_from_array(7, xdyn, y);
  }

  TFunction::TFunction(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* x7, const char* x8, const char* y)
  {
    const char* xdyn[8];
    xdyn[0] = x1;
    xdyn[1] = x2;
    xdyn[2] = x3;
    xdyn[3] = x4;
    xdyn[4] = x5;
    xdyn[5] = x6;
    xdyn[6] = x7;
    xdyn[7] = x8;
    construct_from_array(8, xdyn, y);
  }

  TFunction::TFunction(const TFunction& f)
    : TFnc(f.nb_vars(), f.image_dim(), false)
  {
    *this = f;
  }

  TFunction::~TFunction()
  {
    delete m_ibex_f;
  }

  const TFunction& TFunction::operator=(const TFunction& f)
  {
    if(m_ibex_f != NULL)
      delete m_ibex_f;
    m_ibex_f = new Function(*f.m_ibex_f);
    m_expr = f.m_expr;
    TFnc::operator=(f);
    return *this;
  }

  const TFunction TFunction::operator[](int i) const
  {
    assert(i >= 0 && i < image_dim());
    // todo: check the following
    TFunction fi(*this);
    Function ibex_fi((*fi.m_ibex_f)[i]);
    delete fi.m_ibex_f;
    fi.m_ibex_f = new Function(ibex_fi);
    fi.m_img_dim = 1;
    return fi;
  }
  
  const string& TFunction::expr() const
  {
    return m_expr;
  }
  
  const string TFunction::arg_name(int i) const
  {
    assert(i >= 0 && i < m_nb_vars);
    // t is not considered as part of the arguments
    return m_ibex_f->arg_name(i+1);
  }

  void TFunction::construct_from_array(int n, const char** x, const char* y)
  {
    assert(n >= 0);
    assert(y != NULL && "function's output must be defined");

#ifdef _MSC_VER
    // see https://stackoverflow.com/questions/48459297/is-there-a-vlas-variable-length-arrays-support-workaround-for-vs2017
    const char** xdyn = new const char* [n+1];
#else
    const char* xdyn[n+1];
#endif // _MSC_VER
    xdyn[0] = "t";
    for(int i = 0 ; i < n ; i++)
    {
      assert(string(x[i]) != "t" && "forbidden variable name \"t\"");
      xdyn[i+1] = x[i];
    }

    m_ibex_f = new Function(n+1, xdyn, y);
    m_nb_vars = n;
    m_img_dim = m_ibex_f->image_dim();
    m_intertemporal = false; // not supported yet
    m_expr = y;
#ifdef _MSC_VER
    delete[] xdyn;
#endif // _MSC_VER
  }

  const Interval TFunction::eval(const Interval& t) const
  {
    assert(nb_vars() == 0);
    assert(image_dim() == 1 && "scalar evaluation");
    return eval_vector(t)[0];
  }

  const Interval TFunction::eval(const IntervalVector& x) const
  {
    assert(nb_vars() == x.size() - 1);
    assert(image_dim() == 1 && "scalar evaluation");
    assert(!is_intertemporal());
    return eval_vector(x)[0];
  }

  const Interval TFunction::eval(int slice_id, const TubeVector& x) const
  {
    assert(x.size() == nb_vars());
    assert(slice_id >= 0 && slice_id < x.nb_slices());
    assert(image_dim() == 1 && "scalar evaluation");
    return eval_vector(slice_id, x)[0];
  }

  const Interval TFunction::eval(const Interval& t, const TubeVector& x) const
  {
    assert(x.size() == nb_vars());
    assert(x.tdomain().is_superset(t));
    assert(image_dim() == 1 && "scalar evaluation");
    return eval_vector(t, x)[0];
  }

  const Tube TFunction::eval(const TubeVector& x) const
  {
    assert(x.size() == nb_vars());
    assert(image_dim() == 1 && "scalar evaluation");
    return eval_vector(x)[0];
  }

  const Trajectory TFunction::traj_eval(const TrajectoryVector& x) const
  {
    assert(x.size() == nb_vars());
    assert(image_dim() == 1 && "scalar evaluation");
    return traj_eval_vector(x)[0];
  }

  const IntervalVector TFunction::eval_vector(const Interval& t) const
  {
    assert(nb_vars() == 0);
    IntervalVector box(1, t);
    return m_ibex_f->eval_vector(box);
  }

  const IntervalVector TFunction::eval_vector(const IntervalVector& x) const
  {
    assert(nb_vars() == x.size() - 1);
    assert(!is_intertemporal());
    return m_ibex_f->eval_vector(x);
  }

  const IntervalVector TFunction::eval_vector(int slice_id, const TubeVector& x) const
  {
    assert(slice_id >= 0 && slice_id < x.nb_slices());

    Interval t = x[0].slice_tdomain(slice_id);

    if(nb_vars() == 0)
      return eval_vector(t);

    assert(nb_vars() == x.size());

    if(x(slice_id).is_empty())
      return IntervalVector(image_dim(), Interval::EMPTY_SET);

    IntervalVector box(nb_vars() + 1); // +1 for system variable (t)
    box[0] = t;
    box.put(1, x(slice_id));

    return m_ibex_f->eval_vector(box);
  }

  const IntervalVector TFunction::eval_vector(const Interval& t, const TubeVector& x) const
  {
    if(nb_vars() == 0)
      return eval_vector(t);

    assert(x.tdomain().is_superset(t));
    assert(nb_vars() == x.size());

    if(x(t).is_empty())
      return IntervalVector(image_dim(), Interval::EMPTY_SET);

    IntervalVector box(nb_vars() + 1); // +1 for system variable (t)
    box[0] = t;
    if(nb_vars() != 0)
      for(int i = 0 ; i < x.size() ; i++)
        box[i+1] = x[i](t);

    return m_ibex_f->eval_vector(box);
  }

  const TubeVector TFunction::eval_vector(const TubeVector& x) const
  {
    // Faster evaluation than the generic Fnc::eval method
    // For now, TFunction class does not allow inter-temporal evaluations
    // such as delays or integral computations. Hence, the generic method
    // Fnc::eval(Interval t, TubeVector x) can be replaced by a dedicated evaluation

    // todo: update this comment ^

    if(nb_vars() != 0)
      assert(x.size() == nb_vars());
    
    TubeVector y(x); // keeping slicing the x
    y.resize(image_dim());

    if(x.is_empty())
    {
      y.set_empty();
      return y;
    }

    IntervalVector box(x.size() + 1), result(y.size());

    const Slice **v_sx = new const Slice*[x.size()];
    for(int i = 0 ; i < x.size() ; i++)
      v_sx[i] = NULL;

    Slice **v_sy = new Slice*[y.size()];
    for(int i = 0 ; i < y.size() ; i++)
      v_sy[i] = NULL;

    do
    {
      if(v_sx[0] == NULL) // first iteration
      {
        for(int i = 0 ; i < x.size() ; i++)
          v_sx[i] = x[i].first_slice();
        for(int i = 0 ; i < y.size() ; i++)
          v_sy[i] = y[i].first_slice();
      }

      else
      {
        for(int i = 0 ; i < x.size() ; i++)
          v_sx[i] = v_sx[i]->next_slice();
        for(int i = 0 ; i < y.size() ; i++)
          v_sy[i] = v_sy[i]->next_slice();
      }

      box[0] = v_sx[0]->tdomain();
      for(int i = 0 ; i < x.size() ; i++)
        box[i+1] = v_sx[i]->codomain();
      result = m_ibex_f->eval_vector(box);
      for(int i = 0 ; i < y.size() ; i++)
        v_sy[i]->set_envelope(result[i], false);

      box[0] = box[0].lb();
      for(int i = 0 ; i < x.size() ; i++)
        box[i+1] = v_sx[i]->input_gate();
      result = m_ibex_f->eval_vector(box);
      for(int i = 0 ; i < y.size() ; i++)
        v_sy[i]->set_input_gate(result[i], false);

    } while(v_sx[0]->next_slice() != NULL);
    
    box[0] = v_sx[0]->tdomain().ub();
    for(int i = 0 ; i < x.size() ; i++)
      box[i+1] = v_sx[i]->output_gate();
    result = m_ibex_f->eval_vector(box);
    for(int i = 0 ; i < y.size() ; i++)
      v_sy[i]->set_output_gate(result[i], false);

    delete[] v_sx;
    delete[] v_sy;
    return y;
  }

  const TrajectoryVector TFunction::traj_eval_vector(const TrajectoryVector& x) const
  {
    // Faster evaluation than the generic Fnc::eval method
    // For now, TFunction class does not allow inter-temporal evaluations
    // such as delays or integral computations. Hence, the generic method
    // Fnc::eval(Interval t, TubeVector x) can be replaced by a dedicated evaluation

    // todo: update this comment ^

    if(nb_vars() != 0)
      assert(x.size() == nb_vars());

    assert(x[0].definition_type() == TrajDefnType::MAP_OF_VALUES
      && "eval TFunction not supported for analytic trajectories");
    
    TrajectoryVector y(image_dim());
    for(map<double,double>::const_iterator it = x[0].sampled_map().begin() ;
        it != x[0].sampled_map().end() ; it++)
    {
      Vector v(nb_vars() + 1);
      v[0] = it->first;
      v.put(1, x(it->first));

      y.set(m_ibex_f->eval_vector(v).mid(), it->first);
    }

    return y;
  }

  const TFunction TFunction::diff() const
  {
    TFunction diff_f = *this;
    delete diff_f.m_ibex_f;
    diff_f.m_ibex_f = new Function(m_ibex_f->diff());
    return diff_f;
  }
}