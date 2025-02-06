/** 
 *  \file codac2_Ctc.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <memory>
#include <iostream>
#include "codac2_Index.h"
#include "codac2_assert.h"

namespace codac2
{
  //class Ctc
  //{
  //  public:
  //
  //    //virtual std::shared_ptr<Ctc> copy() const = 0;
  //};
  //template<typename... X>
  //class Ctc
  //{
  //  public:
  //
  //    virtual std::shared_ptr<Ctc<X...>> copy() const
  //    {
  //      return nullptr;
  //    }
  //    
  //    virtual void contract(X&... x) const = 0;
  //};

  template<typename X>
  class CtcBase// : virtual public Ctc<X_>
  {
    public:
    
      using ContractedType = X;

      CtcBase(Index n)
        : _n(n)
      {
        assert(n > 0);
      }

      Index size() const
      {
        return _n;
      }
      
      virtual void contract(X& x) const = 0;

      virtual std::shared_ptr<CtcBase<X>> copy() const = 0;

    protected:

      const Index _n;
  };

  template<typename C,typename X_>
  class Ctc : public CtcBase<X_>
  {
    public:
    
      Ctc(Index n)
        : CtcBase<X_>(n)
      { }

      virtual std::shared_ptr<CtcBase<X_>> copy() const
      {
        return std::make_shared<C>(*dynamic_cast<const C*>(this));
      }
  };
}