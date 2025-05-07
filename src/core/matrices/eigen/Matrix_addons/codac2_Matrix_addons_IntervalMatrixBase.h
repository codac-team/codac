/** 
 *  \file codac2_Matrix_addons_IntervalMatrixBase.h
 *  
 *  This class reuses some of the functions developed for ibex::IntervalMatrixBase. 
 *  The original IBEX code is revised in modern C++ and adapted to the template 
 *  structure proposed in Codac, based on the Eigen library.
 *  See ibex::IntervalMatrixBase (IBEX lib, author: Gilles Chabert)
 * 
 *  This file is included in the declaration of Eigen::MatrixBase,
 *  thanks to the preprocessor token EIGEN_MATRIX_PLUGIN.
 *  See: https://eigen.tuxfamily.org/dox/TopicCustomizing_Plugins.html
 *  and the file codac2_matrices.h
 *
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Gilles Chabert
 *  \copyright  Copyright 2023 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

// does not work.. template<typename OtherDerived1,typename OtherDerived2,typename U=Scalar,int R=RowsAtCompileTime,int C=ColsAtCompileTime>
// does not work..   requires (
// does not work..        IsIntervalDomain<U>
// does not work..     && (!IsIntervalDomain<typename OtherDerived1::Scalar>)
// does not work..     && (!IsIntervalDomain<typename OtherDerived2::Scalar>)
// does not work..     && OtherDerived1::RowsAtCompileTime == R
// does not work..     && OtherDerived2::RowsAtCompileTime == R
// does not work..     && OtherDerived1::ColsAtCompileTime == C
// does not work..     && OtherDerived2::ColsAtCompileTime == C)
// does not work.. Matrix(MatrixBase<OtherDerived1> lb, MatrixBase<OtherDerived2> ub)
// does not work..   : Matrix<U,R,C>(lb)

template<typename U=Scalar,int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsIntervalDomain<U>
Matrix(const Matrix<double,R,C>& lb, const Matrix<double,R,C>& ub)
  : Matrix<U,R,C>(lb)
{
  assert_release(lb.size() == ub.size());

  for(Index i = 0 ; i < this->size() ; i++)
  {
    auto& lbi = *(this->data()+i);
    const auto& ubi = *(ub.data()+i);

    if(lbi.lb() > ubi)
    {
      set_empty();
      break;
    }

    else
      lbi |= ubi;
  }
}

template<typename U=Scalar,int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsIntervalDomain<U>
Matrix(int r, int c, const double bounds[][2])
  : Matrix<U,R,C>(r,c)
{
  assert_release(r > 0 && c > 0);

  Index k = 0;
  for(Index i = 0 ; i < this->rows() ; i++)
    for(Index j = 0 ; j < this->cols() ; j++)
    {
      (*this)(i,j) = codac2::Interval(bounds[k][0],bounds[k][1]);
      k++;
    }
  assert_release(k == this->size() && "incorrect array size");
}

template<typename U=Scalar,int R=RowsAtCompileTime,int C=ColsAtCompileTime,typename OtherDerived>
  requires IsIntervalDomain<U>
inline bool operator==(const MatrixBase<OtherDerived>& x) const
{
  return operator==(x.eval().template cast<codac2::Interval>());
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline double min_rad() const
{
  return (this->data()+extr_diam_index(true))->rad();
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline double max_rad() const
{
  return (this->data()+extr_diam_index(false))->rad();
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline double min_diam() const
{
  return (this->data()+extr_diam_index(true))->diam();
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline double max_diam() const
{
  return (this->data()+extr_diam_index(false))->diam();
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline Index min_diam_index() const
{
  return extr_diam_index(true);
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline Index max_diam_index() const
{
  return extr_diam_index(false);
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline Index extr_diam_index(bool min) const
{
  // This code originates from the ibex-lib
  // See: ibex_TemplateVector.h
  // Author: Gilles Chabert

  double d = min ? codac2::oo : -1; // -1 to be sure that even a 0-diameter interval can be selected
  int selected_index = -1;
  bool unbounded = false;
  assert_release(!this->is_empty() && "Diameter of an empty IntervalVector is undefined");

  Index i;

  for(i = 0 ; i < this->size() ; i++) 
  {
    if((this->data()+i)->is_unbounded()) 
    {
      unbounded = true;
      if(!min) break;
    }
    else
    {
      double w = (this->data()+i)->diam();
      if(min ? w<d : w>d)
      {
        selected_index = i;
        d = w;
      }
    }
  }

  if(min && selected_index == -1)
  {
    assert(unbounded);
    // the selected interval is the first one.
    i = 0;
  }

  // The unbounded intervals are not considered if we look for the minimal diameter
  // and some bounded intervals have been found (selected_index!=-1)
  if(unbounded && (!min || selected_index == -1))
  {
    double pt = min ? -codac2::oo : codac2::oo; // keep the point less/most distant from +oo (we normalize if the lower bound is -oo)
    selected_index = i;

    for(; i < this->size() ; i++)
    {
      if((this->data()+i)->lb() == -codac2::oo)
      {
        if((this->data()+i)->ub() == codac2::oo)
          if(!min)
          {
            selected_index = i;
            break;
          }

        if((min && (-(this->data()+i)->ub() > pt)) || (!min && (-(this->data()+i)->ub() < pt)))
        {
          selected_index = i;
          pt = -(this->data()+i)->ub();
        }
      }

      else if((this->data()+i)->ub() == codac2::oo)
        if((min && ((this->data()+i)->lb() > pt)) || (!min && ((this->data()+i)->lb() < pt)))
        {
          selected_index = i;
          pt = (this->data()+i)->lb();
        }
    }
  }

  return selected_index;
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline void set_empty()
{
  this->init(codac2::Interval::empty());
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline auto& inflate(double r)
{
  assert_release(r >= 0.);

  for(Index i = 0 ; i < this->size() ; i++)
    (this->data()+i)->inflate(r);
  return *this;
}

template<typename OtherDerived,typename U=Scalar>
  requires IsIntervalDomain<U>
inline auto& inflate(const MatrixBase<OtherDerived>& r)
{
  assert_release(this->size() == r.size());
  assert_release(r.min_coeff() >= 0.);

  for(Index i = 0 ; i < this->rows() ; i++)
    for(Index j = 0 ; j < this->cols() ; j++)
      (*this)(i,j).inflate(r(i,j));
  return *this;
}

template<typename OtherDerived>
inline auto& operator&=(const MatrixBase<OtherDerived>& x)
{
  assert_release(this->size() == x.size());

  if constexpr(std::is_same_v<typename MatrixBase<OtherDerived>::Scalar,codac2::Interval>)
  {
    if(x.is_empty())
    {
      set_empty();
      return *this;
    }
  }
  
  for(Index i = 0 ; i < this->rows() ; i++)
    for(Index j = 0 ; j < this->cols() ; j++)
      (*this)(i,j) &= x(i,j);
  return *this;
}

template<typename OtherDerived>
inline auto& operator|=(const MatrixBase<OtherDerived>& x)
{
  assert_release(this->size() == x.size());

  if constexpr(std::is_same_v<typename MatrixBase<OtherDerived>::Scalar,codac2::Interval>)
  {
    if(x.is_empty())
      return *this;
  }

  for(Index i = 0 ; i < this->rows() ; i++)
    for(Index j = 0 ; j < this->cols() ; j++)
      (*this)(i,j) |= x(i,j);
  return *this;
}

template<typename OtherDerived>
inline auto operator&(const MatrixBase<OtherDerived>& x) const
{
  auto y = *this;
  return y &= x;
}

template<typename OtherDerived>
inline auto operator|(const MatrixBase<OtherDerived>& x) const
{
  auto y = *this;
  return y |= x;
}

template<typename U=Scalar,int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsIntervalDomain<U>
inline static auto empty(Index r, Index c)
{
  assert_release(r >= 0 && c >= 0);
  return Matrix<U,R,C>::constant(r,c,codac2::Interval::empty());
}

template<typename U=Scalar,int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsIntervalDomain<U>
inline auto bisect(Index i, float ratio = 0.49) const
{
  assert_release(i >= 0 && i < this->size());
  assert_release((this->data()+i)->is_bisectable());
  assert_release(codac2::Interval(0,1).interior_contains(ratio));

  auto p = std::make_pair(*this,*this);
  auto pi = (this->data()+i)->bisect(ratio);
  *(p.first.data()+i) = pi.first;
  *(p.second.data()+i) = pi.second;
  return p;
}

template<typename U=Scalar,int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsIntervalDomain<U>
inline auto bisect_largest(float ratio = 0.49) const
{
  return bisect(max_diam_index(), ratio);
}