/** 
 *  \file codac2_IntvFullPivLU.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Damien Massé
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <ostream>
#include <cstdlib>
#include "codac2_Matrix.h"
#include "codac2_Interval.h"
#include "codac2_IntervalMatrix.h"
#include "codac2_IntervalVector.h"
#include "codac2_BoolInterval.h"
#include "codac2_IntvFullPivLU.h"
#include "codac2_arith_mul.h"

namespace codac2 {

/* utility function : compute the bound using Gauss-Jordan successive
   algorithm */
IntervalMatrix IntvFullPivLU::build_LU_bounds(const IntervalMatrix &E) {
    const Index nCols = E.cols();
    const Index nRows = E.rows();
    const Index nC = std::min(nCols,nRows-1);
  
    IntervalMatrix res = E;
    for (Index k=0;k<nC;k++) {
       if (k>0) {
         res.block(k,k,1,nCols-k)
            += res.block(k,0,1,k) * res.topRightCorner(k,nCols-k);
         res.block(k+1,k,nRows-k-1,1)
	    += res.bottomLeftCorner(nRows-1-k,k) * res.block(0,k,k,1);
       }
       Interval pivot = 1.0/(1.0-res(k,k));
       res.block(k+1,k,nRows-k-1,1) *= pivot;
    }
    if (nCols>=nRows) { /* one row left */
         res.block(nRows-1,nRows-1,1,nCols-nRows+1)
            += res.block(nRows-1,0,1,nRows-1) * 
	       res.topRightCorner(nRows-1,nCols-nRows+1);
    } 
    return res;
}


/************************************************************/


/** constructor from Matrix of double 
 */
IntvFullPivLU::IntvFullPivLU(const Matrix &M) :
   _LU(M), 
   matrixLU_(M.rows(), M.cols())
{
   this->compute_matrix_LU(IntervalMatrix(M), _LU.maxPivot()*_LU.threshold());
}
  
/** constructor from Matrix of Intervals
 */
IntvFullPivLU::IntvFullPivLU(const IntervalMatrix &M) :
   _LU(M.mid()), 
   matrixLU_(M.rows(), M.cols())
{
   this->compute_matrix_LU(M,_LU.maxPivot()*_LU.threshold());
}

void IntvFullPivLU::compute_matrix_LU(const IntervalMatrix &M, double nonzero) {
   /* specific case if _LU.max_pivot() is 0 (the matrix itself is 0) */
    if (_LU.maxPivot()==0.0) {
       nonzero=1.0; 
	/* strong threshold. The most probable result is oo anyway */
    }
   /* 1) calculer la matrice d'erreurs
          -> déterminer L'^{-1}, U'^{-1}
      2) calcul les exposants de cette matrice
      3) reconstuire L et U
    */
    int nRows=M.rows();
    int nCols=M.cols();
    int dim = std::min(nRows,nCols);
    Matrix mLU = _LU.matrixLU();
    /* modification of mLU if not full rank (check this) */
    for (int i=0;i<dim;i++) {
        if (std::fabs(mLU(i,i))<nonzero) {
           mLU(i,i)=(mLU(i,i)<0.0 ? -nonzero : nonzero); 
        }
    }
    /* "Inversion" of mLU (i.e. (pseudo)inversion of L and U) */
    IntervalMatrix ImLU 
		= IntervalMatrix::Zero(nRows,nCols);
    /* first L */
    for (int c=0;c<nCols;c++) {
      if (c>=nRows-1) break;
      for (int r=c+1;r<nRows;r++) {
         Interval s(mLU(r,c));
         for (int k=c+1;k<r;k++) {
            if (k>=nCols) break;
            s += mLU(r,k)*ImLU(k,c);
         }
         ImLU(r,c)=-s;
      }
    }
    /* then U */
    for (int c=0;c<nCols;c++) {
      int r;
      if (c<nRows) {
         ImLU(c,c)=Interval(1.0)/mLU(c,c);
         r=c-1;
      } else {
         r=nRows-1;
      }
      for (;r>=0;r--) {
         Interval s(mLU(r,c));
         if (c<nRows) s/=mLU(c,c);
         for (int k=r+1;k<c;k++) {
             if (k>=nRows) break;
             s += mLU(r,k)*ImLU(k,c);
         }
         ImLU(r,c)=-s/mLU(r,r);
      }
    }
    
    /* compute the error matrix */
    IntervalMatrix InvL = 
		IntervalMatrix::Identity(nRows,nRows);
    if (nRows>nCols) 
      InvL.leftCols(nCols).triangularView<Eigen::StrictlyLower>() = ImLU;
    else 
      InvL.triangularView<Eigen::StrictlyLower>() = ImLU.leftCols(nRows);
    IntervalMatrix InvU = 
		IntervalMatrix::Identity(nCols,nCols);
    if (nCols>nRows) 
      InvU.topRows(nRows).triangularView<Eigen::Upper>() = ImLU;
    else 
      InvU.triangularView<Eigen::Upper>() = ImLU.topRows(nCols);
    IntervalMatrix error = IntervalMatrix::Identity(nRows,nCols) - 
       InvL * (_LU.permutationP() * M * 
       _LU.permutationQ()) * InvU;

    IntervalMatrix eps = IntvFullPivLU::build_LU_bounds(error);
    
    /* product */
    for (Index c=0;c<nCols;c++) 
    for (Index r=0;r<nRows;r++) {
       matrixLU_(r,c)=mLU(r,c); /* for Id */
       if (r>c) { /* low part */
          matrixLU_(r,c)-=eps(r,c); /* Id for L part */
          for (Index k=c+1;k<r;k++) {
             if (k>=nCols) break;
             matrixLU_(r,c)-=mLU(r,k)*eps(k,c);
          }
       } else { /* high part */
          for (Index k=r;k<=c;k++) {
             if (k>=nRows) break;
             matrixLU_(r,c)-=eps(r,k)*mLU(k,c);
          }
          if (c>=nRows) matrixLU_(r,c)-=eps(r,c); 
			/* Id for U part outside mLU */
       }
    }
}

/* computing the possible rank for a square matrix is easy as
   we have only to look at the diagonal of U
   for non-square matrix, the remaining lines/columns _may_ have
   the rank */
Interval IntvFullPivLU::rank() const {
    Interval res(0.0);
    /* first we compute the rank generated by the diagonal of U */
    Index dim = std::min(matrixLU_.rows(),matrixLU_.cols());
    for (Index i=0;i<dim;i++) {
       if (matrixLU_(i,i).mig()>0.0) res+=1.0;
       else 
       if (matrixLU_(i,i).mag()>0.0) res+=Interval(0.0,1.0);
    }
    if (res.lb()==dim) return res; /* matrix if full rank */
    /* complement with potential other lines / rows */
    Index remain = std::max(matrixLU_.rows(),matrixLU_.cols())-dim;
    if (remain==0.0) return res; /* square matrix */
    res = min(dim,res+Interval(0.0,remain));
    return res;
}


double IntvFullPivLU::max_pivot() const {
    double res=0.0;
    /* first we compute the rank generated by the diagonal of U */
    Index dim = std::min(matrixLU_.rows(),matrixLU_.cols());
    for (Index i=0;i<dim;i++) {
       res = std::max(matrixLU_(i,i).ub(),res);
    }
    return res;
}

/* possible dimension of kernel = number of columns - rank of the matrix */
Interval IntvFullPivLU::dimension_of_kernel() const {
    Interval rk = this->rank();
    return matrixLU_.cols()-rk;
}

BoolInterval IntvFullPivLU::is_surjective() const {
    if (matrixLU_.rows()>matrixLU_.cols()) return BoolInterval::FALSE;
    Interval r=this->rank();
    if (r.lb()==matrixLU_.rows()) return BoolInterval::TRUE;
    if (r.ub()==matrixLU_.rows()) return BoolInterval::UNKNOWN;
    return BoolInterval::FALSE;
}

BoolInterval IntvFullPivLU::is_injective() const {
    if (matrixLU_.rows()<matrixLU_.cols()) return BoolInterval::FALSE;
    Interval r=this->rank();
    if (r.lb()==matrixLU_.cols()) return BoolInterval::TRUE;
    if (r.ub()==matrixLU_.cols()) return BoolInterval::UNKNOWN;
    return BoolInterval::FALSE;
}

BoolInterval IntvFullPivLU::is_invertible() const {
    if (!matrixLU_.is_squared()) return BoolInterval::FALSE;
    Interval r=this->rank();
    if (r.lb()==matrixLU_.cols()) return BoolInterval::TRUE;
    if (r.ub()==matrixLU_.cols()) return BoolInterval::UNKNOWN;
    return BoolInterval::FALSE;
}

Interval IntvFullPivLU::determinant() const {
    assert_release(matrixLU_.is_squared());
    Interval product(1.0);
    for (Index i=0;i<matrixLU_.cols();i++) {
       product *= matrixLU_(i,i);
          /* if matrixLU_(i,i)=0.0, the determinant is 0 even if
             a previous diagonal element was (-oo,oo) */ 
    }
    return product;
}

IntervalMatrix IntvFullPivLU::kernel() const {
    /* like the rank, the computation is complex , and 
       may give strange results in some bases. We'll build the vectors
       one by one and make the matrix afterwards */
    std::vector<IntervalVector> kernel;
    std::vector<bool> generating(matrixLU_.cols(),true);
    for (Index c=0;c<matrixLU_.cols();c++) {
      /* a potential kernel vector is built as follows :
         we consider each column c and states that the vector must be 0
         for all column > c , and 1 for c.
         if we manage to build such a vector by filling dependant indices
         < c, then this index is stated as "generating" and will be 0 for
         the next vectors. Otherwise, it is states as "not generating" and 
         and used in next vectors */
      /* before anything, if c<matrixLU_.rows() and 
         matrixLU_(c,c) does not contains 0, just states that 
         c is not generating and go on */
      if (c<matrixLU_.rows() && !matrixLU_(c,c).contains(0.0)) {
          generating[c]=false;
          continue;
      }
      IntervalVector vect = IntervalVector::Zero(matrixLU_.cols());
      vect[c]=1.0;
      for (Index c1=c-1;c1>=0;c1--) {
         if (c1>=matrixLU_.rows())  {
             /* we will consider two cases :
                1) generating[c1] is true. then 
                       we consider vect[c1]=0.0
                2) generating[c1] is false. we did not manage to
		   build a vector using an element outside the square
		   part of U, which should almost never happens,
		   but means that the decomposition badly failed.
                   just consider vect[c1]=top in this case */
             if (!generating[c1]) vect[c1]=Interval();
             continue;
         }
         /* in the following, either 0.0 \notin matrixLU_(c1,c1)
            and generating(c1) is false, which is fine, 
            or 0.0 in matrixLU_(c1,c1), which is less fine, but we
            go on nevertheless by checking the row */
         Interval z(0.0);
         for (Index c2=c1+1;c2<=c;c2++) {
           z -= matrixLU_(c1,c2)*vect[c2];
         }
         /* vect[c1] * matrixLU_(c1,c1) = z */
         vect[c1]=Interval();
         Interval tmp = matrixLU_(c1,c1);
         MulOp::bwd(z,vect[c1],tmp);
         if (vect[c1].is_empty()) { 
             /* means that matrixLU_(c1,c1)=0.0 
		and z!=0.0... that's bad news */
             generating[c]=false; break;
         }
         /* note: here, in theory if generating[c1] is 0 we should
            be able to put vect[c1]=0.0 even if 0.0 is not in 
            vect[c1], because the actual kernel is a combination
            of found vectors. _however_, it means that the vector
            we are constructing is _not_ in the kernel, just that it
            can be used to build the kernel... but that may be the only
            acceptable approach */
         if (generating[c1]) vect[c1]=0.0;
      }
      if (!generating[c]) continue;
      kernel.push_back(_LU.permutationQ()*vect);
    }
    /* build the matrix */
    IntervalMatrix res(matrixLU_.cols(),kernel.size());
    for (unsigned int c=0;c<kernel.size();c++) {
       res.col(c)=kernel[c];
    }
    return res;
}

IntervalMatrix IntvFullPivLU::solve(const IntervalMatrix &rhs) const {
    assert_release(rhs.rows()==matrixLU_.rows());
    IntervalMatrix prhs = _LU.permutationP()*rhs;
    /* inverse L */
    for (Index r = 0; r<matrixLU_.rows()-1; r++) {
       for (Index r1=r+1;r1<matrixLU_.rows();r1++) {
          prhs.row(r1) -= matrixLU_(r1,r)*prhs.row(r);
       }
    }
    /* first check, if rows>cols, the 0 lines */
    for (Index r=matrixLU_.cols(); r<matrixLU_.rows();r++) {
        for (Index a=0;a<prhs.cols();a++) {
            if (!prhs(r,a).contains(0.0)) {
               Interval emp; emp.set_empty();
               return 
		IntervalMatrix::Constant(matrixLU_.cols(),rhs.cols(),emp);
            }
        }
    }
    /* then use the diagonal elements */
    IntervalMatrix res = IntervalMatrix::Zero(matrixLU_.cols(),rhs.cols());
    Index dim = std::min(matrixLU_.cols(),matrixLU_.rows());
    for (Index r = dim-1;r>=0;r--) {
        for (Index r1=r+1;r1<dim;r1++) {
           prhs.row(r) -= matrixLU_(r,r1)*res.row(r1);
        }
        if (matrixLU_(r,r).contains(0.0)) {
            for (Index a=0;a<prhs.cols();a++) {
              if (!prhs(r,a).contains(0.0)) {
                 for (auto coef : res.reshaped()) coef.set_empty();
                 return res;
              }
            } 
        } else {
            res.row(r) = (1.0/matrixLU_(r,r))*prhs.row(r);
        }
    }
    return _LU.permutationQ()*res;
}

IntervalMatrix IntvFullPivLU::reconstructed_matrix() const {
    Eigen::FullPivLU<Matrix>::PermutationPType Pi = _LU.permutationP().inverse();
    Eigen::FullPivLU<Matrix>::PermutationPType Qi = _LU.permutationQ().inverse();
    IntervalMatrix product = matrixLU_.triangularView<Eigen::Upper>();
    /* for the LU product, we must consider 
       [-oo,oo]*0 = [-oo,oo] */
    for (Index c=0;c<matrixLU_.cols();c++)
    for (Index r=0;r<matrixLU_.rows();r++) {
       for (Index k=0;k<r && k<=c;k++) {
          if (matrixLU_(r,k).is_unbounded() || matrixLU_(k,c).is_unbounded()) {
            product(r,c) = Interval(); break;
          }
          product(r,c) += matrixLU_(r,k)*matrixLU_(k,c);
       }
    }
    return Pi*product*Qi;
}


}
