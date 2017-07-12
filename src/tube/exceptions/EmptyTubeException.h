/* ============================================================================
 *  tubex-lib - EmptyTubeException class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU General Public License (GPL). See the file LICENSE.
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef EMPTYTUBEEXCEPTION_HEADER
#define EMPTYTUBEEXCEPTION_HEADER

#include <iostream>
#include <exception>
#include <string>
#include <sstream>
#include "Tube.h"
#include "TubeException.h"

namespace tubex
{
  /**
   * \brief Emptiness error exception.
   *
   * Thrown when a tube is empty.
   */
  class EmptyTubeException : public TubeException
  {
    public:

      EmptyTubeException(const Tube& x);
  };

  void checkEmptiness(const Tube& x);
}

#endif