#ifndef __CODAC2_VAR__
#define __CODAC2_VAR__

#include <iostream>
#include <vector>
#include <memory>
#include <functional>
#include "codac2_Domain.h"

namespace codac2
{
  class VarBase
  {
    public:
      
      virtual ~VarBase() = default;
      VarBase& operator=(const VarBase& x) = delete;
  };

  template<typename T>
  class Var : public T, public VarBase
  {
    public:
      
      Var()
      {

      }
      
      Var(const T& initial_value)
        : _initial_value(initial_value)
      {

      }

      void reset()
      {
        this->T::operator=(_initial_value);
      }

      void reset(const T& x)
      {
        this->T::operator=(x);
      }

    protected:

      T _initial_value;
  };
}

#endif