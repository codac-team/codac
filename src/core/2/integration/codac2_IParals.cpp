/** 
 *  \file representation of polyhedron as inter M_i X_i
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Damien Massé
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */


#include <iostream>
#include <vector>
#include <memory>
#include <utility>
#include <cstdio>
#include <cstring>
#include "codac_Interval.h"
#include "codac_IntervalVector.h"
#include "codac_IntervalMatrix.h"
#include "codac2_expIMat.h"
#include "codac2_IParals.h"
#include "codac_polygon_arithmetic.h"

namespace codac2 {


  IParals::IParals(int dim) :
      dim(dim), empty(false), nbmat(0), mats(), Vrhs(1)
   {
       this->bbox() = IntervalVector(dim);
   }

  IParals::IParals(const IntervalVector& iv) :
      dim(iv.size()), empty(iv.is_empty()), nbmat(0), mats(), Vrhs(1)
   {
       this->bbox() = iv;
   }
   IParals::IParals(const IParals& iv, const IntervalVector& box) :
       dim(iv.dim), empty(box.is_empty()),
       nbmat(iv.nbmat), mats(iv.mats), 
       Vrhs(iv.Vrhs) { 
       assert(iv.dim == box.size());
       if (box.is_empty()) {
           this->set_empty();
           return;
       }
       for (unsigned int i =0;i<this->nbmat;i++) {
          this->rhs(i) = this->Imat(i) * box;
       }
       this->bbox() = box;
   }
   IParals::IParals(const IParals& iv) :
       dim(iv.dim), empty(iv.empty),
       nbmat(iv.nbmat), mats(iv.mats), Vrhs(iv.Vrhs) { }

   IParals::IParals(const IntervalMatrix& M, const IntervalMatrix& rM,
			const IntervalVector& V) :
       dim(V.size()), empty(V.is_empty()),
       nbmat(0), mats(), Vrhs(1) { 
       this->new_base(M,rM,V);
       if (V.is_empty()) {
           this->set_empty();
           return;
       }
       this->bbox() = M*V;
   }
   IParals::IParals(const IntervalMatrix& M, const IntervalVector& V) :
       dim(V.size()), empty(V.is_empty()),
       nbmat(0), mats(), Vrhs(1) { 
       IntervalMatrix rM = inv_IntervalMatrix(M);
       assert(!rM.is_empty()); /* FIXME : approach for singular M */
       this->new_base(M,rM,V);
       if (V.is_empty()) {
           this->set_empty();
           return;
       }
       this->bbox() = M*V;
   }


   /****** Access ******/
   const IntervalVector& IParals::box() const {
       return this->bbox();
   }
   const bool IParals::is_unbounded() const {
       return this->bbox().is_unbounded();
   }
   /* ``component'' : interval of the bounding box 
     * (only const !!!) */
   const Interval& IParals::operator[](unsigned int i) const {
       assert (this->dim>(int) i);
       return this->bbox()[i];
   }
   Vector IParals::mid() const {
       assert (dim>0);
       return this->bbox().mid(); /* FIXME : better ? */
   }

   bool IParals::contains(const Vector& iv) const {
       assert (dim>0);
       if (this->empty) return false;
       if (!this->bbox().contains(iv)) return false;
       for (unsigned int i=0;i<this->nbmat;i++) {
          if (!this->rhs(i).intersects(this->Imat(i)*iv)) return false;
       }
       return true;
   }
   const IntervalMatrix& IParals::getMat(unsigned int i) const {
       assert (i>=0 && i<nbmat);
       return this->mat(i);
   }
   const IntervalVector& IParals::getVec(unsigned int i) const {
       assert (i>=0 && i<nbmat);
       return this->rhs(i);
   }
   IntervalVector IParals::getPar(const IntervalMatrix& M) const {
       assert (dim>0);
       IntervalVector Res = M*this->bbox();
       for (unsigned int i=0;i<this->nbmat;i++) {
           Res &= (M*this->mat(i))*this->rhs(i);
       }
       return Res;
   }
   
   double IParals::rel_distanceFast(const IParals& iv) const {
        assert(this->nbmat==iv.nbmat);
        double a= this->bbox().rel_distance(iv.bbox());
	for (unsigned int i=0;i<this->nbmat;i++) {
           assert(this->mats[i]==iv.mats[i]);
           double b= this->rhs(i).rel_distance(iv.rhs(i));
           if (a<b) a=b;
        }
	return a;
   }

   /*** modification ***/

   void IParals::clear() {
        assert (dim>0);
        this->empty=false;
        for (unsigned int i=0;i<=this->nbmat;i++) (this->Vrhs[i]).clear();
   }
   /* setComponent ... */
   void IParals::setComponent(int i, const Interval& a) {
      if (this->empty) return;
      assert(i<dim);
      this->bbox()[i]=a;
      for (unsigned int j=0;j<this->nbmat;j++) {
          /* we cannot know if the goal is bigger or smaller... so
             to be safe, we'll say bigger... */
          /* FIXME:  */
          this->rhs(j) = this->Imat(j)*this->bbox();
      }
   }
   IParals& IParals::inflate(double rad) {
        if (this->empty) return *this;
        assert(dim>0);
        IntervalVector V(dim,rad);
        return (*this += V);
   }
   IParals& IParals::inflateBall(double rad) {
        if (this->empty) return *this;
        assert(dim>0);
        for (unsigned int i=0;i<this->nbmat;i++) {
           for (int j=0;j<dim;j++) {
             double a = (rad/this->Imat(i)[j].norm2()).ub();
	     this->rhs(i)[j].inflate(a);
           }
        }
        this->bbox().inflate(rad);
        return *this;
   }
   void IParals::inflate_from_baseFast(const IParals& iv, double fact) {
        assert(dim>0);
        assert(this->nbmat==iv.nbmat);
        for (unsigned int i=0;i<=this->nbmat;i++) {
            if (i<this->nbmat) assert(this->mats[i]==iv.mats[i]);
            IntervalVector& a = this->rhs(i);
            const IntervalVector& b = iv.rhs(i);
            for (int j=0;j<dim;j++) {
                double mn = fact*(b[j].lb() - a[j].lb());
                if (mn>0) mn=0.0;
                double mx = fact*(b[j].ub() - a[j].ub());
                if (mx<0) mx=0.0;
                Interval ev(mn,mx);
                a[j] += ev;
            }
        }
        
    }

   IParals& IParals::homothety(IntervalVector c, double delta) {
        if (this->empty) return *this;
        assert(dim>0);
        IntervalVector nc = (1-delta)*c;
        for (unsigned int i=0;i<=this->nbmat;i++) {
	     this->Vrhs[i] *= delta;
        }
        return (*this += nc);
   }
   IParals& IParals::operator=(const IntervalVector& x) {
        if (dim==0) {
	   this->dim = x.size();
           this->empty = x.is_empty();
           this->bbox() = x;
           return *this;
        }
        assert (dim=x.size());
        if (x.is_empty()) { this->set_empty(); return *this; }
        this->empty=false;
        this->bbox() = x;
        for (unsigned int i=0;i<this->nbmat;i++) {
	     this->rhs(i) = this->Imat(i)*x;
        }
        return *this;
   }
   IParals& IParals::assign(const IParals& iv) {
        assert(this->dim>0 && this->dim == iv.size());
        if (iv.empty) { this->set_empty(); return *this; }
        this->empty=false;
        this->bbox() = iv.bbox();
        for (unsigned int i=0;i<this->nbmat;i++) {
	     this->rhs(i) = this->Imat(i)*iv.bbox();
        }
        for (unsigned int i=0;i<this->nbmat;i++) {
            for (unsigned int j=0;i<iv.nbmat;i++) {
	       this->rhs(i) &= (this->Imat(i)*iv.mat(j))*iv.rhs(j);
            }
        }
        this->simplify();
        return *this;
   }

   /* modify matrix 1 and 2 */
   /* add matrix 1 if does not exist */
   IParals& IParals::linMult(const IntervalMatrix& M,
			const IntervalMatrix& IM) {
        assert(dim>0);
        if (this->is_empty()) return *this;
        if (this->nbmat==0) { /* basic case, add one matrix */
          this->new_base(M,IM,this->bbox());
          this->bbox() = M*this->bbox();
          return *this;
        }
        /* not an optimal algorithm, we may need the 
           piecewise-affine interval functions to get a better
           (but possibly non-optimal application */
       { 
           IntervalVector newRhs(M*this->bbox());
           for (unsigned int j=0;j<this->nbmat;j++) {
               IntervalMatrix R = M*this->mat(j);
               const IntervalMatrix& denom = this->mat(j); 
               for (int k=0;k<dim;k++) 
               for (int l=0;l<dim;l++) {
                 if (denom[k][l].contains(0.0)) 
                        { R[k][l]=1.0; continue; }
                 R[k][l]/=denom[k][l];
               } 
               R = R.mid();
               /* foreach column V of R */
               for (int k=0;k<dim;k++) {
                 /* compute X = V*rhs[nbmat] */
                 IntervalVector X(dim);
                 for (int l=0;l<dim;l++) X[l] = R[l][k]*this->bbox()[l];
                 /* compute K = M-V*Id (line by line) */
                 IntervalMatrix K(M);
                 for (int l=0;l<dim;l++) K[l][l]-=R[l][k];
                 X += (K*denom)*this->rhs(j);
                 newRhs &= X;
               }
            }
            this->bbox()=newRhs;
       }
       for (unsigned int i=0;i<this->nbmat;i++) {
           this->mult_baseLeft(i,M,IM);   
       }
       this->simplify(); // absolutely needed
       return *this;
   }


   /**** intersections ******/
   
   IParals& IParals::operator&=(const IntervalVector& x) {
        assert(dim>0 && x.size()==dim);
        if (this->empty) return *this;
        this->bbox() &= x;
        if (this->bbox().is_empty()) { 
           this->set_empty(); return *this; 
        }
        for (unsigned int i=0;i<this->nbmat;i++) {
            this->rhs(i) &= this->Imat(i)*x;
            if (this->rhs(i).is_empty()) { 
              this->set_empty(); return *this; 
            }
        }
        this->simplify();
        return *this;
   }
   IParals operator&(const IParals& iv, const IntervalVector& x) {
        assert(iv.size()==x.size());
        IParals res(iv);
        res &= x;
        return res;
   }
   IParals& IParals::meetFast(const IParals& iv) {
        assert(dim==iv.size());
        assert(this->nbmat==iv.nbmat);
        if (this->empty) return *this;
        if (iv.empty) { this->set_empty(); return *this; }
        this->bbox() &= iv.bbox();
        if (this->bbox().is_empty()) { 
           this->set_empty(); return *this; 
        }
        for (unsigned int i=0;i<this->nbmat;i++) {
            assert(this->mats[i]==iv.mats[i]);
            this->rhs(i) &= iv.rhs(i);
            if (this->rhs(i).is_empty()) { 
              this->set_empty(); return *this; 
            }
        }
        this->simplify();
        return (*this);
   }
   IParals& IParals::meetKeep(const IParals& iv) {
        assert(dim==iv.size());
        if (this->empty) return *this;
        if (iv.empty) { this->set_empty(); return *this; }
        if (iv.nbmat==0) { (*this) &= iv.bbox(); return *this; }
        this->bbox() &= iv.bbox();
        if (this->bbox().is_empty()) { this->set_empty(); return *this; }
        for (unsigned int i=0;i<this->nbmat;i++) {
          this->rhs(i) &= this->Imat(i) * iv.bbox();
          for (unsigned int j=0;j<iv.nbmat;j++) {
             if (this->mats[i]==iv.mats[j]) 
	        this->rhs(i) &= iv.rhs(j);
	     else
	        this->rhs(i) &= (this->Imat(i)*iv.mat(j)) * iv.rhs(j);
          }
          if (this->rhs(i).is_empty()) { this->set_empty(); return *this; }
       }
       this->simplify();
       return *this;
   }
   IParals& IParals::meet(const IParals& iv, bool ctcG) {
        assert(dim==iv.size());
        if (this->empty) return *this;
        if (iv.empty) { this->set_empty(); return *this; }
        if (iv.nbmat==0) { (*this) &= iv.bbox(); return *this; }
        this->bbox() &= iv.bbox();
        if (this->bbox().is_empty()) { this->set_empty(); return *this; }
        if (this->nbmat==0) {
           for (unsigned int i=0;i<iv.nbmat;i++) {
               this->borrow_base(iv,i,iv.rhs(i));
	   }
	   this->simplify();
           return (*this);
        }
        unsigned int nbcommun=0;
        for (unsigned int i=0;i<this->nbmat;i++) {
          this->rhs(i) &= this->Imat(i) * iv.bbox();
          for (unsigned int j=0;j<iv.nbmat;j++) {
             if (this->mats[i]==iv.mats[j]) {
	        this->rhs(i) &= iv.rhs(j);
                nbcommun++;
             }
	     else
	        this->rhs(i) &= (this->Imat(i)*iv.mat(j)) * iv.rhs(j);
          }
          if (this->rhs(i).is_empty()) { this->set_empty(); return *this; }
       }
        int b=iv.nbmat-1;
        if (iv.nbmat==nbcommun || 
		(this->nbmat==2 && (ctcG || this->mats[1]==iv.mats[b]))) {
	     this->simplify();
             return (*this);
        }
        if (this->nbmat==1 && this->mats[0]==iv.mats[b]) b=0; /* iv.nbmat=2 ! */
	IntervalVector newV = iv.rhs(b);
	newV &= iv.Imat(b)*this->bbox();
        for (unsigned int i=0;i<this->nbmat;i++) {
	   newV &= (iv.Imat(b)*this->mat(i)) * this->rhs(i);
        }
        this->borrow_base(iv,b,newV);
        if (newV.is_empty()) 
        this->simplify(); 
        return *this;
   }

   IParals& IParals::operator&=(const IParals& iv) {
        return this->meet(iv, true);
   }
   IParals operator&(const IParals& iv1, const IParals& iv2) {
        assert(iv1.size()==iv2.size());
        IParals res(iv1);
        res.meet(iv2,false);
        return res;
   }

   bool IParals::meetLN (IntervalVector& V, IntervalVector& C,
	  Interval& b, bool ctc) {
        /* WARNING : use of bwd_mul */
        if (this->empty) return false;
        IntervalVector Rcopy(this->bbox()-C);
        if (!bwd_mul(b,V,Rcopy)) {
            this->set_empty(); return false;
        }
        bwd_sub(Rcopy,this->bbox(),C);
    	for (unsigned int i=0;i<this->nbmat;i++) {
            IntervalVector VM = V*this->mat(i);
            IntervalVector ImC = this->rhs(i) - this->Imat(i)*C;
	    if (!bwd_mul(b,VM,ImC)) {
		this->set_empty(); return false;
	    }
            /* pas de bwd_mul sur VM, parce que this->mat(i) ne doit pas être
               modifié */
            this->rhs(i) &= ImC + this->Imat(i)*C;
        }
        if (ctc && this->nbmat==2) {
	   this->simplify();
           return !(this->empty);
        }
        IntervalVector Vmid = V.mid();
        int numM=-1; int nlig=-1;
        if (this->nbmat==2) {
           this->replaceVectorImat (Vmid, numM, nlig);
        } else {
           this->createVectorImat(Vmid, numM, nlig);
        }
	/* V.(x-C) \in b 
	   => Vmid.(x-C) + Veps.(x-C) \in b + V.C
           => Vmid.x \in b - Veps.(x-C) + Vmid.C
           => Vmid.x \in (b - Veps (bbox-C) + VC) */
        IntervalVector Veps(V-Vmid);
        this->rhs(numM)[nlig] &= b - (Veps*(this->bbox()+C)) + Vmid*C;
        if (this->rhs(numM)[nlig].is_empty()) {
	   this->set_empty(); return false;
        }
        this->simplify();
        return !(this->empty);
   }

	
   bool IParals::meetLN(IntervalVector& V,
			Interval& b, bool ctc) {
        IntervalVector Z(dim,Interval::zero());
        return this->meetLN(V,Z,b,ctc);
   }

   /* FIXME : maybe we can do better... though I'm not sure */
   bool IParals::meetLM(IntervalMatrix& S, IntervalVector &C,
			IntervalVector& b, bool ctc) {
        assert(S.nb_rows()==b.size());
        /* WARNING : use of bwd_mul */
        if (this->empty) return false;
        IntervalVector Rcopy(this->bbox()-C);
        if (!bwd_mul(b,S,Rcopy,1e-3)) {
            this->set_empty(); return false;
        }
        bwd_sub(Rcopy,this->bbox(),C);
    	for (unsigned int i=0;i<this->nbmat;i++) {
            IntervalMatrix SM = S*this->mat(i);
            IntervalVector ImC = this->rhs(i) - this->Imat(i)*C;
	    if (!bwd_mul(b,SM,ImC,1e-3)) {
		this->set_empty(); return false;
	    }
            /* pas de bwd_mul sur SM, parce que this->mat(i) ne doit pas être
               modifié */
            this->rhs(i) &= ImC + this->Imat(i)*C;
        }
        if (ctc && this->nbmat==2) {
	   this->simplify();
           return !(this->empty);
        }
        /* not keep : we apply replaceVectorImat sequentially
	   I'm not sure that's a good idea nevertheless... */
        IntervalMatrix Smid = S.mid();
        IntervalMatrix SCopy = S-Smid;
        std::vector<int> nlig;
        int numM=-1;
        if (this->nbmat==2) {
           this->replaceMatrixImat(Smid, numM, nlig);
        } else {
           this->createMatrixImat(Smid, numM, nlig);
        }

        for (int i=0;i<dim;i=i+1) {
           int nl = nlig[i];
           if (nl<0) continue;

           this->rhs(numM)[i] &= b[nl] - 
			(SCopy[nl]*(this->bbox()+C)) + Smid[nl]*C;
           if (this->rhs(numM)[i].is_empty()) {
	      this->set_empty(); return false;
           }
        }
	this->simplify();
        if (this->empty) return false;
        return true;
   }

   bool IParals::meetLM(IntervalMatrix& S,
			IntervalVector& b, bool keep) {
          IntervalVector Z(dim,Interval::zero());
	  return this->meetLM(S,Z,b,keep);
   }
   bool IParals::meetLM(IntervalMatrix& S, const Vector& C,
			IntervalVector& b, bool keep) {
          IntervalVector NC(C);
	  return this->meetLM(S,NC,b,keep);
   }

   
   /** union with a box 
    */
   IParals& IParals::operator|= (const IntervalVector& x) {
       std::cout << "|=" << *this << " " << x << "\n";
       assert(dim>0);
       if (x.is_empty()) return *this;
       if (this->empty) {
           return (*this = x);
       }
       for (unsigned int i=1;i<this->nbmat;i++) {
	  this->rhs(i) |= this->Imat(i)*x;
       }
       this->bbox() |= x;
       return *this;
   }
   IParals& IParals::operator|= (const IParals& x) {
       if (dim==0) {
           return (*this = x);
       }
       assert(x.size()==dim);
       if (x.is_empty()) return *this;
       if (this->empty) {
           return (*this = x);
       }
       for (unsigned int i=1;i<this->nbmat;i++) {
          this->rhs(i) |= x.getPar(this->Imat(i));
       }
       this->simplify(true);
       return *this;
   }
   IParals operator|(const IParals& iv, const IntervalVector& x) {
       assert(iv.size()==x.size());
       IParals Res(iv);
       Res |= x;
       return Res;
   }


   IParals& IParals::operator+=(const IntervalVector& V) {
        assert(dim>0);
        if (this->empty) return *this;
        for (unsigned int i=0;i<this->nbmat;i++) {
            this->rhs(i) += this->Imat(i)*V;
        }
        this->bbox() += V;
        return *this; 
	/* simplification not needed : max C(x+v) = max Cx + max Cv */
   }
   IParals& IParals::operator-=(const IntervalVector& V) {
        assert(dim>0);
        if (this->empty) return *this;
        for (unsigned int i=0;i<this->nbmat;i++) {
            this->rhs(i) -= this->Imat(i)*V;
        }
        this->bbox() -= V;
        return *this; 
	/* simplification not needed : max C(x+v) = max Cx + max Cv */
   }
   IParals operator+(const IParals& iv, const IntervalVector& V) {
        assert(iv.size()==V.size());
        IParals res(iv);
        res += V;
        return res;
   }
   IParals operator-(const IParals& iv, const IntervalVector& V) {
        assert(iv.size()==V.size());
        IParals res(iv);
        res -= V;
        return res;
   }

      /** product : compared with linMult :
           M is (generally) small and contains singular matrices 
           we just want an IntervalVector */
   IntervalVector operator*(const IntervalMatrix& M, const IParals& iv) {
       IntervalVector res = M*iv.bbox();
       for (unsigned int i=0;i<iv.nbmat;i++) {
           IntervalMatrix MP = M*iv.mat(i);
           res &= MP*iv.rhs(i);
       }
       return res;
   }

   IParals sum_tau(const IParals& iv, const IntervalVector& V, bool keep) {
       IParals res(iv);
       Interval Tau(0.0,1.0);
       for (unsigned int i=0;i<res.nbmat;i++) {
	  res.rhs(i) += Tau*(res.Imat(i)*V);
			/* better than res.Imats[i]*(Tau*V) */
       }  
       res.bbox() += Tau*V;
       res.simplify(true);
       return res;
   }

   void IParals::cmult_and_add (const Vector& center,
			const IntervalMatrix& M, 
			const IntervalMatrix& IM,
			const IntervalVector& V)
   {
       if (this->empty) return;
       this->linMult(M,IM);
       (*this) += (-M*center + center +V);
   }

   /* quick algorithm :
      1) we want to keep the matrices
      2) more precise would require (much) more work
      FIXME : can we adapt a bit nevertheless, from the algorithm of 
              linMult ? */
   void IParals::ctau_mult_and_add
		(const Vector& center,
			const IntervalMatrix& M, 
			const IntervalVector& V)
   {
       if (this->empty) return;
       Interval Tau(0.0,1.0);
       for (unsigned int i=0;i<this->nbmat;i++) {
         this->rhs(i) += Tau*((this->Imat(i)*M*this->mat(i))*this->rhs(i)
			-(this->Imat(i)*M)*center
			+this->Imat(i)*V);
       }
       this->bbox() += Tau*(M*(this->bbox()-center)+V);
       this->simplify(true);
   }
   
   /* FIXME : check and simplify this algorithm */
   bool IParals::join_intersect_with_tau(const IParals& iv,
			const Vector& center, const IntervalMatrix& M,
			const IntervalVector& V,
			const IntervalVector& box, int d, double val) {
        if (iv.empty) return false;
        Interval Tau(0.0,1.0);
        bool ret = false;
        IntervalVector cbox(box);
        cbox[d]=val;
        IntervalVector bbox = iv.bbox();
        Interval v1(val-bbox[d]);
        Interval v2((M*(bbox-center)+V)[d]);
        if (!bwd_mul(v1,Tau,v2)) return false;
        if (Tau.ub()<=0.0) return false;
        v1 &= Tau*v2;
        bbox[d] = val-v1;
        
        IParals ivInter(*this, bbox);
        IntervalMatrix mId(dim,dim);
        mId = (Matrix::eye(dim) + Tau*M);
        ivInter.bbox() &= mId*(iv.bbox())
			+ Tau * (V-M*center);
        if (ivInter.bbox().is_empty()) return false;
        for (unsigned int j=0;j<iv.nbmat;j++) {
            IntervalVector tmpVect= iv.mat(j)*iv.rhs(j);
            ivInter.bbox() &= mId*tmpVect
			+ Tau * (V-M*center);
            if (ivInter.bbox().is_empty()) return false;
            ivInter.rhs(j) &= (ivInter.Imat(j)*mId*ivInter.mat(j))*iv.rhs(j)
			+ ivInter.Imat(j)*(Tau*(V-M*center));
            if (ivInter.rhs(j).is_empty()) return false;
        }
/*
        for (int i=0;i<this->nbmat;i++) {
            IntervalVector Vi(ivInter.Imats[i]*V);
            for (int j=0;j<iv.nbmat;j++) {
                IntervalMatrix tmpM(ivInter.Imats[i] * M);
                ivInter.rhs[i] &= (ivInter.Imats[i] * iv.mats[j]) * iv.rhs[j]
		   + Tau * ((tmpM * iv.mats[j]) * iv.rhs[j] + 
			tmpM*(iv.center - center)+Vi)
			+ ivInter.Imats[i] * (iv.center - ivInter.center);
                if (ivInter.rhs[i].is_empty()) return false;
            }
        }
*/
        ivInter.simplify(true);
        if (ivInter.is_empty()) return false;
        if (this->is_empty()) {
            this->bbox() = iv.bbox();
            for (unsigned int i=0;i<this->nbmat;i++) this->rhs(i) = iv.rhs(i);
	    return true;
        }
        if (!ivInter.bbox().is_subset(this->bbox())) {
           this->bbox() |= ivInter.bbox();
           ret = true;
        }
        for (unsigned int i=0;i<this->nbmat;i++) {
            if (!ivInter.rhs(i).is_subset(this->rhs(i))) {
		this->rhs(i) |= ivInter.rhs(i);
                ret = true;
            }
        }
        if (ret) { this->simplify(true); return true; }
        return false;
   }
 

   IParals& IParals::sumFast(const IParals& iv) {
        if (this->empty) return *this;
        if (iv.empty) { this->set_empty(); return *this; }
        assert(this->nbmat==iv.nbmat);
        for (unsigned int i=0;i<this->nbmat;i++) {
            assert(this->mats[i]==iv.mats[i]);
            this->rhs(i) += iv.rhs(i);
        }
        this->bbox() += iv.bbox();
        return *this; 
   }
   IParals& IParals::diffFast(const IParals& iv) {
        if (this->empty) return *this;
        if (iv.empty) { this->set_empty(); return *this; }
        assert(this->nbmat==iv.nbmat);
        for (unsigned int i=0;i<this->nbmat;i++) {
            assert(this->mats[i]==iv.mats[i]);
            this->rhs(i) -= iv.rhs(i);
        }
        this->bbox() -= iv.bbox();
        return *this; 
   }

   
   bool IParals::is_subset(const IntervalVector& V) const {
       if (this->empty) return true;
       return this->bbox().is_subset(V);
   }
   bool IParals::is_subsetFast(const IParals& iv) const {
       if (this->empty) return true;
       if (iv.empty) return false;
       assert(this->nbmat==iv.nbmat);
       for (unsigned int i=0;i<this->nbmat;i++) {
           assert(this->mats[i]==iv.mats[i]);
           if (!this->rhs(i).is_subset(iv.rhs(i))) return false;
       }
       return this->bbox().is_subset(iv.bbox());
   }
  

   IParals& IParals::toPointMatrices() {
       for (unsigned int i=0;i<this->nbmat;i++) {
           IntervalMatrix M1 = this->mat(i).mid();
           IntervalMatrix IM1 = inv_IntervalMatrix(M1);
           IntervalVector newRhs = (IM1 * this->mat(i))*this->rhs(i);
           this->change_base(i,M1,IM1,newRhs);
       }
       this->simplify();
       return *this;
   }

   /* "orthogonalise" a vector of a matrix:
    *  modify the other vectors to make them orthogonal */
   IParals& IParals::orthogonalise
		(int numM, int ncol) {
       IntervalVector nl = this->mat(numM).col(ncol);
       Interval sqnormNL = nl.sqnorm2();
       /* constructing new inverse matrix :
	    M''^-1 = T2 M'^-1
            with T2 = (Id except nlig which is nl.ci/nl^2 (or 1) )
            and rhs :
               V' = T2 V
       */
       IntervalVector prod = nl*this->mat(numM); /* dot products */
       IntervalMatrix newImat(this->Imat(numM));
       IntervalVector newRhs(this->rhs(numM));
       for (int i=0;i<dim;i=i+1) {
          if (i==ncol) continue;
          newImat[ncol] += (prod[i]/sqnormNL)*newImat[i];
          newRhs[ncol] += (prod[i]/sqnormNL)*newRhs[i];
       }
       /* constructing new matrix (just change nth column) */
       IntervalMatrix newmat(this->mat(numM));
       for (int i=0;i<dim;i=i+1) {
          if (i==ncol) continue;
          Interval md = prod[i]/sqnormNL;
          for (int j=0;i<dim;i=i+1) {
             newmat[j][i] -= md*nl[ncol];
          }
       }
       this->change_base(numM,newmat,newImat,newRhs);
       return *this;
   }
   /* replace a generator (column) in a matrix by another one */
   /* numM= -1 : last matrix  ;   ncol=-1 : find best column */
   /* modify numM and ncol to get the matrix and column changed */
   IParals& IParals::replaceVectorMat
		(const IntervalVector& nl, int& numM, int& ncol,
		 bool ortho) {
     assert(numM>=-1 && numM<(int) this->nbmat);
     if (numM==-1) numM=this->nbmat-1;
     assert(ncol>=-1 && ncol<dim);
     bool tryPoint=false;
     IntervalVector u = this->Imat(numM) * nl;
     while (ncol==-1) {
        /* look for a replaceable column : we consider
	   M^(-1) * nl = (u1,u2,...) and select u_i s.t. 0 \notin u_i
           et mag(rhs[i]/ui) minimal
	   -> pb if all u_i has 0 */
	double valbest=0.0;
        for (int i=0;i<dim;i=i+1) {
            if (!u[i].contains(0.0)) {
               double val = (this->rhs(numM)[i]/u[i]).mag();
	       if (ncol==-1 || val<valbest) {
	  	  ncol=i; valbest=val;
	       }
            }
        }
        if (ncol==-1) {
             assert(!tryPoint); /* FIXME : other approach? */
             this->toPointMatrices();
             u = this->Imat(numM) * nl;
	     tryPoint=true;
	}
     }
     /* constructing new inverse matrix :
	M'^-1 = T M^-1
        with T = (Id except ncol which is -uk/ui (or 1/ui) )
        and rhs :
            V' = T V
      */
     IntervalMatrix newImat(this->Imat(numM));
     IntervalMatrix newmat(this->mat(numM));
     IntervalVector newRhs(this->rhs(numM));
     newImat[ncol] *= (1.0/u[ncol]);
     newRhs[ncol] /= u[ncol];
     for (int i=0;i<dim;i=i+1) {
        if (i==ncol) continue;
        newImat[i] -= u[i]*newImat[ncol];
        newRhs[i] -= u[i]*newRhs[ncol];
     }
     /* constructing new matrix (just change nth column) */
     for (int i=0;i<dim;i=i+1) {
        newmat[i][ncol] = nl[i];
     }
     this->change_base(numM,newmat,newImat,newRhs);
     if (ortho) { /* warning : quasi-point matrices recommended... */
         this->orthogonalise(numM,ncol);
     }
     return *this;
   }
   /* replace a constraint (line of inverse) in a matrix by another one */
   /* numM= -1 : last matrix  ;   nlig=-1 : find best line */
   /* modify numM and ncol to get the matrix and line changed */
   IParals& IParals::replaceVectorImat
		(const IntervalVector& nl, int& numM, int& nlig) {
     assert(numM>=-1 && numM<(int) this->nbmat);
     if (numM==-1) numM=this->nbmat-1;
     assert(nlig>=-1 && nlig<dim);
     bool tryPoint=false;
     IntervalVector u = nl * this->mat(numM);
     while (nlig==-1) {
        /* look for a replaceable column : we consider
	   nl * M= (u1,u2,...) and select u_i s.t. diam(rhs[i])*mig(ui) maximal
		(diam(rhs[i]) : we don't want to lose "strong" constraint)
	   -> pb if all u_i has 0 */
	double valbest=-1.0;
        for (int i=0;i<dim;i=i+1) {
            if (!u[i].contains(0.0)) {
               double val = this->rhs(numM)[i].diam() * u[i].mig();
	       if (val>valbest) {
	  	  nlig=i; valbest=val;
	       }
            }
        }
        if (nlig==-1) {
             assert(!tryPoint); /* FIXME : other approach? */
             this->toPointMatrices();
             u = nl * this->mat(numM);
	     tryPoint=true;
	}
     }
     /* constructing new inverse matrix :
	M'^-1 = replacement of nl in ith line
        M' = M T with T = (Id except nlig which is -uk/ui (or 1/ui)
        and rhs :
            V' = T^-1 V ( T^-1 = (Id except nlig which is u) )
      */
     IntervalMatrix newImat(this->Imat(numM));
     IntervalMatrix newmat(this->mat(numM));
     IntervalVector newRhs(this->rhs(numM));
     newImat[nlig] = nl;
     for (int i=0;i<dim;i=i+1)
          newmat[i][nlig] /= u[i]; 
     for (int i=0;i<dim;i=i+1)
     for (int j=0;j<dim;j=j+1) {
        if (j==nlig) continue;
        newmat[i][j] -= u[j]*newmat[i][nlig];
     }
     newRhs[nlig] = u*newRhs[numM];
     this->change_base(numM,newmat,newImat,newRhs);
     return *this;
   }

   /* create a new constraint domain, with one specific vector */
   /* numM :  matrix number  ;   nlig : line of the vector     */
   /* modify numM and ncol to get the matrix and line changed  */
   IParals& IParals::createVectorImat
		(const IntervalVector& nl, int& numM, int& nlig) {
       assert(this->nbmat<2);
       IntervalMatrix M = Matrix::eye(dim);
       this->new_base(M,M,this->bbox());
       numM = this->nbmat-1;
       return this->replaceVectorImat(nl,numM,nlig);
   }
       
   /* replace a constraint (line of inverse) in a matrix by another one */
   /* numM= -1 : last matrix  ;   nlig : list of lignes (changed) */
   /* modify numM and nlig to get the matrix and lines changed */
   IParals& IParals::replaceMatrixImat
		(const IntervalMatrix& nM, int& numM, std::vector<int>& nlig) {
      if (numM==-1) numM=this->nbmat-1;
      IntervalMatrix MRes(dim,dim), MInv(dim,dim);
      nlig = construct_free_set(nM,MRes,MInv,&(this->Imat(numM)));
      /* reconstruction of the rhs */
      IntervalVector newRhs(dim);
      assert((int) nlig.size()==dim);
      for (int i=0;i<dim;i++) {
         if (nlig[i]>=0) {
            newRhs[i] = MRes[i] * this->bbox();
            newRhs[i] &= (MRes[i]*this->Imat(numM)) * this->rhs(numM);
         } else {
            newRhs[i] = this->rhs(numM)[-nlig[i]-1];
         }
      }
      this->change_base(numM,MInv.transpose(),MRes,newRhs);
      return *this;
   }   

   /* create a constraint (line of inverse) in a matrix by another one */
   /* numM= -1 : last matrix  ;   nlig : list of lignes (changed) */
   /* modify numM and nlig to get the matrix and lines changed */
   IParals& IParals::createMatrixImat
		(const IntervalMatrix& nM, int& numM, std::vector<int>& nlig) {
       assert(this->nbmat<2);
       IntervalMatrix M = Matrix::eye(dim);
       this->new_base(M,M,this->bbox());
       numM = this->nbmat-1;
       return this->replaceMatrixImat(nM,numM,nlig);
   }


   void IParals::simplify(double ratio, int nbit) {
     if (this->empty) return;
     if (this->nbmat==0) return;
     /* FIXME : use ratio, rel_distance and a stack ??? */
     for (int i=0;i<=nbit;i++) {
       this->rhs(0) &= this->Imat(0) * this->bbox();
       this->bbox() &= this->mat(0) * this->rhs(0);
       IntervalMatrix cpy(this->Imat(0)); /* FIXME : should we
                                             use non-const references ??? */
       if (!bwd_mul(this->rhs(0),cpy,
                    this->bbox(),ratio)) {
            this->set_empty(); return; }
       cpy = this->mat(0);
       if (!bwd_mul(this->bbox(),cpy,
                    this->rhs(0),ratio)) {
            this->set_empty(); return; }
       for (unsigned int j=0;j<nbmat-1;j++) {
         IntervalMatrix Jp1J = this->Imat(j+1) * this->mat(j);
         IntervalMatrix JJp1 = this->Imat(j) * this->mat(j+1);
         this->rhs(j+1) &= Jp1J * this->rhs(j);
         this->rhs(j) &= JJp1 * this->rhs(j+1);
         if (this->rhs(j).is_empty() || this->rhs(j+1).is_empty())
		  { this->set_empty(); return; }
         if (!bwd_mul(this->rhs(j+1),Jp1J,this->rhs(j),ratio)) {
              this->set_empty(); return; }
         if (!bwd_mul(this->rhs(j),JJp1,this->rhs(j+1),ratio)) {
              this->set_empty(); return; }
       }
       this->bbox() &= this->mat(nbmat-1) * this->rhs(nbmat-1);
       this->rhs(nbmat-1) &= this->Imat(nbmat-1) * this->bbox();
       cpy=this->mat(nbmat-1);
       if (!bwd_mul(this->bbox(),cpy,
                    this->rhs(nbmat-1),ratio)) {
            this->set_empty(); return; }
       cpy=this->Imat(nbmat-1);
       if (!bwd_mul(this->rhs(nbmat-1),cpy,
                    this->bbox(),ratio)) {
            this->set_empty(); return; }
     }
   }

   /** generate a list of (2D) points, the convex hull of which is an
    * (over)-approximation of the projection of the polyhedron
    */
   ConvexPolygon IParals::over_polygon(const Matrix& M) const {
        /* first we generate a projection of the parallelotope */
        if (this->empty) return ConvexPolygon();
        /* just the first polygon (not manage intersection) */
        Vector V1(this->dim);
        ConvexPolygon res;
        /* compute the projection for large dimension is a bit complex
           (but interesting), will do it dirty */
        for (unsigned int k=0;k<=this->nbmat;k++) {
          bool val[this->dim];
          std::vector<codac::ThickPoint> lpoints;
          for (int i=0;i<this->dim;i++) {
             val[i]=false;
             V1[i] = this->rhs(k)[i].lb(); 
          }
          while (true) {
             if (k<this->nbmat) {
	            lpoints.push_back(codac::ThickPoint(M*(this->mat(k)*V1)));
             } else {
	            lpoints.push_back(codac::ThickPoint(M*V1));
             }
             int j=dim-1;
             while (j>=0 && val[j]==true) {
                  V1[j]=this->rhs(k)[j].lb();
                  val[j]=false; 
                  j--;
             }
             if (j<0) break;
             val[j]=true;
             V1[j] = this->rhs(k)[j].ub(); 
          }
          ConvexPolygon a(lpoints);
          if (k==0) res=a; else res= res & a;
//          res = a; /* FIXME : voir avec Simon l'intersection (ne marche plus???)  */
       }
       return res; 
   }


   std::ostream& operator<<(std::ostream& str, const IParals& iv) {
       if (iv.empty || iv.nbmat==0) { return (str << iv.bbox()); }
       str << "IParals: box " << iv.bbox() << "\n";
       for (unsigned int i=0;i<iv.nbmat;i++) {
            str << " /\\ " << iv.mat(i) << "\n     X " << iv.rhs(i);
       }
       str << "\n" << std::flush;
       return str;
   }

    codac::TubeVector to_codac1(codac2::Tube<IParals>& tube)
    {
      codac::TubeVector x(tube.t0_tf(), tube.size());
      for(const auto& s : tube)
        if(!s.t0_tf().is_unbounded())
          x.set(s.codomain().box(), s.t0_tf());
      for(const auto& s : tube) // setting gate (were overwritten)
        if(s.t0_tf().is_degenerated())
          x.set(s.codomain().box(), s.t0_tf());
      return x;
    }


}

