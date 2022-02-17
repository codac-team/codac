//============================================================================
//                                 P Y I B E X
// File        : ibex_PdcInPolygon.h
// Author      : Benoit Desrochers, Gilles Chabert
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Oct 31, 2014
//============================================================================

#ifndef __IBEX_PDC_IN_POLYGON_H__
#define __IBEX_PDC_IN_POLYGON_H__

#include "ibex_Pdc.h"
#include <vector>

using ibex::Interval;
using ibex::IntervalVector;
using ibex::Pdc;
using ibex::BoolInterval;

namespace pyibex {

/**
 * \ingroup geometry
 *
 * \brief Tests if a box is inside a polygon.
 *
 * The test is based on the Winding Number (see .http://en.wikipedia.org/wiki/Winding_number)
 * But fastest method can be found in http://alienryderflex.com/polygon
 *
 * The polygon is not necessarily convex.
 *
 * The polygon is defined by an union of oriented
 * segment given in counter-clockwise order.
 *
 * Example :
 *  The polygon ABCDE is composed of 5 segments
 *  AB -- BC -- CD -- DE -- EA
 *
 *
 * A------------------------------- E
 *  -                            -
 *   -                          -
 *    -                       -
 *     -          Area       -
 *      -        Inside     - D
 *       -        the        -
 *        -     Polygon       -
 *         -                   -
 *          -                   - C
 *           -            ------
 *            -     ------
 *              ----
 *            B
 *
 */
class PdcInPolygon : public Pdc {
public:

    /**
     * \brief Create the predicate with the list of segments passed as argument.
     *
     * A polygon is defined as an union of segments given in a counter-clockwise order.
     * See the documentation for an example of usage.
     *
     * \param points list of segments representing the edges of the polygon in the format of ( ((a1_x, a1_y), (b1_x, b1_x)), ((a2_x, a2_y), (b2_x, b2_x)), ...)
     */
    PdcInPolygon(std::vector< std::vector< std::vector<double> > >& points);

    /**
     * \brief Create the predicate with the list of segments passed as argument.
     *
     * A polygon is defined as an union of segments given in a counter-clockwise order.
     * See the documentation for an example of usage.
     *
     * \param ax list of x coordinate of the first point of each segment
     * \param ay list of y coordinate of the first point of each segment
     * \param bx list of x coordinate of the second point of each segment
     * \param by list of y coordinate of the second point of each segment
     */
    PdcInPolygon(std::vector<double>& ax, std::vector<double>& ay, std::vector<double>& bx, std::vector<double>& by);

	/**
	 * \brief Test the box.
	 */
	virtual BoolInterval test(const IntervalVector& box);

protected:
    /**
     * Definition of the segment of the polygon
     */
    std::vector<double> ax;
    std::vector<double> ay;
    std::vector<double> bx;
    std::vector<double> by;
};

} // namespace pyibex

#endif // __IBEX_PDC_IN_POLYGON_H__
