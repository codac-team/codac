/* ============================================================================
 *  tubex-lib - Tube class (operators)
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_Tube.h"
#include "tubex_Trajectory.h"
#include "tubex_DomainException.h"
#include "tubex_DimensionException.h"
#include "tubex_StructureException.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  #define assignment_op_scalar(f) \
    \
    Tube& Tube::f(const Tube& x) \
    { \
      StructureException::check(*this, x); \
      Interval y; \
      TubeSlice *slice, *first_slice = getFirstSlice(); \
      const TubeSlice *slice_x, *first_slice_x = x.getFirstSlice(); \
      slice = first_slice; \
      slice_x = first_slice_x; \
      while(slice != NULL) \
      { \
        y = slice->codomain()[0]; \
        y.f(slice_x->codomain()[0]); \
        slice->setEnvelope(IntervalVector(1, y)); \
        slice = slice->nextSlice(); \
        slice_x = slice_x->nextSlice(); \
      } \
      slice = first_slice; \
      slice_x = first_slice_x; \
      while(slice != NULL) \
      { \
        if(slice == first_slice) \
        { \
          y = slice->inputGate()[0]; \
          y.f(slice_x->inputGate()[0]); \
          slice->setInputGate(IntervalVector(1, y)); \
        } \
        y = slice->outputGate()[0]; \
        y.f(slice_x->outputGate()[0]); \
        slice->setOutputGate(IntervalVector(1, y)); \
        slice = slice->nextSlice(); \
        slice_x = slice_x->nextSlice(); \
      } \
      return *this; \
    } \
    \
    Tube& Tube::f(const Interval& x) \
    { \
      Interval y; \
      TubeSlice *slice, *first_slice = getFirstSlice(); \
      slice = first_slice; \
      while(slice != NULL) \
      { \
        y = slice->codomain()[0]; \
        y.f(x); \
        slice->setEnvelope(IntervalVector(1, y)); \
        slice = slice->nextSlice(); \
      } \
      slice = first_slice; \
      while(slice != NULL) \
      { \
        if(slice == first_slice) \
        { \
          y = slice->inputGate()[0]; \
          y.f(x); \
          slice->setInputGate(IntervalVector(1, y)); \
        } \
        y = slice->outputGate()[0]; \
        y.f(x); \
        slice->setOutputGate(IntervalVector(1, y)); \
        slice = slice->nextSlice(); \
      } \
      return *this; \
    } \
    \
    Tube& Tube::f(const Trajectory& x) \
    { \
      DomainException::check(*this, x); \
      Interval y; \
      TubeSlice *slice, *first_slice = getFirstSlice(); \
      slice = first_slice; \
      while(slice != NULL) \
      { \
        y = slice->codomain()[0]; \
        y.f(x[slice->domain()]); \
        slice->setEnvelope(IntervalVector(1, y)); \
        slice = slice->nextSlice(); \
      } \
      slice = first_slice; \
      while(slice != NULL) \
      { \
        if(slice == first_slice) \
        { \
          y = slice->inputGate()[0]; \
          y.f(x[Interval(slice->domain().lb())]); \
          slice->setInputGate(IntervalVector(1, y)); \
        } \
        y = slice->outputGate()[0]; \
        y.f(x[Interval(slice->domain().ub())]); \
        slice->setOutputGate(IntervalVector(1, y)); \
        slice = slice->nextSlice(); \
      } \
      return *this; \
    }

  assignment_op_scalar(operator*=);
  assignment_op_scalar(operator/=);
  assignment_op_scalar(operator&=);
  assignment_op_scalar(operator|=);

  // Note: other operators are defined in the vector case
}