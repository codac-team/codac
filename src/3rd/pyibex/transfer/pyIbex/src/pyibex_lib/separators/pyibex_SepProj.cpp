//============================================================================
//                                  I B E X
// File        : ibex_sepProj.cpp
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : May 04, 2015
//============================================================================
#include "pyibex_SepProj.h"
#include <iostream>
#include <sstream>
#include <cmath>
// #include "vibes.h"
#include "ibex_NoBisectableVariableException.h"
using namespace std;

namespace pyibex {



SepProj::SepProj(Sep& sep, const IntervalVector& y_init, double prec) : Sep(sep.nb_var), sep(sep),
    y_init(y_init), prec(prec), nbx(0)
{
    // The LargestFirst minimal size is set to a very small number to avoid
    //  NoBisectableVariableException to be raised
    bsc = new LargestFirst(1e-10*prec);
}

SepProj::SepProj(Sep& sep, const Interval& y_init, double prec) : Sep(sep.nb_var), sep(sep),
    y_init(1, y_init), prec(prec), nbx(0)
{
    // The LargestFirst minimal size is set to a very small number to avoid
    //  NoBisectableVariableException to be raised
    bsc = new LargestFirst(1e-10*prec);
}

SepProj::~SepProj() {}


/**
 * @brief SepProj::process Separate cartesian product [x_in].[y] and [x_out].[y]
 *              if an inner (or outer) contraction happends, the flags impact_cin is set to true
 *              and the removed part of the box is stored in first_cin_boxes.
 *
 * @param x_in : projected inner box
 * @param x_out : projected outer box
 * @param y :  parameter box
 * @return true if x_in or x_out is empty.
 */
bool SepProj::process(IntervalVector& x_in, IntervalVector& x_out, IntervalVector &y, ImpactStatus& impact, bool use_point){
    assert(x_in == x_out);
    IntervalVector x = (x_in & x_out);
    if (x.is_empty()) return true;

    // Separate the product [x].[y]
    IntervalVector XinFull = cart_prod(x, y);
    IntervalVector XoutFull = cart_prod(x, y);

    // IntervalVector XinFull0(XinFull);
    // IntervalVector XoutFull0(XoutFull);
    // std::cerr << "XinFull " << XinFull << "\n";
    sep.separate(XinFull, XoutFull);
    nbx++;

    if (!((XinFull | XoutFull)  == cart_prod(x, y))){
      std::cerr << "##########################################################\n";
      std::cerr << "XinFull   " << XinFull << "\nXoutFull  " << XoutFull << "\n";
      std::cerr << "X0        " << cart_prod(x, y) << "\nUnion     "  << (XinFull | XoutFull)  << "\n";
      std::cerr << "##########################################################\n";

      assert((XinFull | XoutFull)  == cart_prod(x, y));
    }

    // if (XoutFull.is_empty()) assert(XinFull == XinFull0);

    // std::cerr << "ctcStats " << true << " " << (XinFull == XinFull0) << " " << (XoutFull == XoutFull0) << "\n";
    // std::cerr << "ctcStats " << (XinFull) << " " << (XoutFull) << "\n";

    // Handle error case
    if (XinFull.is_empty() && XoutFull.is_empty()){
        cout << "Erreur !!!! line" << __LINE__  << "\n ";
        exit(-1);
    }

    if ( XinFull.is_empty() || !(y == XinFull.subvector(x_in.size(), XinFull.size() - 1 )))
//         ( !y.is_flat() &&  XinFull.subvector(x_in.size(), XinFull.size() - 1 ).is_strict_subset(y) ) )
    {
        x_in.set_empty();
        impact.setCinFlags(x_in, x);
        return true;
    } else {
        x_in = XinFull.subvector(0, x_in.size()-1);
        impact.setCinFlags(x_in, x);
    }

    if( XoutFull.is_empty()){
        x_out.set_empty();
        if( use_point == false)
            impact.setCoutFlags(x_out, x);
        y.set_empty();
        return true;
    } else {
        x_out = XoutFull.subvector(0, x_out.size()-1);
        y = XoutFull.subvector(x_out.size(), XoutFull.size()-1);
        if( use_point == false)
            impact.setCoutFlags(x_out, x);
    }
    return false;
}


void complementaryUnion(IntervalVector& x, const IntervalVector& y, const IntervalVector& x0){
  IntervalVector *res;
  if (x == x0) return;
  int n = x0.diff(y, res);
  for(int i = 0; i < n; i++){
    x |= res[i];
  }
  delete[] res;
  if (( x | y ) != x0){
    std::cerr << "##########################################################################\n";
    // std::cerr << std::hexfloat;
    std::cerr << "x_in     " <<  x << "\n";
    std::cerr << "x_out    " <<  y << "\n";
    std::cerr << "x_old0 " <<  x0 << "\n";
    // std::cerr << std::defaultfloat;
    std::cerr << "##########################################################################\n";
    assert( ( x | y ) == x0);
  }
}

bool SepProj::fixpoint(IntervalVector& x_in, IntervalVector& x_out, IntervalVector& y){
    // std::cerr <<  "###########################################\n";
    IntervalVector x0(x_in | x_out);
    // std::cerr << "X0  "<< x0 << "\nXIN " << x_in << "\nXOUT" << x_out << "\n";
    double ratio = 0.001;

    ImpactStatus impact(x_in, x_out);
    // std::cerr << "Impact cin " << impact.impact_cin << "cout "<< impact.impact_cout << "\n";
    x_in &= x_out;   x_out &= x_in;


    IntervalVector x = x_in & x_out;
    IntervalVector x_old(x); // tmporary box use during the fix point.
    bool stop;

    do {
        x_old = x;
        assert(x_in == x_out);
        IntervalVector x_out0(x_out);
        IntervalVector xin0(x_in);
        // std::cerr <<  "------------------------------------------------\n";
        // std::cerr << ">>>> "<< x0 << "\n" << x_in << "\n" << x_out << "\n";
        stop = process(x_in, x_out, y, impact, false);
        // std::cerr << ">>>> "<< x0 << " " << x_in << " " << x_out << "\n";
        // std::cerr << "XOUT > " << x_out << " STOP  " << stop << "\n";
        // std::cerr << "XIN > " << x_in << " OLD  " << xin0 << " " << (x_in == xin0) << "\n";
        if (!stop){
            IntervalVector y_mid(y.mid());
            IntervalVector x_out_mid(x_out0 & x_in);
            stop = process(x_in, x_out_mid, y_mid, impact, true);

            // IntervalVector y_ub(y.ub());
            // IntervalVector x_out_ub(x_out0);
            // stop = process(x_in, x_out_ub, y_ub, impact, true);
            //
            // IntervalVector y_lb(y.lb());
            // IntervalVector x_out_lb(x_out0);
            // stop = process(x_in, x_out_lb, y_lb, impact, true);
        }
        x = x_in & x_out;
        break;

      // The condifion || is more efficient
    } while (false && !x.is_empty() && ( x_old.rel_distance(x_in)>ratio || x_old.rel_distance(x_out)>ratio ));

    // std::cout << x_in << " "  << x_out << " \n";
    impact.reconstrut_v2(x_in, x_out, x0);
    if (! ((x_in | x_out) == x0)){
      std::cerr << "Error in reconstruct_v2\n";
      std::cerr << x_in << " " << x_out << " " << x0 << "\n";
      assert(false);
    }
    return !x.is_empty() ? true : false;
}



//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================





void SepProj::separate(IntervalVector &x_in, IntervalVector &x_out){

    //    clearFlags();
//#define SIMPLE_ALG

    assert(x_in == x_out);
    IntervalVector x_old0(x_in & x_out); // Initial box
    IntervalVector y(y_init);
    IntervalVector x_res  = IntervalVector::empty(x_in.size());
    std::queue<TwoItv> l;
    IntervalVector x(x_in & x_out);

    // vibes::drawBox(x_old0, "y");
    l.push(TwoItv(x_out, y_init));
    static int k = 0;
    // std::cerr << "###########################################################\n";
    while(!l.empty()){
        IntervalVector x_out_save(l.front().first);
        IntervalVector y = l.front().second;

        l.pop();
        if (x_out_save.is_subset(x_res))
          continue;

        complementaryUnion(x_out_save, x_in, x_old0);
        // std::cerr << std::hexfloat << x_in << " \n" << x_out_save << "\n";
        // std::cerr << x_old0 << std::defaultfloat << "\n";
        // std::cerr << l.size()  << " " << y.max_diam() << "\n#################\n";
        // if ((x_in & x_out_save).is_empty()){
        //   // std::cout << x_in << " " << x_out_save << "\n";
        //   x_res |= x_out_save;
        //   continue;
        // };
        IntervalVector y0(y);
        if (( x_in | x_out_save ) != x_old0){
          std::cerr << "##########################################################################\n";
          // std::cerr << std::hexfloat;
          std::cerr << "x_in     " <<  x_in << "\n";
          std::cerr << "x_out    " <<  x_out_save << "\n";
          std::cerr << "x_old0 " <<  x_old0 << "\n";
          // std::cerr << std::defaultfloat;
          std::cerr << "##########################################################################\n";
          assert( ( x_in | x_out_save ) == x_old0);
        }
        fixpoint(x_in, x_out_save, y);

        IntervalVector x = x_in & x_out_save;
        if (x_out_save.is_empty()) continue;
        // std::cerr << x_in << " " << x_out_save << "\n";
        // std::cerr << "x0: " << x_out0 << "\n";
        // std::cerr << "x: " << x << " x_in: " << x_in << " x_out: " << x_out_save << "\n"
                  // << "x_res " << x_res  <<  " y: "  << y << "\n";
        if ( ! (( x_in | x_out_save ) == x_old0)){
          std::cerr << x_in << " " << x_out_save << " \n" << x_old0 << "\n";
          // std::cerr << std::hexfloat;
          std::cerr << x_in << " \n" << x_out_save << "\n";
          std::cerr << x_old0 << "\n";
          // std::cerr << std::defaultfloat;
          std::cerr << l.size() << "\n";
          assert( ( x_in | x_out_save ) == x_old0);
        }
        // std::cerr << x.is_empty() << " " << (x.max_diam()) << " " << prec << " " << y.is_empty() << " " << y.max_diam() << " " << x.max_diam() << " " << l.size()<< "\n";
        if (x.is_empty() || x.is_flat() || x.max_diam() < prec || y0.is_empty() || y.max_diam() < 0.1*x.max_diam()){
            x_res |= x_out_save;
            // std::cerr << "------> end y bisection\n";
        } else {
          if (!y.is_empty() && !x_out_save.is_subset(x_res) ){
            try{
              TwoItv cut = bsc->bisect(y);
              l.push(TwoItv(x_out_save, cut.first));
              l.push(TwoItv(x_out_save, cut.second));
              // std::cerr << "la \n";
            } catch (ibex::NoBisectableVariableException& e){
                std::cerr << "Error while trying to bisect y" << y <<"\n";
                // assert(falsse);

            }
          }
      }
    }

    x_out = x_res;
    x_in = x_in;

}


} // namespace pyibex
