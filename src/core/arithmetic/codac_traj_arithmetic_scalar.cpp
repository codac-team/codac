/** 
 *  Arithmetic operations on trajectories
 * ----------------------------------------------------------------------------
 *  \date       2019
 *  \author     Simon Rohou
 *  \bugfixes   Bertrand Neveu
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac_traj_arithmetic.h"

using namespace std;
using namespace ibex;

namespace codac
{
  const Trajectory operator+(const Trajectory& x)
  {
    return x;
  }

  const Trajectory operator-(const Trajectory& x)
  {
    assert(x.definition_type() == TrajDefnType::MAP_OF_VALUES
      && "not supported yet for trajectories defined by a Function");

    map<double,double> map_y = x.sampled_map();

    for(map<double,double>::iterator it = map_y.begin() ;
      it != map_y.end() ; it++)
      it->second = -it->second;

    return Trajectory(map_y);
  }
    
  #define macro_scal_unary(f) \
    \
    const Trajectory f(const Trajectory& x) \
    { \
      assert(x.definition_type() == TrajDefnType::MAP_OF_VALUES \
        && "not supported yet for trajectories defined by a Function"); \
      \
      map<double,double> map_y = x.sampled_map(); \
      \
      for(map<double,double>::iterator it = map_y.begin() ; \
        it != map_y.end() ; it++) \
        it->second = std::f(it->second); \
      \
      return Trajectory(map_y); \
    } \
    \

  macro_scal_unary(cos);
  macro_scal_unary(sin);
  macro_scal_unary(abs);
    
  const Trajectory sqr(const Trajectory& x)
  {
    assert(x.definition_type() == TrajDefnType::MAP_OF_VALUES
      && "not supported yet for trajectories defined by a Function");

    map<double,double> map_y = x.sampled_map();

    for(map<double,double>::iterator it = map_y.begin() ;
      it != map_y.end() ; it++)
      it->second = std::pow(it->second,2);

    return Trajectory(map_y);
  }

  macro_scal_unary(sqrt);
  macro_scal_unary(exp);
  macro_scal_unary(log);
  macro_scal_unary(tan);
  macro_scal_unary(acos);
  macro_scal_unary(asin);
  macro_scal_unary(atan);
  macro_scal_unary(cosh);
  macro_scal_unary(sinh);
  macro_scal_unary(tanh);
  macro_scal_unary(acosh);
  macro_scal_unary(asinh);
  macro_scal_unary(atanh);

  #define macro_scal_unary_param(f, p) \
    \
    const Trajectory f(const Trajectory& x, p param) \
    { \
      assert(x.definition_type() == TrajDefnType::MAP_OF_VALUES && \
        "not supported yet for trajectories defined by a Function"); \
      \
      map<double,double> map_y = x.sampled_map(); \
      \
      for(map<double,double>::iterator it = map_y.begin() ; \
        it != map_y.end() ; it++) \
        it->second = std::f(it->second, param); \
      \
      return Trajectory(map_y); \
    } \
    \
  
  macro_scal_unary_param(pow, int);
  macro_scal_unary_param(pow, double);

  const Trajectory root(const Trajectory& x, int p)
  {
    assert(x.definition_type() == TrajDefnType::MAP_OF_VALUES &&
      "not supported yet for trajectories defined by a Function");

    map<double,double> map_y = x.sampled_map();
    for(map<double,double>::iterator it = map_y.begin() ;
      it != map_y.end() ; it++)
      it->second = std::pow(it->second, 1. / p);

    return Trajectory(map_y);
  }

  #define macro_scal_binary_arith(f) \
    \
    const Trajectory operator f(const Trajectory& x1, const Trajectory& x2) \
    { \
      assert(x1.tdomain() == x2.tdomain()); \
      assert(!(x1.definition_type() == TrajDefnType::ANALYTIC_FNC && x2.definition_type() == TrajDefnType::ANALYTIC_FNC) && \
        "not supported yet for two trajectories defined by a Function"); \
      \
      Trajectory x1_sampled(x1), x2_sampled(x2); \
      if(x2.definition_type() == TrajDefnType::MAP_OF_VALUES) \
        x1_sampled.sample(x2); \
      if(x1.definition_type() == TrajDefnType::MAP_OF_VALUES) \
        x2_sampled.sample(x1); \
      map<double,double> new_map; \
      map<double,double>::const_iterator it_x1 = x1_sampled.sampled_map().begin(); \
      map<double,double>::const_iterator it_x2 = x2_sampled.sampled_map().begin(); \
      \
      while(it_x1 != x1_sampled.sampled_map().end()) \
      { \
        new_map[it_x1->first] = it_x1->second f it_x2->second; \
        it_x1++; it_x2++; \
      } \
      \
      return Trajectory(new_map); \
    } \
    \
    const Trajectory operator f(const Trajectory& x1, double x2) \
    { \
      assert(x1.definition_type() == TrajDefnType::MAP_OF_VALUES && \
        "not supported yet for trajectories defined by a Function"); \
      \
      Trajectory y(x1); \
      map<double,double> map_y = y.sampled_map(); \
      \
      for(map<double,double>::iterator it = map_y.begin() ; \
        it != map_y.end() ; it++) \
        it->second = it->second f x2; \
      \
      return Trajectory(map_y); \
    } \
    \
    const Trajectory operator f(double x1, const Trajectory& x2) \
    { \
      assert(x2.definition_type() == TrajDefnType::MAP_OF_VALUES && \
        "not supported yet for trajectories defined by a Function"); \
      \
      Trajectory y(x2); \
      map<double,double> map_y = y.sampled_map(); \
      \
      for(map<double,double>::iterator it = map_y.begin() ; \
        it != map_y.end() ; it++) \
        it->second = x1 f it->second; \
      \
      return Trajectory(map_y); \
    } \
    \

  macro_scal_binary_arith(+);
  macro_scal_binary_arith(-);
  macro_scal_binary_arith(*);
  macro_scal_binary_arith(/);

  #define macro_scal_binary_f(f) \
    \
    const Trajectory f(const Trajectory& x1, const Trajectory& x2) \
    { \
      assert(x1.tdomain() == x2.tdomain()); \
      assert(!(x1.definition_type() == TrajDefnType::ANALYTIC_FNC && x2.definition_type() == TrajDefnType::ANALYTIC_FNC) && \
        "not supported yet for trajectories defined by a Function"); \
    \
      Trajectory x1_sampled(x1), x2_sampled(x2); \
      if(x2.definition_type() == TrajDefnType::MAP_OF_VALUES) \
        x1_sampled.sample(x2); \
      if(x1.definition_type() == TrajDefnType::MAP_OF_VALUES) \
        x2_sampled.sample(x1); \
      map<double,double> map_x1 = x1.sampled_map(), map_x2 = x2.sampled_map(); \
    \
      map<double,double>::iterator it_x1 = map_x1.begin(); \
      map<double,double>::iterator it_x2 = map_x2.begin(); \
    \
      while(it_x1 != map_x1.end()) \
      { \
        it_x1->second = std::f(it_x1->second, it_x2->second); \
        it_x1++; it_x2++; \
      } \
    \
      return Trajectory(map_x1); \
    } \
    \
    const Trajectory f(const Trajectory& x1, double x2) \
    { \
      assert(x1.definition_type() == TrajDefnType::MAP_OF_VALUES && \
        "not supported yet for trajectories defined by a Function"); \
    \
      Trajectory y(x1); \
      map<double,double> map_y = y.sampled_map(); \
    \
      for(map<double,double>::iterator it = map_y.begin() ; \
        it != map_y.end() ; it++) \
        it->second = std::f(it->second, x2); \
    \
      return Trajectory(map_y); \
    } \
    \
    const Trajectory f(double x1, const Trajectory& x2) \
    { \
      assert(x2.definition_type() == TrajDefnType::MAP_OF_VALUES && \
        "not supported yet for trajectories defined by a Function"); \
    \
      Trajectory y(x2); \
      map<double,double> map_y = y.sampled_map(); \
    \
      for(map<double,double>::iterator it = map_y.begin() ; \
        it != map_y.end() ; it++) \
        it->second = std::f(x1, it->second); \
    \
      return Trajectory(map_y); \
    } \

  macro_scal_binary_f(atan2);
  macro_scal_binary_f(min);
  macro_scal_binary_f(max);
}