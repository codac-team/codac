/** 
 *  codac2_Zonotope.cpp
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Maël Godard, Damien Massé
 *  \copyright  Copyright 2026 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_Zonotope.h"
#include "codac2_inversion.h"
#include "codac2_arith_mul.h"

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

  Zonotope &Zonotope::operator&=(const IntervalVector &box) {
     if (this->is_empty()) return (*this);
     if (box.is_empty()) { this->set_empty(); return (*this); }
     if (this->box().is_subset(box)) return (*this);
     IntervalVector rhs = IntervalVector::constant(A.cols(),{-1,1});
     IntervalMatrix cpA = this->A;
     MulOp::bwd(box-this->c,cpA,rhs);
     if (rhs.is_empty()) { this->set_empty(); return (*this); }
     Vector delta = this->A*rhs.mid();
     this->c += delta;
     for (Index i=0;i<this->A.cols();i++) {
        this->A.col(i) *= rhs[i].rad();
     }
     return (*this);
  }
  
  Zonotope Zonotope::operator& (const IntervalVector &box) const {
      Zonotope ret(*this);
      ret  &= box;
      return ret;
  }


  Zonotope Zonotope::reduction(double o, bool invert) const {
     assert_release(o>=1.0 && "Zonotope reduction must give an order >= 1.");
     if (this->is_empty()) return (*this);
     Index dim = A.rows();
     if (A.cols() <= dim*o && !invert) return (*this);
     Index newcols = dim*o;
     Matrix newA(dim, newcols);
     Eigen::FullPivLU<Matrix>  lu(A);
     Matrix A2 = A*lu.permutationQ();
     Index rk = lu.rank();
     newA.leftCols(rk) = A2.leftCols(rk);
     Index zerocols = 0;
     if (lu.rank()<dim) { 
			/* we need to complete with other columns,
			   we select the last columns on the P^-1 matrix 
			   (which corresponds to extend U with 1 on the
			   diagonal) */
        zerocols = dim - rk; /* should be considered as new generators
				unrelated to previous ones */
        Matrix tP = lu.permutationP().transpose();
        newA.middleCols(rk,zerocols) =
              tP.rightCols(zerocols);
     } 
     IntervalMatrix invB = 
		 inverse_enclosure(newA.leftCols(dim));
     if (invB.mag().maxCoeff()== oo) { 
             /* inversion failed, which should not happen... */
             /* in this case, we make something completely different:
                we keep the first newcols-dim columns, add 
                the identity, and compute the enclosing box for the
                remaining generators */
        Index available = std::min(newcols-dim, A.cols());
        if (available>0) 
            newA.leftCols(available) = A2.leftCols(available);
        else available=0;
        newA.middleCols(available,dim) = Matrix::Identity(dim,dim);
        Index nblostgen = A2.cols()-available;
        if (nblostgen==0) { /* newcols>=dim+A.cols() => invert=true */
	   /* just create a very small box */
           newA.middleCols(available,dim) = 1e-5 * Matrix::Identity(dim,dim);
        } else {
          IntervalVector sum =
	    (A2.template cast<Interval>()).rightCols(nblostgen)*
	      IntervalVector::Constant(nblostgen,Interval(-1.0,1.0));
          for (Index i=0;i<dim;i++) {
             double u= sum[i].mag();
             newA(newcols-dim+i,i)=(u==0.0 && invert ? 1e-5 : u);
          }
        }
        return Zonotope(this->c, newA);
     }
     /* add "kept" generators (newcols-dim), starting from rk */
     newA.middleCols(dim,newcols-dim) = A2.middleCols(rk,newcols-dim);
     /* compute transformation of lost generators */
     Index nblostgen = A2.cols()-newcols+zerocols;
     IntervalVector sum = 
	(invB*(A2.template cast<Interval>()).rightCols(nblostgen))
	* IntervalVector::Constant(nblostgen,Interval(-1.0,1.0));
     for (Index i=0;i<rk;i++) {
         newA.col(i) *= (1.0+sum[i].mag());
     }
     for (Index i=rk;i<dim;i++) {
         double u= sum[i].mag();
         newA.col(i) *= (u==0.0 && invert ? 1e-5 : u);
     }
     return Zonotope(this->c, newA);
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
