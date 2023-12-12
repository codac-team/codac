/** 
 *  Utility functions on matrices (exponential, inversion...)
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Damien Massé
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac_Interval.h"
#include "codac_IntervalVector.h"
#include "codac_IntervalMatrix.h"
#include "codac_Vector.h"
#include "codac2_expIMat.h"
#include <cmath>
#include <vector>

namespace codac2 {

/// A) Utility functions

// 1) quadratic expressions (optimal computation of M(M+A)  )

// returns [x]([x]+a) 
Interval quad_I(const Interval& x, double a) {
    return sqr(x+(a/2))-a*a/4;
}
// returns [x]([x]+[a]) (optimal if [x] and [a] independant)
Interval quad_II(const Interval& x, const Interval& a) {
    return quad_I(x,a.lb())|quad_I(x,a.ub());
}

// union of matrices
void join_mat(int dim, IntervalMatrix& Res, const IntervalMatrix& M) {
   for (int i=0;i<dim;i++)
   for (int j=0;j<dim;j++) Res[i][j] |= M[i][j];
}

// compute [M]([M]+a I)
IntervalMatrix quad_M(const IntervalMatrix& M, double a) {
   const int n = M.nb_rows();
   IntervalMatrix Res(n,n,Interval::zero());
   for (int k=0;k<n;k++) {
      for (int i=0;i<n;i++) {
         if (i==k) continue;
         for (int j=0;j<n;j++) {
             if (j==k) continue;
             Res[i][j] += M[i][k]*M[k][j];
         }
      }
   }
   for (int i=0;i<n;i++)
   for (int j=0;j<n;j++) {
      if (i==j) {
         Res[i][i] += quad_I(M[i][i],a);
      } else {
         Res[i][j] += M[i][j] * (M[i][i]+M[j][j]+a);
      }
   }
   return Res;
}


// compute [M](tau[M]+a I) with tau \in [0,1]
IntervalMatrix quad_Mtau(const IntervalMatrix& M, double a) {
   const int n = M.nb_rows();
   IntervalMatrix Res(n,n,Interval::zero());
   for (int k=0;k<n;k++) {
      for (int i=0;i<n;i++) {
         if (i==k) continue;
         for (int j=0;j<n;j++) {
             if (j==k) continue;
             Res[i][j] += M[i][k]*M[k][j];
         }
      }
   }
   for (int i=0;i<n;i++)
   for (int j=0;j<n;j++) {
      if (i==j) {
         Res[i][i] += quad_I(M[i][i],a);
         Res[i][i] |= (M[i][i]*a);
      } else {
         Res[i][j] += M[i][j] * (M[i][i]+M[j][j]+a);
         Res[i][j] |= (M[i][j]*a);
      }
   }
   return Res;
}
// compute [M]([M]+[A]) inter ([M]+[A])[M]
IntervalMatrix quad_MM(const IntervalMatrix& M, const IntervalMatrix& A) {
   const int n = M.nb_rows();
   IntervalMatrix Res(n,n,Interval::zero());
   IntervalMatrix Res2(n,n,Interval::zero());
   for (int k=0;k<n;k++) {
      for (int i=0;i<n;i++) {
         if (i==k) continue;
         for (int j=0;j<n;j++) {
             if (j==k) continue;
             Res[i][j] += M[i][k]*(M[k][j]+A[k][j]);
             Res2[i][j] += (M[i][k]+A[i][k])*M[k][j];
         }
      }
   }
   for (int i=0;i<n;i++)
   for (int j=0;j<n;j++) {
      if (i==j) {
         Interval u = quad_II(M[i][i],A[i][i]);
         Res[i][i] += u;
         Res2[i][i] += u;
      } else {
         Interval c = M[j][j]+A[j][j];
         Interval clb = (M[i][i]).lb() + c;
         Interval cub = (M[i][i]).ub() + c;
         Interval a1 = M[i][j]*clb+(M[i][i]).lb()*A[i][j];
         Interval a2 = M[i][j]*cub+(M[i][i]).ub()*A[i][j];
         Res[i][j] += (a1|a2);
         Interval b1 = M[j][i]*clb+(M[i][i]).lb()*A[j][i];
         Interval b2 = M[j][i]*cub+(M[i][i]).ub()*A[j][i];
         Res2[j][i] += (b1|b2);
      }
   }
   Res&= Res2;
   return Res;
}
      
// compute [M](tau [M]+[A]) inter (tau [M]+[A])[M]
IntervalMatrix quad_MMtau(const IntervalMatrix& M, const IntervalMatrix& A) {
   const int n = M.nb_rows();
   IntervalMatrix Res = quad_MM (M,A);
   IntervalMatrix Res2 = M*A;
   Res2 &= A*M;
   join_mat(n,Res,Res2);
   return Res;
}
      

// compute k1*[A]+k2*([A]*[B] cap [B]*[A])+k3*[B] (A and B are square)
// commutation if commute=true
// could also be done using (A+(k3/k2) Id)(k2 B + k1 Id) - ((k1k3)/k2) Id
IntervalMatrix quad_prod(const IntervalMatrix& A, const IntervalMatrix& B, double k1, double k2, double k3,bool commute) {
   const int n = A.nb_rows();
   IntervalMatrix Res(n,n,Interval::zero());
   IntervalMatrix Res2(n,n,Interval::zero());
   for (int k=0;k<n;k++) {
      for (int i=0;i<n;i++) {
         if (i==k) continue;
         for (int j=0;j<n;j++) {
             if (j==k) continue;
             Res[i][j] += k2*A[i][k]*B[k][j];
         }
      }
   }
   if (commute) {
     for (int k=0;k<n;k++) {
        for (int i=0;i<n;i++) {
           if (i==k) continue;
           for (int j=0;j<n;j++) {
               if (j==k) continue;
               Res2[i][j] += k2*B[i][k]*A[k][j];
           }
         }
      }
   }
   for (int i=0;i<n;i++)
   for (int j=0;j<n;j++) {
      if (i==j) {
         if (commute) Res[i][j] &= Res2[i][j];
         Interval a1 = A[i][i]*(k1+k2*(B[i][i]).lb()) + k3*(B[i][i]).lb();
         Interval a2 = A[i][i]*(k1+k2*(B[i][i]).ub()) + k3*(B[i][i]).ub();
         Res[i][i] += (a1|a2);
      } else {
         Res[i][j] += A[i][j]*(k1+k2*B[j][j]) + (k2*A[i][i]+k3)*B[i][j];
         if (commute) {
           Res2[i][j] += B[i][j]*(k3+k2*A[j][j]) + (k1+k2*B[i][i])*A[i][j];
           Res[i][j] &= Res2[i][j];
         }
      }
   }
   return Res;
}

// upper bound for the integral of the absolute value of linear intervalsi
// ( [I]+t[P] between 0 and delta )
double integral_abs_linint(const Interval& I, const Interval& P, const Interval& V, double delta) {
   if (I.lb()>=0 && I.lb()+P.lb()>=0) { /* pure >=0 */ 
      return V.ub();
   } else if (I.ub()<=0 && I.ub()+P.ub()<=0) { /* pure <=0 */
      return -V.lb();
   }
   double mdI = I.mid();
   double mdP = P.mid();
   double modif;
   bool useub; /* true -> use ub ; false -> use lb */
   double t = (mdP==0.0 ? 1.0 : (mdI/mdP));
   if ((t>0) || (t<=-delta)) { // no change of bound
      useub =  (mdI>=0.0);
      modif=0.0;
   } else {
      useub = (mdP>0);
      modif = mdI*t; // useub <=> mdI<0 => modif>0
   }
   Interval in = delta*(I+0.5*delta*P)+modif;
   if (useub) return in.ub();
   return - in.lb();
}

// 2) power functions

// returns [M]^2 (special case of quad_M(M,0))
IntervalMatrix square_IntervalMatrix (const IntervalMatrix& M) {
   return quad_M(M,0.0);
}
//  M^n with n>=0 (complexity (dim M)^3 * (log n))
// uses squaring and intersected-symetric product
IntervalMatrix power_IntervalMatrix (const IntervalMatrix& M, unsigned int n) {
    if (n==0) return Matrix::eye(M.nb_rows());
    IntervalMatrix Res = M;
    int u=0;
    while ((n >> u)>1) u++;
    u--;
    while (u>=0) {
       Res = square_IntervalMatrix(Res);
       if (n & (1 << u)) {
           IntervalMatrix Res2 = M*Res;
           Res *= M;
           Res &= Res2;
       }
       u--;
    }
    return Res;
}
// squaring (Id+M) : compute Phi(M,l) = [Id+M]^(2^l)-Id , k>=0
// Phi(M,l+1) = quad_M(Phi(M,l),2)    
void squaring_IntervalMatrix(IntervalMatrix& M, unsigned int k) {
    for (unsigned int i=0;i<k;i=i+1) {
        M = quad_M(M,2.0);
    }
}
// squaring (Id+tauM) : compute Phi(M,l) = ([Id+tau M]^(2^l)-Id)/tau , k>=0
// Phi(M,l+1) = quad_M(Phi(M,l),2)    
void squaringTau_IntervalMatrix(IntervalMatrix& M, unsigned int k) {
    for (unsigned int i=0;i<k;i=i+1) {
        M = quad_Mtau(M,2.0);
    }
}
// "squaring" for integral : 
// compute Phi(S,M,k) such that Phi(S,M,0) = S
// Phi(S,M,k+1) = Phi(S,M,k) + 2^(k-1) M [Phi(S,M,k)]^2
void primSquaring_IntervalMatrix(IntervalMatrix& S,
             const IntervalMatrix&M, unsigned int k) {
    double base=0.5;
    for (unsigned int i=0;i<k;i=i+1) {
        S += base*M*square_IntervalMatrix(S);
        base = base*2.0;
    }
}

/// B) Matrix inversion

// 1) Inversion

// matrix inversion
// a) Res :=  A^(-1)Res   ( false if A is a singular matrix)
bool inv_Matrix(const Matrix& A, Matrix& Res) {
     Matrix M_copy(A);
     const int order = A.nb_rows();
     assert(order == A.nb_cols());
     for (int col=0;col<order;col++) {
        // looking for a line without 0, and largest absolute value
        double best_val=0.0; int best_line=-1;
        for (int lin=col;lin<order;lin++) {
           if (fabs(M_copy[lin][col])>best_val) {
             best_val=fabs(M_copy[lin][col]);
             best_line=lin;
           }
        }
        if (best_line==-1) { // singular
            return false;
        }
        int lin = best_line;
        for (int lin2=0;lin2<order;lin2++) {
            if (lin2==lin) continue;
            // transfer lines  to put M_copy[lin2][col] to 0
            double fact = -M_copy[lin2][col]/M_copy[lin][col];
            M_copy[lin2][col]=0.0;
            for (int col2=col+1;col2<order;col2++) {
                M_copy[lin2][col2] += fact*M_copy[lin][col2];
            }
            for (int col2=0;col2<order;col2++) {
                Res[lin2][col2] += fact*Res[lin][col2];
            }
        }
        // exchange line lin and line col
        if (lin!=col) { 
          {
            Vector swp(M_copy[lin]);
            M_copy[lin] = M_copy[col]; 
            M_copy[col] = swp;
          }
          {
            Vector swp(Res[lin]);
            Res[lin] = Res[col]; 
            Res[col] = swp;
          }
        }
     }
     // simplify to 1
     for (int lin=0;lin<order;lin++) {
         double rem = 1.0/M_copy[lin][lin];
         Res[lin] *= rem;
    } 
    return true;
}

// matrix inversion, crude Gaussian elimination using Rows
// returns empty if [M] may contains a singular matrix (finding a subset
// of M would be useful...)
IntervalMatrix inv_IntervalMatrix(const IntervalMatrix& M, bool prec) {
     IntervalMatrix Res (Matrix::eye (M.nb_rows()));
     inv_IntervalMatrix(M,Res,prec);
     return Res;
}


// 2) Res :=  A^(-1)Res   ( empty if A contains a singular matrix)

// (still crude Gaussian elimination using Rows)
void inv_IntervalMatrix(const IntervalMatrix& A, 
					IntervalMatrix& Res, bool prec) {
     IntervalMatrix M_copy(A);
     if (prec) {
       Matrix Mid= A.mid();
       Matrix IMid= Matrix::eye(A.nb_rows());
       if (!inv_Matrix(Mid,IMid)) {
          Res.set_empty(); return;       
       }
       Res = IMid * Res;
       M_copy = IMid * M_copy;
     }
     const int order = A.nb_rows();
     assert(order == A.nb_cols());
     for (int col=0;col<order;col++) {
        // looking for a line without 0, and smallest diam/mignitude
        double best_ratio; int best_line=-1;
        for (int lin=col;lin<order;lin++) {
           Interval& I=M_copy[lin][col];
           if (!I.contains(0.0)) {
             double ratio = I.diam()/I.mig();
             if (best_line==-1 || ratio<best_ratio) {
                best_ratio=ratio;
                best_line=lin;
             }
           }
        }
        if (best_line==-1) { // singular
            Res = IntervalMatrix::empty(order,order);
            return;
        }
        int lin = best_line;
        for (int lin2=0;lin2<order;lin2++) {
            if (lin2==lin) continue;
            // transfer lines  to put M_copy[lin2][col] to 0
            Interval fact = -M_copy[lin2][col]/M_copy[lin][col];
            M_copy[lin2][col]=0;
            for (int col2=col+1;col2<order;col2++) {
                M_copy[lin2][col2] += fact*M_copy[lin][col2];
            }
            for (int col2=0;col2<order;col2++) {
                Res[lin2][col2] += fact*Res[lin][col2];
            }
        }
        // exchange line lin and line col
        if (lin!=col) { 
          {
            IntervalVector swp(M_copy[lin]);
            M_copy[lin] = M_copy[col]; 
            M_copy[col] = swp;
          }
          {
            IntervalVector swp(Res[lin]);
            Res[lin] = Res[col]; 
            Res[col] = swp;
          }
        }
     }
     // simplify to 1
     for (int lin=0;lin<order;lin++) {
         Interval rem = 1.0/M_copy[lin][lin];
         Res[lin] *= rem;
    } 
}

/* punctualize a couple "matrix" - "inverse of matrix" :
   starting from ([A],[Ai]) such that [A].[Ai] ~ Id, compute
   [A'] <Aim>...
   technique used : Am = [A].mid    Aim = [Ai].mid
   then compute [A'] = (Am [Aim])^-1 Am
   we use the algorithm of inv_IntervalMatrix with simplification
   (assume Am [Aim] is almost Id */
void punctualize_coupleMatrix(IntervalMatrix& A,
        IntervalMatrix &Ai) {
     Ai = Ai.mid();
     A = A.mid();
     IntervalMatrix M_copy(A*Ai);
     const int order = A.nb_rows();
     assert(order == A.nb_cols());
     for (int col=0;col<order;col++) {
        int lin = col; /* we consider that the best line is always col 
                          (the matrix is almost Id) */
        assert (!M_copy[lin][col].contains(0.0));
        for (int lin2=0;lin2<order;lin2++) {
            if (lin2==lin) continue;
            // transfer lines  to put M_copy[lin2][col] to 0
            Interval fact = -M_copy[lin2][col]/M_copy[lin][col];
            M_copy[lin2][col]=0;
            for (int col2=col+1;col2<order;col2++) {
                M_copy[lin2][col2] += fact*M_copy[lin][col2];
            }
            for (int col2=0;col2<order;col2++) {
                A[lin2][col2] += fact*A[lin][col2];
            }
        }
     }
     // simplify to 1
     for (int lin=0;lin<order;lin++) {
         Interval rem = 1.0/M_copy[lin][lin];
         A[lin] *= rem;
    } 
}

/* compute M*Mt */
IntervalMatrix gramMatrix(const IntervalMatrix &M) {
    int dim = M.nb_rows();
    IntervalMatrix Res(dim,dim);
    for (int i=0;i<dim;i++) {
       for (int j=i+1;j<dim;j++) {
	  Res[i][j] = Res[j][i] = M[i]*M[j];
       }
       Res[i][i] = M[i].sqnorm2();
    }
    return Res;
}
/* compute V(col)*Vt , like outerproduct on itself */
IntervalMatrix gramMatrix(const IntervalVector &V) {
    int dim = V.size();
    IntervalMatrix Res(dim,dim);
    for (int i=0;i<dim;i++) {
       for (int j=i+1;j<dim;j++) {
	  Res[i][j] = Res[j][i] = V[i]*V[j];
       }
       Res[i][i] = sqr(V[i]);
    }
    return Res;
}

/* compute P*M*Pt, with P a row vector  */
Interval productPMPt(const IntervalVector &P, const IntervalMatrix &M) {
     assert(P.size()==M.nb_rows() && P.size()==M.nb_cols());
     Interval Res(0.0);
     for (int i=0;i<P.size();i++) {
        Interval Res2(0.0);
        for (int j=i+1;j<P.size();j++) {
 	   Res2 += P[j]*(M[i][j]+M[j][i]);
        }
        Res += P[i]*Res2 + sqr(P[i])*M[i][i];
     }
     return Res;
}


// Determination of a rank a set of vectors A, with Af the "maximal" set
// of free vectors, and E = pseudo-inverse (A Et = id)
// if needed, we complete to get a square matrix with other vectors
// return : >=0 : index inside A, <0 : in complete
std::vector<int> construct_free_set(const IntervalMatrix &A,
		IntervalMatrix &ARes, 
		IntervalMatrix &E, const IntervalMatrix *compM) {
    int rank=0;
    std::vector<int> res;
    std::vector<bool> used(A.nb_rows(),false);
    /* get the first vector (max norm) */
    int place=0;
    Interval max_sqn = A[0].sqnorm2();
    double max_norm=max_sqn.mig()/(max_sqn.diam()+1e-10);
    for (int i=1;i<A.nb_rows();i++) {
       Interval sqn = A[i].sqnorm2();
       double m2 = sqn.mig()/(sqn.diam()+1e-10);
       if (m2>max_norm) {
	  max_norm=m2; place=i; max_sqn = sqn;
       }
    }
    if (max_norm==0.0) return res; /* rank may be 0 ! */
    res.push_back(place);
    used[place]=true;
    ARes.resize(1,A.nb_cols());
    ARes[0]=A[place];
    E.resize(1,A.nb_cols());
    max_sqn = 1.0/max_sqn;
    E[0]=max_sqn*A[place];
    rank=1;
    IntervalMatrix G(A.nb_cols(),A.nb_cols());
    G = Matrix::eye(A.nb_cols()); 
    G -= max_sqn*gramMatrix(ARes[0]);
    /* G = Id - E^T A is a symetric matrix used to compute other results 
       TODO: geometric interpretation ? (minimal square ?) */
    while (rank<A.nb_rows()) {
      /* next vector */
      int place=-1;
      Interval delta(0.0);
      IntervalVector bestPU;
      for (int i=0;i<A.nb_rows();i++) {
         if (used[i]) continue;
			/* we can use P and PT indifferently */
         Interval delta2 = productPMPt(A[i],G);
         if (delta2.contains(0.0)) used[i]=true; /* not usable */
         else if (place==-1 ||
	   delta2.mig()/(1e-10+delta2.diam())>
			delta.mig()/(1e-10+delta.diam())) {
  	    delta=delta2; place=i;
         }
      }
      if (place==-1) break; /* can't go further */
      res.push_back(place);
      used[place]=true;
      rank++;
      Interval Idelta=1.0/delta;
      IntervalMatrix H = Matrix::eye(A.nb_cols()) - 
			Idelta*(gramMatrix(A[place])*G);
      E *= H;
      E.resize(rank,A.nb_cols());
      E[rank-1]= Idelta*(A[place]*G);
      G *= H;
      ARes.resize(rank,A.nb_cols());
      ARes[rank-1]=A[place];
   }
   if (compM!=NULL) {
      while (rank<A.nb_cols()) {
        /* next vector */
        int place=-1;
        Interval delta(0.0);
        for (int i=1;i<A.nb_cols();i++) {
           const IntervalVector& V = (*compM)[i];
           Interval delta2 = productPMPt(V,G);
           if (!delta2.contains(0.0) && (place==-1 ||
	     delta2.mig()/(1e-10+delta2.diam())>
				delta.mig()/(1e-10+delta.diam()))) {
  	      delta=delta2; place=i;
           }
        }
        if (place==-1) break; /* or assert(place==-1) ? big pb here */ 
        const IntervalVector& V = (*compM)[place];
        res.push_back(-place-1);
        rank++;
        Interval Idelta=1.0/delta;
        IntervalMatrix H = Matrix::eye(A.nb_cols()) - 
			Idelta*(gramMatrix(V)*G);
        E *= H;
        E.resize(rank,A.nb_cols());
        E[rank-1]= Idelta*(V*G);
        G *= H;
        ARes.resize(rank,A.nb_cols());
        ARes[rank-1]=V;
      }
   }
   return res;
}



/// C) Matrix exponential

// 1) Horner scheme

// compute the exponential using Horner scheme, including or not error term
// k>=2. Use (k/2)+1 matrix products
//          M*(I+M/2+M²/(2*3)(I+M/4+M²/(4*5)(I+M/... 
//                      ||M||^(k+1)/((k+1)*(1-||M||/(k+2)))
IntervalMatrix exp_Horner(const IntervalMatrix& M,
                 unsigned int k, bool error) {
    Matrix Id = Matrix::eye (M.nb_rows());
    IntervalMatrix Res = Matrix::zeros(M.nb_rows());
    if (k%2==0 && k>2)  Res = (Interval::one()/k)*M;
    Interval norm = infinite_norm(M);
    Interval rem = norm*norm/(k*(k+1))/(1.0-norm/(k+2));
    int i=k-1;
    if (i%2==1) {
         rem *= norm/i;
         i--;
    }
    if (i>0) {
      IntervalMatrix sqM = square_IntervalMatrix(M);
      while (i>0) { // i>=2 (i is even)
         rem *= norm*norm/i/(i-1);
         Res+=Id;
         Res *= ((Interval::one()/(i+1))*sqM);
         if (i>2) { Res += M; // special case for i==2 : M/2 is added later
                    Res *= (Interval::one()/i);
         }
         i-=2;
      }
    }
    Res = 0.5*quad_MM(M,Res+2.0*Id);
    if (error) {
      Res.inflate(rem.ub());
    }
    return Res;
}
// adapted Horner scheme for degre 4, minus ID
// (TODO) check usefulness
// result      0.5 M(2I+M)*(I+M²/6) - M⁴/24
IntervalMatrix exp_Horner4(const IntervalMatrix& M, bool error) {
    Matrix Id = Matrix::eye (M.nb_rows());

    Interval onediv6 = Interval::one()/6.0;
    IntervalMatrix A1 = 0.5*quad_M(M,2.0);
    IntervalMatrix A2 = square_IntervalMatrix(M);
    IntervalMatrix Cf = A1*(Id+onediv6*A2);
    IntervalMatrix A3 = 0.25*onediv6*square_IntervalMatrix(A2);
    Cf = Cf-A3;
    if  (error) {
      Interval norm = infinite_norm(M);
      Interval rem = norm*norm/20/(1-norm/(6));
      int i = 3;
      while (i>0) {
         rem *= norm/i;
         i--;
      }
//      std::cout << "norm : " << norm << " remainder  : " << rem << "\n";
      Cf.inflate(rem.ub());
    }
    return Cf;
}
    

// (exp(tau A)-ID)/tau with tau in [0,1] (suppose A almost constant)
IntervalMatrix exp_HornerTau(const IntervalMatrix& M,
                 unsigned int k, bool error) {
    Matrix Id = Matrix::eye (M.nb_rows());
    IntervalMatrix Res = Matrix::zeros(M.nb_rows());
    Interval Unt(0.0,1.0);
    if (k%2==0 && k>2)  Res = (Unt/k)*M;
    Interval norm = infinite_norm(M);
    Interval rem = norm*norm/(k*(k+1))/(1.0-norm/(k+2));
    int i=k-1;
    if (i%2==1) {
         rem *= norm/i;
         i--;
    }
    if (i>0) {
      IntervalMatrix sqM = square_IntervalMatrix(M);
      while (i>0) { // i>=2 (i is even)
         rem *= norm*norm/i/(i-1);
         Res+=Id;
         Res *= ((Unt/(i+1))*sqM);
         if (i>2) { Res += M; // special case for i==2 : M/2 is added later
                    Res *= (Unt/i);
         }
         i-=2;
      }
    }
    Res = 0.5*(quad_Mtau(M,2.0)+M*Res);
    if (error) {
      Res.inflate(rem.ub());
    }
    return Res;
}

// 2) Padé approximant

void addId(IntervalMatrix &M) {
    Matrix Id = Matrix::eye (M.nb_rows());
    M += Id;
}

// Padé 3,2 approximation (simplified numerator), no error term (FIXME)
// using Den^(-1)*(M*Num)
IntervalMatrix exp_pade32b(const IntervalMatrix& M) {
    Matrix Id = Matrix::eye (M.nb_rows());
    IntervalMatrix Mden = Id+0.04*quad_M(M,-8.0);
    IntervalMatrix Mnum = Id+(1.0/60.0)*quad_M(M,6.0);
    IntervalMatrix Res = M*Mnum;
    inv_IntervalMatrix(Mden,Res);
    addId(Res);
    return Res;
}

// 3) Scaling and squaring

// using generic Horner scheme with error term
IntervalMatrix exp_SSHorner(const IntervalMatrix& M,
           unsigned int k, unsigned int l) {
    double a = 1.0;
    for (unsigned int i=0;i<l;i++) a *=0.5;
    IntervalMatrix A = a*M;
    IntervalMatrix Res = exp_Horner(A,k,true);
    squaring_IntervalMatrix(Res,l);
    addId(Res);
    return Res;
}
// using generic Horner scheme with error term
IntervalMatrix exp_SSHornerTau(const IntervalMatrix& M,
           unsigned int k, unsigned int l) {
    double a = 1.0;
    const int dim = M.nb_rows();
    for (unsigned int i=0;i<l;i++) a *=0.5;
    IntervalMatrix A = a*M;
    Interval Unt(0.0,1.0);
    IntervalMatrix Res = Unt*exp_HornerTau(A,k,true);
    IntervalMatrix Res2 = exp_Horner(A,k,true);
    Matrix Id = Matrix::eye (M.nb_rows());
    for (unsigned int i=0;i<l;i++) {
       IntervalMatrix Res3 = (Res+Id)*(Id+Res2)-Id;
       for (int j=0;j<dim;j++) for (int k=0;k<dim;k++) Res[j][k]|= Res3[j][k];
       Res2 = quad_M(Res2,2.0);
    }
    addId(Res);
    return Res;
}
// using degre 5 Horner scheme, and bound for infinity norm instead of l
IntervalMatrix exp_SSHorner4(const IntervalMatrix& M,
           double norm) {
    double a = 1.0;
    unsigned l=0;
    double normM = infinite_norm(M);
    while (normM>norm) { l=l+1; normM=normM/2; a *=0.5; }
    IntervalMatrix A = a*M;
    IntervalMatrix Res = exp_Horner4(A,true);
    squaring_IntervalMatrix(Res,l);
    addId(Res);
    return Res;
}


/// D) Integral of matrix exponential
// Compute S(M,t) = Int_{0}^{t} e^Ms ds = Int_{0}{t} e^M(t-s) ds
// with t>0 

// S(M,t) = t*S0(M,t)
// S0(M,t) = Id + (tM)/(2!) + (tM)²/(3!) + ... + (tM)^k/((k+1)!) + err
//          with err = ||M||^(k+1) |t|^(k+1) / ((k+1)!(k+2-||M|| |t|)) E
//       ( TODO : prove it )
//                      ||M||^(k+1)/((k+1)*(1-||M||/(k+2)))

// FIXME : everything below is false
// furthermore, we have :
// S((2^k)M,t) = 1/(2^k) (S(M,t))^(2^k)   (idem with S0)
// and if Phi(M,t,k) = S0((2^k)M,t) - Id = 1/(2^k) (S0(M,t)^(2^k) - Id)
//    then Phi(M,t,k+1) = 1/(2^(k+1)) (S0(M,t)^(2^(k+1)) - Id)
//                      = 1/(2^(k+1)) ((2^k Phi(M,t,k) + Id)^2 - Id)
//          = 1/(2^(k+1)) (2^(2k) Phi(M,t,k)^2 + 2^(k+1) Phi(M,t,k) + Id - Id)
//          = 2^(k-1) Phi(M,t,k)^2 + Phi(M,t,k)
//          = 2^(k-1) [ Phi(M,t,k)* [Phi(M,t,k) + 1/(2^(k-1)) Id]]


// compute S0(Mt,k) using Horner scheme, including or not error term,
// minus Id. Condition : k>=2, use (k/2)+1 matrix products
IntervalMatrix expI_Horner(const IntervalMatrix& Mt,
                 unsigned int k, bool error) {
    Matrix Id = Matrix::eye (Mt.nb_rows());
    IntervalMatrix Res = Matrix::zeros(Mt.nb_rows());
    if (k%2==0 && k>2)  Res = (Interval::one()/(k+1))*Mt;
    Interval norm = infinite_norm(Mt);
    Interval rem = norm*norm/(k*(k+1)*(k+2-norm));
    int i=k-1;
    if (i%2==1) {
         rem *= norm/i;
         i--;
    }
    if (i>0) {
      IntervalMatrix sqMt = square_IntervalMatrix(Mt);
      while (i>0) { // i>=2 (i is even)
         rem *= norm*norm/i/(i-1);
         Res+=Id;
         Res *= ((Interval::one()/(i+2))*sqMt);
         if (i>2) {
            Res += Mt; // special case for i==2 : M/2 is added later
            Res *= (Interval::one()/(i+1));
         }
         i-=2;
      }
    }
    Res = (Interval::one()/6)*quad_MM(Mt,Res+3.0*Id);
//    std::cout << "norm : " << norm << " remainder  : " << rem << "\n";
    if (error) {
      Res.inflate(rem.ub());
    }
    return Res;
}

// using Horner scheme with error term, scaling and squaring
IntervalMatrix expI_SSHorner(const IntervalMatrix& M,
           const Interval &t, unsigned int k, unsigned int l) {
    double a = 1.0;
    for (unsigned int i=0;i<l;i++) a *=0.5;
    IntervalMatrix A = a*t*M;
    IntervalMatrix Res = expI_Horner(A,k,true);
    addId(Res);
    primSquaring_IntervalMatrix(Res,A,l);
    return t*Res;
}

// using Horner scheme with error term, scaling and squaring
IntervalMatrix expI_SSHorner4(const IntervalMatrix& M,
           const Interval &t, double norm) {
    double a = 1.0;
    int l=0;
    IntervalMatrix A = t*M;
    double normM = infinite_norm(A);
    while (normM>norm) {
        normM /= 2.0; l++; a*=0.5;
    }
    A *= a;
    IntervalMatrix Res = expI_Horner(A,5,true);
    addId(Res);
//    std::cout << "expI_SSHorner4 : " << l << "\n";
    primSquaring_IntervalMatrix(Res,A,l);
    return t*Res;
}


/// E) General computation
// Input : matrix M, time t, centered uncertainty u (positive vector)
// Compute simultaneously 
//    a) exp(Mt) 
//    b) (int_0^t exp(tau M) dtau)
//    c) int_0^t | exp(tau M) Diag(u) | dtau 
//                  (where Diag(u) is the matrix of diagonal u)

#if 0
// constrain exp(tau M) (with tau = [0,1])
// we know that for v in [0,1], exp(v M) \in Id + v M exp(tau M) 
// and for v in [0,1], exp(v M) \in (exp(M) - (1-v) M exp(tau M))
// here expM = exp(M)-Id and tauExpM = 1/t (exp (tau M) - Id)
// TODO : not really defined
void constrain_tauEM(IntervalMatrix& tauExpM, 
              const IntervalMatrix& M, const IntervalMatrix& expM,
              const IntervalMatrix& Id) {
     IntervalMatrix C1 = M*(expM+Id);
     IntervalMatrix C2 = M*(expM+Id);
     
}
#endif

// computes A*B inter B*A
inline IntervalMatrix prod_com(const IntervalMatrix &A, const IntervalMatrix &B) {
   IntervalMatrix M=(A*B);
   return (M&= (B*A));
//   return A*B;
}
inline void prod_com_with(IntervalMatrix &A, const IntervalMatrix &B) {
   IntervalMatrix M=(B*A);
   A *= B;
   A &= M;
}

// "compute" alpha (I + alpha I1) with alpha in [0,1]
Interval sumAlphaInterval(const Interval &I, const Interval &I1) {
    Interval resI = Interval::zero();
    resI |= I+I1; /* alpha=1 */
    if (2.0*I1.lb()>-I.lb() && I.lb()<0) {
       Interval V(I.lb());
       V = sqr(V)/(-2.0*I1.lb());
       resI |= V;
    } else if (2.0*I1.ub()<-I.ub() && I.ub()>0) {
       Interval V(I.ub());
       V = sqr(V)/(-2.0*I1.ub());
       resI |= V;
    }
    return resI;
}
IntervalMatrix sumAlphaMatrix(const IntervalMatrix &M, const IntervalMatrix &M1){
    IntervalMatrix R(M.nb_rows(),M.nb_cols());
    for (int i=0;i<M.nb_rows();i++) 
      for (int j=0;j<M.nb_cols();j++) {
         R[i][j] = sumAlphaInterval(M[i][j],M1[i][j]);
    }
    return R;
}
    
// specific function : alpha(alpha-1)/2*Id + alpha^2*(2alpha-3)/12 * M
void vtauexpbase(IntervalMatrix &M) {
    for (int i=0;i<M.nb_rows();i++) 
      for (int j=0;j<M.nb_cols();j++) {
         if (i!=j) M[i][j] *= Interval(-1.0,0.0)/12.0;
         else {
             Interval u;
             double y = M[i][j].lb();
             if (y>=0) {
		u = Interval::zero();
             } else {
                u = -y;
		u /= 12.0;
             }
             y = M[i][j].ub();
             if (y==0) { u |= Interval(-0.125); }
             else {
                Interval a(y);
                Interval b1 = 1.0-sqr(a)/16.0;
                b1|=1.0;
                b1 /= -24.0; /* approximation of 1/(6y^2)(2-sqrt(4+y^2)) */
                Interval v = sqrt(sqr(a)+4);
                b1 &= 1.0/(6.0*sqr(a))*(2.0-v);
                u |= 1.0/24.0*(-y-v)+b1;
             }
//             std::cout << "M[i][j] = " << M[i][j] << "\tu = " << u << "\n";
             M[i][j] = u;
         }
      }
}

// basic version without squaring or equivalent, M (or Mt) is very small,
// k is number of iterations and must be odd, >=5
// We need : 
// a) ExpA = exp(A)-Id =~ sum(1,k+1) A^n/(n!)
//         = 1/2 A(A+2Id)+1/6A²(A+ Base1) + ET0
//    error term : nET0 = NA^(k+2) / ((k+2)!(1-NA/(k+3)))
//    Base1 = A²/4(Id+1/5(A+A²/6(Id+1/7(A+...+A²/(k+1)))))
// b) tauExpA = { 1/tau * (exp(tau A)-Id) | tau \in U } avec u = [0,1]
//            = 1/2 A(UA+2Id)+UA²/6(A+U * UBase1) + ET0
//    UBase1 = A²/4(Id+U/5(A+UA²/6(Id+U/7(A+...+UA²/(k+1))))) 
// c) IexpA =~ sum(1,k) A^n/((n+1)!)
//          = 1/6A(A+3Id + Base1) + ET1
//    error term : nET1 = NA^(k+1) / ((k+2)!(1-NA/(k+3)))
// d) tauIexpA =~ sum(1,k) tau^(n-1)A^n/((n+1)!)
//          = 1/6A(UA+3Id + U* UBase1) + ET1
// e) VexpA = sum(1,k) -nA^n/(2(n+2)!)
//      = -1/24A(A+2Id+1/2 A²/5(3Id+1/6(4A+A²/7(5Id + 
//                          1/8(6A+A²/9(...+A²*k/(k+2))))))) + nET2
//    error term : nET2 = (k+5) NA^(k+1)/(2 (k+1)!(k+2-NA)(k+3))
// f) tauVexpA = { (tau²phi_2(tau A) - 2^l *tau/2 phi_1(tau A) - 
//                    (tau²-tau*2^l)/2 Id)/(tau²(2tau-3.2^l)/12) | tau \in U } 
//             =  sum(0,k) tau(2tau-(n+2)*2^l)(tau A)^n/(2(n+2)!)
//      = A + [0,1+1/(2-2^(1-l))]/3 A² (Id + [0,1+1/(3-2^(1-l)]/4 (A +
//			+ A² [0,1+1/(4-2^(1-l))]/5 ...))...) + ET2
// FIXME : check nET2 !!!!!!!
// tauIexpA is needed only with slices, VexpA with non-autonomous eqdiff,
// tauVexpA with both
void global_exp_base(const IntervalMatrix& A,
          int k, bool with_slices, bool with_time,
          IntervalMatrix& ExpA,
          IntervalMatrix& invExpA,
          IntervalMatrix& tauExpA,
          IntervalMatrix& IexpA,
          IntervalMatrix& tauIexpA,
          IntervalMatrix& VexpA,
          IntervalMatrix& tauVexpA,
	  int l) {
    int dim = A.nb_rows();
    Matrix Id = Matrix::eye (dim);
    Interval Unt(0.0,1.0);
    Interval norm = infinite_norm(A);
    // ensure k is odd
    if (k%2==0) k=k+1;
    Interval baseErrorTerm = norm*norm/(k*(k+1)*(k+2));
    IntervalMatrix sqA = square_IntervalMatrix(A);
    ExpA = (Interval::one()/(k+1))*sqA; // will be used for base1 
    invExpA = ExpA; // inverse = same with -A (but (-A)^2 = A^2)
    tauExpA = ExpA; // Ubase1 (/Unt)
    double pow2 = pow(2.0,(1-l));
    if (with_time) {
       IexpA = Id; // temporary use
       VexpA = (k*Interval::one()/(k+2))*sqA;
       if (with_slices) tauVexpA = Interval(0.0,1.0+1.0/((k+1)-pow2))/(k+2.0)*sqA;
    }
    while (k>3) {
       ExpA += A; ExpA *= Interval::one()/k; ExpA += Id;
       invExpA -= A; invExpA *= Interval::one()/k; invExpA += Id;
       prod_com_with (ExpA , (Interval::one()/(k-1))*sqA);
       prod_com_with (invExpA, (Interval::one()/(k-1))*sqA);
       tauExpA = sumAlphaMatrix(A,tauExpA); tauExpA *= Interval::one()/k;
       tauExpA += Id; prod_com_with (tauExpA , (Interval::one()/(k-1))*sqA);
       // tauExpA += A; tauExpA *= Unt/k; tauExpA += Id;
       // prod_com_with (tauExpA , (Unt/(k-1))*sqA);
       if (with_time) {
           VexpA += (k-1)*A; VexpA *= Interval::one()/(k+1); 
           VexpA += (k-2)*Id; VexpA *= (Interval::one()/k)*sqA;
           if (with_slices) {
             tauVexpA += A; tauVexpA *= Interval(0.0,1.0+1.0/(k-pow2))/(k+1.0);
             tauVexpA += Id; tauVexpA *= Interval(0.0,1.0+1.0/(k-1-pow2))/k*sqA;
           }
       }
       k-=2;
       baseErrorTerm *= norm*norm/(k*(k+1));
    }
    // here, ExpA is base1, tauExpA is Ubase1,
    // VexpA and tauVexpA are as expected (?)
    // baseErrorTerm = NA^(k-1)/(3*4*...*(k+1))
    // first without error term
    // VexpA and tauVexpA if needed
    Interval one6 = Interval::one()/6.0;
    if (with_time) {
       if (with_slices) {
//            if (l==0) {
//		tauVexpA = Interval(-0.125,0.0) *Id
//			- 0.5*one6*Unt*A*(Id+Interval(0.0,0.5)*(A+tauVexpA));
//                tauVexpA=prod_com(A,(Id+Interval(0.0,0.5)*(A+tauVexpA)));
//                vtauexpbase(tauVexpA);
//	    } else {
//		tauVexpA = Interval(2.0/pow2,1.0-2.0/pow2)*
//		  (Id + one6*Interval(0.0,2.0+2.0/(2-pow2))*A*(Id+
//			0.25*Interval(0.0,1.0+1.0/(3-pow2))*(A+tauVexpA)));
//         }
   	   tauVexpA = 
		prod_com(A,Id+Interval(0.0,1.0+1.0/(3-pow2))*(A+tauVexpA));
       }
    }
    /* computation of the ET (/A^2) */
    baseErrorTerm *= 0.5*norm; // NA^k/(k+2)!
    Interval nET0 = baseErrorTerm*(k+3)/(k+3-norm);
                      // nET0 = NA^k/((k+2)! (1-norm/(k+3)))


    // VexpA, "linear" and "normal" error term
    Interval nET1 = nET0 * norm;
    if (with_time) {
      IntervalMatrix VexpA1(VexpA);
      VexpA1.inflate(24.0*nET0.ub());
      VexpA1 = -0.25*one6*quad_MM(A,2.0*Id+0.5*VexpA1);
      VexpA = -0.25*one6*quad_MM(A,2.0*Id+0.5*VexpA);
      VexpA.inflate(0.5*nET1.ub());
      VexpA &= VexpA1;
      if (with_slices) {
   	   tauVexpA = 
		prod_com(A,Id+Interval(0.0,1.0+1.0/(3-pow2))*(A+tauVexpA));
           tauVexpA.inflate(1.0/(6-pow2)*nET1.ub());
      }
    }

    // IexpA and VexpA, "linear" and "normal" error term
    IntervalMatrix pId(Id);
    pId.inflate(2.0*nET0.ub());
    IntervalMatrix IexpA1 = one6*quad_MM(A,3.0*pId+ExpA);
    IexpA = one6*quad_MM(A,3.0*Id+ExpA);
    IexpA.inflate(nET1.ub());
    IexpA &= IexpA1;
    if (with_slices) {
      IntervalMatrix tauIexpA1 = one6*quad_MMtau(A,3.0*pId+Unt*tauExpA);
      tauIexpA = one6*quad_MMtau(A,3.0*Id+Unt*tauExpA);
      tauIexpA &= prod_com(one6*A,3.0*Id+sumAlphaMatrix(A,tauExpA));
      tauIexpA.inflate(nET1.ub());
      tauIexpA &= tauIexpA1;
    }
    

    // ExpA, tauExpA and invExpA, with "square" error term
        // ExpA = 1/2 A(A+2Id) + A²/6( A+ Base1) + ET0
    ExpA += A;
    IntervalMatrix ExpA1(ExpA);
    ExpA1.inflate(6.0*nET0.ub());
    prod_com_with(ExpA1, sqA); ExpA1 *= one6;
    ExpA1 = 0.5*quad_M(A,2.0)+ExpA1;
    invExpA -= A;
    IntervalMatrix invExpA1(invExpA);
    invExpA1.inflate(6.0*nET0.ub());
    prod_com_with(invExpA1, sqA); invExpA1 *= one6;
    invExpA1 = 0.5*quad_M(-A,2.0)+invExpA1;

          // tauExpA    = 1/2 A(UA+2Id+ U/3(A² + UA UBase1)
          //            = 1/2 A(UA+2Id)+UA²/6(A+U * UBase1) + ET0
    prod_com_with(tauExpA,A); 
    tauExpA = sumAlphaMatrix(sqA,tauExpA);
    IntervalMatrix tauExpA1(tauExpA);
    tauExpA1.inflate(6.0*nET0.ub());
    tauExpA1 = 0.5*quad_MMtau(A,2.0*(Id+one6*tauExpA1));
    // and "normal" error term
    nET0 *= norm*norm;
    prod_com_with(ExpA, sqA); ExpA *= one6;
    ExpA = 0.5*quad_M(A,2.0)+ExpA;
    ExpA.inflate(nET0.ub());
    ExpA &= ExpA1;
    prod_com_with(invExpA, sqA); invExpA *= one6;
    invExpA = 0.5*quad_M(-A,2.0)+invExpA;
    invExpA.inflate(nET0.ub());
    invExpA &= invExpA1;

    tauExpA = 0.5*quad_MMtau(A,2.0*(Id+one6*tauExpA));
    tauExpA.inflate(nET0.ub());
    tauExpA &= tauExpA1;

//    std::cout << "M" << A << "\nExpM" << ExpA << "\ntauExpM" << tauExpA << "\nIExpM" << IexpA << "\ntauIExpM" << tauIexpA << "\nVExpM" << VexpA << "\ntauVExpM" << tauVexpA << "\n";
}


#if 0

// basic version without squaring or equivalent, M (or Mt) is very small,
// k is number of iterations
// Base = M²/4 + M³/20 + ... (3! M^k)/((k+2)!)
// BaseP = M/5 + M²/30 + ... (4! M^(k-1))/((k+1)!)
// IexpM = Mt/2 + (Mt)²/6 + (Mt)/6*Base = 1/6 * Mt (Mt+3Id+Base) 
//       = (int_0^t exp(Ms) ds) / t - Id
// IIexpM = (Mt)/6 + (Mt)²/24 + (Mt)²/4!*BaseP... = 1/24 * Mt (Mt + 4Id + BaseP)
//       = ((int_0^t int_0^s exp(Mv) dv ds) - t)
// expM = M + M²/2 + M²/6 * Base = 0.5 M (M+2Id) + M²/6*(M+Base) 
//      = exp(M)-Id
// tauExpM = M + tau M²/2 + tau²M³/6 + ... = 0.5 M (tauM+2Id) + M²/6*Tau Base
//         = (exp(tau M) - Id)/tau
void global_exp_base(const IntervalMatrix& M,
          int k,
          IntervalMatrix& expM,
          IntervalMatrix& IexpM,
          IntervalMatrix& IIexpM,
          IntervalMatrix& tauExpM,
          bool needs_IIexpM) {
    int dim = M.nb_rows();
    Matrix Id = Matrix::eye (dim);
    Interval Unt(0.0,1.0);
    Interval norm = infinite_norm(M);
    Interval remIExpM = norm*norm/(k*(k+1));
    IexpM = Matrix::zeros(dim,dim);
    IIexpM = Matrix::zeros(dim,dim);
    tauExpM = Matrix::zeros(dim,dim);
    if (k%2==0 && k>2) {
         IexpM = (Interval::one()/(k+1))*M;
         tauExpM = Unt * IexpM;
    }
//    std::cout << IexpM << "\n    " << remIExpM << "\n";
    int i=k-1;
    if (i%2==1) {
         remIExpM *= norm/i;
         i--;
    }
    IntervalMatrix sqM = square_IntervalMatrix(M);
    while (i>0) { // i>=2 (i is even)
       remIExpM *= norm*norm/i/(i-1);
       IexpM+=Id;
//    std::cout << IexpM << "\n    " << remIExpM << "\n";
       IexpM *= ((Interval::one()/(i+2))*sqM);
       tauExpM *= ((Unt/(i+2))*sqM);
       if (i>2) { // special case for i==2 : stops at M²/4
                  IexpM += M;
                  IexpM *= (Interval::one()/(i+1));
                  tauExpM += M;
                  tauExpM *= Unt/(i+1);
                  if (needs_IIexpM && i==4) IIexpM = IexpM; // baseP
       }
       i-=2;
    }
    Interval one6 = Interval::one()/6.0;
    expM = 0.5*quad_M(M,2.0)+one6*sqM*(M+IexpM);
    IexpM = one6*quad_MM(M,IexpM+3.0*Id);
    if (needs_IIexpM) IIexpM = 0.25*one6*quad_MM(M,IIexpM+4.0*Id);
    tauExpM = 0.5*(quad_Mtau(M,2.0))+Unt*one6*sqM*(M+tauExpM);
    // adding error
    Interval remExpM = norm*remIExpM/((k+2)*(1-norm/(k+3)));
    remIExpM = remIExpM/(k+2-norm);
    expM.inflate(remExpM.ub());
    IexpM.inflate(remIExpM.ub());
    if (needs_IIexpM)
       IIexpM.inflate(remIExpM.ub()); // FIXME : correct error term of IIexpM
    tauExpM.inflate(remExpM.ub());
//    std::cout << expM << "\n  " << IexpM << "\n    " << remIExpM << "\n";
//    std::cout << tauExpM << "\n\n";
//    constrain_tauEM(tauExpM, M, expM);
}
#endif

/* add tau*mag(M*diag(V)) to Res */
void add_prod_MV(int dim,
                Matrix& Res, const IntervalMatrix& M,
                const IntervalVector V, double tau) {
   for (int i=0;i<dim;i++)
   for (int j=0;j<dim;j++) Res[i][j]+= tau*(M[i][j]*V[j]).mag();
}

/* add integral of absolute value of delta (A + tB), with t between 0 and 1 */
/* and delta>0 */
void add_int_absL (int dim, Matrix& Res,
         const IntervalMatrix& A, const IntervalMatrix& B, 
	 const IntervalMatrix& V,
         double delta) {
   for (int i=0;i<dim;i++)
   for (int j=0;j<dim;j++) 
        Res[i][j] += delta*integral_abs_linint(A[i][j], B[i][j], V[i][j], 1.0);
}

    
/* "squaring" for the non-autonomous continuous term ("tauVexpM").
 * start with four terms (matrix of intervals) M1, M2, M3, M4 (and l)
 * and approximate :
 *  (2tau - 3*2^i) tau^2 M1 + tau^2 M2 + tau M3 + M4
 *  ------------------------------------------------
 *        (tau+1)^2 (tau+1 - 3*2^(i-1))
 * by appromating each rational fractions on four points: 0, 1/4, 3/4, 1
 * (s.t. [0,1/4] points included in 0 \cup 1/4, and so on...)
 * new approach : transform M4 + tau M3 as
 *       (K + tau) M3 + (M4 - K M3), as M4 - K M3 is close to 0
 *         with K = (1-3*2^(l-1)) / (3*(1-2^l))
 * other approach : place M4 as a constant : here K = 1-3*2^(l-1)
 * M4/(1-3*2^(l-1)) + 
 *     ( (2tau-3*2^l)tau^2(M1-M4/(2K)) + tau^2(M2-3M4/K) 
 *                + tau (M3-(3-6*2^(l-1))M4/K) )/(tau+1^2)(tau+1-3*2^(l-1))
 */
IntervalMatrix squaring_tauVexpM(const IntervalMatrix &M1,
          const IntervalMatrix &M2,  const IntervalMatrix &M3,
          const IntervalMatrix &M4, int l) {
        int pow2lm1 = 1<<(l-1); /* 2^(l-1) */
	Interval u1(1-3.0*pow2lm1); /* 1-3*2^(l-1) */
        // Interval K = (u1/(3.0-6.0*pow2lm1));
        // IntervalMatrix NM4 = M4-K*M3; 
        /* constant value : P_0(0)*M4 */
        IntervalMatrix NM4 = (1.0/u1)*M4;
//        std::cout << "constant : " << NM4[0][0] << "\n";
        IntervalMatrix NM3 = M3-(3.0-6.0*pow2lm1)*NM4;
        IntervalMatrix NM2 = M2-3.0*NM4;
        IntervalMatrix NM1 = M1-0.5*NM4;
        /* values of 1/4 */
        Interval u2(5.0-12.0*pow2lm1); /* 5-3*(2^(l+1)) */
        Interval i25m3 = 16.0/(25.0*u2);
        i25m3 |= 1.0/(4.0*u1);
        Interval i25m2 = 4.0/(25.0*u2);
        i25m2 |= 0.0;
        Interval i25m1 = 4.0*(0.5-6.0*pow2lm1)/(25.0*u2);
        i25m1 |= 0.0;
//        std::cout << "I25 : " << i25m3 << i25m2 << i25m1 << "\n";
//        std::cout << "I25M1 : " << (i25m1*NM1[0][0]) << "M2 : " << (i25m2*NM2[0][0]) << "M3 : " << (i25m3*NM3[0][0]) << "\n";
        IntervalMatrix Res =
                i25m3 * NM3 + i25m2 * NM2 + i25m1 * NM1;
//        std::cout << "Res25 : " << Res[0][0] << "\n";
        /* values of 3/4 */
        Interval u3=u2+2.0; /* 5-3*(2^(l+1)) */
        Interval u4 = u1+1.0; /* 2-3*2^(l-1) */
        Interval i75m3 = 48.0/(49.0*u3);
        i75m3 |= 3.0*(3.0-4.0*pow2lm1)/(16*sqr(u4));
        Interval i75m2 = 36.0/(49.0*u3);
        i75m2 |= (7.0-9.0*pow(2.0,l-1))/(16*sqr(u4));
        Interval i75m1 = (1.5-6.0*pow2lm1)*36.0/(49.0*u3);
        i75m1 |= (4.0*(1.0-6.0*pow2lm1)*u4 + 
		  12.0*(1.0-3.0*pow2lm1)*(1.0-pow2lm1))/(32*sqr(u4));
        IntervalMatrix R75 =
                i75m3 * NM3 + i75m2 * NM2 + i75m1 * NM1;
//        std::cout << "I75 : " << i75m3 << i75m2 << i75m1 << "\n";
//        std::cout << "I75M1 : " << (i75m1*NM1[0][0]) << "M2 : " << (i75m2*NM2[0][0]) << "M3 : " << (i75m3*NM3[0][0]) << "\n";
//        std::cout << "Res75 : " << R75[0][0] << "\n";
        join_mat(Res.nb_rows(),Res,R75);
        /* values of 1 */
        Interval i100m2 = 1.0/(4.0*u4); /* idem M3 */
        Interval i100m1 = (2.0-6.0*pow2lm1)/(4.0*u4);
//        std::cout << "I100 : " << i100m2 << i100m1 << "\n";
        IntervalMatrix R100 =
                i100m2 * (NM3+NM2) + i100m1 * NM1;
//        std::cout << "Res100 : " << R100[0][0] << "\n";
        join_mat(Res.nb_rows(),Res,R100);
        Res += NM4;
        return Res;
}


static const double mnorm = 0.025;
static const int nbitbase = 5;
void global_exp(const IntervalMatrix& M,
          double tim, bool with_slices, bool with_time,
          IntervalMatrix& expM,
	  IntervalMatrix& invExpM,
          IntervalMatrix& tauExpM,
          IntervalMatrix& IexpM,
          IntervalMatrix& tauIexpM,
          IntervalMatrix& VexpM,
          IntervalMatrix& tauVexpM,
          Matrix& intAbsEM,
	  int nbiterations, int nbscaling)
{
     double a =1.0;
     int l=0;
     double objnorm = infinite_norm(tim*M)/mnorm;
     int dim = M.nb_rows();
     Matrix Id = Matrix::eye (dim);
     IntervalMatrix MId (Id);
     Interval Unt(0.0,1.0);

     if (nbscaling==-1) {
       while (a*objnorm>1) {
         a = a/2.0;
         l = l+1;
     //  if (l==2) break;
       }
     } else {
       for (int j=0;j<nbscaling;j++) {
         a = a/2.0;
         l = l+1;
       }
     }
//     std::cout << "squaring : " << l << "   iterations : " << (l/2+nbitbase) << "\n";
     IntervalMatrix MS = a*tim*M;
     global_exp_base(MS,(nbiterations==-1?l/2+nbitbase:nbiterations),
	with_slices,with_time,
	expM,invExpM,tauExpM,IexpM,tauIexpM, VexpM, tauVexpM,l);
     // global_exp_base gives tauIexpM/tau...

/* for tauIexpM, do we compute { 1/alpha \phi*_1(alpha M) } or
   { phi*_1(alpha M) }. Let's consider the former, as the latter is
   disappointing */
/*     if (with_slices)
        tauIexpM *= Unt; */ /*  decomment if the latter */


     intAbsEM = Matrix::zeros(dim,dim);
     IntervalMatrix prodIntAbs = MId+IexpM;
     add_int_absL(dim,intAbsEM, MId, tauExpM,prodIntAbs,a*tim);
//     std::cout << "tim : " << a*tim << "tauExpM : " << tauExpM << "IntAbs : " << intAbsEM << "\n";
     
     // squaring process 
     for (int i=l;i>0;i=i-1) {
         // integral of absolute value
         IntervalMatrix tmpM1 = MId + expM;
         IntervalMatrix tmpM2 = prod_com(tmpM1,tauExpM);
         prodIntAbs = MId+IexpM;
         prod_com_with(prodIntAbs,tmpM1);
         add_int_absL(dim, intAbsEM, tmpM1 , tmpM2, prodIntAbs, a*tim);
//     std::cout << "tim : " << a*tim << "tmpM1 : " << tmpM1 << "tmpM2 : " << tmpM2 << "IntAbs : " << intAbsEM << "\n";
         // tauExpM
         IntervalMatrix tauEM2 = 2.0*expM+Unt*(-expM+tmpM2);
         tauExpM = 2.0*tauExpM;
//         std::cout << "join tauExpM" << tauExpM << "\n" << tauEM2 << "\nDone\n";
         join_mat(dim,tauExpM, tauEM2);
         if (with_time) {
            IntervalMatrix tmpM4 = 2*VexpM - IexpM;
            IntervalMatrix tmpM3 = 12.0*VexpM-6.0*(pow(2,i)-1)*IexpM;
            if (with_slices) {
//              double K1 = 1.0/(1.0-3.0*pow(2,i-1));
//              double K2 = 0.25/(2.0-3.0*pow(2,i-1));
              IntervalMatrix tmpM5 = prod_com(tauVexpM, tmpM1);
              IntervalMatrix sqTerm = 6.0*expM;
              IntervalMatrix lnTerm = 12.0*IexpM-6.0*pow(2,i)*expM;
              IntervalMatrix cstTerm = squaring_tauVexpM(tmpM5,sqTerm,lnTerm,tmpM3,i);
			
#if 0
		std::cout << "tauVexpA (ancien) : " << tauVexpM[0][0]<< "\n";
		std::cout << "phi*0 : " << expM[0][0] << "\n";
		std::cout << "phi*1 : " << IexpM[0][0] << "\n";
		std::cout << "u2 : " << VexpM[0][0] << "\n";
              IntervalMatrix squareTerm = (2.0 *Unt-3*pow(2,i))*prod_com(tauVexpM,tmpM1)+6.0*expM;
              squareTerm *= K2*Unt;
		std::cout << "squareTerm : " << squareTerm[0][0] << "\n";
              IntervalMatrix linTerm = 12.0*IexpM-6.0*pow(2,i)*expM;
		std::cout << "linTerm : " << linTerm[0][0] << "\n";
              squareTerm += K2*linTerm;
              IntervalMatrix cstTerm = Interval(K2)*tmpM3;
		std::cout << "cstTerm : " << cstTerm[0][0] << "\n";
              double coeftmp = 12.0*(1.0-pow(2,i-1))*K2*K2;
              IntervalMatrix invlinTerm = Interval(K1-K2,coeftmp)*tmpM3/* +Interval(K1-2*K2,-K2)*linTerm */;
              IntervalMatrix quadlinTerm = Interval(K1-K2,coeftmp)*linTerm;
		std::cout << "invlinTerm : " << invlinTerm[0][0] << "\n";
              IntervalMatrix pt025 = 0.25*squareTerm+0.75*invlinTerm+0.25*quadlinTerm;
              IntervalMatrix pt075 = 0.75*squareTerm+0.25*invlinTerm+0.25*quadlinTerm;
              join_mat(M.nb_cols(),invlinTerm,squareTerm);
              join_mat(M.nb_cols(),invlinTerm,pt025);
              join_mat(M.nb_cols(),invlinTerm,pt075);
              cstTerm += invlinTerm;
/*
              tmpM5 += 1.0/(1.0-3*pow(2,i-1)) * Unt* (12.0*IexpM-6.0*pow(2,i)*expM);
              tmpM5 += (0.25/(2.0-3*pow(2,i-1)) - 1.0/(1.0-3*pow(2,i-1)))*tmpM3;
              tmpM5 = Unt*tmpM5+1.0/(1.0-3*pow(2,i-1))*tmpM3;
*/
#endif
              tauVexpM *= 2.0;
//		std::cout << "tauVexpA (0,1/2) : " << tauVexpM << "\n";
//		std::cout << "tauVexpA (1/2,1) : " << cstTerm << "\n";
              join_mat(dim,tauVexpM,cstTerm);
//		std::cout << "nouveau tauVexpA" << tauVexpM << "\n";
            }
            VexpM = 0.125*expM*(tmpM4-MId) + 0.5*VexpM;
         }
         if (with_slices) {
            IntervalMatrix tmpM5 = 
		2*IexpM + 0.5*Unt*(Unt*(prod_com(tmpM1,tauIexpM)-IexpM)
                                  +(expM-2*IexpM));
            tauIexpM *= 2.0;
            join_mat(dim,tauIexpM, tmpM5); 

/*            IntervalMatrix tmpM5 = 
		IexpM + 0.5*Unt*(expM-IexpM+prod_com(tmpM1,tauIexpM));
            join_mat(dim,tauIexpM, tmpM5); 
*/
         }
         // IexpM
         IexpM = quad_prod(IexpM,expM,1.0,0.5,0.5,true);
         // expM
         expM = quad_M(expM,2.0);
         invExpM = quad_M(invExpM, 2.0);
         a = a*2.0;
     }
     // end operations
     // expM = e^M
     expM += Id;
     // invExpM = e^(-M)
     invExpM += Id;
     // IexpM = int_0^tim exp tauM dtau = tim * (Id + IexpM)
     IexpM += Id;
     IexpM *= tim;
     // VexpM = int_0^tim (tau - tim/2) e^(tim-tau) dtau = tim^2 * VexpM
     if (with_time) {
         VexpM *= (tim*tim);
     }
     // tauExpM (returns  - Id / alpha )
     // tauExpM = Id + Unt*tauExpM;
     // tauIexpM  = tau * delta * (Id+tauIexpM);
     if (with_slices) { // all returns are / alpha 
        tauIexpM = tim*(Id+ Unt * tauIexpM); 
        /* or tauIexpM = tim*(Id + tauIexpM); */
        if (with_time) {
	   vtauexpbase(tauVexpM);
           tauVexpM *= (tim*tim);
        }
     }
}

}
