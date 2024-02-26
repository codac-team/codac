/** 
 *  \file
 *  Tools
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_TOOLS_H__
#define __CODAC_TOOLS_H__

#include <string>
#include "codac_Interval.h"

namespace codac
{
  /**
   * \class Tools
   * \brief Basic features provided here in order to avoid overkill dependencies
   */
  class Tools
  {
    public:

      /**
       * \brief Replaces all occurrences of the search string in the input with the format string. The input sequence is modified in-place.
       *
       * \note Same signature as boost::algorithm::replace_all
       *
       * \param input an input string
       * \param search a substring to be searched for 
       * \param format a substitute string
       * \return void
       */
      static void replace_all(std::string& input, const std::string& search, const std::string& format);
      
      /**
       * \brief Trims a string (removes both leading and trailing whitespace from the string)
       *
       * \param str the string to trim
       * \return void
       */
      static void trim(std::string& str);

      /**
       * \brief Returns a random number inside an interval
       *
       * \param intv the bounds
       * \return a random double
       */
      static double rand_in_bounds(const Interval& intv);
  };
}

#endif