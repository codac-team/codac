/** 
 *  Serialization tools (intervals)
 * ----------------------------------------------------------------------------
 *  \date       2016
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac_serialize_intervals.h"
#include "codac_Exception.h"

using namespace std;
using namespace ibex;

namespace codac
{
  enum class IntervalType { BOUNDED, EMPTY_SET, ALL_REALS, POS_REALS, NEG_REALS };

  void serialize_Interval(ofstream& bin_file, const Interval& intv)
  {
    if(!bin_file.is_open())
      throw Exception(__func__, "ofstream& bin_file not open");

    IntervalType intv_type;

    if(intv == Interval::EMPTY_SET)
      intv_type = IntervalType::EMPTY_SET;

    else if(intv == Interval::ALL_REALS)
      intv_type = IntervalType::ALL_REALS;

    else if(intv == Interval::POS_REALS)
      intv_type = IntervalType::POS_REALS;

    else if(intv == Interval::NEG_REALS)
      intv_type = IntervalType::NEG_REALS;

    else // at least one bound is set, e.g. [5,oo], or [-oo,-1]
      intv_type = IntervalType::BOUNDED;

    bin_file.write((const char*)&intv_type, sizeof(IntervalType));

    // todo: what about [5,oo]?

    if(intv_type == IntervalType::BOUNDED)
    {
      double lb = intv.lb(), ub = intv.ub();
      bin_file.write((const char*)&lb, sizeof(double));
      bin_file.write((const char*)&ub, sizeof(double));
    }
  }

  void deserialize_Interval(ifstream& bin_file, Interval& intv)
  {
    if(!bin_file.is_open())
      throw Exception(__func__, "ifstream& bin_file not open");

    IntervalType intv_type;
    bin_file.read((char*)&intv_type, sizeof(IntervalType));

    switch(intv_type)
    {
      case IntervalType::EMPTY_SET:
        intv = Interval::EMPTY_SET;
        break;

      case IntervalType::ALL_REALS:
        intv = Interval::ALL_REALS;
        break;

      case IntervalType::POS_REALS:
        intv = Interval::POS_REALS;
        break;

      case IntervalType::NEG_REALS:
        intv = Interval::NEG_REALS;
        break;

      case IntervalType::BOUNDED: // at least one bound is set, e.g. [5,oo], or [-oo,-1]
        double lb, ub;
        bin_file.read((char*)&lb, sizeof(double));
        bin_file.read((char*)&ub, sizeof(double));
        intv = Interval(lb, ub);
        break;

      default:
        throw Exception(__func__, "unhandled case");
    }
  }

  void serialize_IntervalVector(ofstream& bin_file, const IntervalVector& box)
  {
    if(!bin_file.is_open())
      throw Exception(__func__, "ofstream& bin_file not open");

    short int size = box.size();
    bin_file.write((const char*)&size, sizeof(short int));
    for(int i = 0 ; i < size ; i++)
      serialize_Interval(bin_file, box[i]);
  }

  void deserialize_IntervalVector(ifstream& bin_file, IntervalVector& box)
  {
    if(!bin_file.is_open())
      throw Exception(__func__, "ifstream& bin_file not open");

    short int size;
    bin_file.read((char*)&size, sizeof(short int));
    box = IntervalVector(size);
    for(int i = 0 ; i < size ; i++)
      deserialize_Interval(bin_file, box[i]);
  }
}