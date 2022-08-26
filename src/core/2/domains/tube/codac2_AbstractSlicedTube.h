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

#ifndef __CODAC2_ABSTRACTTUBE_H__
#define __CODAC2_ABSTRACTTUBE_H__

#include "codac2_TDomain.h"

namespace codac2
{
  class AbstractSlice;

  class AbstractSlicedTube
  {
    public:

      AbstractSlicedTube(const std::shared_ptr<TDomain>& tdomain);

      virtual const std::shared_ptr<AbstractSlice>& first_abstract_slice_ptr() const = 0;
      virtual const std::shared_ptr<AbstractSlice>& last_abstract_slice_ptr() const = 0;

      const std::shared_ptr<TDomain>& tdomain() const;
      Interval t0_tf() const;


    protected:

      std::shared_ptr<TDomain> _tdomain;
  };
} // namespace codac

#endif