/** 
 *  codac2_Zonotope.cpp
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_Zonotope.h"

namespace codac2
{
  Zonotope::Zonotope(const IntervalVector& box) 
    : c(box.mid())
  {
    A = Matrix::Identity(box.size(),box.size());
    if (box.is_empty()) {
       empty_flag = true;
       return;
    }
    for (Index i=0;i<box.size();i++) A(i,i) = box[i].rad();
  }

  Zonotope::Zonotope(const Vector& c_, const Matrix& A_)
    : c(c_), A(A_)
  {
    assert_release(c.size() == A.rows());
  }

  Zonotope Zonotope::empty(Index n)
  {
    Zonotope z(Vector::Constant(n, std::numeric_limits<double>::quiet_NaN()), Matrix::Identity(n,n));
    z.empty_flag = true;
    return z;
  }

  bool Zonotope::is_empty() const
  {
    return empty_flag;
  }

  void Zonotope::set_empty()
  {
    empty_flag = true;
    c = Vector::Constant(c.size(), std::numeric_limits<double>::quiet_NaN());
  }

  IntervalVector Zonotope::box() const
  {
    return c + (A.template cast<Interval>())*IntervalVector::constant(A.cols(),{-1,1});
  }

  Zonotope Zonotope::proj(const std::vector<Index>& indices) const
  {
    assert_release(*std::min_element(indices.begin(), indices.end()) >= 0 && "indices out of range");
    assert_release(*std::max_element(indices.begin(), indices.end()) <= c.size() && "indices out of range");

    Matrix A_cropped (indices.size(), A.cols());
    Vector c_cropped (indices.size());

    for (size_t i = 0; i < indices.size(); ++i)
    {
      A_cropped.row(i) = A.row(indices[i]);
      c_cropped[i] = c[indices[i]];
    }

    return Zonotope(c_cropped, A_cropped);
  }

  Zonotope Zonotope::operator+(const Zonotope& zonotope) const
  {
    assert_release(c.size() == zonotope.c.size() && "Zonotopes must have the same dimension");

    Vector c_sum = c + zonotope.c;
    Matrix A_sum (A.rows(), A.cols() + zonotope.A.cols());

    A_sum << A, zonotope.A;

    return Zonotope(c_sum, A_sum);
  }

  Zonotope& Zonotope::operator+=(const IntervalVector& box)
  {
    assert_release(c.size() == box.size() && "The zonotope and the box must have the same dimension");
    assert_release(!this->is_empty() && "Invalid operation on empty zonotope");

    Vector c_sum = c + box.mid();
    IntervalVector box2 = (box - c_sum) + c; /* (c_sum - c) may be rounded */
    c = c_sum;
    Index nbtoadd=0;
    for (Index i=0;i<box2.size();i++) {
       if (box2[i].mag()>0.0) nbtoadd++;
    }
    if (nbtoadd==0) return (*this);
    for (Index i=A.cols()-1;i>=0;i--) { 
	/* decreasing because axis generators may have been added at the end */
      int nbnotzero=0;
      Index idnotzero=-1;
      for (Index j=0;j<A.rows();j++) {
        if (A(j,i)!=0.0) {
	  nbnotzero++;
          if (nbnotzero>1) break;
          idnotzero=j;
        }
      }
      if (nbnotzero==1 && box2[idnotzero].mag()>0.0) {
          A(idnotzero,i) += box2[idnotzero].mag(); /* rounded up ? */
          box2[idnotzero]=0.0;
          nbtoadd--;
          if (nbtoadd==0) return (*this);
      }
    }
    Index col = A.cols();
    A.conservativeResize(Eigen::NoChange, A.cols()+nbtoadd);
    for (Index i=0;i<box2.size();i++) {
       if (box2[i].mag()>0.0) {
          A.col(col)= Vector::Zero(A.rows());
          A(i,col) = box2[i].mag();
	  col++;
       }
    }
    return (*this);
  }

  Zonotope Zonotope::transform(const IntervalMatrix &mat) const {
    assert_release(!this->is_empty() && "Invalid operation on empty zonotope");
    Matrix matmid = mat.mid();
    Matrix newA = matmid*this->A;
    IntervalVector boxadd = ((mat-matmid)*this->A.template cast<Interval>())*
                    IntervalVector::constant(A.cols(),{-1,1});
    Zonotope Z(this->c,newA);
    return (Z += boxadd);
  } 


  std::ostream& operator<<(std::ostream& str, const Zonotope& z)
  {
    str << "{ ";
    
    if (z.is_empty())
      str << "empty " << (z.size()) << "-d zonotope";
    else
      str << "Zonotope center: \n" << z.c.transpose() << "\nZonotope shape matrix: \n" << z.A;

    str << " }";
    return str;
  }
}
