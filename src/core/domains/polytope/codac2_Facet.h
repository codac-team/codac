/**
 * \file codac2_Facet.h  
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Damien Massé
 *  \copyright  Copyright 2025
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include <utility> /* uses swap */
#include <bitset>
#include <cmath>
#include <set>

#include "codac2_Index.h"
#include "codac2_Vector.h"
#include "codac2_Row.h"
#include "codac2_Interval.h"
#include "codac2_IntervalVector.h"
#include "codac2_IntervalRow.h"
#include "codac2_Zonotope.h"
#include "codac2_BoolInterval.h"
#include "codac2_operators.h"

namespace codac2 {

class FacetBase;
class FacetRhs;
/** \brief a facet is a pair of FacetBase and FacetRhs */
using Facet = std::pair<FacetBase,FacetRhs>;

/** Encapsulation of Row (key of facet maps) */
class FacetBase {
   /* friends */
   friend class CollectFacets;
   friend class Polytope;
   friend class DDbuildF2V;
   friend class DDbuildV2F;
   friend struct FacetBaseComp;

   public:

      /** \brief build a new FacetBase from a row 
       *  \param row the Row 
       */
      FacetBase(const Row &row) : bdim(-1), vdim(0.0), row(row) {
        this->compute_key();
      }

      /** \brief build a new FacetBase from a row 
       *  \param row the Row 
       */
      FacetBase(Row &&row) : bdim(-1), vdim(0.0), row(row) {
        this->compute_key();
      }  

      FacetBase(const FacetBase &fc) = default;
      FacetBase(FacetBase &&fc) = default;

      /** \brief get the row 
       *  \return the row */
      inline const Row &get_row() const { return row; }
   
      /** \brief test if the Row is zero 
       *  \return true if row = 0 */
      inline bool is_null() const { return (bdim==-1); }
   
      /** \brief test if the row has only one non-zero coordinate 
       *  \return true if row is zero for all coordinates except one */
      inline bool is_coord() const {
         if (row.size()==0) return false;
         return (bdim%(2*row.size())==0);
      }

      /** \brief return the index of the greatest coordinate (in absolute value)
       *  suppose the row is not zero
       *  \return a value between 0 and size-1 */
      inline Index gt_dim () const {
         if (row.size()==0) return -1; 
         return (bdim/(2*row.size()))%row.size();
      }

      inline void swap(FacetBase &fb) {
         this->row.swap(fb.row);
         std::swap(this->bdim,fb.bdim);
         std::swap(this->bdim,fb.bdim);
      }

   private:

      Index bdim; /* if A is the index of the greatest abs value and B
                  the second greatest (=A if 0 outside)
		    v[A]>0 v[B]>0     A*(2*dim)+A-B (+2*dim if B>A)
                    v[A]>0 v[B]<0     A*(2*dim)+A-(B+dim)+2*dim
                    v[A]<0 v[B]>0     (A+dim)*(2*dim)+A+dim-B
                    v[A]<0 v[B]<0     (A+dim)*(2*dim)+(A+dim)-(B+dim)
							 (+2*dim if B>A)
                  noting Adim = A if v[A]>0 and A+dim if v[A]<0
                  and Bdim = B if v[B]>0 and B+dim if v[B]<0
                  bdim = Adim*(2*dim+1) - Bdim (+2*dim if Adim>Bdim)
                  if null vector, value = -1 */
      double vdim; /* |v[B]|/|v[A]| (0 if no value for B) */
      Row row;

  
      /* create a "min" and "max" FacetBase for constraints of the form
         a_i x_i <= b_i (or - a_i x_i <= b_i if neg is true */
      static std::pair<FacetBase,FacetBase> 
		base_range(Index dim, Index i, bool neg) {
         Row r = Row::zero(dim);
         Index d = (i+(neg?dim:0))*(2*dim);
         return std::make_pair<FacetBase,FacetBase>
  		(FacetBase(d,-1.0,r),FacetBase(d,1.0,r));
      }

      inline void negate_row() {
         this->row=-row;
         Index u = 2*row.size()*row.size();
         if (this->bdim>=u) this->bdim-=u; else this->bdim+=u;
      }
   
      inline void change_row(Row &&row) {
         this->row=row; this->bdim=-1; this->vdim=0.0;
         this->compute_key();
      }
      inline void change_row(const Row &row) {
         this->row=row; this->bdim=-1; this->vdim=0.0;
         this->compute_key();
      }

      FacetBase(Index bdim, double vdim, const Row &row) : 
		bdim(bdim), vdim(vdim), row(row) { }

      void compute_key() {
        if (row.size()==0) return; 
        double val=row[0];
        double valabs=std::abs(val);
        if (val<0.0) bdim=row.size(); else if (val>0.0) bdim=0;
        Index b2dim = -1;
        double val2abs=0.0;
        for (Index i=1;i<row.size();++i) {
          double val_i=std::abs(row[i]);
          if (val_i>valabs) {
	     val2abs=valabs;
	     valabs=val_i;
             val=row[i];
 	     b2dim=bdim;
             if (val<0.0) bdim=row.size()+i; else bdim=i;
          } else if (val_i>val2abs) {
             val2abs=val_i;
             if (row[i]<0.0) b2dim=row.size()+i; else b2dim=i;
          }
        }
        if (valabs==0.0) return; /* bdim=-1 */
        if (b2dim==-1) { bdim = 2*row.size()*bdim; vdim=0.0; return; }
        if (b2dim>bdim) 
            bdim = bdim * (2*row.size()+1) + 2*row.size() - b2dim;
        else
            bdim = bdim * (2*row.size()+1) - b2dim;
        vdim = val2abs/valabs;
     }
     friend std::ostream& operator<<(std::ostream& os, const Facet& f);
     friend void output_normalised_facet(std::ostream& os, const Facet& f);

};

/** Second part of the facet: nature of the facet (equality or not),
 *  right-hand side. An Id number is also kept for internal use inside
 *  a collection of facets. This number may change when the collection is
 *  modified */
class FacetRhs {
   /* friends */
   friend class CollectFacets;
   friend class Polytope;
   friend class DDbuildF2V;
   friend class DDbuildV2F;

   public:
     /** \brief value of the rhs 
      * \return the rhs */
     double get_rhs() const { return rhs; }

     /** \brief nature of the facet (= or <=)
      * \return true if equality, false if inequality */
     bool is_eqcst() const { return eqcst; }

     /** \brief id number of the facet 
      * \return id number */
     Index get_Id() const { return Id; }

     FacetRhs(double rhs, bool eqcst, Index Id=0) : rhs(rhs), eqcst(eqcst), Id(Id)
	 {};
     FacetRhs(const FacetRhs &rhs) = default;

     void swap(FacetRhs &r) {
       std::swap(rhs,r.rhs);
       std::swap(eqcst,r.eqcst);
     }

   private :

     double rhs;
     bool eqcst;
     Index Id;
     friend std::ostream& operator<<(std::ostream& os, const Facet& f);
     friend void output_normalised_facet(std::ostream& os, const Facet& f);

};


/** namespace Facet_ is used to encapsulate Facet functions */ 
namespace Facet_ {

  inline Facet make(const Row &row, double rhs, bool eqcst, Index Id) {
     return std::make_pair(FacetBase(row),FacetRhs(rhs,eqcst,Id));
  }
  inline Facet make(Row &&row, double rhs, bool eqcst, Index Id) {
     return std::make_pair(FacetBase(row),FacetRhs(rhs,eqcst,Id));
  }

  /** \brief create a new facet from a row, rhs, eqcst
   *  \param row the row
   *  \param rhs the rhs
   *  \param eqcst true for equalities, false for <= 
   *  \return the new Facet */
  inline Facet make(const Row &row, double rhs, bool eqcst) {
     return std::make_pair(FacetBase(row),FacetRhs(rhs,eqcst,0));
  }

  /** \brief create a new facet from a row, rhs, eqcst
   *  \param row the row
   *  \param rhs the rhs
   *  \param eqcst true for equalities, false for <= 
   *  \return the new Facet */
  inline Facet make(Row &&row, double rhs, bool eqcst) {
     return std::make_pair(FacetBase(row),FacetRhs(rhs,eqcst,0));
  }

  inline Facet make(const FacetBase &base, double rhs, bool eqcst) {
     return std::make_pair(base,FacetRhs(rhs,eqcst,0));
  }

  inline void swap_Facet(Facet &f1,Facet &f2) {
     f1.first.swap(f2.first);
     f1.second.swap(f2.second);
  }

   
  /** \brief Potential inclusion relation between sets 
   * INCLUDES : A is included in B
   * MAYINCLUDE : A may be included in B
   * NOTINCLUDE : A\\B is not empty
   * INTERSECTS : A inter B is non empty
   * MAYINTERSECT : A inter B may be non empty
   * DISJOINT : A inter B is empty
   */
  enum INCLREL {
    INCLUDES,
    MAYINCLUDE,
    NOTINCLUDE,
    INTERSECTS,
    MAYINTERSECT,
    DISJOINT,
    SIZEINCLREL
  };
  using polytope_inclrel = std::bitset<SIZEINCLREL>;
  const polytope_inclrel inclrel_includes = 1<<INCLUDES;
  const polytope_inclrel inclrel_mayinclude = 1<<MAYINCLUDE;
  const polytope_inclrel inclrel_notinclude = 1<<NOTINCLUDE;
  const polytope_inclrel inclrel_intersects = 1<<INTERSECTS;
  const polytope_inclrel inclrel_mayintersect = 1<<MAYINTERSECT;
  const polytope_inclrel inclrel_disjoint = 1<<DISJOINT;

  /* test only includes and intersects */
  polytope_inclrel relation_Box(const Facet &f, const IntervalVector &b, bool strict=false);

   /* give the bound for a Row and a box, based on the largest index (bdim).
      ie compute R(bdim)/r(bdim) rhs + (R - R(bdim)/r(bdim) r) B,
      assuming R(bdim)/r(bdim) is positive, or the constraint is an equality */
  Interval bound_linear_form(const Facet &f, 
		const Row &row2, const IntervalVector &b);

  void contract_Box(const Facet &f, IntervalVector &b);
  void contract_out_Box(const Facet &f, IntervalVector &b);
}

  /** \brief print a Facet
   *  \param os the output stream
   *  \param f a facet
   *  \return os */
std::ostream& operator<<(std::ostream& os, const Facet& f); 

struct FacetBaseComp {
   inline bool operator()(const FacetBase& lhs, const FacetBase& rhs) const
    {
        if (lhs.bdim<rhs.bdim) return true;
        else if (lhs.bdim>rhs.bdim) return false;
        if (lhs.vdim<rhs.vdim) return true;
        else if (lhs.vdim>rhs.vdim) return false;
        for (int i=0;i<lhs.row.size();i++) 
           if (lhs.row[i]<rhs.row[i]) return true;
           else if (lhs.row[i]>rhs.row[i]) return false;
        return false;
    }
};

/** encapsulate a collection of facets with double index :
    by the base vector, and by Id number. Renumbering can happen
    when facets are removed. */
class CollectFacets {
   public:
     using mapType = std::map<FacetBase,FacetRhs,FacetBaseComp>;
     using mapIterator = mapType::iterator;
     using mapCIterator = mapType::const_iterator;

     /** generate an empty collection, without dimension */
     CollectFacets() : dim(-1) {};
     /** generate an empty collection, with dimension
      *  @param dim the dimension
      */
     CollectFacets(Index dim) : dim(dim) {};
     /** generate the set of facets from a matrix and a vector, i.e.
      *  mat X <= rhs (eqSet states which rows of mat are equalities)
      *  @param mat the matrix
      *  @param rhsvect the vector of rhs
      *  @param eqSet the set of equalities */
     CollectFacets(const Matrix &mat, const Vector &rhsvect, 
			const std::vector<Index> &eqSet);

     /** @brief create a copy of the CollectFacets, updating _allFacets
      *  @param cf the CollectFacets 
      */
     CollectFacets(const CollectFacets& cf);

     /** move the elements of CollectFacets. Perform a renumbering of Id if
      *  nb_removed_facets > 0
      *  @param cf the CollectFacets 
      */
     CollectFacets(CollectFacets&& cf);

     ~CollectFacets() = default;
  
     /** return the dimension of the facets 
      *  @return the dimension */
     Index getDim() const;
     /** return the number of facets, as the size of _allfacets */
     Index nbfcts() const;
     /** return the number of equality facets, as the size of _eqfacets */
     Index nbeqfcts() const;
     /** return the map */
     const mapType &get_map() const;

     enum ACTION_DUPLICATE { /* operation to do when a modification
				create a duplicate facet */
       KEEP_RHS, /* change nothing (default behaviour) */
       CHANGE_RHS, /* change the value in all cases */
       MAX_RHS, /* maximize the RHS. if two equalities,
			duplicate the constraint. if one equality, 
			change into inequality */
       MIN_RHS  /* minimize the RHS. if needed, return "empty" (-1) */
     };
       
   
     /** insert a new facet row X <= rhs (or row X = rhs)
      * @param row Row
      * @param rhs right-hand side 
      * @param eqcst equality constraint
      * @param act action if an rhs with the same row exists
      * @return the Id of the constraint, and true if a new facet
      * has been inserted, false if old facet */
     std::pair<Index,bool> insert_facet(const Row &row, 
			double rhs, bool eqcst, ACTION_DUPLICATE act=KEEP_RHS);
     /** insert a new facet row X <= rhs (or row X = rhs)
      * @param row Row
      * @param rhs right-hand side 
      * @param eqcst equality constraint
      * @param act action if an rhs with the same row exists
      * @return the Id of the new constraint, and true if a new facet
      * has been inserted, false if old facet. If old facet and no change,
      * returns 0 */
     std::pair<Index,bool> insert_facet(Row &&row, double rhs, bool eqcst,
			ACTION_DUPLICATE act=KEEP_RHS);
     /** insert a new facet row X <= rhs or row X = rhs,
      * using the Facet constuction
      * @param facet the facet
      * @param act action if an rhs with the same row exists
      * @return the Id of the constraint, and true if a new facet
      * has been inserted, false if old facet. If old facet and no change
      * returns 0 */
     std::pair<Index,bool> insert_facet(const Facet &facet,
		ACTION_DUPLICATE act=KEEP_RHS);

     /** get a facet by Id, starting from 0 (i.e. facet of index Id+1)
      * @param id the Id of the facet, minus 1
      * @return an iterator on the facet */
     mapIterator operator[](Index id);
     /** get a equality facet by the equality Id,
      * starting from 0 (i.e. facet of index eqIndex(Id+1))
      * @param eqId the equality Id of the facet
      * @return an iterator on the facet */
     mapIterator get_eqFacet(Index eqId);
     /** get the end iterator of the facet
      * @return an iterator on the facet */
     mapIterator endFacet(); 
     /** get the end iterator of the facet
      * @return an iterator on the facet */
     mapCIterator endFacet() const; 
     /** get the first iterator 
      * @return an iterator on the facet */
     mapIterator beginFacet(); 
     /** get the first iterator 
      * @return an iterator on the facet */
     mapCIterator beginFacet() const; 


     /** change a eqfacet, keeping its Id (unless the new row creates
      * a duplicate)
      *  @param eqId (the eqId of the facet)
      *  @param nrow (the new row of the facet)
      *  @param nrhs (the new right-hand side)
      *  @param act action if duplicate
      *  @return an iterator on the new facet, true if the modified facet
      *  has been inserted, false if there was a duplicate. In this case
      *  if the existing facet is not changed, return endFacet() */
     std::pair<mapIterator,bool> change_eqFacet(Index eqId, Row&& nrow,
			 double nrhs, ACTION_DUPLICATE act=KEEP_RHS);
     /** change a facet, keeping its Id (unless the new row creates
      * a duplicate)
      *  @param id the id of the facet starting from 1
      *  @param nrow the new row of the facet
      *  @param nrhs the new right-hand side
      *  @param act action if duplicate
      *  @return an iterator on the new facet, true if the modified facet
      *  has been inserted, false if there was a duplicate. In this
      *  cas, without change, return endFacet() */
     std::pair<mapIterator,bool> change_ineqFacet(Index id, Row&& nrow,
			 double nrhs, ACTION_DUPLICATE act=KEEP_RHS);

     /** change the rhs of a facet
      *  @param it the iterator of the facet
      *  @param nrhs the new right-hand side
      *  @return the iterator of the facet (=it) */
     mapIterator change_ineqFacet_rhs(mapIterator it, double nrhs);

     /** transform an equality facet to an unequality facet, and add
        a new facet with a new bound nbound
        if nbound<rhs, keep row x <= rhs and add -row x <= -nbound
        if nbound>rhs, put row x <= nbound and add -row x <= -rhs
        if nbound = -oo or +oo, do not modify/add the new facet
        @param eqId the equality Id of the equality facet
        @param nbound the new bound
        @param act action if duplicate
        @return depends on the construction
            nbound=-oo => current facet iterator
            -oo<nbound<rhs => new facet iterator (for nbound), or endFacet if
                insertion "failed"
            rhs<nbound<+oo => new facet iterator (for rhs), or endFacet if 
                insertion "failed"
            nbound=+oo => new facet iterator (for rhs), or endFacet if 
	        insertion "failed" */
     mapIterator dissociate_eqFacet(Index eqId , double nbound,
		ACTION_DUPLICATE act=MAX_RHS);

     /** insert a complete collection of facets 
         @param collect the collection
         @param act handling of the duplicate 
         @return the number of added facets, -1 if empty
      */
     int merge_CollectFacets(const CollectFacets &collect, 
			ACTION_DUPLICATE act=MIN_RHS);

     /** remove a facet by its Id
      *  @param id Id of the facet (starting from 1)
      *  @return true if the facet existed and has been removed */
     bool removeFacetById(Index id);

     /** extract the constraints corresponding to the bounding box,
      *  or a possible "emptiness constraint".
      *  remove some contraints, may put endFacet() for some Id */
     IntervalVector extractBox();
   
     /** \brief renumber the set, if _allFacets have removed facets.
      *  do not modify the iterators, but modify the indices of the 
      *  facets and equality facets. 
      *  \return the matching old Id => new Id */
     std::vector<Index> renumber();

     /** \brief modify the rhs such that the polyhedron encompasses a list
      *  of intervalVector. Also adapt the bbox.
      *  if tight=true, may tighten the bound, and not only increase them.
      *  may add facets if needed to dissociate equality facets. 
      * \param vertices the list of vertices
      * \param bbox the bbox (to be updated)
      * \param tight if false, the rhs are only increased */
     void encompass_vertices(const std::vector<IntervalVector> &vertices,
		IntervalVector &bbox, bool tight);
     void encompass_vertices(const std::vector<Vector> &vertices,
		IntervalVector &bbox, bool tight);

     /** \brief modify the rhs such that the polyhedron encompasses an
      *  "interval zonotope" z+A v. 
      *  if tight=true, may tighten the bound, and not only increase them.
      *  may add facets if needed to dissociate equality facets. 
      * \brief z center of the interval zonotope
      * \brief A matrix of the interval zonotope
      * \brief v interval vector for the interval zonotope
      * \brief tight if false, the rhs are only increased */
     void encompass_zonotope(const IntervalVector &z, const IntervalMatrix &A,
		const IntervalVector &range, bool tight);

     /** \brief dump the set of facets, for debugging purposes */
     friend std::ostream& operator<<(std::ostream& os, 
			const CollectFacets& cf); 


   private:
      mutable Index dim;
      mapType _map;
      std::vector<mapIterator> _allFacets;
      std::vector<Index> _eqFacets;   /* index in _allFacets */
      int nb_removed_facets=0;   /* number of removed facets in _allFacets */

      /* look for and remove a value in eqFacet */
      void remove_in_eqFacets(Index id);
      /* finish an insertion process */
      std::pair<Index,bool> result_insertion(std::pair<mapIterator,bool> res,
		 double rhs, bool eqcst, ACTION_DUPLICATE act);
      /* return a range of iterators between two bases */
      std::pair<mapIterator,mapIterator> range_between_bases
                 (const std::pair<FacetBase,FacetBase> &bases);

};

inline CollectFacets::CollectFacets(const CollectFacets& cf)  :
    dim(cf.getDim()), _map(cf._map), 
	_allFacets(cf._allFacets.size(),_map.end()),
    _eqFacets(cf._eqFacets), nb_removed_facets(cf.nb_removed_facets)
{
    /* redirect the facets */
    for (mapIterator it = _map.begin(); it!=_map.end(); ++it) {
        _allFacets[it->second.Id-1]=it;
    }
}

inline CollectFacets::CollectFacets(CollectFacets&& cf)  :
    dim(cf.getDim()), _map(std::move(cf._map)), 
	_allFacets(std::move(cf._allFacets)),
    _eqFacets(cf._eqFacets), nb_removed_facets(cf.nb_removed_facets)
{
    if (nb_removed_facets>0) this->renumber();
}

inline Index CollectFacets::getDim() const {
   if (this->dim!=-1) return this->dim;
   if (_map.size()==0) return -1;
   this->dim=(_map.begin()->first.row.size());
   return this->dim;
} 

inline const CollectFacets::mapType &CollectFacets::get_map() const
   { return _map; }

inline CollectFacets::mapIterator CollectFacets::operator[](Index id)  
   { return this->_allFacets[id]; }
inline CollectFacets::mapIterator CollectFacets::get_eqFacet(Index id) 
   { return this->_allFacets[this->_eqFacets[id]]; }

inline Index CollectFacets::nbfcts() const {
   return this->_map.size(); 
}

inline Index CollectFacets::nbeqfcts() const {
   return this->_eqFacets.size(); 
}

inline CollectFacets::mapIterator CollectFacets::endFacet() { 
   return this->_map.end();
}
inline CollectFacets::mapCIterator CollectFacets::endFacet() const { 
   return this->_map.cend();
}
inline CollectFacets::mapIterator CollectFacets::beginFacet() { 
   return this->_map.begin();
}
inline CollectFacets::mapCIterator CollectFacets::beginFacet() const { 
   return this->_map.cbegin();
}


inline std::pair<CollectFacets::mapIterator,CollectFacets::mapIterator>
 CollectFacets::range_between_bases
   (const std::pair<FacetBase,FacetBase> &bases) {
     return std::pair
             (_map.lower_bound(bases.first),_map.lower_bound(bases.second));
}

}
