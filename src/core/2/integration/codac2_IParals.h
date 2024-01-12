/** 
 *  \file representation of polyhedron as inter M_i X_i
 *  
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Damien Massé
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_IPARALS_H__
#define __CODAC2_IPARALS_H__

#include <iostream>
#include <vector>
#include <memory>
#include "codac_TubeVector.h"
#include "codac_Interval.h"
#include "codac_IntervalVector.h"
#include "codac_IntervalMatrix.h"
#include "codac_ConvexPolygon.h"
#include "codac2_Tube.h"
#include "codac2_expIMat.h"


namespace codac2 
{

  using codac::Interval;
  using codac::IntervalVector;
  using codac::IntervalMatrix;
  using codac::Matrix;
  using codac::Vector;
  using codac::ConvexPolygon;


  class IParals;

  IParals sum_tau(const IParals& iv, const IntervalVector& V,
				 bool keep=false);
  ConvexPolygon over_polygon(const IParals &ip, const Matrix& M);

  class IParals
  {
     public :

      /******************** CONSTRUCTORS *******************/
      /**
       * empty constructor, without dimension (needed for e.g. tube
         initialisation)
         must be resized before use (see resize())
       */
      IParals();
      /**
       * constructor from dimension (full domain, ID matrix)
       */
      IParals(int dim);
      /**
       * constructor from an initial box (id matrix)
       */
      IParals(const IntervalVector& iv);
      /**
       * constructor from an initial box and matrices from an existing IVparal
       */
      IParals(const IParals& iv, const IntervalVector& box);
      /**
       * copy
       */
      IParals(const IParals& iv);
      /**
       * from a slanted box  ( M.V, with reverse known )
       */
      IParals(const IntervalMatrix& M, const IntervalMatrix &rM, 
			const IntervalVector &V);
      /**
       * from a slanted box  ( M.V, reverse computed )
       */
      IParals(const IntervalMatrix& M, const IntervalVector &V);

      /***************** ACCESS **************************/
      /**
       * number of variables
       */
      int get_dim() const;
      int size() const;

      /**
       * number of matrices
       */
      int get_nbmat() const;
      /** is_empty
       */
      bool is_empty() const;
      /** bounding box
       */
      const IntervalVector& box() const;
      /** is unbounded
       */
      const bool is_unbounded() const;
      /** ``component'' : interval of the bounding box 
        * (only const !!!) */
      const Interval& operator[](unsigned int i) const;
      /**
       * ``middle'' (e.g. center of the bounding box ?
       */
      Vector mid() const;
      /** contains point (approximate...) true = maybe
       */
      bool contains(const Vector& iv) const;

      /** intersects (approximate) : true = maybe */
      bool intersects(const IntervalVector& x) const;
      /** intersects (approximate) : true = maybe */
      bool intersects(const IParals& x) const;

      /** get i-th matrice (i<nbmat) */
      const IntervalMatrix& getMat(unsigned int i) const;
      /** get i-th vector, not modified */
      const IntervalVector& getVec(unsigned int i) const;
      /** get inter_i (M*Mi) Vi */
      IntervalVector getPar(const IntervalMatrix& M) const;
      /**
       * relative distance, fast (with same base) 
       * max of the distance of each box
       */
       double rel_distanceFast(const IParals& iv) const;
 

      /************* Modification **********************/
      /** empty */
      void set_empty();
      /** return to 0, not modifying the matrices */
      void clear();
      /** setComponent : equivalent to 
       *  x[i] = interval... */
      void setComponent(int i, const Interval& a);
      /** inflation by a cube
       *  this <- this + [-r,r]^d
       *  keep the matrices
       *  return this
       */
      IParals& inflate(double rad);
      /** inflation by a ball
       *  this <- this + ball(rad)
       *  keep the matrices
       *  return this
       */
      IParals& inflateBall(double rad);
      /** centered homothety
       *  x <- [c] + delta*([x]-[c]) ( (1-delta)[c] + delta*[x] )
       *  keep the matrices
       *  return this
       */
      IParals& homothety(IntervalVector c, double delta);
      /** set this to x
       *  keep the matrices
       */
      IParals& operator=(const IntervalVector& x);
      /** "project" x on this, get an overapproximation
       *  keep the matrices
       */
      IParals& assign(const IParals& iv);

      /** expansion form a bigger set, **with the same base** */
      /* this |= this+(iv-this)*fact */
      void inflate_from_baseFast(const IParals& iv, double fact);


      /** apply a (non-singular) endomorphism ;
          change the matrices.
          this <- M*this ;    IM : inverse of M */
      IParals& linMult(const IntervalMatrix& M, const IntervalMatrix& IM);

      /***** Intersections *****/

      /** intersection with a box, keep the matrices */
      IParals& operator&=(const IntervalVector& x);
      /** intersection with a box, new value */
      friend IParals operator&(const IParals& iv, const IntervalVector& x);
      /** intersection with a box, new value */
      friend IParals operator&(const IParals& iv, const IntervalVector& x);
      /** intersection with IParals, equals matrices (fast) */
      IParals& meetFast(const IParals& iv);
      /** meet with complete keeping of the base (used for .set on a gate),
       *  to avoid using nearby matrices */
      IParals& meetKeep(const IParals& iv);
      /** intersection with IParals :
       *  meet : may modify the base (subset not guaranteed, 
						except if ctcG=true)
       *  &= : can just add new bases (subset guaranteed)
       */
      IParals& meet(const IParals& iv, bool ctcG=false);
      IParals& operator&=(const IParals& iv);
      friend IParals operator&(const IParals& iv1, const IParals& iv2);
      /** intersection with a interval linear constraint,
       *      M x \in b 
       *      ctc = true => contractance is guaranteed 
       *      V and b may also be contracted */
      bool meetLN(IntervalVector& V, Interval& b, bool ctc);
      /** intersection with a centered interval linear constraint,
       *      M (x-c) \in b 
       *      keep = true => contractance is guaranteed */
      bool meetLN(IntervalVector& V, IntervalVector& C,
				Interval& b, bool ctc);
      /** with a set of linear constraints */
      bool meetLM(IntervalMatrix& S, IntervalVector& b, bool ctc);
      /** with a centered set of linear constraints */
      bool meetLM(IntervalMatrix& S, IntervalVector& C,
				IntervalVector& b, bool ctc);
      /** with a centered set of linear constraints */
      bool meetLM(IntervalMatrix& S, const Vector& C,
				IntervalVector& b, bool ctc);
      
      /** union with a box 
       */
      IParals& operator|=(const IntervalVector& x);
      IParals& operator|=(const IParals& x);
      friend IParals operator|(const IParals& iv, const IntervalVector& x);

      /***** operations  *****/

      /** sum, difference */
      IParals& operator+=(const IntervalVector& V);
      IParals& operator-=(const IntervalVector& V);
      friend IParals operator+(const IParals& iv, const IntervalVector& V);
      friend IParals operator-(const IParals& iv, const IntervalVector& V);
      friend IParals sum_tau(const IParals& iv, const IntervalVector& V,
						bool keep);


      /** product : compared with linMult :
           M is (generally) small and contains singular matrices 
           we just want an IntervalVector */
      friend IntervalVector operator*(const IntervalMatrix& M, 
				const IParals& iv);

      IParals& sumFast(const IParals& iv);
      IParals& diffFast(const IParals& iv);

      /*** specific function from diffincl */
 
      /** centered multiply and add 
          this <- center + M *(this-center) + V
       */
      void cmult_and_add(const Vector&center,
		const IntervalMatrix& M, const IntervalMatrix &invM,
		const IntervalVector& V);
      /** tau-centered multiply and add (not modifying the matrices)
       * IV = IV + [0,1]*(M (IV-center) + V)
       * quick algorithm, maybe not precise (generally M is thick)
       */
      void ctau_mult_and_add(const Vector&center,
		const IntervalMatrix& M,
		const IntervalVector& V);
      /**
       * union with     box[d->val] 
				intersected with 
			iv + ]0,1]*(M (iv-center)+V) */
      bool join_intersect_with_tau
		(const IParals& iv, const Vector&center,
                const IntervalMatrix& M,
                const IntervalVector& V,
		 const IntervalVector& box, int d, double val);

      /** comparison **/
      bool is_subset(const IntervalVector& V) const;
      bool is_subsetFast(const IParals& iv) const;
      /** imprecise */
      bool is_subset(const IParals& ip) const;


      /** replace the matrices with almost-points matrices */
      IParals& toPointMatrices();
      /** "orthogonalise" a vector of a matrix : change the
        * other M-generators s.t. they become orthogonal to the
        * vector (numM : number of the matrix, ncol : column of the vector */
      IParals& orthogonalise (int numM, int ncol);
      /** replace a M-generator (column) in a matrix by a new vector
       * numM= -1 : last matrix  ;   ncol=-1 : find best column 
       * modify numM and ncol to get the matrix and column changed 
       * ortho : "orthogonalise" afterwards */
      IParals& replaceVectorMat
                (const IntervalVector& nl, int& numM, int& ncol,
                 bool ortho);
      /** replace a constraint (line of inverse) in a matrix by another one 
       * numM= -1 : last matrix  ;   nlig=-1 : find best line 
       * modify numM and ncol to get the matrix and line changed */
      IParals& replaceVectorImat
                (const IntervalVector& nl, int& numM, int& nlig);
      IParals& createVectorImat
                (const IntervalVector& nl, int& numM, int& nlig);
      IParals& replaceMatrixImat
                (const IntervalMatrix& nl, int& numM, std::vector<int>& nlig);
      IParals& createMatrixImat
                (const IntervalMatrix& nl, int& numM, std::vector<int>& nlig);


      /**
       * simplification
       */
      void simplify(double ratio = 1e-4, int nbit = 2);

      /** generate a ConvexPolygon which is an overapproximation of the
       * projection of the polyhedron (basic form)
       */
      friend ConvexPolygon over_polygon(const IParals &ip, const Matrix& M);

      /** display
       */
      friend std::ostream& operator<<(std::ostream& str, const IParals& iv);
      /** determine it displays the direct (false) or inverse (true) matrices 
          (default false)
	  returns the ancient value
       */
       static bool set_display_inv(bool newVal); 


    private:

      int dim; // number of variables
      bool empty;
      unsigned int nbmat; // number of intersections, without BBox
      std::vector<std::shared_ptr<std::pair<IntervalMatrix,IntervalMatrix>>> mats;
				 // nbmat matrix 
      std::vector<IntervalVector> Vrhs; // nbmat+1 rhs, the first one is BBox



      /** customizable fields */
      int debug_level = 3;
      static bool display_inv;

      void new_base(const IntervalMatrix &M, 
				const IntervalMatrix &rM, 
				const IntervalVector &V);
      void change_base(unsigned int numM, const IntervalMatrix &M, 
				const IntervalMatrix &rM, 
				const IntervalVector &V);
      void borrow_base(const IParals& iv, unsigned int n,
  			const IntervalVector &V);
      void mult_baseLeft(unsigned int n,const IntervalMatrix &M, 
				const IntervalMatrix &rM);
      const IntervalMatrix& mat(unsigned int i) const;
      const IntervalMatrix& Imat(unsigned int i) const;
      const IntervalVector& rhs(unsigned int i) const;
      IntervalVector& rhs(unsigned int i);
      const IntervalVector& bbox() const;
      IntervalVector& bbox();

};


}


namespace codac2 {
      inline IParals::IParals() :
          dim(0), empty(true), nbmat(0), mats(), Vrhs(1) { };
      inline int IParals::get_dim() const { return this->dim; }
      inline int IParals::size() const { return this->get_dim(); }
      inline int IParals::get_nbmat() const { return this->nbmat; }
      inline bool IParals::is_empty() const { return this->empty; }
      inline const IntervalMatrix& IParals::mat(unsigned int i) const {
	return this->mats[i]->first;
      }
      inline const IntervalMatrix& IParals::Imat(unsigned int i) const {
	return this->mats[i]->second;
      }
      inline const IntervalVector& IParals::rhs(unsigned int i) const {
	return this->Vrhs[i==nbmat ? 0 : i+1];
      }
      inline IntervalVector& IParals::rhs(unsigned int i) {
	return this->Vrhs[i==nbmat ? 0 : i+1];
      }
      inline const IntervalVector& IParals::bbox() const {
	return this->Vrhs[0];
      }
      inline IntervalVector& IParals::bbox() {
	return this->Vrhs[0];
      }
      inline void IParals::new_base(const IntervalMatrix &M, 
				const IntervalMatrix &rM, 
				const IntervalVector &V) {
	       this->mats.push_back(
        std::make_shared<std::pair<IntervalMatrix,IntervalMatrix>>
                        (std::pair<IntervalMatrix,IntervalMatrix>(M,rM)));
               this->Vrhs.push_back(V);
               this->nbmat++;
      }
      inline void IParals::change_base(unsigned int numM,
				const IntervalMatrix &M, 
				const IntervalMatrix &rM, 
				const IntervalVector &V) {
               assert(numM<this->nbmat);
	       this->mats[numM] = 
        std::make_shared<std::pair<IntervalMatrix,IntervalMatrix>>
                        (std::pair<IntervalMatrix,IntervalMatrix>(M,rM));
               this->Vrhs[numM+1]=V;
      }
      inline void IParals::borrow_base(const IParals& iv, unsigned int n,
  			const IntervalVector &V) {
//          std::cout << "Appel à borrow base, this : " << *this << 
//               "  autre iparals : " << iv << "\n";
          if (this->nbmat<2) {
	       this->mats.push_back(iv.mats[n]);
	       this->Vrhs.push_back(V);
               this->nbmat++;
          } else {
              this->mats[1]=iv.mats[n];
	      this->Vrhs[2]=V;
          }
      }
      inline void IParals::mult_baseLeft
			(unsigned int n,const IntervalMatrix &M, 
				const IntervalMatrix &rM) {
          assert(n>=0 && n<nbmat);
          IntervalMatrix newM = M*this->mats[n]->first;
          IntervalMatrix newMi = this->mats[n]->second*rM;
          punctualize_coupleMatrix(newM,newMi);
	  std::shared_ptr<std::pair<IntervalMatrix,IntervalMatrix>> nPtr 
		= std::make_shared<std::pair<IntervalMatrix,IntervalMatrix>>
		  (std::pair<IntervalMatrix,IntervalMatrix>(newM,newMi));
	  this->mats[n] = nPtr;
      }
      inline void IParals::set_empty() {
        this->empty=true;
        for (unsigned int i=0;i<=this->nbmat;i++) (this->Vrhs[i]).set_empty();
      }

      inline bool IParals::set_display_inv(bool newVal) {
         bool old = IParals::display_inv;
         IParals::display_inv=newVal;
         return old;
      }
      bool set_display_inv(bool newVal);
      codac::TubeVector to_codac1(codac2::Tube<IParals>& tube);


}

#endif
