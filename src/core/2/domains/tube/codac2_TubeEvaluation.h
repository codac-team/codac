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

namespace codac2
{
  template<class T>
  class TubeEvaluation
  {
    public:

      TubeEvaluation& operator=(const T& x)
      {
        // Sampling the tube only if affectation is performed
        // (i.e. this is not done in the constructor)
        std::list<TSlice>::iterator it_lb = _tube->tdomain()->sample(_t.lb(), _t.is_degenerated());
        std::list<TSlice>::iterator it_ub;
        
        if(!_t.is_degenerated())
        {
          it_ub = _tube->tdomain()->sample(_t.ub(), false);
          it_ub--; // pointing to the tslice [..,_t.ub()]

          if(it_lb->t0_tf().ub() == _t.lb())
            it_lb++;
        }

        else
          it_ub = it_lb;

        do
        {
          _tube->operator()(it_lb).set(x);
        } while(it_lb != it_ub && (++it_lb) != _tube->tdomain()->tslices().end());

        return *this;
      }

      explicit operator T() const
      {
        return _tube->eval(_t);
      }

      friend std::ostream& operator<<(std::ostream& os, const TubeEvaluation<T>& x)
      {
        os << x._tube->eval(x._t) << std::flush;
        return os;
      }
      

    protected:

      explicit TubeEvaluation(Tube<T> *tubevector, double t) :
        _t(Interval(t)), _tube(tubevector)
      {

      }

      explicit TubeEvaluation(Tube<T> *tubevector, const Interval& t) :
        _t(t), _tube(tubevector)
      {

      }

      const Interval _t;
      Tube<T>* _tube;
      template<typename U>
      friend class Tube;
  };

  template<class T>
  class ConstTubeEvaluation
  {
    public:

      explicit operator T() const
      {
        return _tube->eval(_t);
      }

      friend std::ostream& operator<<(std::ostream& os, const ConstTubeEvaluation<T>& x)
      {
        os << x._tube->eval(x._t) << std::flush;
        return os;
      }
      

    protected:

      explicit ConstTubeEvaluation(const Tube<T> *tubevector, double t) :
        _t(Interval(t)), _tube(tubevector)
      {

      }

      explicit ConstTubeEvaluation(const Tube<T> *tubevector, const Interval& t) :
        _t(t), _tube(tubevector)
      {

      }

      const Interval _t;
      const Tube<T>* _tube;
      template<typename U>
      friend class Tube;
  };
}

#endif