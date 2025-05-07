/** 
 *  \file codac2_Segment.h
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Simon Rohou
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <vector>
#include <utility>
#include "codac2_Vector.h"
#include "codac2_BoolInterval.h"
#include "codac2_IntervalVector.h"

namespace codac2
{
  class ConvexPolygon;
  /**
   * \class Segment
   * \brief Represents a geometric segment defined by two points enclosed in ``IntervalVector``s.
   *
   * Inherits from ``std::array<IntervalVector, 2>``.
   */
  class Segment : public std::array<IntervalVector,2>
  {
    public:

      /**
       * \brief Constructs a ``Segment`` from an array of two ``IntervalVector``s.
       *
       * \param x An array of two ``IntervalVector``s enclosing the segment endpoints.
       */
      Segment(const std::array<IntervalVector,2>& x);

      /**
       * \brief Constructs a ``Segment`` from two ``IntervalVector``s.
       *
       * \param x1 The first endpoint \f$\mathbf{x}_1\f$ of the segment.
       * \param x2 The second endpoint \f$\mathbf{x}_2\f$ of the segment.
       */
      Segment(const IntervalVector& x1, const IntervalVector& x2);

      /**
       * \brief Computes the bounding box of the segment.
       *
       * \return The ``IntervalVector`` hull box.
       */
      IntervalVector box() const;

      /**
       * \brief Checks whether the segment intersects with another segment.
       *
       * \param e Another segment.
       * \return A ``BoolInterval`` indicating possible intersection.
       */
      BoolInterval intersects(const Segment& e) const;

      /**
       * \brief Checks whether the segment contains a given point.
       *
       * \param p The point to check, enclosed in a ``IntervalVector``.
       * \return A ``BoolInterval`` indicating possible containment.
       */
      BoolInterval contains(const IntervalVector& p) const;

      /**
       * \brief Comparison operator.
       *
       * Equality means that boxes enclosing the endpoints are strictly equal.
       * 
       * \param p Another segment.
       * \return True if both segments are equal.
       */
      bool operator==(const Segment& p) const;
  };

  /**
   * \brief Computes the intersection of two segments.
   * 
   * If the segments do not intersect, an empty ``IntervalVector`` is returned.
   * If the segments are colinear, the set of intersection points is returned
   * as a box.
   *
   * \param e1 The first segment.
   * \param e2 The second segment.
   * \return An ``IntervalVector`` enclosing the intersection point.
   */
  IntervalVector operator&(const Segment& e1, const Segment& e2);

  Segment operator&(const Segment& e1, const ConvexPolygon& p2);

  /**
   * \brief Computes the projected intersection of two segments.
   * 
   * This corresponds to the intersection of the two lines related
   * to the two segments.
   * Therefore, the intersection point may not belong to the segments.
   * 
   * If the segments are parallel but not colinear, an empty ``IntervalVector``
   * is returned. If the segments are colinear, the set of intersection
   * points is returned as a box.
   *
   * \param e1 The first segment.
   * \param e2 The second segment.
   * \return An ``IntervalVector`` enclosing the intersection point.
   */
  IntervalVector proj_intersection(const Segment& e1, const Segment& e2);

  /**
   * \brief Checks if two segments are colinear.
   *
   * \param e1 The first segment.
   * \param e2 The second segment.
   * \return A ``BoolInterval`` indicating possible colinearity.
   */
  BoolInterval colinear(const Segment& e1, const Segment& e2);

  /**
   * \brief Stream output operator for ``Segment``.
   *
   * \param str Output stream.
   * \param e The segment to print.
   * \return The output stream with the segment information.
   */
  std::ostream& operator<<(std::ostream& str, const Segment& e);
}