/* ============================================================================
 *  tubex-lib - TubeVector class (operators)
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
#include "tubex_TubeVector.h"
#include "tubex_TrajectoryVector.h"
#include "tubex_DomainException.h"
#include "tubex_DimensionException.h"
#include "tubex_SamplingException.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  #define assignment_op_vector(f) \
    \
    const TubeVector& TubeVector::f(const TubeVector& x) \
    { \
      SamplingException::check(*this, x); \
      DimensionException::check(*this, x); \
      IntervalVector y(x.dim()); \
      TubeSlice *slice, *first_slice = getFirstSlice(); \
      const TubeSlice *slice_x, *first_slice_x = x.getFirstSlice(); \
      slice = first_slice; \
      slice_x = first_slice_x; \
      while(slice != NULL) \
      { \
        y = slice->codomain(); \
        y.f(slice_x->codomain()); \
        slice->setEnvelope(y); \
        slice = slice->nextSlice(); \
        slice_x = slice_x->nextSlice(); \
      } \
      slice = first_slice; \
      slice_x = first_slice_x; \
      while(slice != NULL) \
      { \
        if(slice == first_slice) \
        { \
          y = slice->inputGate(); \
          y.f(slice_x->inputGate()); \
          slice->setInputGate(y); \
        } \
        y = slice->outputGate(); \
        y.f(slice_x->outputGate()); \
        slice->setOutputGate(y); \
        slice = slice->nextSlice(); \
        slice_x = slice_x->nextSlice(); \
      } \
      return *this; \
    } \
    \
    const TubeVector& TubeVector::f(const IntervalVector& x) \
    { \
      DimensionException::check(*this, x); \
      IntervalVector y(x); \
      TubeSlice *slice, *first_slice = getFirstSlice(); \
      slice = first_slice; \
      while(slice != NULL) \
      { \
        y = slice->codomain(); \
        y.f(x); \
        slice->setEnvelope(y); \
        slice = slice->nextSlice(); \
      } \
      slice = first_slice; \
      while(slice != NULL) \
      { \
        if(slice == first_slice) \
        { \
          y = slice->inputGate(); \
          y.f(x); \
          slice->setInputGate(y); \
        } \
        y = slice->outputGate(); \
        y.f(x); \
        slice->setOutputGate(y); \
        slice = slice->nextSlice(); \
      } \
      return *this; \
    } \
    \
    const TubeVector& TubeVector::f(const TrajectoryVector& x) \
    { \
      DomainException::check(*this, x); \
      DimensionException::check(*this, x); \
      IntervalVector y(x.dim()); \
      TubeSlice *slice, *first_slice = getFirstSlice(); \
      slice = first_slice; \
      while(slice != NULL) \
      { \
        y = slice->codomain(); \
        y.f(x[slice->domain()]); \
        slice->setEnvelope(y); \
        slice = slice->nextSlice(); \
      } \
      slice = first_slice; \
      while(slice != NULL) \
      { \
        if(slice == first_slice) \
        { \
          y = slice->inputGate(); \
          y.f(x[Interval(slice->domain().lb())]); \
          slice->setInputGate(y); \
        } \
        y = slice->outputGate(); \
        y.f(x[Interval(slice->domain().ub())]); \
        slice->setOutputGate(y); \
        slice = slice->nextSlice(); \
      } \
      return *this; \
    }

  assignment_op_vector(operator+=);
  assignment_op_vector(operator-=);
  assignment_op_vector(operator|=);
  assignment_op_vector(operator&=);
  assignment_op_vector(operator|);
  assignment_op_vector(operator&);

  // Note: operator*= and operator/= are not defined for vector items


  #define assignment_op_vector_with_scalar(f) \
    \
    const TubeVector& TubeVector::f(const Tube& x) \
    { \
      /* todo: SamplingException::check(*this, x); */\
      IntervalVector y(dim()); \
      TubeSlice *slice, *first_slice = getFirstSlice(); \
      const TubeSlice *slice_x, *first_slice_x = x.getFirstSlice(); \
      slice = first_slice; \
      slice_x = first_slice_x; \
      while(slice != NULL) \
      { \
        y = slice->codomain(); \
        y.f(slice_x->codomain()); \
        slice->setEnvelope(y); \
        slice = slice->nextSlice(); \
        slice_x = slice_x->nextSlice(); \
      } \
      slice = first_slice; \
      slice_x = first_slice_x; \
      while(slice != NULL) \
      { \
        if(slice == first_slice) \
        { \
          y = slice->inputGate(); \
          y.f(slice_x->inputGate()); \
          slice->setInputGate(y); \
        } \
        y = slice->outputGate(); \
        y.f(slice_x->outputGate()); \
        slice->setOutputGate(y); \
        slice = slice->nextSlice(); \
        slice_x = slice_x->nextSlice(); \
      } \
      return *this; \
    } \
    \
    const TubeVector& TubeVector::f(const Interval& x) \
    { \
      IntervalVector y(dim()); \
      TubeSlice *slice, *first_slice = getFirstSlice(); \
      slice = first_slice; \
      while(slice != NULL) \
      { \
        y = slice->codomain(); \
        y.f(x); \
        slice->setEnvelope(y); \
        slice = slice->nextSlice(); \
      } \
      slice = first_slice; \
      while(slice != NULL) \
      { \
        if(slice == first_slice) \
        { \
          y = slice->inputGate(); \
          y.f(x); \
          slice->setInputGate(y); \
        } \
        y = slice->outputGate(); \
        y.f(x); \
        slice->setOutputGate(y); \
        slice = slice->nextSlice(); \
      } \
      return *this; \
    } \
    \
    const TubeVector& TubeVector::f(const Trajectory& x) \
    { \
      DomainException::check(*this, (TrajectoryVector)x); \
      IntervalVector y(dim()); \
      TubeSlice *slice, *first_slice = getFirstSlice(); \
      slice = first_slice; \
      while(slice != NULL) \
      { \
        y = slice->codomain(); \
        y.f(x[slice->domain()]); \
        slice->setEnvelope(y); \
        slice = slice->nextSlice(); \
      } \
      slice = first_slice; \
      while(slice != NULL) \
      { \
        if(slice == first_slice) \
        { \
          y = slice->inputGate(); \
          y.f(x[Interval(slice->domain().lb())]); \
          slice->setInputGate(y); \
        } \
        y = slice->outputGate(); \
        y.f(x[Interval(slice->domain().ub())]); \
        slice->setOutputGate(y); \
        slice = slice->nextSlice(); \
      } \
      return *this; \
    }

  assignment_op_vector_with_scalar(operator+=);
  assignment_op_vector_with_scalar(operator-=);
  assignment_op_vector_with_scalar(operator|);
  assignment_op_vector_with_scalar(operator&);
}