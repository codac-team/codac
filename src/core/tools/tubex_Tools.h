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
       * \Note Same signature as boost::algorithm::replace_all
       *
       * \param input an input string
       * \param search a substring to be searched for 
       * \param format a substitute string
       */
      static void replace_all(std::string& input, const std::string& search, const std::string& format);
      
      /**
       * \brief Adds an integer suffix to a string
       *
       * \param name the string to be updated
       * \param id the integer to add
       * \param sep string separator between the name and the integer ("_" by default)
       * \return the string value `string`+`sep`+`id`
       */
      static std::string add_int(const std::string& name, int id, const std::string& sep = "_");
      
      /**
       * \brief Adds an integer suffix to a string between separators
       *
       * \param name the string to be updated
       * \param sep1 string separator before the integer
       * \param id the integer to add
       * \param sep2 string separator after the integer
       * \return the string value `string`+`sep1`+`id`+`sep2`
       */
      static std::string add_int(const std::string& name, const std::string& sep1, int id, const std::string& sep2);
  };
}

#endif