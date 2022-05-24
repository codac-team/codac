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

#ifndef __CODAC2_TUBEVECTORCOMPONENT_H__
#define __CODAC2_TUBEVECTORCOMPONENT_H__

#include <memory>
#include "codac_Interval.h"
#include "codac2_TubeVector.h"

namespace codac2
{
  class TubeVector_;

  /*template<typename T_TubeVector>
  class TubeVectorComponent// : public DomainItem_<Trajectory_>, public TemporalItem_
  {
    public:

      const Interval_& tdomain() const;

      bool contains(const Trajectory_& value) const;

      void set_codomain(const Interval_& codomain);
      

    protected:

      friend class TubeVector_;
      explicit TubeVectorComponent(T_TubeVector& tubevector, size_t i);

      size_t m_i;
      T_TubeVector& m_tubevector;

  };*/
} // namespace codac

#endif