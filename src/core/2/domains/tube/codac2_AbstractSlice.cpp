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

#include "codac2_AbstractSlice.h"
#include "codac2_TSlice.h"
#include "codac_Exception.h"

using namespace std;

namespace codac2
{
  AbstractSlice::AbstractSlice(const AbstractSlicedTube& tubevector, const list<TSlice>::iterator& it_tslice) :
    _tubevector(tubevector), _it_tslice(it_tslice)
  {

  }

  const Interval& AbstractSlice::t0_tf() const
  {
    return _it_tslice->t0_tf();
  }

  const TSlice& AbstractSlice::tslice() const
  {
    return *_it_tslice;
  }

  const std::shared_ptr<AbstractSlice> AbstractSlice::prev_abstract_slice_ptr() const
  {
    if(&(*_tubevector.first_abstract_slice_ptr()) == this)
      return nullptr;
    return prev(_it_tslice)->slices().at(&_tubevector);
  }

  const std::shared_ptr<AbstractSlice> AbstractSlice::next_abstract_slice_ptr() const
  {
    if(&(*_tubevector.last_abstract_slice_ptr()) == this)
      return nullptr;
    return next(_it_tslice)->slices().at(&_tubevector);
  }

} // namespace codac