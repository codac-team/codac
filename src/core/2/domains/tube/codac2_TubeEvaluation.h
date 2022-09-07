/** 
 *  \file
 *  
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Simon Rohou
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_TUBEEVALUATION_H__
#define __CODAC2_TUBEEVALUATION_H__

template<class T>
class TubeEvaluation
{
  public:

    const TubeEvaluation& operator=(const T& x)
    {
      // Sampling the tube only if affectation is performed
      // (i.e. this is not done in the constructor)
      std::list<TSlice>::iterator it_lb = _tubevector->_tdomain->sample(_t.lb(), false);
      std::list<TSlice>::iterator it_ub = _tubevector->_tdomain->sample(_t.ub(), _t.is_degenerated());

      do
      {
        std::static_pointer_cast<Slice<T>>(it_lb->_slices.at(_tubevector))->set(x);
        it_lb++;
      } while(it_lb != it_ub);

      return *this;
    }

    explicit operator T()
    {
      return _tubevector->eval(_t);
    }

    friend std::ostream& operator<<(std::ostream& os, const TubeEvaluation<T>& x)
    {
      os << x._tubevector->eval(x._t) << std::flush;
      return os;
    }
    

  protected:

    explicit TubeEvaluation(const Tube<T>* tubevector, double t) :
      _t(Interval(t)), _tubevector(tubevector)
    {

    }

    explicit TubeEvaluation(const Tube<T>* tubevector, const Interval& t) :
      _t(t), _tubevector(tubevector)
    {

    }

    const Interval _t;
    const Tube<T>* _tubevector;
};

#endif