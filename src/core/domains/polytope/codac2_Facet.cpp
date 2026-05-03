/**
 * \file codac2_Facet.cpp
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Damien Massé
 *  \copyright  Copyright 2025
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */


#include <utility> /* uses swap */
#include <stdexcept>

#include "codac2_Index.h"
#include "codac2_Vector.h"
#include "codac2_Row.h"
#include "codac2_Interval.h"
#include "codac2_IntervalVector.h"
#include "codac2_IntervalRow.h"
#include "codac2_BoolInterval.h"
#include "codac2_Facet.h"

namespace codac2 {

namespace Facet_ {

polytope_inclrel relation_Box(const Facet &f, const IntervalVector &b, bool strict) {
      const Row &row = f.first.get_row();
      const double rhs = f.second.get_rhs();
      const bool eqcst = f.second.is_eqcst();
      if (eqcst && strict) return inclrel_notinclude | inclrel_disjoint;
      if (b.is_empty()) return inclrel_includes | inclrel_disjoint;
      IntervalVector a(b);
      /* check the vertex that maximizes row */
      for (Index i=0;i<row.size();i++) {
          if (row[i]>0) a[i]=a[i].ub(); else a[i]=a[i].lb();
      }
      Interval maxv = row.dot(a)-rhs;
      polytope_inclrel r1=0;
      if (maxv.ub()<=0.0 && (!strict || maxv.ub()<0.0)) {
         if (!eqcst) return inclrel_includes | inclrel_intersects;
         else if (maxv.ub()<0.0) return inclrel_notinclude | inclrel_disjoint;
         r1 = inclrel_includes;
      } else if (maxv.lb()<=0.0 && (!strict || maxv.lb()<0.0)) {
         r1 = inclrel_mayinclude;
      } else {
         r1 = inclrel_notinclude;
      }
      /* check the vertex that minimizes row */
      for (Index i=0;i<row.size();i++) {
          if (row[i]>0) a[i]=b[i].lb(); else a[i]=b[i].ub();
      }
      Interval minv = row.dot(a)-rhs;
      if (minv.lb()>0.0 || (strict && minv.lb()>=0.0)) 
	  return inclrel_notinclude | inclrel_disjoint;
      if (!eqcst) {
         if (minv.ub()>0.0 || (strict && minv.ub()>=0.0)) 
		return r1 | inclrel_mayintersect;
         return r1 | inclrel_intersects;
      } else { /* eqcst==true -> strict == false */
         if (r1[INCLUDES]) { /* maxv.ub == 0 */
              if (minv.lb()>=0.0) return 
				inclrel_includes | inclrel_intersects;
              return (maxv.lb()==0.0 ? inclrel_intersects : inclrel_mayintersect)
                  |  (minv.ub()<0.0 ? inclrel_notinclude : inclrel_mayinclude);
         }
         if (minv.ub()<0.0) r1 = inclrel_notinclude;
         if (maxv.lb()>=0.0 && minv.ub()<=0.0) 
			return r1 | inclrel_intersects;
         else return r1 | inclrel_mayintersect;
      }
} 

void contract_Box(const Facet &f, IntervalVector &b) {
   const Row &row = f.first.get_row();
   const double rhs = f.second.get_rhs();
   /* use MulOp:bwd */
   IntervalRow x1(row);
   MulOp::bwd(f.second.is_eqcst() ? Interval(rhs,rhs) : Interval(-oo,rhs),x1,b);
   if (b[0].is_empty()) b.set_empty();
}

void contract_out_Box(const Facet &f, IntervalVector &b) {
   const Row &row = f.first.get_row();
   const double rhs = f.second.get_rhs();
   if (f.first.is_coord()) {
      Index c = f.first.gt_dim();
      Interval val = Interval(rhs)/row[c];
      if (f.second.is_eqcst()) {
           if (!val.is_degenerated()) return; /* can't use a value */
           if (val==b[c]) b.set_empty();
      } else {
         if (row[c]>0.0) { /* x[c]>val.lb() */
            if (b[c].ub()<=val.lb()) { b.set_empty(); }
            else if (b[c].lb()<=val.lb()) 
		{ b[c] = Interval(val.lb(),b[c].ub()); }
         }
      }
      return;
   }
   Interval a = b.dot(row);
   if (a.ub()<=rhs) { b.set_empty(); return; }
   if (f.second.is_eqcst()) {
	if (a.lb()>=rhs) b.set_empty();
        return; 
   }
   /* use MulOp:bwd */
   IntervalRow x1(row);
   MulOp::bwd(Interval(rhs,oo),x1,b);
   if (b[0].is_empty()) b.set_empty();
}

Interval bound_linear_form(const Facet &f,
		const Row &row2, const IntervalVector &b) {
   if (f.first.is_null()) return Interval();
   const Row &row = f.first.get_row();
   const double rhs = f.second.get_rhs();
   const bool eqcst = f.second.is_eqcst();
   const Index abdim = f.first.gt_dim();
   bool neg = (row[abdim]<0.0);
   if (!eqcst && 
       ((!neg && row2[abdim]<=0.0) || (neg && row2[abdim]>=0))) 
	return Interval();
   Interval quotient = Interval(row2[abdim])/Interval(row[abdim]);
   Interval res=quotient*rhs;
   IntervalRow rem=row2-quotient*row;
   rem[abdim]=0.0;
   return res + rem.dot(b);
}

} /* end of namespace Facet_ */

std::ostream& operator<<(std::ostream& os, const Facet& f) {
   os << f.second.Id << "(" << f.first.bdim << "," << f.first.vdim << ") : " << f.first.row << (f.second.eqcst ? "=" : "<=" ) << f.second.rhs;
   return os;
}

void output_normalised_facet(std::ostream& os, const Facet& f) {
   os << f.second.Id << " : " << (f.first.row/std::abs(f.first.row[f.first.gt_dim()])) << (f.second.eqcst ? "=" : "<=" ) << (f.second.rhs/std::abs(f.first.row[f.first.gt_dim()]));
}

std::ostream& operator<<(std::ostream& os, const CollectFacets& cf) {
   os << " Collectfacets : " << cf._map.size() << " facets" << std::endl;
   for (const auto &f : cf._map) {
     output_normalised_facet(os,f);
     os << std::endl;
   }
   os << " end Collectfacets" << std::endl;
   return os;
}

CollectFacets::CollectFacets(const Matrix &mat, const Vector &rhsvect,
		const std::vector<Index> &eqSet) : dim(mat.cols()) {
   assert(mat.rows()==rhsvect.size());
   for (Index i=0;i<mat.rows();i++) {
      std::pair<Index,bool> a =this->insert_facet(mat.row(i),rhsvect[i],false);
      if (!a.second) 
	throw std::domain_error("CollectFacets construction with duplicate rows");
   }
   for (Index i: eqSet) { 
      _eqFacets.push_back(i);
      (_allFacets[i])->second.eqcst=true;
   }
}

std::pair<Index,bool> CollectFacets::result_insertion
	(std::pair<mapIterator,bool> res,
	double rhs, bool eqcst, ACTION_DUPLICATE act) {
   if (res.second) {
     Index id = _allFacets.size()+1;
     res.first->second.Id=id;
     _allFacets.push_back(res.first);
     if (eqcst) _eqFacets.push_back(id-1);
     return { id, true };
   } else {
      if (act==KEEP_RHS) return { 0, false };
      double old_rhs=res.first->second.rhs;
      bool old_eqcst = res.first->second.eqcst;
      double new_rhs;
      bool new_eqcst;
      if (act==CHANGE_RHS) { new_eqcst = eqcst; new_rhs = rhs; }
      else if (act==MAX_RHS) {
              if (old_eqcst && eqcst) {
                 if (old_rhs==rhs) return { 0, false };
                 /* complex case */
                 res.first->second.rhs=std::max(old_rhs,rhs);
                 res.first->second.eqcst=false;
                 remove_in_eqFacets(res.first->second.Id-1);
                 std::pair<Index,bool> rinsert =
			this->insert_facet(-res.first->first.row,
				-std::min(old_rhs,rhs),false,MAX_RHS);
                          /* can't loop, as we insert an inequality */
                 return rinsert; 
              } else {
                 if (old_rhs>=rhs && !old_eqcst) return { 0, false };
                 new_eqcst=false;
                 new_rhs=std::max(old_rhs,rhs);
              }
      } else { /* MIN_RHS */
              if (old_rhs<=rhs) {
                 if (!eqcst) return {0, false};
                 if (old_rhs<rhs) return {-1, true}; /* empty */
              } else {
                 if (old_eqcst) return {-1,true}; /*empty */
              }
              new_eqcst=eqcst;
              new_rhs=rhs;
      }
      res.first->second.rhs=new_rhs;
      res.first->second.eqcst=new_eqcst;
      if (new_eqcst && !old_eqcst) 
	_eqFacets.push_back(res.first->second.Id-1);
      else if (!new_eqcst && old_eqcst) 
 	 remove_in_eqFacets(res.first->second.Id-1);
      return { res.first->second.Id, false };
   }
   return { 0, false };
}

void CollectFacets::remove_in_eqFacets(Index id) {
   for (Index i=0; i<(Index)_eqFacets.size(); i++) {
      if (_eqFacets[i]==id) {
         _eqFacets[i]=_eqFacets.back();
         _eqFacets.pop_back();
         return;
      }
   }
}

std::pair<Index,bool> CollectFacets::insert_facet(const Row &row, 
		double rhs, bool eqcst, ACTION_DUPLICATE act) {
   std::pair<mapIterator,bool> res =
	_map.emplace(std::move(Facet_::make(row,rhs,eqcst,0)));
   return result_insertion(res,rhs,eqcst,act);
}
std::pair<Index,bool> CollectFacets::insert_facet(Row &&row, double rhs, bool eqcst, ACTION_DUPLICATE act) {
   std::pair<mapIterator,bool> res =
	_map.emplace(std::move(Facet_::make(row,rhs,eqcst,0)));
   return result_insertion(res,rhs,eqcst,act);
}
std::pair<Index,bool> CollectFacets::insert_facet(const Facet &fct, ACTION_DUPLICATE act) {
   std::pair<mapIterator,bool> res = _map.insert(fct);
   return result_insertion(res,fct.second.rhs,fct.second.eqcst,act);
}

std::pair<CollectFacets::mapIterator,bool>
   CollectFacets::change_eqFacet(Index id, Row &&nrow, double nrhs,
		ACTION_DUPLICATE act) {
   mapIterator fcIt = _allFacets[_eqFacets[id]];
   auto nde = _map.extract(fcIt);
   nde.key().change_row(nrow);
   nde.mapped().rhs=nrhs;
   const auto result_insert = _map.insert(std::move(nde));
   if (!result_insert.inserted) {
      double old_rhs=result_insert.position->second.rhs;
      if (act==CHANGE_RHS ||
	   (act==MAX_RHS && old_rhs<nrhs) || (act==MIN_RHS && old_rhs>nrhs)) {
           bool old_eqcst = result_insert.position->second.eqcst;
           result_insert.position->second.rhs=nrhs;
           result_insert.position->second.eqcst=true;
	   result_insert.position->second.Id=_eqFacets[id]+1;
           _allFacets[_eqFacets[id]] = result_insert.position;
           if (old_eqcst) 
		remove_in_eqFacets(result_insert.position->second.Id-1);
	   _allFacets[result_insert.position->second.Id-1] = endFacet();
 	   nb_removed_facets++;
           return { result_insert.position, false };
      } else {
         _allFacets[_eqFacets[id]] = endFacet();
	 nb_removed_facets++;
         _eqFacets[id] = _eqFacets.back();
         _eqFacets.pop_back();
         return { this->_map.end(), false };
      }
   }
   _allFacets[_eqFacets[id]] = result_insert.position;
   return { result_insert.position, true };
} 

std::pair<CollectFacets::mapIterator,bool>
   CollectFacets::change_ineqFacet(Index id, Row &&nrow, double nrhs,
		ACTION_DUPLICATE act) {
   mapIterator fcIt = _allFacets[id-1];
   auto nde = _map.extract(fcIt);
   nde.key().change_row(nrow);
   nde.mapped().rhs=nrhs;
   const auto result_insert = _map.insert(std::move(nde));
   if (!result_insert.inserted) {
      double old_rhs=result_insert.position->second.rhs;
      if (act==CHANGE_RHS ||
	   (act==MAX_RHS && old_rhs<nrhs) || (act==MIN_RHS && old_rhs>nrhs)) {
         bool old_eqcst = result_insert.position->second.eqcst;
         result_insert.position->second.rhs=nrhs;
         result_insert.position->second.eqcst=true;
         if (old_eqcst) 
		remove_in_eqFacets(result_insert.position->second.Id-1);
	 _allFacets[result_insert.position->second.Id-1] = endFacet();
 	 nb_removed_facets++;
	 result_insert.position->second.Id=id;
         _allFacets[id-1] = result_insert.position;
         return { result_insert.position, false };
      } else {
         _allFacets[id-1] = endFacet();
 	 nb_removed_facets++;
         return { this->_map.end(), false };
      }
   }
   _allFacets[id-1] = result_insert.position;
   return { result_insert.position, true };
} 

CollectFacets::mapIterator CollectFacets::change_ineqFacet_rhs(mapIterator it,
			 double nrhs) {
   it->second.rhs=nrhs;
   return it;
}

CollectFacets::mapIterator 
	CollectFacets::dissociate_eqFacet(Index id, double nbound,
		ACTION_DUPLICATE act) {
   Index aId = _eqFacets[id]; 
		/* start with removing id in _eqFacets */
   _eqFacets[id] = _eqFacets.back();
   _eqFacets.pop_back();
   mapIterator fcIt = _allFacets[aId];
   mapIterator ret = this->_map.end();
   double rhs = fcIt->second.rhs;
   if (nbound<=rhs) {
       fcIt->second.eqcst=false; 
       if (nbound>-oo) {
          std::pair<Index,bool> rinsert =
		 this->insert_facet(-fcIt->first.row,-nbound,false,act);
          if (rinsert.first!=0) ret = _allFacets[rinsert.first-1];
       } else ret = fcIt;
   } else {
       fcIt->second.eqcst=false; 
       if (nbound<+oo) {
          std::pair<Index,bool> rinsert = this->insert_facet(-fcIt->first.row,
				-fcIt->second.rhs,false,act);
          fcIt->second.rhs = nbound;  
          if (rinsert.first!=0) ret = _allFacets[rinsert.first-1];
       } else { /* we need to extract and reinsert the facet */
          auto nde = _map.extract(fcIt);
          nde.key().negate_row();
          double nrhs = -nde.mapped().rhs;
          nde.mapped().rhs= nrhs;
          nde.mapped().eqcst= false;
          const auto result_insert = _map.insert(std::move(nde));
          if (!result_insert.inserted) {
             double old_rhs=result_insert.position->second.rhs;
	     /* CHECKME : not the best idea ? */
             if (act==CHANGE_RHS ||
	        (act==MAX_RHS && old_rhs<nrhs) || 
		(act==MIN_RHS && old_rhs>nrhs)) {
                bool old_eqcst = result_insert.position->second.eqcst;
                result_insert.position->second.rhs=nrhs;
                result_insert.position->second.eqcst=false;
                if (old_eqcst) 
	   	   remove_in_eqFacets(result_insert.position->second.Id-1);
	        _allFacets[result_insert.position->second.Id-1] = endFacet();
	  	nb_removed_facets++;
	        result_insert.position->second.Id=aId+1;
	        _allFacets[aId] = result_insert.position;
                ret=result_insert.position;
             } else {
                _allFacets[aId] = endFacet();
		nb_removed_facets++;
                ret=endFacet();
             }
         } else {
            _allFacets[aId] = result_insert.position;
            ret=result_insert.position;
         }
      }
   }
   return ret;
}

bool CollectFacets::removeFacetById(Index id) {
   if (_allFacets[id-1]==_map.end()) return false;
   if (_allFacets[id-1]->second.eqcst) {
      this->remove_in_eqFacets(id-1);
   }
   _map.erase(_allFacets[id-1]);
   _allFacets[id-1]=endFacet();
   nb_removed_facets++;
   return true;
}

IntervalVector CollectFacets::extractBox() {
   assert_release(this->getDim()!=-1);
   /* check emptiness with null row */
   Row row = Row::zero(this->dim);
   mapIterator it = _map.find(FacetBase(row));
   if (it!=_map.end()) {
      FacetRhs &rhs = it->second;
      if (rhs.rhs<0.0 || (rhs.rhs>0.0 && rhs.eqcst))
         return IntervalVector::constant(this->getDim(),Interval::empty());
      _allFacets[it->second.Id-1]= endFacet();
      nb_removed_facets++;
      if (rhs.eqcst) remove_in_eqFacets(it->second.Id-1);
      _map.erase(it);
   }
   /* check bounds if IV */
   IntervalVector ret = IntervalVector(this->getDim());
   for (Index i=0;i<this->getDim();i++) {
      Interval &bound = ret[i];
      std::pair<mapIterator,mapIterator> pIt 
	= this->range_between_bases(FacetBase::base_range(this->getDim(),i,
				false));
      while (pIt.first!=pIt.second && !bound.is_empty()) {
         FacetRhs &rhs = pIt.first->second;
         Interval val(pIt.first->first.row[i]);
         if (rhs.eqcst) bound &= rhs.rhs/val;
         else bound = min(bound,rhs.rhs/val);
         _allFacets[pIt.first->second.Id-1]=endFacet();
         nb_removed_facets++;
         if (rhs.eqcst) remove_in_eqFacets(pIt.first->second.Id-1);
         pIt.first = _map.erase(pIt.first); /* pIt points to the next element */
      }
      pIt = this->range_between_bases(FacetBase::base_range(this->getDim(),i,
				true));
      while (pIt.first!=pIt.second && !bound.is_empty()) {
         FacetRhs &rhs = pIt.first->second;
         Interval val(pIt.first->first.row[i]);
         if (rhs.eqcst) bound &= rhs.rhs/val;
         else bound = max(bound,rhs.rhs/val);
         _allFacets[pIt.first->second.Id-1]=endFacet();
         nb_removed_facets++;
         if (rhs.eqcst) remove_in_eqFacets(pIt.first->second.Id-1);
         pIt.first = _map.erase(pIt.first); /* pIt points to the next element */
      }
      if (bound.is_empty()) { ret.set_empty(); break; }
   }
   return ret;
}

std::vector<Index> CollectFacets::renumber() {
   /* renumber the constraints, only if there are removed facets */
   if (nb_removed_facets==0) return std::vector<Index>();
   std::vector<Index> ret(_allFacets.size(),-1);
   Index sineq=0, seq=0;
   _allFacets.resize(_map.size());
   for (mapIterator it = _map.begin(); it!=_map.end(); ++it) {
       if (it->second.eqcst) {
          if (seq>=(Index)_eqFacets.size()) 
	     _eqFacets.push_back(sineq);
          else 
             _eqFacets[seq]=sineq;
	  ++seq;
       }
       _allFacets[sineq++]=it;
       ret[it->second.Id-1]=sineq;
       it->second.Id = sineq;
   }
   if (seq<(Index)_eqFacets.size())
     _eqFacets.resize(seq);
   nb_removed_facets=0;
   return ret;
}

void CollectFacets::encompass_vertices(const 
		std::vector<IntervalVector> &vertices, 
	IntervalVector &bbox, bool tight) {
   assert_release(this->getDim()!=-1);
   if (tight) bbox=IntervalVector::constant(this->getDim(),Interval::empty());
   if (vertices.size()==0) return;
   for (mapIterator it = _map.begin(); it!=_map.end(); ++it) {
      IntervalRow row = it->first.row;
      Interval a = (tight ? Interval::empty() : Interval(it->second.rhs));
      for (const IntervalVector &v : vertices) {
          a |= row.dot(v);
      }
      it->second.rhs=a.ub(); 
      if (it->second.eqcst && !a.is_degenerated()) {
         this->remove_in_eqFacets(it->second.Id-1);
         it->second.eqcst=false;
         this->insert_facet(-it->first.row,-a.lb(),false); 
		/* note : possible that the new facet will be visited
		   later, but that should not be a real problem */
      }
   }
   for (const IntervalVector &v : vertices) {
       bbox |= v;
   }
}
void CollectFacets::encompass_vertices(const std::vector<Vector> &vertices, 
	IntervalVector &bbox, bool tight) {
   assert_release(this->getDim()!=-1);
   if (tight) bbox=IntervalVector::constant(this->getDim(),Interval::empty());
   if (vertices.size()==0) return;
   for (mapIterator it = _map.begin(); it!=_map.end(); ++it) {
      IntervalRow row = it->first.row;
      Interval a = (tight ? Interval::empty() : Interval(it->second.rhs));
      for (const Vector &v : vertices) {
          a |= row.dot(v);
      }
      it->second.rhs=a.ub(); 
      if (it->second.eqcst && !a.is_degenerated()) {
         this->remove_in_eqFacets(it->second.Id-1);
         it->second.eqcst=false;
         this->insert_facet(-it->first.row,-a.lb(),false); 
		/* note : possible that the new facet will be visited
		   later, but that should not be a real problem */
      }
   }
   for (const Vector &v : vertices) {
       bbox |= IntervalVector(v);
   }
}

void CollectFacets::encompass_zonotope(const IntervalVector &z, 
		const IntervalMatrix &A,
                const IntervalVector &range, bool tight) {
   assert_release(this->getDim()!=-1);
   for (mapIterator it = _map.begin(); it!=_map.end(); ++it) {
      Interval a = (tight ? Interval::empty() : Interval(it->second.rhs));
      a |= ((z.dot(it->first.row)) + (it->first.row * A).dot(range));
      it->second.rhs=a.ub(); 
      if (it->second.eqcst && !a.is_degenerated()) {
         this->remove_in_eqFacets(it->second.Id-1);
         it->second.eqcst=false;
         this->insert_facet(-it->first.row,-a.lb(),false); 
		/* note : possible that the new facet will be visited
		   later, but that should not be a real problem */
      }
   }
}

int CollectFacets::merge_CollectFacets(const CollectFacets &collect, 
		ACTION_DUPLICATE act) {
  int count=0;
  for (const auto &fct : collect._map) {
     std::pair<Index,bool> r = this->insert_facet(fct,act);
     if (r.first==-1) return -1;
     if (r.second) count++;
  }
  return count;
}

}
