/** 
 *  \file codac2_geometry.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_BoolInterval.h"
#include "codac2_IntervalVector.h"

namespace codac2
{
  /**
   * \enum OrientationInterval
   * \brief Enumeration representing feasible orientations.
   * Can be used to assess an oriented angle, or the alignement of three points.
   * 
   * The logical operator ``|`` can be used to combine ``OrientationInterval`` values.
   */
  enum class OrientationInterval
  {
    COLINEAR = 0x01,
    CLOCKWISE = 0x02,
    COUNTERCLOCKWISE = 0x04,
    EMPTY = 0x00,
    /** ``UNKNOWN``: the orientation can be either clockwise, counter-clockwise, or aligned. */
    UNKNOWN = 0x01 | 0x02 | 0x04
  };

  constexpr OrientationInterval operator|(OrientationInterval a, OrientationInterval b)
  { return static_cast<OrientationInterval>(static_cast<int>(a) | static_cast<int>(b)); }

  /**
   * \brief Streams out a OrientationInterval
   * 
   * \param os The stream to be updated
   * \param x The orientation interval to stream out
   * \return A Reference to the updated stream
   */
  inline std::ostream& operator<<(std::ostream& os, const OrientationInterval& x)
  {
    if(x == OrientationInterval::EMPTY)
      os << "[ empty ]";
    else if(x == OrientationInterval::COLINEAR)
      os << "[ col ]";
    else if(x == OrientationInterval::CLOCKWISE)
      os << "[ cw ]";
    else if(x == OrientationInterval::COUNTERCLOCKWISE)
      os << "[ ccw ]";
    else if(x == (OrientationInterval::COLINEAR | OrientationInterval::CLOCKWISE))
      os << "[ col, cw ]";
    else if(x == (OrientationInterval::COLINEAR | OrientationInterval::COUNTERCLOCKWISE))
      os << "[ col, ccw ]";
    else if(x == OrientationInterval::UNKNOWN)
      os << "[ col, cw, ccw ]";
    return os;
  }

  /**
   * \brief Computes the orientation of an ordered triplet of 2D points.
   *
   * Determines whether the oriented angle \f$\widehat{p_1 p_2 p_3}\f$ is positive
   * (counterclockwise), negative (clockwise), or if the points are colinear (flat or 0 angle).
   * Depending on floating point uncertainties, the test may not be able to conclude
   * (a ``UNKNOWN`` value would then be returned).
   *
   * \param p1 First point (2d ``IntervalVector``) of the triplet.
   * \param p2 Second point (2d ``IntervalVector``) of the triplet (vertex of the angle).
   * \param p3 Third point (2d ``IntervalVector``) of the triplet.
   * 
   * \return An orientation of type ``OrientationInterval``
   */
  OrientationInterval orientation(const IntervalVector& p1, const IntervalVector& p2, const IntervalVector& p3);

  /**
   * \brief Checks whether three 2D points are aligned (colinear).
   *
   * Determines if the points lie on the same straight line using an 
   * orientation test (cross product). Depending on floating point uncertainties,
   * the test may not be able to conclude (a ``UNKNOWN`` value would then be returned).
   *
   * \param p1 First point (2d ``IntervalVector``) of the triplet.
   * \param p2 Second point (2d ``IntervalVector``) of the triplet.
   * \param p3 Third point (2d ``IntervalVector``) of the triplet.
   * 
   * \return A ``BooleanInterval``
   */
  BoolInterval aligned(const IntervalVector& p1, const IntervalVector& p2, const IntervalVector& p3);

  /**
   * \brief Computes the convex hull of a set of 2d points.
   *
   * Given a set of 2d points enclosed in tiny boxes, the function computes
   * their convex hull. The method is based on a Graham scan algorithm.
   * The output list of the algorithm is a subset of the input list,
   * with same uncertainties and a possible different order.
   * 
   * \param pts 2d points in any order
   * 
   * \return Points on the convex hull in counterclockwise order.
   */
  std::vector<IntervalVector> convex_hull(std::vector<IntervalVector> pts);

  /**
   * \brief Rotates a 2D interval vector around a point by a given angle.
   *
   * This function performs a 2D rotation of the input interval vector ``p`` by an interval angle ``a``,
   * optionally around a center of rotation ``c`` (default is the origin).
   * This operation propagates uncertainties through interval arithmetic.
   *
   * \param p The input 2D interval vector to be rotated.
   * \param a The rotation angle as an interval (in radians).
   * \param c The center of rotation as an interval vector (default is the origin (0,0)).
   * \return The rotated interval vector.
   */
  IntervalVector rotate(const IntervalVector& p, const Interval& a, const IntervalVector& c = Vector::zero(2));

  /**
   * \brief Rotates a 2D object (``Segment``, ``Polygon``, *etc*) around a point by a given angle.
   *
   * This function performs a 2D rotation of the input object ``x`` by an interval angle ``a``,
   * optionally around a center of rotation ``c`` (default is the origin).
   * This operation propagates uncertainties through interval arithmetic.
   *
   * \param x The input 2D object to be rotated.
   * \param a The rotation angle as an interval (in radians).
   * \param c The center of rotation as an interval vector (default is the origin (0,0)).
   * \return The rotated object.
   */
  template<typename T>
  inline T rotate(const T& x, const Interval& a, const IntervalVector& c = Vector::zero(2))
  {
    T x_(x);
    for(auto& ei : x_)
      ei = rotate(ei,a,c);
    return x_;
  }

  /**
   * \brief Merges overlapping or adjacent ``IntervalVector`` points within a list.
   *
   * This function iteratively scans a list of ``IntervalVector`` objects and merges
   * any pairs that intersect.
   * The process repeats until no further merges are possible.
   *
   * This operation is useful for simplifying collections of intervals or bounding boxes
   * that may overlap due to uncertainty propagation or geometric tolerance.
   *
   * \param l list of ``IntervalVector`` objects to be merged. The list is modified in-place.
   */
  void merge_adjacent_points(std::list<IntervalVector>& l);
}