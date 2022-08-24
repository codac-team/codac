//============================================================================
//                                  I B E X
// File        : ibex_SepFixPoint.h
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : May 04, 2015
//============================================================================

#ifndef __IBEX_SEP_FIXPOINT_H__
#define __IBEX_SEP_FIXPOINT_H__


#include <ibex_IntervalVector.h>
#include <ibex_LargestFirst.h>
#include <ibex_Sep.h>
#include <vector>

using ibex::Sep;
using ibex::IntervalVector;

// Compute the fixed point of a separator
/**
 * \ingroup Set
 * @brief FixPoint of a separator
 * The fixpoint of a separator is computed by calling the "::"separate function
 * on a given box until the Hausdorff distance between two iterations is less than
 * a given ratio. This operation can be seen as a contractor on the boundary of the solution set.
 *
 * At the end, two boxes x_in and x_out, need to be reconstructed
 * by merging the resulting box and parts of the initial box which
 * belong (or not) to the solution set.
 *
 */
namespace codac {
  /**
   * \class SepFixPoint
   * @brief Fix point of a Separator.
   */
class SepFixPoint : public Sep {

public:
    /**
     * @brief build a fix point Separator from the one given as arguments.
     * When the Hausdorff distance between
     * two iterations is less than ratio*diameter
     * the fix-point is considered to be reached.
     * 
     * @param sep Separator to use
     * @param ratio iteration stop criteria
     * 
     */
    SepFixPoint(Sep& sep, double ratio=default_ratio);

    /**
     * @brief Delete *this.
     */
    ~SepFixPoint();

    /**
     * @brief Separate a box. 
     * @see ibex::Sep
     * @param x_in the n-dimensional box \f$[\mathbf{x}_{\textrm{in}}]\f$ to be inner-contracted
     * \param x_out the n-dimensional box \f$[\mathbf{x}_{\textrm{out}}]\f$ to be outer-contracted
     */
    void separate(IntervalVector &x_in, IntervalVector &x_out);


protected:

    /**
     * @brief The Separator.
     */
    Sep& sep;

    /**
      * @brief backtrace if the inner contractor had an impact
      */
    bool impact_cin;
    /**
      * @brief backtrace if the outer contractor had an impact
      */
    bool impact_cout;

    /**
      * @brief store the first box contracted by the outer contractor
      * It is the result of the ibex function  IntervalVector::diff
      */
    IntervalVector *first_cin_boxes;

    /**
      * @brief store the first box contracted by the inner / outer contractor
      * It is the result of the ibex function  IntervalVector::diff
      */
    IntervalVector *first_cout_boxes;

    /**
     * @brief number of boxes in  SepFixPoint::first_cin_boxes
     */
    int n_in;
    
    /**
     * @brief number of boxes in  SepFixPoint::first_cout_boxes
     */
    int n_out;


    /**
     * @brief When the Hausdorff distance between
     * two iterations is less than ratio*diameter
     * the fix-point is considered to be reached.
     */
    double ratio;

    /** 
     * @brief  ratio used, set to 0.1. 
     */
    static const double default_ratio;

private:

    /**
     * Internal function used to compute the fix point of a separator.
     */

    /**
     * @brief clearFlags : reset impact_cin, impact_cout flags to false
     * delete first_cin_boxes and first_cout_boxes
     * n_in =0 and n_out = 0
     */
    void clearFlags();
    /**
     * @brief setCoutFlags
     * if x_out is stritly included in x0 set impact_cout to true
     * and put one box which belongs to x0 \ x_out into first_cout_boxes
     *
     * @param x_out outer box resulting from the separation
     * @param x0 initial box separated.
     */
    void setCoutFlags(IntervalVector &x_out, IntervalVector &x0);

    /**
     * @brief setCinFlags
     * if x_out is stritly included in x0 set impact_cin to true
     * and put one box which belongs to x0 \ x_in into first_cin_boxes
     *
     * @param x_in inner box resulting from the separation
     * @param x0 initial box separated.
     */
    void setCinFlags(IntervalVector &x_in, IntervalVector &x0);

    /**
     * @brief reconstrut
     * At this end of the separation process we have the box x,
     * stored in x_in and x_out.
     * this function reconstructs x_in and x_out
     * such as
     *      - \f$x_in \cup x_out = x_old\f$ and
     *      - \f$x_in \cap x_out = x\f$
     *
     */
    bool reconstruct(IntervalVector &x_in, IntervalVector &x_out, IntervalVector &x_old);

};

} // namespace pyibex

#endif // __IBEX_SEP_FIXPOINT_H__
