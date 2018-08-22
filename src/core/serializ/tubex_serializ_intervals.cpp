/* ============================================================================
 *  tubex-lib - Serialization tools
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2016
 * ---------------------------------------------------------------------------- */

#include "tubex_serializ_intervals.h"
#include "tubex_Exception.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  enum IntervalType { BOUNDED, EMPTY_SET, ALL_REALS, POS_REALS, NEG_REALS };

  void serialize_Interval(ofstream& bin_file, const Interval& intv)
  {
    if(!bin_file.is_open())
      throw Exception("serialize_Interval()", "ofstream& bin_file not open");

    char intv_type;

    if(intv == Interval::EMPTY_SET)
      intv_type = EMPTY_SET;

    else if(intv == Interval::ALL_REALS)
      intv_type = ALL_REALS;

    else if(intv == Interval::POS_REALS)
      intv_type = POS_REALS;

    else if(intv == Interval::NEG_REALS)
      intv_type = NEG_REALS;

    else
      intv_type = BOUNDED;

    bin_file.write((const char*)&intv_type, sizeof(char));

    if(intv_type == BOUNDED)
    {
      double lb = intv.lb(), ub = intv.ub();
      bin_file.write((const char*)&lb, sizeof(double));
      bin_file.write((const char*)&ub, sizeof(double));
    }
  }

  void deserialize_Interval(ifstream& bin_file, Interval& intv)
  {
    if(!bin_file.is_open())
      throw Exception("deserialize_Interval()", "ifstream& bin_file not open");

    char intv_type;
    bin_file.read((char*)&intv_type, sizeof(char));

    switch(intv_type)
    {
      case EMPTY_SET:
        intv = Interval::EMPTY_SET;
        break;

      case ALL_REALS:
        intv = Interval::ALL_REALS;
        break;

      case POS_REALS:
        intv = Interval::POS_REALS;
        break;

      case NEG_REALS:
        intv = Interval::NEG_REALS;
        break;

      case BOUNDED:
        double lb, ub;
        bin_file.read((char*)&lb, sizeof(double));
        bin_file.read((char*)&ub, sizeof(double));
        intv = Interval(lb, ub);
        break;

      default:
        throw Exception("deserialize_Interval()", "unhandled case");
    }
  }

  void serialize_IntervalVector(ofstream& bin_file, const IntervalVector& box)
  {
    if(!bin_file.is_open())
      throw Exception("serialize_IntervalVector()", "ofstream& bin_file not open");

    short int size = box.size();
    bin_file.write((const char*)&size, sizeof(short int));
    for(int i = 0 ; i < size ; i++)
      serialize_Interval(bin_file, box[i]);
  }

  void deserialize_IntervalVector(ifstream& bin_file, IntervalVector& box)
  {
    if(!bin_file.is_open())
      throw Exception("deserialize_IntervalVector()", "ifstream& bin_file not open");

    short int size;
    bin_file.read((char*)&size, sizeof(short int));
    box = IntervalVector(size);
    for(int i = 0 ; i < size ; i++)
      deserialize_Interval(bin_file, box[i]);
  }
}