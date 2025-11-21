/** 
 *  \file codac2_ConvexPolygon.h
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Simon Rohou
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Polygon.h"

namespace codac2
{
  /**
   * \class ConvexPolygon
   * \brief Represents a convex polygon defined by vertices enclosed in ``IntervalVector``s.
   *
   * A convex polygon is a special case of polygon where all internal angles are less than 180°,
   * and every line segment between any two points in the polygon lies entirely within it.
   * It inherits all functionality from ``Polygon`` and ensures convexity of the structure.
   */
  class ConvexPolygon : public Polygon
  {
    public:

      /**
       * \brief Constructs a ``ConvexPolygon`` from an initializer list of ``Vector`` vertices.
       *
       * \param vertices A list of vertices defining the convex polygon.
       */
      ConvexPolygon(std::initializer_list<Vector> vertices);

      /**
       * \brief Constructs a ``ConvexPolygon`` from a vector of ``Vector`` vertices.
       *
       * \param vertices A vector of vertices defining the convex polygon.
       * \param compute_convex_hull if true, will create the shape as the convex hull
       *   of the provided points. This option may be set to false when one can ensure
       *   that the points are already convex and provided in counterclockwise order.
       *   This would speed up computations, avoiding a Graham scan procedure.
       */
      ConvexPolygon(const std::vector<Vector>& vertices, bool compute_convex_hull = true);

      /**
       * \brief Constructs a ``ConvexPolygon`` from a vector of ``IntervalVector`` vertices.
       *
       * \param vertices A vector of ``IntervalVector``s enclosing the convex polygon vertices.
       * \param compute_convex_hull if true, will create the shape as the convex hull
       *   of the provided points. This option may be set to false when one can ensure
       *   that the points are already convex and provided in counterclockwise order.
       *   This would speed up computations, avoiding a Graham scan procedure.
       */
      explicit ConvexPolygon(const std::vector<IntervalVector>& vertices, bool compute_convex_hull = true);

      /**
       * \brief Constructs a ``ConvexPolygon`` from an initializer list of ``Segment`` edges.
       *
       * \param edges A list of ``Segment``s forming the convex polygon.
       */
      ConvexPolygon(std::initializer_list<Segment> edges);

      /**
       * \brief Constructs a ``ConvexPolygon`` from a vector of ``Segment`` edges.
       *
       * \param edges A vector of ``Segment``s forming the convex polygon.
       */
      ConvexPolygon(const std::vector<Segment>& edges);

      /**
       * \brief Constructs a box as a ``ConvexPolygon``.
       *
       * Typically used to create a rectangular convex polygon.
       *
       * \param x An ``IntervalVector`` representing the bounds of the convex polygon.
       */
      explicit ConvexPolygon(const IntervalVector& x);

      /**
       * \brief Provides an empty convex polygon.
       * 
       * \return An empty convex polygon without vertices.
       */
      static ConvexPolygon empty();

      /**
       * \brief Self intersection of this and p
       * 
       * \param p the other convex polygon
       * \return a reference to this
       */
      ConvexPolygon& operator&=(const ConvexPolygon& p);

      /**
       * \brief Self union of this and p
       * 
       * \param p the other convex polygon
       * \return a reference to this
       */
      ConvexPolygon& operator|=(const ConvexPolygon& p);

    protected:

      /**
       * \brief Default protected constructor used internally.
       */
      ConvexPolygon();
  };

  /**
   * \brief Computes the intersection of two convex polygons.
   *
   * The result is a new ``ConvexPolygon`` enclosing the intersection area.
   * If the polygons do not intersect, the result is an empty convex polygon.
   *
   * \param p1 The first convex polygon.
   * \param p2 The second convex polygon.
   * \return A ``ConvexPolygon`` enclosing the intersection area.
   */
  ConvexPolygon operator&(const ConvexPolygon& p1, const ConvexPolygon& p2);
}