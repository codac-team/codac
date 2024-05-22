/** 
 *  \file
 *  Operators
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include <memory>
#include "codac2_set_variables.h"
#include "codac2_SepInter.h"
#include "codac2_SepUnion.h"
#include "codac2_SepProj.h"
#include "codac2_SepCartProd.h"
#include "codac2_SepInverse.h"
#include "codac2_SepNot.h"

namespace codac2
{
  struct InterSetOp
  {
    static std::shared_ptr<Sep> create_sep(const std::shared_ptr<Sep>& s1, const std::shared_ptr<Sep>& s2)
    {
      return std::make_shared<SepInter>(s1,s2);
    }
  };

  struct UnionSetOp
  {
    static std::shared_ptr<Sep> create_sep(const std::shared_ptr<Sep>& s1, const std::shared_ptr<Sep>& s2)
    {
      return std::make_shared<SepUnion>(s1,s2);
    }
  };

  struct ProjSetOp
  {
    static std::shared_ptr<Sep> create_sep(const std::shared_ptr<Sep>& s1, const IntervalVector& y)
    {
      return std::make_shared<SepProj>(s1,y);
    }
  };

  struct CartProdSetOp
  {
    static std::shared_ptr<Sep> create_sep(const std::shared_ptr<Sep>& s1, const std::shared_ptr<Sep>& s2)
    {
      return std::make_shared<SepCartProd>(s1,s2);
    }
  };

  struct InverseSetOp
  {
    static std::shared_ptr<Sep> create_sep(const AnalyticFunction<VectorOpValue>& f, const std::shared_ptr<Sep>& s1)
    {
      return std::make_shared<SepInverse<IntervalVector>>(f,s1);
    }
  };

  struct NotSetOp
  {
    static std::shared_ptr<Sep> create_sep(const std::shared_ptr<Sep>& s1)
    {
      return std::make_shared<SepNot>(s1);
    }
  };
}