/** 
 *  @file
 *  Serialization tools
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2017
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_SERIALIZ_INTERVALS_H__
#define __TUBEX_SERIALIZ_INTERVALS_H__

#include <fstream>
#include "ibex_Interval.h"
#include "ibex_IntervalVector.h"

namespace tubex
{
  /**
   * \brief Writes an Interval object into a binary file
   * 
   * Interval binary structure: <br>
   *   [char_intv_type] <br>
   *   [double_lb] <br>
   *   [double_ub]
   * 
   * char_intv_type refers the type of Interval:
   *   either BOUNDED, EMPTY_SET, ALL_REALS, POS_REALS, NEG_REALS.
   *
   * In case of unbounded intervals, the two last fields disappear.
   *
   * \param bin_file binary file (ofstream object)
   * \param intv Interval object to be serialized
   */
  void serialize_Interval(std::ofstream& bin_file, const ibex::Interval& intv);

  /**
   * \brief Creates an Interval object from a binary file.
   *
   * The binary file has to be written by the serialize_Interval() function.
   *
   * \param bin_file binary file (ifstream object)
   * \param intv Interval object to be deserialized
   */
  void deserialize_Interval(std::ifstream& bin_file, ibex::Interval& intv);

  /**
   * \brief Writes an IntervalVector object into a binary file
   * 
   * IntervalVector binary structure: <br>
   *   [short_int_size] <br>
   *   [Interval_1] <br>
   *   ... <br>
   *   [Interval_n]
   *
   * \param bin_file binary file (ofstream object)
   * \param box IntervalVector object to be serialized
   */
  void deserialize_IntervalVector(std::ifstream& bin_file, ibex::IntervalVector& box);

  /**
   * \brief Creates an IntervalVector object from a binary file.
   *
   * The binary file has to be written by the serialize_IntervalVector() function.
   *
   * \param bin_file binary file (ifstream object)
   * \param box IntervalVector object to be deserialized
   */
  void serialize_IntervalVector(std::ofstream& bin_file, const ibex::IntervalVector& box);
}

#endif