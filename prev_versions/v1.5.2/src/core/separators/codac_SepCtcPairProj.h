//============================================================================
//                                  I B E X
// File        : ibex_SepCtcPairProj.h
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : May 04, 2015
//============================================================================

#ifndef __IBEX_SEP_CTCPAIR_PROJ_H__
#define __IBEX_SEP_CTCPAIR_PROJ_H__



#include <ibex_IntervalVector.h>
#include <ibex_SepCtcPair.h>
#include <ibex_SepFwdBwd.h>
#include <ibex_CtcForAll.h>
#include <ibex_CtcExist.h>
#include <ibex_SepFwdBwd.h>

#include <ibex_BitSet.h>

using ibex::Ctc;
using ibex::Sep;
using ibex::IntervalVector;
using ibex::CtcExist;
using ibex::CtcForAll;
using ibex::BitSet;
using ibex::SepCtcPair;
using ibex::SepFwdBwd;

namespace codac {

/**
 * @brief Build a contractor with a separator
 *        Wrt the 
 * 
 */
class CtcFromSep : public Ctc {
public:
  /**
   * @brief Construct a new Ctc From Sep object
   *        wrt to the value of return_ctc_in 
   *        it use the result from the separate method to return x_in or x_out
   *
   * @param sep separator to use
   * @param return_ctc_in if true, if true return inner contracted box else the outer one
   */
  CtcFromSep(Sep &sep, bool return_ctc_in) : Ctc(sep.nb_var), sep(sep), return_ctc_in(return_ctc_in) {}

  /**
   * @brief contract method
   *        call separate on the input box [x] and return x_in or x_out
   * 
   * @param x input box
   */
  void contract(IntervalVector& x){
    IntervalVector x_in(x), x_out(x);
    sep.separate(x_in, x_out);
    x &=  ( (return_ctc_in == true) ? x_in : x_out );
  }

private:
  /**
   * @brief Separator to use
   * 
   */
  Sep& sep;

  /**
   * @brief if true, return the inner box else the outer one
   * 
   */
  bool return_ctc_in;
};


/**
 * @brief projection of a separator using ibexlib algorithm \see SepProj
 *
 * The inner contraction is build using the CtcExist algorithm of ibexlib
 * The inner contraction is build using the CtcForAll algorithm of ibexlib
 * 
 * The two contractions are performed separately.
 */
class SepCtcPairProj : public Sep {

public:
  /**
   * @brief Construct a new Sep Ctc Pair Proj object
   * 
   * @param ctc_in inner separator
   * @param ctc_out outer separator
   * @param y_init initial box for the parameters [y] 
   * @param prec Bisection precision on the parameters (the contraction involves a
	 *             bisection process on y)
   */
    SepCtcPairProj(Ctc& ctc_in, Ctc& ctc_out, const IntervalVector& y_init, double prec);

    /**
     * @brief Construct a new Sep Ctc Pair Proj object build from a SepCtcPair object
     * 
     * @param sep separator to use given as a SepCtcPair object.  
     *            It will be splited internally into an inner ans outer contractor using CtcFromSep object
     * @param y_init initial box for the parameters [y] 
     * @param prec Bisection precision on the parameters (the contraction involves a
	   *             bisection process on y)
     */
    SepCtcPairProj(SepCtcPair &sep, const IntervalVector& y_init, double prec);

    /**
     * @brief Construct a new Sep Ctc Pair Proj object build for a generic Sep Object
     * 
     * @param sep separator to use
     *            It will be splited internally into an inner ans outer contractor using CtcFromSep object
     * @param y_init initial box for the parameters [y] 
     * @param prec Bisection precision on the parameters (the contraction involves a
	   *             bisection process on y)
     */
    SepCtcPairProj(Sep &sep, const IntervalVector& y_init, double prec);

    /**
     * @brief Destroy the Sep Ctc Pair Proj object
     * 
     */
    ~SepCtcPairProj();

    /**
     * @brief Separate method
     * 
     * @param x_in the n-dimensional box \f$[\mathbf{x}_{\textrm{in}}]\f$ to be inner-contracted
     * @param x_out the n-dimensional box \f$[\mathbf{x}_{\textrm{out}}]\f$ to be outer-contracted
     */
    void separate(IntervalVector &x_in, IntervalVector &x_out);

protected:
    /** Internal inner contractor */
    Ctc& ctc_in;
    /** Internal outer contractor */
    Ctc& ctc_out;

    /** Internal contractor used for the inner projection */
    CtcExist *ctcExist;
    
    /** Internal contractor used for the outer projection */
    CtcForAll *ctcForAll;

    /** initial box of parameters */
    IntervalVector y_init;

    /** BitSet used to link parameter to variables */ 
    BitSet vars;

};

} // namespace pyibex

#endif // __IBEX_SEP_CTCPAIR_PROJ_H__
