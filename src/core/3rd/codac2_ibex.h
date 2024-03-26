/** 
 *  \file
 *  IBEX-related tools
 * 
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_IBEX_H__
#define __CODAC2_IBEX_H__

#include <ibex_Interval.h>
#include <ibex_Vector.h>
#include <ibex_Matrix.h>
#include <ibex_IntervalVector.h>
#include <ibex_IntervalMatrix.h>
#include "codac2_Interval.h"
#include "codac2_Vector.h"
#include "codac2_IntervalVector.h"
#include "codac2_Matrix.h"
#include "codac2_IntervalMatrix.h"

namespace codac2
{
  /**
   * \brief Casts a reference to a Codac Interval object into a reference to an IBEX Interval object
   * 
   * \param x Codac type Interval
   * \return IBEX type Interval
   */
  ibex::Interval& to_ibex(codac2::Interval& x);

  /**
   * \brief Casts a Codac Interval object into an IBEX Interval object
   * 
   * \param x const Codac type Interval
   * \return const IBEX type Interval
   */
  const ibex::Interval& to_ibex(const codac2::Interval& x);

  /**
   * \brief Casts an IBEX Interval object into a Codac Interval object
   * 
   * \param x const IBEX type Interval
   * \return Codac type Interval
   */
  codac2::Interval to_codac(const ibex::Interval& x);
  
  #define cast_vector(OutputType,convert) \
    \
    OutputType x_(x.size()); \
    for(size_t i = 0 ; i < (size_t)x.size() ; i++) \
      x_[i] = convert(x[i]); \
    return x_; \

  /**
   * \brief Casts a Codac Vector object into an IBEX Vector object
   * 
   * \param x const Codac type Vector
   * \return IBEX type Vector
   */
  template<int N=Dynamic>
  ibex::Vector to_ibex(const codac2::Vector_<N>& x)
  {
    cast_vector(ibex::Vector,double);
  }

  /**
   * \brief Casts an IBEX Vector object into a Codac Vector object
   * 
   * \param x const IBEX type Vector
   * \return Codac type Vector
   */
  inline codac2::Vector to_codac(const ibex::Vector& x)
  {
    cast_vector(codac2::Vector,double);
  }

  /**
   * \brief Casts a Codac IntervalVector object into an IBEX IntervalVector object
   * 
   * \param x const Codac type IntervalVector
   * \return IBEX type IntervalVector
   */
  inline ibex::IntervalVector to_ibex(const codac2::IntervalVector& x)
  {
    cast_vector(ibex::IntervalVector,to_ibex);
  }

  /**
   * \brief Casts an IBEX IntervalVector object into a Codac IntervalVector object
   * 
   * \param x const IBEX type IntervalVector
   * \return Codac type IntervalVector
   */
  inline codac2::IntervalVector to_codac(const ibex::IntervalVector& x)
  {
    cast_vector(codac2::IntervalVector,to_codac);
  }
  
  #define cast_matrix(OutputType,convert_f,rows_f,cols_f,output_ij,input_ij) \
    \
    OutputType x_(x.rows_f(), x.cols_f()); \
    for(size_t i = 0 ; i < (size_t)x.rows_f() ; i++) \
      for(size_t j = 0 ; j < (size_t)x.cols_f() ; j++) \
        output_ij = convert_f(input_ij); \
    return x_; \

  /**
   * \brief Casts a Codac Matrix object into an IBEX Matrix object
   * 
   * \param x const Codac type Matrix
   * \return IBEX type Matrix
   */
  template<int R=Dynamic,int C=Dynamic>
  ibex::Matrix to_ibex(const codac2::Matrix_<R,C>& x)
  {
    cast_matrix(ibex::Matrix,double,rows,cols,x_[i][j],x(i,j));
  }

  /**
   * \brief Casts an IBEX Matrix object into a Codac Matrix object
   * 
   * \param x const IBEX type Matrix
   * \return Codac type Matrix
   */
  inline codac2::Matrix to_codac(const ibex::Matrix& x)
  {
    cast_matrix(codac2::Matrix,double,nb_rows,nb_cols,x_(i,j),x[i][j]);
  }

  /**
   * \brief Casts a Codac IntervalMatrix object into an IBEX IntervalMatrix object
   * 
   * \param x const Codac type IntervalMatrix
   * \return IBEX type IntervalMatrix
   */
  inline ibex::IntervalMatrix to_ibex(const codac2::IntervalMatrix& x)
  {
    cast_matrix(ibex::IntervalMatrix,to_ibex,rows,cols,x_[i][j],x(i,j));
  }

  /**
   * \brief Casts an IBEX IntervalMatrix object into a Codac IntervalMatrix object
   * 
   * \param x const IBEX type IntervalMatrix
   * \return Codac type IntervalMatrix
   */
  inline codac2::IntervalMatrix to_codac(const ibex::IntervalMatrix& x)
  {
    cast_matrix(codac2::IntervalMatrix,to_codac,nb_rows,nb_cols,x_(i,j),x[i][j]);
  }

} // namespace codac2

#endif