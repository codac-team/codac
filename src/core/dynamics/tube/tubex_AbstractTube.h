/* ============================================================================
 *  tubex-lib - AbstractTube class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_ABSTRACTTUBE_H__
#define __TUBEX_ABSTRACTTUBE_H__

#include "tubex_DynamicalItem.h"

namespace tubex
{
  class AbstractTube : public DynamicalItem
  {
    public:

      static bool share_same_slicing(const AbstractTube& x1, const AbstractTube& x2);
  };
}

#endif