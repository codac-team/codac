/** 
 *  codac2_ibex.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Gilles Chabert, (Simon Rohou)
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <cassert>
#include "codac2_ibex.h"

using namespace std;

namespace codac2
{
  ibex::Interval to_ibex(const Interval& x)
  {
    return { x.lb(), x.ub() };
  }

  Interval to_codac(const ibex::Interval& x)
  {
    return { x.lb(), x.ub() };
  }
  
  #define cast_vector(OutputType,convert) \
    \
    OutputType x_(x.size()); \
    for(Index i = 0 ; i < (Index)x.size() ; i++) \
      x_[i] = convert(x[i]); \
    return x_; \

  ibex::Vector to_ibex(const codac2::Vector& x)
  {
    cast_vector(ibex::Vector,double);
  }

  codac2::Vector to_codac(const ibex::Vector& x)
  {
    cast_vector(codac2::Vector,double);
  }

  ibex::IntervalVector to_ibex(const codac2::IntervalVector& x)
  {
    cast_vector(ibex::IntervalVector,to_ibex);
  }

  codac2::IntervalVector to_codac(const ibex::IntervalVector& x)
  {
    cast_vector(codac2::IntervalVector,to_codac);
  }
  
  #define cast_matrix(OutputType,convert_f,output_ij,input_ij,rows_,cols_) \
    \
    OutputType x_(x.rows_(), x.cols_()); \
    for(Index i = 0 ; i < (Index)x.rows_() ; i++) \
      for(Index j = 0 ; j < (Index)x.cols_() ; j++) \
        output_ij = convert_f(input_ij); \
    return x_; \

  ibex::Matrix to_ibex(const codac2::Matrix& x)
  {
    cast_matrix(ibex::Matrix,double,x_[i][j],x(i,j),rows,cols);
  }

  codac2::Matrix to_codac(const ibex::Matrix& x)
  {
    cast_matrix(codac2::Matrix,double,x_(i,j),x[i][j],nb_rows,nb_cols);
  }

  ibex::IntervalMatrix to_ibex(const codac2::IntervalMatrix& x)
  {
    cast_matrix(ibex::IntervalMatrix,to_ibex,x_[i][j],x(i,j),rows,cols);
  }

  codac2::IntervalMatrix to_codac(const ibex::IntervalMatrix& x)
  {
    cast_matrix(codac2::IntervalMatrix,to_codac,x_(i,j),x[i][j],nb_rows,nb_cols);
  }

} // namespace codac