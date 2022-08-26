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

#ifndef __CODAC2_TUBECOMPONENT_H__
#define __CODAC2_TUBECOMPONENT_H__

template<class T>
class TubeComponent //: public AbstractConstTube<Interval,TubeComponent<T>>
{
  protected:

    TubeComponent(Tube<T>& tubevector, size_t i) :
      _i(i), _tubevector(tubevector)
    {
      assert(i >= 0 && i < tubevector.size());
    }

  public:

    TubeComponent(const TubeComponent<T>& tubevector_i) :
      _i(tubevector_i._i), _tubevector(tubevector_i._tubevector)
    {

    }
    
    size_t size() const
    {
      return 1;
    }

    const std::shared_ptr<TDomain>& tdomain() const
    {
      return _tubevector.tdomain();
    }
    
    Interval t0_tf() const
    {
      return _tubevector.t0_tf();
    }
    
    Interval codomain() const
    {
      Interval codomain(Interval::EMPTY_SET);
      for(const auto& s : _tubevector)
        codomain |= s.codomain()[_i];
      return codomain;
    }

    bool contains(const Trajectory& value) const
    {
      assert(false);
      return true;
    }
    
    void set(const Interval& codomain)
    {
      for(auto& s : _tubevector)
        s.set_component(_i, codomain);
    }

    const TubeComponent<T>& operator=(const TubeComponent<T>& x)
    {
      assert(x.tdomain() == tdomain());
      for(auto& s : _tubevector)
        s.set_component(_i, std::static_pointer_cast<Slice<T>>(s._it_tslice->_slices.at(&x._tubevector))->codomain()[x._i]);
      return *this;
    }

    const TubeComponent<T>& operator=(std::pair<std::function<Interval(const Interval&)>,const TubeComponent<T>> rel)
    {
      assert(rel.second.tdomain() == tdomain());
      for(auto& s : _tubevector)
        s.set_component(_i, rel.first(std::static_pointer_cast<Slice<T>>(s._it_tslice->_slices.at(&rel.second._tubevector))->codomain()[rel.second._i]));
      return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const TubeComponent<T>& x)
    {
      os << "Component " << x._i << " of: " << x._tubevector << std::flush;
      return os;
    }

    std::pair<std::function<Interval(const Interval&)>,const TubeComponent<T>> cos(const TubeComponent<T>& x)
    {
      return std::make_pair(static_cast<Interval(*)(const Interval&)>(ibex::cos), x);
    }
    
    codac::Tube to_codac1() const
    {
      codac::Tube x(t0_tf());
      for(const auto& s : _tubevector)
        if(!s.t0_tf().is_unbounded())
          x.set(s.codomain()[_i], s.t0_tf());
      for(const auto& s : _tubevector) // setting gate (were overwritten)
        if(s.t0_tf().is_degenerated())
          x.set(s.codomain()[_i], s.t0_tf());
      return x;
    }
    

  protected:

    size_t _i;
    Tube<T>& _tubevector;

    template<typename U>
    friend class Tube;
};

template<class T>
std::pair<std::function<Interval(const Interval&)>,const TubeComponent<T>> cos(const TubeComponent<T>& x);

#endif