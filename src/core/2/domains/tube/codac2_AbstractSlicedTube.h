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

      AbstractSlicedTube(TDomain& tdomain);

      virtual const std::shared_ptr<AbstractSlice>& first_abstract_slice() const = 0;
      virtual const std::shared_ptr<AbstractSlice>& last_abstract_slice() const = 0;

      TDomain& tdomain() const;
      Interval t0_tf() const;


    protected:

      TDomain& _tdomain;
  };
} // namespace codac

#endif