/* ============================================================================
 *  tubex-lib - Arithmetic
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_Arithmetic.h"
#include "tubex_DimensionException.h"
#include "tubex_DomainException.h"
#include "tubex_StructureException.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  #define unary_op(f) \
    \
    Tube f(const Tube& x1) \
    { \
      Tube x2(x1); \
      const TubeSlice *slice_x1, *first_slice_x1 = x1.getFirstSlice(); \
      TubeSlice *slice_x2, *first_slice_x2 = x2.getFirstSlice(); \
      slice_x1 = first_slice_x1; \
      slice_x2 = first_slice_x2; \
      while(slice_x1 != NULL) \
      { \
        IntervalVector box = slice_x1->codomain(); \
        for(int i = 0 ; i < box.size() ; i++) \
          box[i] = ibex::f(box[i]); \
        slice_x2->setEnvelope(box); \
        slice_x1 = slice_x1->nextSlice(); \
        slice_x2 = slice_x2->nextSlice(); \
      } \
      slice_x1 = first_slice_x1; \
      slice_x2 = first_slice_x2; \
      while(slice_x1 != NULL) \
      { \
        if(slice_x1 == first_slice_x1) \
        { \
          IntervalVector box = slice_x1->inputGate(); \
          for(int i = 0 ; i < box.size() ; i++) \
            box[i] = ibex::f(box[i]); \
          slice_x2->setInputGate(box); \
        } \
        IntervalVector box = slice_x1->outputGate(); \
        for(int i = 0 ; i < box.size() ; i++) \
          box[i] = ibex::f(box[i]); \
        slice_x2->setOutputGate(box); \
        slice_x1 = slice_x1->nextSlice(); \
        slice_x2 = slice_x2->nextSlice(); \
      } \
      return x2; \
    }

  unary_op(cos);
  unary_op(sin);
  unary_op(abs);
  unary_op(sqr);
  unary_op(sqrt);
  unary_op(exp);
  unary_op(log);
  unary_op(tan);
  unary_op(acos);
  unary_op(asin);
  unary_op(atan);
  unary_op(cosh);
  unary_op(sinh);
  unary_op(tanh);
  unary_op(acosh);
  unary_op(asinh);
  unary_op(atanh);

  Tube operator+(const Tube& x)
  {
    return x;
  }

  Tube operator-(const Tube& x)
  {
    return 0. - x;
  }

  #define unary_param_op(f, p) \
    \
    Tube f(const Tube& x1, p param) \
    { \
      Tube x2(x1); \
      const TubeSlice *slice_x1, *first_slice_x1 = x1.getFirstSlice(); \
      TubeSlice *slice_x2, *first_slice_x2 = x2.getFirstSlice(); \
      slice_x1 = first_slice_x1; \
      slice_x2 = first_slice_x2; \
      while(slice_x1 != NULL) \
      { \
        IntervalVector box = slice_x1->codomain(); \
        for(int i = 0 ; i < box.size() ; i++) \
          box[i] = ibex::f(box[i], param); \
        slice_x2->setEnvelope(box); \
        slice_x1 = slice_x1->nextSlice(); \
        slice_x2 = slice_x2->nextSlice(); \
      } \
      slice_x1 = first_slice_x1; \
      slice_x2 = first_slice_x2; \
      while(slice_x1 != NULL) \
      { \
        if(slice_x1 == first_slice_x1) \
        { \
          IntervalVector box = slice_x1->inputGate(); \
          for(int i = 0 ; i < box.size() ; i++) \
          box[i] = ibex::f(box[i], param); \
          slice_x2->setInputGate(box); \
        } \
        IntervalVector box = slice_x1->outputGate(); \
        for(int i = 0 ; i < box.size() ; i++) \
          box[i] = ibex::f(box[i], param); \
        slice_x2->setOutputGate(box); \
        slice_x1 = slice_x1->nextSlice(); \
        slice_x2 = slice_x2->nextSlice(); \
      } \
      return x2; \
    }

  unary_param_op(pow, int);
  unary_param_op(pow, double);
  unary_param_op(pow, const Interval&);
  unary_param_op(root, int);

  #define binary_op(f) \
    \
    Tube f(const Tube& tube_x1, const Tube& tube_x2) \
    { \
      DomainException::check(tube_x1, tube_x2); \
      StructureException::check(tube_x1, tube_x2); \
      DimensionException::check(tube_x1, tube_x2); \
      Tube new_tube_x(tube_x1); \
      TubeSlice *slice_x, *first_slice_x = new_tube_x.getFirstSlice(); \
      const TubeSlice *slice_x1, *first_slice_x1 = tube_x1.getFirstSlice(); \
      const TubeSlice *slice_x2, *first_slice_x2 = tube_x2.getFirstSlice(); \
      slice_x = first_slice_x; \
      slice_x1 = first_slice_x1; \
      slice_x2 = first_slice_x2; \
      while(slice_x1 != NULL) \
      { \
        slice_x->setEnvelope(IntervalVector(1, ibex::f(slice_x1->codomain()[0], slice_x2->codomain()[0]))); \
        slice_x = slice_x->nextSlice(); \
        slice_x1 = slice_x1->nextSlice(); \
        slice_x2 = slice_x2->nextSlice(); \
      } \
      slice_x = first_slice_x; \
      slice_x1 = first_slice_x1; \
      slice_x2 = first_slice_x2; \
      while(slice_x1 != NULL) \
      { \
        if(slice_x == first_slice_x) \
          slice_x->setInputGate(IntervalVector(1, ibex::f(slice_x1->inputGate()[0], slice_x2->inputGate()[0]))); \
        slice_x->setOutputGate(IntervalVector(1, ibex::f(slice_x1->outputGate()[0], slice_x2->outputGate()[0]))); \
        slice_x = slice_x->nextSlice(); \
        slice_x1 = slice_x1->nextSlice(); \
        slice_x2 = slice_x2->nextSlice(); \
      } \
      return new_tube_x; \
    } \
    \
    Tube f(const Interval& x1, const Tube& tube_x2) \
    { \
      Tube new_tube_x(tube_x2); \
      TubeSlice *slice_x, *first_slice_x = new_tube_x.getFirstSlice(); \
      slice_x = first_slice_x; \
      while(slice_x != NULL) \
      { \
        slice_x->setEnvelope(IntervalVector(1, ibex::f(x1, slice_x->codomain()[0]))); \
        slice_x = slice_x->nextSlice(); \
      } \
      slice_x = first_slice_x; \
      while(slice_x != NULL) \
      { \
        if(slice_x == first_slice_x) \
          slice_x->setInputGate(IntervalVector(1, ibex::f(x1, slice_x->inputGate()[0]))); \
        slice_x->setOutputGate(IntervalVector(1, ibex::f(x1, slice_x->outputGate()[0]))); \
        slice_x = slice_x->nextSlice(); \
      } \
      return new_tube_x; \
    } \
    \
    Tube f(const Tube& tube_x1, const Interval& x2) \
    { \
      Tube new_tube_x(tube_x1); \
      TubeSlice *slice_x, *first_slice_x = new_tube_x.getFirstSlice(); \
      slice_x = first_slice_x; \
      while(slice_x != NULL) \
      { \
        slice_x->setEnvelope(IntervalVector(1, ibex::f(slice_x->codomain()[0], x2))); \
        slice_x = slice_x->nextSlice(); \
      } \
      slice_x = first_slice_x; \
      while(slice_x != NULL) \
      { \
        if(slice_x == first_slice_x) \
          slice_x->setInputGate(IntervalVector(1, ibex::f(slice_x->inputGate()[0], x2))); \
        slice_x->setOutputGate(IntervalVector(1, ibex::f(slice_x->outputGate()[0], x2))); \
        slice_x = slice_x->nextSlice(); \
      } \
      return new_tube_x; \
    } \
    \
    Tube f(const Trajectory& x1, const Tube& tube_x2) \
    { \
      DomainException::check(tube_x2, x1); \
      DimensionException::check(tube_x2, x1); \
      Tube new_tube_x(tube_x2); \
      TubeSlice *slice_x, *first_slice_x = new_tube_x.getFirstSlice(); \
      slice_x = first_slice_x; \
      while(slice_x != NULL) \
      { \
        slice_x->setEnvelope(IntervalVector(1, ibex::f(x1[slice_x->domain()], slice_x->codomain()[0]))); \
        slice_x = slice_x->nextSlice(); \
      } \
      slice_x = first_slice_x; \
      while(slice_x != NULL) \
      { \
        if(slice_x == first_slice_x) \
          slice_x->setInputGate(IntervalVector(1, ibex::f(x1[Interval(slice_x->domain().lb())], slice_x->inputGate()[0]))); \
        slice_x->setOutputGate(IntervalVector(1, ibex::f(x1[Interval(slice_x->domain().ub())], slice_x->outputGate()[0]))); \
        slice_x = slice_x->nextSlice(); \
      } \
      return new_tube_x; \
    } \
    \
    Tube f(const Tube& tube_x1, const Trajectory& x2) \
    { \
      DomainException::check(tube_x1, x2); \
      DimensionException::check(tube_x1, x2); \
      Tube new_tube_x(tube_x1); \
      TubeSlice *slice_x, *first_slice_x = new_tube_x.getFirstSlice(); \
      slice_x = first_slice_x; \
      while(slice_x != NULL) \
      { \
        slice_x->setEnvelope(IntervalVector(1, ibex::f(slice_x->codomain()[0], x2[slice_x->domain()]))); \
        slice_x = slice_x->nextSlice(); \
      } \
      slice_x = first_slice_x; \
      while(slice_x != NULL) \
      { \
        if(slice_x == first_slice_x) \
          slice_x->setInputGate(IntervalVector(1, ibex::f(slice_x->inputGate()[0], x2[Interval(slice_x->domain().lb())]))); \
        slice_x->setOutputGate(IntervalVector(1, ibex::f(slice_x->outputGate()[0], x2[Interval(slice_x->domain().ub())]))); \
        slice_x = slice_x->nextSlice(); \
      } \
      return new_tube_x; \
    }

  binary_op(atan2);
  binary_op(operator+);
  binary_op(operator-);
  binary_op(operator*);
  binary_op(operator/);
  binary_op(operator|);
  binary_op(operator&);
}