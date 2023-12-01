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
  };

  template<typename T>
  class Var : public T, public VarBase
  {
    public:
      
      Var()
      {

      }
  };
}

#endif