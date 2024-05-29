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

namespace codac2
{
  class Ctc
  {
    public:

      virtual std::shared_ptr<Ctc> copy() const = 0;
  };

  template<typename X_>
  class Ctc_ : virtual public Ctc
  {
    public:
    
      using X = X_;

      Ctc_(size_t n)
        : _n(n)
      { }

      size_t size() const
      {
        return _n;
      }
      
      virtual void contract(X_& x) const = 0;

    protected:

      const size_t _n;
  };
}