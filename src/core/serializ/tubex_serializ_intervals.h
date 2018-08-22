/* ============================================================================
 *  tubex-lib - Serialization tools
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
   * \brief Write an Interval object into a binary file.
   * 
   * Interval binary structure
   *   format: [char_intv_type][double_lb][double_ub]
   *   char_intv_type refers the type of Interval:
   *   either BOUNDED, EMPTY_SET, ALL_REALS, POS_REALS, NEG_REALS.
   *   In case of unbounded intervals, the two last fields disappear.
   *
   * \param bin_file binary file (ofstream object)
   * \param intv Interval object to be serialized
   */
  void serialize_Interval(std::ofstream& bin_file, const ibex::Interval& intv);

  /**
   * \brief Create an Interval object from a binary file.
   *
   * The binary file has to be written by the serialize_interval() function.
   *
   * \param bin_file binary file (ifstream object)
   * \param intv Interval object to be deserialized
   */
  void deserialize_Interval(std::ifstream& bin_file, ibex::Interval& intv);
  
  void deserialize_IntervalVector(std::ifstream& bin_file, ibex::IntervalVector& box);
  void serialize_IntervalVector(std::ofstream& bin_file, const ibex::IntervalVector& box);
}

#endif