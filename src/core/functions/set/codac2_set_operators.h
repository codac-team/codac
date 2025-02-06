/** 
 *  \file codac2_set_operators.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <stdexcept>
#include <memory>
#include "codac2_set_variables.h"
#include "codac2_CtcInter.h"
#include "codac2_SepInter.h"
#include "codac2_CtcUnion.h"
#include "codac2_SepUnion.h"
#include "codac2_SepProj.h"
#include "codac2_CtcCartProd.h"
#include "codac2_SepCartProd.h"
#include "codac2_CtcInverse.h"
#include "codac2_SepInverse.h"
#include "codac2_CtcNot.h"
#include "codac2_SepNot.h"
#include "codac2_CtcAction.h"
#include "codac2_SepAction.h"

namespace codac2
{
  struct InterSetOp
  {
    static std::shared_ptr<CtcBase<IntervalVector>> create_ctc(const std::shared_ptr<CtcBase<IntervalVector>>& s1, const std::shared_ptr<CtcBase<IntervalVector>>& s2)
    {
      return std::make_shared<CtcInter<IntervalVector>>(s1,s2);
    }

    static std::shared_ptr<SepBase> create_sep(const std::shared_ptr<SepBase>& s1, const std::shared_ptr<SepBase>& s2)
    {
      return std::make_shared<SepInter>(s1,s2);
    }
  };

  struct UnionSetOp
  {
    static std::shared_ptr<CtcBase<IntervalVector>> create_ctc(const std::shared_ptr<CtcBase<IntervalVector>>& s1, const std::shared_ptr<CtcBase<IntervalVector>>& s2)
    {
      return std::make_shared<CtcUnion<IntervalVector>>(s1,s2);
    }

    static std::shared_ptr<SepBase> create_sep(const std::shared_ptr<SepBase>& s1, const std::shared_ptr<SepBase>& s2)
    {
      return std::make_shared<SepUnion>(s1,s2);
    }
  };

  struct ProjSetOp
  {
    static std::shared_ptr<CtcBase<IntervalVector>> create_ctc(const std::shared_ptr<CtcBase<IntervalVector>>& s1, const std::vector<Index>& proj_indices, double eps)
    {
      throw std::logic_error("CtcProj not yet available");
      return nullptr;
    }

    static std::shared_ptr<CtcBase<IntervalVector>> create_ctc(const std::shared_ptr<CtcBase<IntervalVector>>& s1, const std::vector<Index>& proj_indices, const IntervalVector& y, double eps)
    {
      throw std::logic_error("CtcProj not yet available");
      return nullptr;
    }

    static std::shared_ptr<SepBase> create_sep(const std::shared_ptr<SepBase>& s1, const std::vector<Index>& proj_indices, double eps)
    {
      return std::make_shared<SepProj>(s1,proj_indices,eps);
    }

    static std::shared_ptr<SepBase> create_sep(const std::shared_ptr<SepBase>& s1, const std::vector<Index>& proj_indices, const IntervalVector& y, double eps)
    {
      return std::make_shared<SepProj>(s1,proj_indices,y,eps);
    }
  };

  struct CartProdSetOp
  {
    static std::shared_ptr<CtcBase<IntervalVector>> create_ctc(const std::shared_ptr<CtcBase<IntervalVector>>& s1, const std::shared_ptr<CtcBase<IntervalVector>>& s2)
    {
      return std::make_shared<CtcCartProd>(s1,s2);
    }

    static std::shared_ptr<SepBase> create_sep(const std::shared_ptr<SepBase>& s1, const std::shared_ptr<SepBase>& s2)
    {
      return std::make_shared<SepCartProd>(s1,s2);
    }
  };

  struct InverseSetOp
  {
    static std::shared_ptr<CtcBase<IntervalVector>> create_ctc(const AnalyticFunction<VectorType>& f, const std::shared_ptr<CtcBase<IntervalVector>>& s1)
    {
      return std::make_shared<CtcInverse_<IntervalVector,IntervalVector>>(f,s1);
    }

    static std::shared_ptr<SepBase> create_sep(const AnalyticFunction<VectorType>& f, const std::shared_ptr<SepBase>& s1)
    {
      return std::make_shared<SepInverse>(f,s1);
    }
  };

  struct NotSetOp
  {
    static std::shared_ptr<CtcBase<IntervalVector>> create_ctc(const std::shared_ptr<CtcBase<IntervalVector>>& s1)
    {
      return std::make_shared<CtcNot>(s1);
    }

    static std::shared_ptr<SepBase> create_sep(const std::shared_ptr<SepBase>& s1)
    {
      return std::make_shared<SepNot>(s1);
    }
  };

  struct ActionSetOp
  {
    static std::shared_ptr<CtcBase<IntervalVector>> create_ctc(const OctaSym& a, const std::shared_ptr<CtcBase<IntervalVector>>& s1)
    {
      return std::make_shared<CtcAction>(s1,a);
    }

    static std::shared_ptr<SepBase> create_sep(const OctaSym& a, const std::shared_ptr<SepBase>& s1)
    {
      return std::make_shared<SepAction>(s1,a);
    }
  };
}