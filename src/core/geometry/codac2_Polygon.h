/** 
 *  \file codac2_Polygon.h
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Simon Rohou
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <list>
#include <vector>
#include <utility>
#include "codac2_IntervalVector.h"
#include "codac2_BoolInterval.h"
#include "codac2_Segment.h"

namespace codac2
{
  /**
   * \class Polygon
   * \brief Represents a polygon (convex or non-convex) defined by its vertices enclosed in ``IntervalVector``s.
   *
   * A polygon can be constructed from a list of vertices (either ``Vector`` or ``IntervalVector``)
   * or from a list of edges (``Segment``s). Internally, it stores a list of edges enclosed in ``Segment``s.
   */
  class Polygon : public std::vector<Segment>
  {
    public:

      /**
       * \brief Constructs a ``Polygon`` from an initializer list of ``Vector`` vertices.
       *
       * \param vertices A list of vertices defining the polygon.
       */
      Polygon(std::initializer_list<Vector> vertices);

      /**
       * \brief Constructs a ``Polygon`` from a vector of ``Vector`` vertices.
       *
       * \param vertices A vector of vertices defining the polygon.
       */
      Polygon(const std::vector<Vector>& vertices);

      /**
       * \brief Constructs a ``Polygon`` from a vector of ``IntervalVector`` vertices.
       *
       * \param vertices A vector of ``IntervalVector``s enclosing the polygon vertices.
       */
      explicit Polygon(const std::vector<IntervalVector>& vertices);

      /**
       * \brief Constructs a ``Polygon`` from an initializer list of ``Segment`` edges.
       *
       * \param edges A list of ``Segment``s forming the polygon.
       */
      Polygon(std::initializer_list<Segment> edges);

      /**
       * \brief Constructs a ``Polygon`` from a vector of ``Segment`` edges.
       *
       * \param edges A vector of ``Segment``s forming the polygon.
       */
      Polygon(const std::vector<Segment>& edges);

      /**
       * \brief Constructs a box as a ``Polygon``.
       *
       * Typically used to create a rectangular polygon.
       *
       * \param x An ``IntervalVector`` representing the bounds of the polygon.
       */
      explicit Polygon(const IntervalVector& x);

      /**
       * \brief Returns the list of edges of the polygon.
       *
       * \return A constant reference to the vector of ``Segment``s.
       */
      const std::vector<Segment>& edges() const;

      /**
       * \brief Returns the list of unique vertices in no particular order.
       *
       * If a vertex is involved several times in the polygon definition,
       * then it will be returned only once in the output list.
       * 
       * \return A list of ``IntervalVector``s enclosing the unique vertices.
       */
      std::list<IntervalVector> unsorted_vertices() const;

      /**
       * \brief Returns the list of vertices sorted in polygonal order.
       *
       * \return A vector of ``IntervalVector``s enclosing the ordered vertices.
       */
      std::vector<IntervalVector> sorted_vertices() const;

      /**
       * \brief Computes the bounding box of the polygon.
       *
       * \return The ``IntervalVector`` hull box.
       */
      IntervalVector box() const;

      /**
       * \brief Checks whether the polygon is empty (has no vertex).
       *
       * \return True if the polygon is empty.
       */
      bool is_empty() const;

      /**
       * \brief Checks whether the polygon contains a given point.
       *
       * \param p The point to check, enclosed in an ``IntervalVector``.
       * \return A ``BoolInterval`` indicating possible containment.
       */
      BoolInterval contains(const IntervalVector& p) const;

      /**
       * \brief Comparison operator.
       *
       * Equality means that both polygons have the same edges,
       * possibly in a different order (clockwise or counterclockwise).
       * 
       * \param p Another polygon.
       * \return True if both polygons are strictly equal.
       */
      bool operator==(const Polygon& p) const;

      /**
       * \brief Provides an empty polygon
       * 
       * \return An empty polygon without vertices.
       */
      static Polygon empty();

    protected:

      /**
       * \brief Default protected constructor used internally.
       */
      Polygon();
  };

  /**
   * \brief Stream output operator for ``Polygon``.
   *
   * \param str Output stream.
   * \param p The polygon to print.
   * \return The output stream with the polygon information.
   */
  std::ostream& operator<<(std::ostream& str, const Polygon& p);
}