#ifndef __CODAC2_VAR__
#define __CODAC2_VAR__

#include <initializer_list>
#include <codac2_IntervalVector.h>
#include <codac2_IntervalMatrix.h>

namespace codac2
{
  class VarBase
  {
    public:

      VarBase();
      virtual ~VarBase() = default;
  };

  template<typename T>
  class Var_ : public T, public VarBase
  {
    public:

      Var_(T&& x) : T(x), _reset_value(std::forward<T>(x))
      { }

      Var_& operator=(const Var_<T>& v)
      {
        // This method may be used for DomainCaster
        T::operator=(v);
        // _reset_var is not updated
        return *this;
      }

      void set_value(const T& v)
      {
        T::operator=(v);
      }

      void reset_to_default_value()
      {
        T::operator=(_reset_value);
      }

    protected:

      const T _reset_value;
  };

  template<typename T>
  class Var : public Var_<T>
  {
    public:

      Var() : Var_<T>(T())
      { }

      template<typename... Args>
      Var(const Args&... x) : Var_<T>(T(x...))
      { }
  };

  // Some template specializations are performed for specific arguments that cannot be deduced, 
  // such as initializer_lists.

  template<>
  class Var<IntervalVector> : public Var_<IntervalVector>
  {
    public:

      Var(std::initializer_list<Interval> l) : Var_<IntervalVector>(IntervalVector(l))
      { }

      template<typename... Args>
      Var(const Args&... x) : Var_<IntervalVector>(IntervalVector(x...))
      { }
  };

  template<>
  class Var<IntervalMatrix> : public Var_<IntervalMatrix>
  {
    public:

      Var(std::initializer_list<std::initializer_list<Interval>> l) : Var_<IntervalMatrix>(IntervalMatrix(l))
      { }

      template<typename... Args>
      Var(const Args&... x) : Var_<IntervalMatrix>(IntervalMatrix(x...))
      { }
  };
}

#endif