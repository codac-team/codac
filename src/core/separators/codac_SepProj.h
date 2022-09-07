//============================================================================
//                                  I B E X
// File        : ibex_sepProj.h
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : May 04, 2015
//============================================================================

#ifndef __IBEX_SEP_PROJ_H__
#define __IBEX_SEP_PROJ_H__


#include <ibex_IntervalVector.h>
#include <ibex_Sep.h>
//#include "ibex_SepFixPoint.h"
#include <ibex_LargestFirst.h>
#include <vector>
#include <queue>
#include <stack>

using ibex::IntervalVector;
using ibex::Interval;
using ibex::Sep;
using ibex::LargestFirst;

namespace codac {

typedef std::pair<IntervalVector, IntervalVector> TwoItv;

typedef struct ImpactStatus_{
    bool impact_cin;
    bool impact_cout;
    IntervalVector *first_cin_boxes, *first_cout_boxes;
    int n_in, n_out;

    ImpactStatus_ () : impact_cin(false), impact_cout(false),
        first_cin_boxes(NULL), first_cout_boxes(NULL){
        n_in = 0; n_out = 0;
        clearFlags();
    }

    ImpactStatus_(IntervalVector& x_in, IntervalVector& x_out):
    impact_cin(false), impact_cout(false),
        first_cin_boxes(NULL), first_cout_boxes(NULL){
        n_in = 0; n_out = 0;
        clearFlags();
        if( !( x_in == x_out) ){ // already contracted
            IntervalVector x0(x_in | x_out);
            setCinFlags(x_in, x0);
            setCoutFlags(x_out, x0);
        }
    }

//    ImpactStatus_(const ImpactStatus_& other):
//        impact_cin(other.impact_cin),
//        impact_cout(other.impact_cout),
//        n_in(other.n_in),
//        n_out(other.n_out) {

//        if (n_in > 0){
//            first_cin_boxes = new IntervalVector[n_in];
//            for (uint i = 0; i < n_in; i++){
//                first_cin_boxes[i] = new IntervalVector(other.first_cin_boxes[i]);
//            }
//        }
//        if (n_out > 0){
//            first_cout_boxes = new IntervalVector[n_out];
//            for (uint i = 0; i < n_out; i++){
//                first_cout_boxes[i] = new IntervalVector(other.first_cout_boxes[i]);
//            }
//        }
//    }



    ~ImpactStatus_(){
        clearFlags();
    }

    void clearFlags(){
        n_in = 0;
        n_out = 0;
        if (first_cin_boxes != NULL) delete[] first_cin_boxes;
        if (first_cout_boxes != NULL) delete[] first_cout_boxes;
        first_cin_boxes = NULL;
        first_cout_boxes = NULL;
        impact_cin = false;
        impact_cout = false;
    }


    void setCinFlags(IntervalVector& x_in, IntervalVector& x0){
        if( !impact_cin ){
            if ( ! (x_in == x0)){
                impact_cin = true;
                n_in = x0.diff(x_in,first_cin_boxes); // calculate the set difference
            }
        }
    }

    void setCoutFlags(IntervalVector& x_out, IntervalVector& x0){
        if(!impact_cout){
            if ( !(x_out == x0)){
                impact_cout = true;
                n_out = x0.diff(x_out,first_cout_boxes); // calculate the set difference
            }
        }
    }

    void reset_and_setCoutFlags(IntervalVector& x_out, IntervalVector& x0){
        impact_cout = false;
        if (first_cout_boxes != NULL){
            delete[] first_cout_boxes;
            first_cout_boxes = NULL;
        }
        setCoutFlags(x_out, x0);
    }

    void reconstrut_v2(IntervalVector &x_in, IntervalVector& x_out, IntervalVector& x_old){
        // std::cerr << x_in << " " << x_out << " " << x_old << "\n";
        // std::cerr << impact_cout << " " << impact_cin << "\n";
        if (x_out.is_empty() && impact_cout && impact_cin){
          x_in  = x_old;
          return;
        }
        if (!x_in.is_empty() && !x_out.is_empty()){
            if (impact_cin == true){
                if (impact_cout == false){
                    x_out = x_old;
                } else {
                    for(int i = 0; i < n_in; i++){
                        x_out |= first_cin_boxes[i];
                    }
                }
            }
            if(impact_cout == true){
                if(impact_cin == false){
                    x_in  = x_old;
                } else {
                    for(int i = 0; i < n_out; i++){
                        x_in |= first_cout_boxes[i];
                    }
                }
            }
        } else if (x_in.is_empty() && impact_cin == true){
            if(impact_cout == false){
                x_out  = x_old;
            } else {
                for(int i = 0; i < n_in; i++){
                    x_out |= first_cin_boxes[i];
                }
            }
        } else if (x_out.is_empty() && impact_cout == true){
            if(impact_cin == false){
                x_in  = x_old;
            } else {
                for(int i = 0; i < n_out; i++){
                    x_in |= first_cout_boxes[i];
                }
            }
        } else {
            std::cout << "#########################################################\n";
            std::cout << "x_in " << x_in << " x_out " << x_out << "\n";
            std::cout << "x_old " << x_old << "n_in " << n_in << " n_out " << n_out;
            std::cout << " impact_cin " << impact_cin << " ";
            std::cout << " impact_cout " << impact_cout << " ";
            std::cout << "\n";
            std::cout << "#########################################################\n";
            assert(false);
        }
    }


    void reconstrut(IntervalVector &x_in, IntervalVector& x_out, IntervalVector& x_old){
        IntervalVector x = x_in & x_out;
        IntervalVector *rest;

        if (x == x_old) return;
        if (impact_cin == true && impact_cout == false){
            x_out = x_old;
            x_in = x;
        } else if (impact_cin == false && impact_cout == true){
            x_in = x_old;
            x_out = x;
        } else if (impact_cin == true && impact_cout == true){
            int n = x_old.diff(x, rest);
//            for(uint i = 0; i < n; i++){
//                std::cout << "rest " << i << ": " << rest[i] << "\n" << std::flush;
//            }
//            for(uint i = 0; i < n_in; i++){
//                std::cout << "in " << i << ": " << first_cin_boxes[i] << "\n" << std::flush;
//            }
//            for(uint i = 0; i < n_out; i++){
//                std::cout << "out " << i << ": " << first_cout_boxes[i] << "\n" << std::flush;
//            }
    //        std::cout << rest[0] << " " << rest_in[0] << " " << rest_out[0] << std::endl;
            std::cerr << n_in << " " << n_out << " " << n << "\n";
            assert(n_in  == 1);
            assert(n_out == 1);
            assert(n     == 2);
            for (int i = 0; i < n; i++){
                for(int j = 0; j < n_in; j++){
                    if(first_cin_boxes[j].intersects(rest[i])){
                        x_out |= rest[i];
                        break;
                    }
                }
                for(int j = 0; j < n_out; j++){
                    if(first_cout_boxes[j].intersects(rest[i])){
                        x_in |= rest[i];
                        break;
                    }
                }
            }
            delete[] rest;
        } else {
            assert(false);
        }
    }

} ImpactStatus;

/**
 * \ingroup seprator
 *
 * \brief Projection of a separator
 * 
 * Given \f$\mathbb{X}\subset\mathbb{R}^{n}\f$ and \f$\mathbb{Y}\subset\mathbb{R}^{p}\f$. If \f$\mathbb{Z}=\mathbb{X}\times\mathbb{Y}\f$, 
 * then the projection of a subset \f$\mathbb{Z}_{1}\f$ of \f$\mathbb{Z}\f$ onto \f$\mathbb{X}\f$ (with respect to \f$\mathbb{Y}\f$) is defined as:
 * 
 * \f$ {proj}_{\mathbb{X}}(\mathbb{Z}_{1})=\left\{ \mathbf{x}\in\mathbb{X}\mid\exists\mathbf{y}\in\mathbb{Y},(\mathbf{x},\mathbf{y})\in\mathbb{Z}_{1}\right\}\f$
 *
 * Given a separator \f$\mathcal{S}([\mathbf{x}],\left[\mathbf{y}\right])=\left\{ \mathcal{S}^{\text{in}}([\mathbf{x}],\left[\mathbf{y}\right]),\mathcal{S}^{\text{out}}([\mathbf{x}],\left[\mathbf{y}\right])\right\}\f$,
 * the projection of \f$\mathcal{S}\f$ is defined by : 
 * 
 * \f${proj}_{\mathbf{x}}(\mathcal{S})([\textbf{x}])=\left\{ \bigcap_{\textbf{y}\in[\textbf{y}]}\partial_{\textbf{x}}\mathcal{S}^{in}([\textbf{x}],\mathbf{y}),\bigcup_{\textbf{y}\in[\textbf{y}]}\partial_{\textbf{x}}\mathcal{S}^{out}([\textbf{x}],\mathbf{y})\right\}\f$
 * 
 * The separator algorythm is inspired from the ibexlib ones 
 * but performs the inner and outer contraction concurently.
 * 
 * See Ibexlib (CtcForAll and CtcExist)[http://www.ibex-lib.org/doc/contractor.html#exists-and-forall] 
 * documentation for more details.
 */
class SepProj : public Sep {

public:
    /**
     * @brief Construct a new Sep Proj object
     * 
     * @param sep Separator to use for the projection. The input box dimension must be equal to dim([x]) + dim([y_init]).
     *            given a two IntervalVector \f$[\mathbf{x}]\f$ and \f$[\mathbf{y}]\f$, the separator is called 
     *            with a new box \f$[\mathbf{z}] = [\mathbf{y}] X [\mathbf{y}]\f$.
     * @param y_init Initial box for the parameters 
     * @param prec Bisection precision on the parameters (the contraction involves a
	 *             bisection process on y)
     */
    SepProj(Sep& sep, const IntervalVector& y_init, double prec);

    /**
    * @brief Construct a new Sep Proj object
     * 
     * @param sep Separator used for the projection. The input dimension must be equal to size(x) + size(y_init).
     *            given a two IntervalVector \f$[\mathbf{x}]\f$ and \f$[\mathbf{y}]\f$, the separator is called 
     *            with a new box \f$[\mathbf{z}] = [\mathbf{y}] X [\mathbf{y}]\f$.
     * @param y_init Initial Interval for the parameters 
     * @param prec Bisection precision on the parameters (the contraction involves a
	 *             bisection process on y)
     */
    SepProj(Sep& sep, const Interval& y_init, double prec);

    /**
     * @brief Destroy the Sep Proj object
     * 
     */
    ~SepProj();


    /**
     * @brief Separate method
     * 
     * @param x_in the n-dimensional box \f$[\mathbf{x}_{\textrm{in}}]\f$ to be inner-contracted
     * @param x_out the n-dimensional box \f$[\mathbf{x}_{\textrm{out}}]\f$ to be outer-contracted
     */
    void separate(IntervalVector &x_in, IntervalVector &x_out);

protected:

    /**
     * @brief SepProj::process Separate cartesian product [x_in].[y] and [x_out].[y]
     *              if an inner (or outer) contraction happends, the flags impact_cin is set to true
     *              and the removed part of the box is stored in first_cin_boxes.
     *
     * @param x_in : projected inner box
     * @param x_out : projected outer box
     * @param y :  parameter box
     * @param impact : used to trace contraction of the input box
     * @param use_point: 
     * @return true if x_in or x_out is empty.
     */
    bool process(IntervalVector &x_in, IntervalVector &x_out, IntervalVector &y, ImpactStatus &impact, bool use_point);
    

    bool separate_fixPoint(IntervalVector& x_in, IntervalVector& x_out, IntervalVector &y);

    /**
     * \brief The Separator.
     */
    Sep& sep;


    /**
     * \brief Initial box of the parameters (can be set dynamically)
     */
    IntervalVector y_init;

    /**
     * \brief precision
     */
    double prec;

    /**
     * @brief internal variable used to count the number of call of the SepProj::process method
     * 
     */
    // int nbx;


    /**
      * \brief the outer contractor / inner contractor has an impact
      */
//    bool impact_cin, impact_cout;

    /**
      * \brief store the firsr box contractor by the inner/ outer contractor
      */
//    IntervalVector *first_cin_boxes, *first_cout_boxes;
//    int n_in, n_out;

    /**
     * \brief a bisector
     */
    LargestFirst* bsc;

    /**
      * Number of bisection along y
      */



private:
    bool fixpoint(IntervalVector &x, IntervalVector &x_out_res, IntervalVector &y);


};




} // namespace pyibex

#endif // __IBEX_SEP_PROJ_H__
