/** 
 *  \file
 *  Tools
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_TOOLS_H__
#define __TUBEX_TOOLS_H__

#include <string>
#include "ibex_Interval.h"

namespace tubex
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
       */
      static void replace_all(std::string& input, const std::string& search, const std::string& format);
      
      /**
       * \brief Adds an integer suffix to a string
       *
       * \param str the string to be updated
       * \param id the integer to add
       * \param sep string separator between the str and the integer ("_" by default)
       * \return the string value `string`+`sep`+`id`
       */
      static std::string add_int(const std::string& str, int id, const std::string& sep = "_");
      
      /**
       * \brief Adds an integer suffix to a string between separators
       *
       * \param str the string to be updated
       * \param sep1 string separator before the integer
       * \param id the integer to add
       * \param sep2 string separator after the integer
       * \return the string value `string`+`sep1`+`id`+`sep2`
       */
      static std::string add_int(const std::string& str, const std::string& sep1, int id, const std::string& sep2);
      
      /**
       * \brief Returns a random number inside an interval
       *
       * \param intv the bounds
       * \return a random double
       */
      static double rand_in_bounds(const ibex::Interval& intv);
  };
}

#endif