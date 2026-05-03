/**
 * \file codac2_Polytope.cpp
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Damien Massé
 *  \copyright  Copyright 2025
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <vector>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <ctime>
#include <cmath>
#include <bitset>
#include <memory>

#include "codac2_Index.h"
#include "codac2_Matrix.h"
#include "codac2_Vector.h"
#include "codac2_Row.h"
#include "codac2_IntervalRow.h"
#include "codac2_IntervalVector.h"
#include "codac2_IntervalMatrix.h"
#include "codac2_inversion.h"
#include "codac2_Parallelepiped.h"
#include "codac2_Zonotope.h"
#include "codac2_Polytope.h"
#include "codac2_Facet.h"

using namespace codac2;


namespace codac2 {

Polytope::Polytope() : _dim(-1), state(pol_state_empty | pol_state(1<<INVALID))
{
}

Polytope::Polytope(Index dim) : Polytope(dim,false)
{
}

Polytope::Polytope(Index dim, bool empty) : _dim(dim), 
		_box(IntervalVector::constant(_dim, 
				empty ? Interval::empty() : Interval())),
		_facets(std::make_shared<CollectFacets>(dim)),
		state(empty ? pol_state_empty : pol_state_init)
{
}

Polytope::Polytope(const IntervalVector &box) : _dim(box.size()), _box(box),
		_facets(std::make_shared<CollectFacets>(_dim)),
                state(box.is_empty() ? pol_state_empty : pol_state_init)

{
}

Polytope::Polytope(const Polytope &P) : _dim(P._dim), _box(P._box),
	_facets(P.state[EMPTY] ? std::make_shared<CollectFacets>(_dim) :
		           std::make_shared<CollectFacets>(*(P._facets))),
        state(P.state & (pol_state_empty | 
		pol_state(1<<NOTEMPTY | 1<<INVALID))) 
		/* keep EMPTY, NOTEMPTY, MINIMIZED, BOXUPDATED, INVALID */
{
}

Polytope &Polytope::operator=(const Polytope &P) {
   assert(!P.state[INVALID]);
   this->_dim = P._dim;
   this->_box = P._box;
   this->_facets = P.state[EMPTY] ? std::make_shared<CollectFacets>(_dim) :
                           std::make_shared<CollectFacets>(*(P._facets));
   this->_DDbuildF2V=nullptr;
   this->_DDbuildV2F=nullptr;
   this->state = P.state & (pol_state_empty | 
		pol_state(1<<NOTEMPTY | 1<<INVALID));
   return *this;
}

Polytope::Polytope(const std::vector<Vector> &vertices) : 
  Polytope()
{
   if (vertices.empty()) return;
   _dim=vertices[0].size();
   /* build the V2F form */
   _DDbuildV2F = std::make_unique<DDbuildV2F>(1,vertices[0]);
   for (Index i=1;i<(Index)vertices.size();i++) {
       _DDbuildV2F->add_vertex(i+1,vertices[i]);
   }
   /* get the CollectFacets */
   _facets=_DDbuildV2F->getFacets();
   _box = _facets->extractBox();
   _facets->encompass_vertices(vertices,_box,true);
   state = pol_state_init;
   _facets->renumber();
   _DDbuildV2F.reset(); /* encompass may have added constraints */
}   

Polytope::Polytope(const std::vector<IntervalVector> &vertices) : 
  Polytope()
{
   _dim=vertices[0].size();
   /* build the V2F form */
   _DDbuildV2F = std::make_unique<DDbuildV2F>(1,vertices[0].mid());
   for (Index i=1;i<(Index)vertices.size();i++) {
       _DDbuildV2F->add_vertex(i+1,vertices[i].mid());
   }
   /* get the CollectFacets */
   _facets=_DDbuildV2F->getFacets();
   _box = _facets->extractBox();
   _facets->encompass_vertices(vertices,_box,true);
   _facets->renumber();
   state = pol_state_init;
   _DDbuildV2F.reset(); /* encompass may have added constraints */
}   

Polytope::Polytope(const std::vector<IntervalVector> &vertices,
		const CollectFacets &facetsform) :  Polytope()
{
  _dim = facetsform.getDim();
  if (_dim==-1) return;
  _facets=std::make_shared<CollectFacets>(facetsform);
  _box = IntervalVector::constant(_dim,Interval::empty());
  if (vertices.empty()) {
     state = pol_state_empty;
     return;
  }
  for (Index i=0;i<(Index)vertices.size();i++) {
      _box |= vertices[i];
  }
  _facets->encompass_vertices(vertices,_box,true);
  state = pol_state_init;
}


Polytope::Polytope(const IntervalVector &box, 
		const std::vector<std::pair<Row,double>> &facets,
		bool minimize) : Polytope(box) {
   for (const std::pair<Row,double> &cst : facets) {
      this->add_constraint(cst);
   }
   state = 0;
   if (minimize) this->minimize_constraints();
}



Polytope::Polytope(const Parallelepiped &par) :
	Polytope(par.box()) {
   /* we can use the vertices, or inverse the shape,
      we consider the inversion */
   IntervalMatrix u = inverse_enclosure(par.A);
   for (Index i=0;i<u.rows();i++) {
      this->add_constraint_band(IntervalRow(u.row(i)),
			u.row(i).dot(par.z)+Interval(-1.0,1.0),0.0);
   }   
   IntervalVector range = IntervalVector::constant(par.A.cols(),
					Interval(-1.0,1.0));
   _box = par.z+par.A*range; /* FIXME : use box */
}

Polytope::Polytope(const Zonotope &zon) :
	Polytope() {
   _dim=zon.z.size();
   /* we use the vertices (note : can do better) */
   IntervalVector v = IntervalVector::constant(zon.A.cols(),-1.0);
   this->_DDbuildV2F = std::make_unique<DDbuildV2F>(1,(zon.z+zon.A*v).mid());
   Index idV = 2;
   Index a = 0;
   while (a<v.size()) {
     while (a<v.size() && v[a]==1.0) {
       v[a]=-1.0;
       a++;
     } 
     if (a<v.size()) {
       v[a]=1.0; a=0;
       this->_DDbuildV2F->add_vertex(idV++,IntervalVector(zon.z+zon.A*v));
     }
   }
   /* get the CollectFacets */
   _facets=_DDbuildV2F->getFacets();
   IntervalVector range = IntervalVector::constant(zon.A.cols(),
					Interval(-1.0,1.0));
   _box = zon.z+zon.A*range; /* FIXME : use box */
   _facets->encompass_zonotope(IntervalVector(zon.z), 
		IntervalMatrix(zon.A), range, true);
   _DDbuildV2F.reset(); /* encompass may have added constraints */
   state = pol_state_init;
}

Polytope::Polytope(CollectFacets &&facets) : _dim(facets.getDim()), state(0) {
   _box = facets.extractBox();
   _facets = std::make_shared<CollectFacets>(std::move(facets)); 
       /* include renumbering if needed */
   state = 0;
}

Polytope::Polytope(IntervalVector &&box,
		CollectFacets &&facets) : _dim(facets.getDim()), 
				_box(std::move(box)), state(0) {
   _box &= facets.extractBox();
   _facets = std::make_shared<CollectFacets>(std::move(facets)); 
}


Polytope Polytope::from_ineFile(const char *filename) {
   std::shared_ptr<CollectFacets> fcts
	= std::make_shared<CollectFacets>(read_ineFile(filename));
   if (fcts->getDim()==-1) return Polytope();
   Polytope p(fcts->getDim(),false);
   p._facets = fcts;
   p.state = 0;
   return p;
}

Polytope Polytope::from_extFile(const char *filename) {
   std::vector<Vector> vts = read_extFile(filename);
   return Polytope(vts);
}


Interval Polytope::fast_bound(const FacetBase &base) const {
   if (base.is_null()) return Interval::zero();
   if (state[EMPTY]) return Interval();
   if (is_box()) return base.row.dot(_box).ub();
   Index gdim = base.gt_dim();
   Interval res;
   if (base.row[gdim]>0.0) {
      res= Interval(_box[gdim].ub())*base.row[gdim];
   } else {
      res= Interval(_box[gdim].lb())*base.row[gdim];
   }
   if (!base.is_coord()) {
      Row bcopy = base.row;
      bcopy[gdim]=0.0;
      res += bcopy.dot(_box);
      if (_facets->get_map().empty()) return res;
      std::pair<CollectFacets::mapCIterator,CollectFacets::mapCIterator>
         pIt = _facets->get_map().equal_range(base);
      if (pIt.first!=pIt.second) { /* we found an exact key */
         return Interval(pIt.first->second.rhs);
      } else {
         if (pIt.first!=_facets->get_map().begin()) --pIt.first;
         Interval a1 = Facet_::bound_linear_form(*(pIt.first), base.row, _box);
         if (pIt.second!=_facets->endFacet()) {
             Interval a2 = Facet_::bound_linear_form(*(pIt.second), base.row, _box);
             if (a2.ub()<a1.ub()) a1=a2;
         }
         if (a1.ub()<res.ub()) res=a1;
      }
      /* check the equalities ? */
      for (Index i=0;i<_facets->nbeqfcts();i++) {
         CollectFacets::mapIterator it = _facets->get_eqFacet(i);
         Interval a = Facet_::bound_linear_form(*it, base.row, _box);
         if (a.ub()<res.ub()) res=a;
      }
   }
   /* check the vertices if _buildF2V is up-to-date */
   if (state[F2VFORM]) {
       const std::forward_list<DDvertex> &vertices=_DDbuildF2V->get_vertices();
       for (const DDvertex &vt : vertices) {
            IntervalVector vtB = _DDbuildF2V->compute_vertex(vt.vertex);
            Interval a = base.row.dot(vtB);
            if (a.ub()<res.ub()) res=a;
       }
   }
   return res;
}

double Polytope::bound_row(const Row &r) const {
   assert(r.size()==_dim);
   if (state[EMPTY]) return -oo;
   if (is_box()) return r.dot(_box).ub();
   this->build_DDbuildF2V();
   double a = -oo;
   if (state[EMPTY]) return a;
   const std::forward_list<DDvertex> &vertices=_DDbuildF2V->get_vertices();
   for (const DDvertex &vt : vertices) {
        IntervalVector vtB = _DDbuildF2V->compute_vertex(vt.vertex);
        Interval b = r.dot(vtB);
        a = std::max(a,b.ub());
   }
   return a;
}

void Polytope::build_DDbuildF2V() const {
   if (state[F2VFORM]) return;
   if (state[EMPTY]) { 
        _DDbuildF2V=nullptr;
        return;
   }
   /* TODO : insert box progressively ? */
   _DDbuildF2V = std::make_unique<DDbuildF2V>(_dim,_box,_facets,true);
   for (CollectFacets::mapCIterator it = _facets->get_map().begin();
		it!=_facets->get_map().end(); ++it) {
      if ((*it).second.eqcst) continue;
      _DDbuildF2V->add_facet(it);
      if (_DDbuildF2V->is_empty()) { 
         this->set_empty_private();
         return;
      }
      state[NOTEMPTY]=true;
   }
   state[F2VFORM]=true;
}

void Polytope::minimize_constraints() const { 
   if (state[MINIMIZED]) return;
   if (state[EMPTY]) return;
   this->build_DDbuildF2V();
   if (state[EMPTY]) return;
   std::set<Index> redundant = _DDbuildF2V->redundantFacets();
   _box &= _DDbuildF2V->build_bbox();
   state[BOXUPDATED]=true;
   bool changed=false;
   for (const Index &a : redundant) {
       if (a<=0) continue;
       _facets->removeFacetById(a);
       changed=true;
   }
   if (changed) {
      std::vector<Index> corresp = _facets->renumber();
      _DDbuildF2V->update_renumber(corresp);
      state[V2FFORM]=false;
   }
   state[MINIMIZED]=true;
}

void Polytope::update_box() const {
   if (state[BOXUPDATED]) return;
   if (state[EMPTY]) return;
   if (is_box()) { state[BOXUPDATED]=true; return; }
   assert_release(!state[BOXUPDATED]);
   this->build_DDbuildF2V();
   if (state[EMPTY]) return;
   _box &= _DDbuildF2V->build_bbox();
   state[BOXUPDATED]=true;
}

bool Polytope::check_empty() const {
   if (state[NOTEMPTY]) return false;
   if (state[EMPTY]) return true;
   if (is_box()) { 
	if (_box.is_empty()) { 
	  state[EMPTY]=true; 
	  return true; 
        } else { 
	  state[NOTEMPTY]=true; 
	  return false; 
        } 
   }
   this->build_DDbuildF2V();
   return state[EMPTY];
}

BoolInterval Polytope::contains(const IntervalVector& p) const {
   assert_release(p.size()==_dim);
   if (p.is_empty()) return BoolInterval::TRUE;
   if (state[EMPTY]) return BoolInterval::FALSE;
   if (!p.is_subset(_box)) return BoolInterval::FALSE;
   BoolInterval r = BoolInterval::TRUE;
   for (const auto &fct : _facets->get_map()) {
       Facet_::polytope_inclrel pincl = 
		Facet_::relation_Box(fct,p);
       if (pincl[Facet_::NOTINCLUDE]) return BoolInterval::FALSE;
       if (pincl[Facet_::MAYINCLUDE]) r = BoolInterval::UNKNOWN;
   }
   return r;
}

BoolInterval Polytope::interior_contains(const IntervalVector& p) const {
   assert_release(p.size()==_dim);
   if (p.is_empty()) return BoolInterval::TRUE;
   if (state[EMPTY]) return BoolInterval::FALSE;
   if (!p.is_interior_subset(_box)) return BoolInterval::FALSE;
   BoolInterval r = BoolInterval::TRUE;
   for (const auto &fct : _facets->get_map()) {
       Facet_::polytope_inclrel pincl = 
		Facet_::relation_Box(fct,p,true);
       if (pincl[Facet_::NOTINCLUDE]) return BoolInterval::FALSE;
       if (pincl[Facet_::MAYINCLUDE]) r = BoolInterval::UNKNOWN;
   }
   return r;
}

bool Polytope::box_is_subset(const IntervalVector& x) const {
   return this->box().is_subset(x);
}

bool Polytope::is_subset(const Polytope& P)
	 const {
   const IntervalVector &b2 = P.box(true);
   const IntervalVector &b1 = this->box(true);
   if (!b1.is_subset(b2)) return false;
   for (const auto &fctP : P._facets->get_map()) {
       if (this->fast_bound(fctP.first).ub() <= fctP.second.rhs) continue;
       double l1 = this->bound_row(fctP.first.row);
       if (l1<=fctP.second.rhs) continue;
       return false;
   }
   return true;
}

bool Polytope::is_superset(const Polytope& P)
	 const {
   return P.is_subset((*this));
}

bool Polytope::is_interior_subset(const Polytope& P)
	 const {
   const IntervalVector &b2 = P.box(true);
   const IntervalVector &b1 = this->box(true);
   if (!b1.is_interior_subset(b2)) return false;
   for (const auto &fctP : P._facets->get_map()) {
       if (this->fast_bound(fctP.first).ub() < fctP.second.rhs) continue;
       double l1 = this->bound_row(fctP.first.row);
       if (l1<fctP.second.rhs) continue;
       return false;
   }
   return true;
}

bool Polytope::intersects(const IntervalVector& p) const {
   assert(!status[INVALID]);
   const IntervalVector &b1 = this->box(true);
   if (!b1.intersects(p)) return false;
   Polytope q(*this); /* TODO : copy DDbuildF2V */
   q.meet_with_box(p);
   return !(q.check_empty());
}

bool Polytope::intersects(const Polytope& p) const {
   assert(!status[INVALID]);
   const IntervalVector &b1 = this->box();
   if (!b1.intersects(p.box())) return false;
   Polytope q(*this); /* TODO : copy DDbuildF2V */
   q.meet_with_polytope(p);
   return !(q.check_empty());
}

bool Polytope::is_disjoint(const Polytope& p) const {
   return (!this->intersects(p));
}

void Polytope::contract_out_Box(IntervalVector &b) const {
   assert(!status[INVALID]);
   const IntervalVector &b1 = this->_box;
   /* we can compute the diff, but as the goal is to return one interval vector,
      we do it "by hand" to avoid the generation of too many elements.
      also, we quit as soon a the vector is "full" */
   IntervalVector result = IntervalVector::constant(_dim,Interval::empty());
   for (Index i=0;i<_dim;i++) {
      std::vector<Interval> vt = b[i].diff(b1[i]);
      if (vt.empty()) continue;
      IntervalVector r1(b);
      r1[i] = vt[0];
      if (vt.size()>1) r1[i] |= vt[1];
      if (r1[i]==b[i]) return;
      result |= r1;
      if (b.is_subset(result)) return;
   }
   for (auto &facet : _facets->get_map()) {
       IntervalVector r1(b);
       Facet_::contract_out_Box(facet,r1);
       result |= r1;
       if (b.is_subset(result)) return;
   }
   b = result;
}

#if 0

polytope_inclrel Polytope::relation_Box(const IntervalVector& p) const {
   assert_release(_dim>=0);
   if (p.is_empty()) return (1<<INCLUDES) | (1<<DISJOINT);
   if (_empty) return (1<<NOTINCLUDE) | (1<<DISJOINT);
   if (!_clpForm) return (1<<INCLUDES) | (1<<INTERSECTS);
   /* to get INCLUDES, must have INCLUDES for all 
      to get NOTINCLUDE, must have NOTINCLUDE once
      to get INTERSECTS, must have INCLUDES for all but one, and INTERSECTS
                         for this one 
      to get DISJOINT, must have DISJOINT once */
   bool notinclude=false;
   bool allinclude=true;
   bool intersects=true;
   const std::vector<Facet>& facets = _clpForm->getFacets();
   for (const Facet &facet : facets) {
       polytope_inclrel res2 = facet.relation_Box(p);
       if (res2[DISJOINT]) return (1<<NOTINCLUDE) | (1<<DISJOINT);
       if (res2[INCLUDES]) continue;
       if (allinclude && res2[INTERSECTS]) { allinclude=false; }
       else { allinclude=false; intersects=false; }
       if (res2[NOTINCLUDE]) notinclude=true;
   }
   return (allinclude ? (1<<INCLUDES | 1<<INTERSECTS)
	: ((intersects ? 1<<INTERSECTS : 1<<MAYINTERSECT) |
           (notinclude ? 1<<NOTINCLUDE : 1<<MAYINCLUDE)));
}

BoolInterval Polytope::contains(const IntervalVector& p) const {
   polytope_inclrel r = this->relation_Box(p);
   return (r[INCLUDES] ? BoolInterval::TRUE : 
		(r[NOTINCLUDE] ? BoolInterval::FALSE : BoolInterval::UNKNOWN));
}

BoolInterval Polytope::intersects(const IntervalVector& p) const {
   polytope_inclrel r = this->relation_Box(p);
   return (r[INTERSECTS] ? BoolInterval::TRUE : 
		(r[DISJOINT] ? BoolInterval::FALSE : BoolInterval::UNKNOWN));
}

const Interval& Polytope::operator[](Index i) const {
   assert_release(i<=_dim);
   return _box[i];
}

double Polytope::;istance_cst(const Facet &fc) const {
   assert_release(_dim==fc.row.size());
   if (_empty) return -oo;
   if (!_clpForm) return (fc.row.dot(_box)-fc.rhs).ub();
   _clpForm->setObjective(fc.row);
   LPclp::lp_result_stat stat = _clpForm->solve(false,0);
   if (stat[LPclp::EMPTY]) return -oo;
   return (_clpForm->getValobj()-fc.rhs).ub();
}

#endif

Vector Polytope::mid() const {
   assert_release(!state[INVALID]);
   this->update_box();
   return (_box.mid());
}

Vector Polytope::mid_in() const {
   assert_release(!state[INVALID]);
   if (this->is_box()) return (_box.mid());
   std::vector<IntervalVector> vt = this->vertices();
   if (vt.empty()) return _box.mid();
   Vector ret = vt[0].mid();
   for (Index i=1;i<(Index)vt.size();i++) 
      ret += vt[i].mid();
   return ret/vt.size();
}

void Polytope::clear() {
   assert_release(!state[INVALID]);
   state=pol_state_init;
   _box = IntervalVector::Zero(_dim);
   _facets=nullptr;
   _DDbuildF2V=nullptr;
   _DDbuildV2F=nullptr;
}


bool Polytope::add_constraint(const std::pair<Row,double>& facet, 
	double tolerance) {
   if (state[EMPTY]) return false;
   FacetBase base = FacetBase(facet.first);
   if (base.is_null()) {
      if (facet.second>=tolerance) return false;
      this->set_empty(); return true;
   }
   Interval act = this->fast_bound(base);
   if (act.ub()<=facet.second+tolerance) return false;
   if (base.is_coord()) {
      Index gdim = base.gt_dim();
      Interval val = Interval(facet.second)/facet.first[gdim];
      if (facet.first[gdim]>0.0) {
         _box[gdim] &= Interval(-oo,val.ub());
         if (_box[gdim].is_empty()) { 
	   this->set_empty_private(); 
           return true;
	 }
#if 0
         if (state[CLPFORM]) {
            this->_clpForm->set_bbox(_box);
         }
#endif
         if (state[F2VFORM]) {
           int ret = _DDbuildF2V->add_bound_var(gdim,true,val.ub());
           if (ret==-1) { 
		this->set_empty_private();
                return true;
           }
           if (ret==0) return false;
         }
      } else {
         _box[gdim] &= Interval(-val.ub(),+oo);
         if (_box[gdim].is_empty()) { 
	   this->set_empty_private(); 
           return true;
	 }
#if 0
         if (state[CLPFORM]) {
            this->_clpForm->set_bbox(_box);
         }
#endif
         if (state[F2VFORM]) {
           int ret = _DDbuildF2V->add_bound_var(gdim,false,-val.ub());
           if (ret==-1) { 
		this->set_empty_private();
                return true;
           }
           if (ret==0) return false;
         }
      }
      state[V2FFORM]=false;
      state[MINIMIZED]=false;
      state[NOTEMPTY]=false;
      return true; 
   }
   auto res= _facets->insert_facet(facet.first, facet.second, 
		false, CollectFacets::MIN_RHS);
   if (res.first==-1) { this->set_empty_private(); return true; }
   if (res.first==0) return false;
#if 0
   if (state[CLPFORM]) {
      this->_clpForm->updateConstraint(res.first);
   }
#endif
   if (state[F2VFORM]) {
      int ret = this->_DDbuildF2V->add_facet((*_facets)[res.first-1]);
      if (ret==-1) {
	this->set_empty_private();
        return true;
      }
      if (ret==0) return false;
   }
   state[V2FFORM]=state[MINIMIZED]=state[BOXUPDATED]=false;
   state[NOTEMPTY]=false;
   return true;
}

bool Polytope::add_constraint(const IntervalRow &cst, double rhs,
	double tolerance) {
   if (state[EMPTY]) return false;
   Row cstmid = cst.mid();
   IntervalRow rem = cst-cstmid;
   Interval d = rhs+rem.dot(_box);
   if (d.ub()==+oo) return false;
   return  this->add_constraint(std::pair(cstmid, d.ub()), tolerance);
}

std::pair<bool,bool> Polytope::add_constraint_band(const IntervalRow &cst,
	 const Interval &rhs, double tolerance) {
   if (state[EMPTY]) return { false, false };
   Row cstmid = cst.mid();
   IntervalRow rem = cst-cstmid;
   Interval d = rhs+rem.dot(_box);
   return this->add_constraint_band(cstmid,d,tolerance);
}
     
std::pair<bool,bool> Polytope::add_constraint_band(const Row &cst,
	 const Interval &rhs, double tolerance) {
   if (state[EMPTY]) return { false, false };
   bool rub, rlb;
   if (rhs.ub()==+oo) rub=false;
   else rub = this->add_constraint(std::pair(cst, rhs.ub()), tolerance);
   if (rhs.lb()==-oo) rlb=false;
   else rlb = this->add_constraint(std::pair(-cst, -rhs.lb()), tolerance);
   return { rlb, rub };
}
     
int Polytope::add_equality(const std::pair<Row,double>& facet) {
   if (state[EMPTY]) return -1;
   FacetBase base(facet.first);
   if (base.is_null()) {
      if (facet.second==0.0) return 0;
      this->set_empty(); return -1;
   }
   if (base.is_coord()) {
      Index gdim = base.gt_dim();
      Interval val = Interval(facet.second)/facet.first[gdim];
      _box[gdim] &= val;
      if (_box[gdim].is_empty()) { 
	   this->set_empty_private(); 
           return -1;
      }
#if 0
      if (state[CLPFORM]) {
          this->_clpForm->set_bbox(_box);
      }
#endif
      state[F2VFORM]=false;
      state[V2FFORM]=false;
      state[MINIMIZED]=false;
      state[NOTEMPTY]=false;
      return 1;
   }
   FacetBase negBase(base);
   negBase.negate_row();
   Interval actub = this->fast_bound(base);
   Interval actlb = this->fast_bound(negBase);
   if (actub.ub()<facet.second) { this->set_empty(); return -1; }
   if (actlb.ub()<-facet.second) { this->set_empty(); return -1; }
   auto res= _facets->insert_facet(facet.first, facet.second, 
		true, CollectFacets::MIN_RHS);
   if (res.first==-1) { this->set_empty_private(); return -1; }
   if (res.first==0) return 0;
#if 0
   if (state[CLPFORM]) {
      this->_clpForm->updateConstraint(res.first);
   }
#endif
   state[F2VFORM]=false;
   state[V2FFORM]=state[MINIMIZED]=state[BOXUPDATED]=false;
   state[NOTEMPTY]=false;
   return 1;
}

int Polytope::meet_with_box(const IntervalVector &b) {
   assert(!state[INVALID]);
   if (_box.is_subset(b)) return 0;
   if (_box.is_disjoint(b)) { this->set_empty(); return -1; }
   _box &= b;
#if 0
   if (state[CLPFORM]) {
       this->_clpForm->set_bbox(_box); 
   }
#endif
   if (state[F2VFORM]) {
      if (!b.is_degenerated()) {
          int ret = this->_DDbuildF2V->add_constraint_box(b); 
          if (ret==-1) { 
    	    this->set_empty();
            return -1;
          }
          if (ret==0) return 0;
      } else { /* we do not keep the representation */
          state[F2VFORM]=false;
          _DDbuildV2F=nullptr;
      }
   }
   state[V2FFORM]=state[MINIMIZED]=state[BOXUPDATED]=false;
   state[NOTEMPTY]=false;
   return 1;
}

Polytope &Polytope::operator&=(const IntervalVector &b) {
   this->meet_with_box(b);
   return (*this);
}

Polytope Polytope::meet_with_hyperplane(Index dm, double x) const {
   assert(!state[INVALID]);
   if (!_box[dm].contains(x)) { return Polytope(this->_dim,true); }
   IntervalVector nbox(_box);
   nbox[dm]=x;
   CollectFacets cf(_dim);
   for (auto &facet : _facets->get_map()) {
      Row r(facet.first.row);
      Interval rhs = facet.second.rhs - nbox[dm]*r[dm];
      r[dm]=0.0;
      if (!facet.second.eqcst || rhs.is_degenerated()) {
         std::pair<Index,bool> ret =
		 cf.insert_facet(std::move(r),rhs.ub(),facet.second.eqcst,
				CollectFacets::MIN_RHS);
         if (ret.first==-1) return Polytope(this->_dim, true);
      } else {
         std::pair<Index,bool> ret =
		 cf.insert_facet(r,rhs.ub(),false,
				CollectFacets::MIN_RHS);
         if (ret.first==-1) return Polytope(this->_dim, true);
         ret = cf.insert_facet(-r,-rhs.lb(),false,
				CollectFacets::MIN_RHS);
         if (ret.first==-1) return Polytope(this->_dim, true);
      } 
   }  
   return Polytope(std::move(nbox),std::move(cf));
}
   
int Polytope::meet_with_polytope(const Polytope &P) {
   assert_release(!P.state[INVALID]);
   assert_release(!state[INVALID]);
   if (state[EMPTY]) return -1;
   if (P.state[EMPTY]) { this->set_empty(); return -1; }
   if (P._facets->nbfcts()==0) { return this->meet_with_box(P._box); }
   int ret=0;
   if (!_box.is_subset(P._box)) {
     _box &= P._box;
     ret=1;
     if (_box.is_empty()) { this->set_empty(); return -1; }
   }
   int ret2= this->_facets->merge_CollectFacets(*(P._facets));
   if (ret2==-1) { this->set_empty(); return -1; }
   if (ret2==0 && ret==0) return 0;
   state[V2FFORM]=state[MINIMIZED]=state[BOXUPDATED]=false;
   state[NOTEMPTY]=false;
   state[F2VFORM]=false;
#if 0
   state[CLPFORM]=false;
#endif
   return 1;
}

Polytope &Polytope::homothety(const IntervalVector &c, double delta) {
   assert_release(!state[INVALID]);
   assert(delta>0);
   Interval deltaI (delta); /* for interval computation */
   if (state[EMPTY]) return (*this);
   _box = (1.0-deltaI) * c + delta * _box;
#if 0
   if (state[CLPFORM]) {
       this->_clpForm->set_bbox(_box);
   }
#endif
   for (CollectFacets::mapIterator it = _facets->beginFacet() ;
		it != _facets->endFacet(); ++it) {
      if (it->second.eqcst) continue;
      Interval nrhs = deltaI*it->second.rhs + 
			(1.0-deltaI)*it->first.row.dot(c);
      _facets->change_ineqFacet_rhs(it,nrhs.ub());
   }
   for (Index i=_facets->nbeqfcts()-1;i>=0;i--) {
       CollectFacets::mapIterator it = _facets->get_eqFacet(i);
       Interval nrhs = deltaI*it->second.rhs +
                        (1.0-deltaI)*it->first.row.dot(c);
       _facets->change_ineqFacet_rhs(it,nrhs.ub());
       if (!nrhs.is_degenerated()) {
          _facets->dissociate_eqFacet(i,nrhs.lb());
       }
   }
   state[V2FFORM]=false;
   state[MINIMIZED]=false;
   state[F2VFORM]=false;
#if 0
   state[CLPFORM]=false; /* TODO : possible if c is punctual */
#endif
   return (*this);
}

Polytope Polytope::reverse_affine_transform(const IntervalMatrix &M,
	const IntervalVector &P, const IntervalVector &bbox) const {
   assert_release(!state[INVALID]);
   assert_release(M.rows()==_dim);
   assert_release(bbox.size()==M.cols());
   assert_release(P.size()==_dim);
   if (state[EMPTY]) return Polytope(M.cols(),true);
   CollectFacets cf(M.cols());
   /* first the box */
   for (Index i=0;i<_dim;i++) {
      IntervalRow rI = M.row(i);
      Interval rhs = _box[i] - P[i];
      Row r = rI.mid();
      rI = rI - r;
      rhs -= rI.dot(bbox);
      if (rhs.is_degenerated()) {
         if (rhs.ub()==+oo) continue;
         std::pair<Index,bool> ret =
		 cf.insert_facet(std::move(r),rhs.ub(),true,
				CollectFacets::MIN_RHS);
         if (ret.first==-1) return Polytope(M.cols(), true);
      } else {
         if (rhs.ub()!=+oo) {
           std::pair<Index,bool> ret =
		 cf.insert_facet(r,rhs.ub(),false,
				CollectFacets::MIN_RHS);
           if (ret.first==-1) return Polytope(M.cols(), true);
         }
         if (rhs.lb()!=-oo) {
            std::pair<Index,bool> ret =
                cf.insert_facet(-r,-rhs.lb(),false,
				CollectFacets::MIN_RHS);
            if (ret.first==-1) return Polytope(M.cols(), true);
         }
      }
   }
   for (auto &facet : _facets->get_map()) {
      IntervalRow rI = facet.first.row * M;
      Interval rhs = facet.second.rhs - facet.first.row.dot(P);
      Row r = rI.mid();
      rI = rI - r;
      rhs -= rI.dot(bbox);
      if (!facet.second.eqcst || rhs.is_degenerated()) {
         if (rhs.ub()==+oo) continue;
         std::pair<Index,bool> ret =
		 cf.insert_facet(std::move(r),rhs.ub(),facet.second.eqcst,
				CollectFacets::MIN_RHS);
         if (ret.first==-1) return Polytope(M.cols(), true);
      } else {
         if (rhs.ub()!=+oo) {
            std::pair<Index,bool> ret =
		 cf.insert_facet(r,rhs.ub(),false,
				CollectFacets::MIN_RHS);
            if (ret.first==-1) return Polytope(M.cols(), true);
         }
         if (rhs.lb()!=-oo) {
            std::pair<Index,bool> ret =
 		cf.insert_facet(-r,-rhs.lb(),false,
				CollectFacets::MIN_RHS);
            if (ret.first==-1) return Polytope(M.cols(), true);
         }
      } 
   }  
   return Polytope(IntervalVector(bbox),std::move(cf));
}

Polytope Polytope::affine_transform(const IntervalMatrix &M,
        const IntervalVector &P, const IntervalVector &B,
	bool use_direct) const {
   assert_release(!state[INVALID]);
   if (state[EMPTY]) return Polytope(_dim,true);
   assert_release(M.cols()==_dim);
   assert_release(M.rows()==P.size());
   assert_release(B.size()==P.size());
   /* x' \in [M] x + [P]
         becomes x' \in Mmid x + ([P] + ([M]-Mmid) B) */
   Matrix Mmid = M.mid();
   IntervalVector P2 = P + (M - Mmid)*_box;
   if (Mmid.rows()==_dim) {
      IntervalMatrix Minv = inverse_enclosure(Mmid);
      if (!Minv.is_unbounded()) {
        IntervalVector M2 = B & (M*_box+P);
	if (!use_direct) 
           return this->reverse_affine_transform(Minv,-Minv*P2,M2);
	else {
           Polytope R = this->reverse_affine_transform(Minv,-Minv*P2,M2);
	   return (R &= this->direct_affine_transform(M,P));
        }
      }
   } else if (M.rows()>_dim) {
      IntvFullPivLU LUdec(Matrix(Mmid.transpose())); 
		/* we use the tranpose for solve as we did not define
                   leftSolve */
      if (LUdec.is_surjective()==BoolInterval::TRUE) {
         IntervalVector M2 = B & (M*_box+P);
         IntervalMatrix inv = 
		LUdec.solve(IntervalMatrix::Identity(_dim,_dim));
         IntervalMatrix leftInv = inv.transpose();
         Polytope R = this->reverse_affine_transform(leftInv,-leftInv*P2,M2);
	 IntervalMatrix leftKer = LUdec.kernel().transpose();
	 for (Index k = 0; k<leftKer.rows(); k++) {
             IntervalRow r = leftKer.row(k);
             Interval u = r.dot(P2);
             if (r.is_degenerated() && u.is_degenerated()) 
		R.add_equality(
			std::pair<Row,double>(r.mid(),u.mid()));
	     else R.add_constraint_band(r,u);
         }
	if (!use_direct) 
           return R;
	else 
	   return (R &= this->direct_affine_transform(M,P));
      }
   }
   Polytope R =  this->direct_affine_transform(M,P);   
   R.meet_with_box(B);
   return R;
}

Polytope Polytope::direct_affine_transform(const IntervalMatrix &M,
        const IntervalVector &P) const {
   assert_release(!state[INVALID]);
   if (state[EMPTY]) return Polytope(_dim,true);
   this->build_DDbuildF2V();
   IntervalVector nBox = M*this->box()+P;
   std::vector<IntervalVector> resultat;
   if (state[EMPTY]) return Polytope(*this);
   const std::forward_list<DDvertex> &vertices=_DDbuildF2V->get_vertices();
   for (const DDvertex &vt : vertices) {
        IntervalVector a = M*_DDbuildF2V->compute_vertex(vt.vertex)+P;
        resultat.push_back(a);
   }
   Polytope res(resultat);
   return (res &= nBox);
}

Polytope Polytope::time_elapse(const IntervalVector &P,
        const Interval &T) const {
   this->build_DDbuildF2V();
   IntervalVector nBox = this->box()+T*P;
   std::vector<IntervalVector> resultat;
   if (state[EMPTY]) return Polytope(*this);
   const std::forward_list<DDvertex> &vertices=_DDbuildF2V->get_vertices();
   for (const DDvertex &vt : vertices) {
        IntervalVector vert = _DDbuildF2V->compute_vertex(vt.vertex);
        resultat.push_back(vert+T.lb()*P);
        resultat.push_back(vert+T.ub()*P);
   }
   Polytope res(resultat);
   return (res &= nBox);
}

Polytope &Polytope::operator&=(const Polytope &P) {
   this->meet_with_polytope(P);
   return (*this);
}

Polytope Polytope::union_of_polytopes(std::initializer_list<Polytope> lst) {
   if (lst.size()==0) return Polytope();
   if (lst.size()==1) return Polytope(*(lst.begin()));
   Index dim = lst.begin()->dim();
   IntervalVector box = IntervalVector::constant(dim,Interval::empty());
   std::vector<IntervalVector> lvert;
   for (auto &P : lst) {
      std::vector<IntervalVector> tmp = 
		P.vertices();
      lvert.insert(lvert.end(),tmp.begin(),tmp.end());
      box |= P.box();
   }
   if (lvert.size()==0) {
       return Polytope(dim,true);
   }
   Polytope ret= Polytope(lvert);
   return (ret &= box);
}

Polytope Polytope::union_of_polytopes(const std::vector<Polytope> &lst) {
   if (lst.size()==0) return Polytope();
   if (lst.size()==1) return Polytope(*(lst.begin()));
   Index dim = lst.begin()->dim();
   IntervalVector box = IntervalVector::constant(dim,Interval::empty());
   std::vector<IntervalVector> lvert;
   for (auto &P : lst) {
      std::vector<IntervalVector> tmp = 
		P.vertices();
      lvert.insert(lvert.end(),tmp.begin(),tmp.end());
      box |= P.box();
   }
   if (lvert.size()==0) {
       return Polytope(dim,true);
   }
   Polytope ret= Polytope(lvert);
   return (ret &= box);
}

int Polytope::join_with_polytope(const Polytope &P) {
   assert(!state[INVALID]);
   assert(!P.state[INVALID]);
   if (P.state[EMPTY]) return 0;
   if (state[EMPTY]) { *this=P; return 1; }
   *this=union_of_polytopes( { *this, P } );
   return 1;
}

Polytope &Polytope::operator|=(const Polytope &P) {
   assert(!state[INVALID]);
   assert(!P.state[INVALID]);
   if (P.state[EMPTY]) return *this;
   if (state[EMPTY]) { *this=P; return *this; }
   *this=union_of_polytopes( { *this, P } );
   return *this;
}

Polytope &Polytope::inflate(double rad) {
   assert(!state[INVALID]);
   return (this->inflate(IntervalVector::constant(_dim,Interval(-rad,rad))));
}

Polytope &Polytope::inflate_ball(double rad) {
   assert(!state[INVALID]);
   if (state[EMPTY]) return *this;
   if (rad<=0.0) return *this;
   _box.inflate(rad);
   /* first, we consider the inequality facets */
   for (CollectFacets::mapIterator it = _facets->beginFacet() ;
		it != _facets->endFacet(); ++it) {
      if (it->second.eqcst) continue;
      double nrm = (it->first.row).norm()*rad;
      double nrhs = it->second.rhs+nrm;
      _facets->change_ineqFacet_rhs(it,nrhs);
   }
   /* now, we consider the equality facets */
   /* decreasing order because we "destroy" the eqFacet array */
   for (Index i=_facets->nbeqfcts()-1;i>=0;i--) {
       CollectFacets::mapIterator it = _facets->get_eqFacet(i);
       double nrm = (it->first.row).norm()*rad;
       Interval rhs2 = it->second.rhs+Interval(-nrm,nrm);
       _facets->change_ineqFacet_rhs(it,rhs2.ub());
       _facets->dissociate_eqFacet(i,rhs2.lb());
   }
   /* TODO : update */
   state[V2FFORM]=false;
   state[F2VFORM]=false;
   _DDbuildF2V=nullptr;
   _DDbuildV2F=nullptr;
#if 0
   state[CLPFORM]=false;
   _clpForm=nullptr;
#endif
   return *this;
}

Polytope &Polytope::inflate(const IntervalVector& box) {
   assert(!state[INVALID]);
   if (state[EMPTY]) return *this;
   if (box.is_empty()) { this->set_empty(); return (*this); }
   _box += box;
   /* first, we consider the inequality facets */
   for (CollectFacets::mapIterator it = _facets->beginFacet() ;
		it != _facets->endFacet(); ++it) {
      if (it->second.eqcst) continue;
      double nrhs = (it->second.rhs+box.dot(it->first.row)).ub();
      _facets->change_ineqFacet_rhs(it,nrhs);
   }
   /* now, we consider the equality facets */
   /* decreasing order because we "destroy" the eqFacet array 
      however, some facets are kept as equalities if there is no
      expansion */
   for (Index i=_facets->nbeqfcts()-1;i>=0;i--) {
       CollectFacets::mapIterator it = _facets->get_eqFacet(i);
       Interval rhs2 = it->second.rhs+box.dot(it->first.row);
       _facets->change_ineqFacet_rhs(it,rhs2.ub());
       if (!rhs2.is_degenerated()) 
           _facets->dissociate_eqFacet(i,rhs2.lb());
   }
   /* TODO : update */
   state[V2FFORM]=false;
   state[F2VFORM]=false;
   _DDbuildF2V=nullptr;
   _DDbuildV2F=nullptr;
#if 0
   _clpForm=nullptr;
   state[CLPFORM]=false;
#endif
   return (*this);
}


Polytope &Polytope::unflat(Index dm, double rad) {
   assert(!state[INVALID]);
   if (state[EMPTY]) return *this;
   if (rad<=0.0) { return *this; }
   _box[dm].inflate(rad);
   /* first, we consider the inequality facets */
   Interval radI(-rad,rad); /* for interval computations */
   for (CollectFacets::mapIterator it = _facets->beginFacet() ;
		it != _facets->endFacet(); ++it) {
      if (it->second.eqcst) continue;
      if (it->first.row[dm]==0.0) continue;
      double nrhs = (it->second.rhs+radI*it->first.row[dm]).ub();
      _facets->change_ineqFacet_rhs(it,nrhs);
   }
   /* now, we consider the equality facets */
   /* decreasing order because we "destroy" the eqFacet array 
      however, some facets are kept as equalities if there is no
      expansion */
   for (Index i=_facets->nbeqfcts()-1;i>=0;i--) {
       CollectFacets::mapIterator it = _facets->get_eqFacet(i);
       if (it->first.row[dm]==0.0) continue;
       Interval rhs2 = it->second.rhs+radI*it->first.row[dm];
       _facets->change_ineqFacet_rhs(it,rhs2.ub());
       if (!rhs2.is_degenerated()) 
           _facets->dissociate_eqFacet(i,rhs2.lb());
   }
   /* TODO : update */
   state[V2FFORM]=false;
   state[F2VFORM]=false;
   _DDbuildF2V=nullptr;
   _DDbuildV2F=nullptr;
#if 0
   state[CLPFORM]=false;
   _clpForm=nullptr;
#endif
   return (*this);
}

Polytope operator+ (const Polytope &p1, const Polytope &p2) {
   assert(!p1.state[INVALID]);
   assert(!p2.state[INVALID]);
   if (p1.is_box() && p2.is_box()) return Polytope(p1.box()+p2.box());
   std::vector<IntervalVector> vt1 = p1.vertices();
   std::vector<IntervalVector> vt2 = p2.vertices();
   IntervalVector bres = p1.box() + p2.box();
   std::vector<IntervalVector> result;
   for (auto &v1 : vt1) 
     for (auto &v2 : vt2) 
       result.push_back(v1+v2);
   Polytope ret = Polytope(result);
   return (ret &= bres);
}
   
Polytope Polytope::operator-() const {
   assert_release(!state[INVALID]);
   if (state[EMPTY]) return Polytope(_dim,true);
   CollectFacets cf(_dim);
   IntervalVector bbox(-_box);
   for (auto &facet : _facets->get_map()) {
      FacetBase nf = facet.first;
      nf.negate_row();
      std::pair<Index,bool> ret =
		 cf.insert_facet(Facet_::make(nf,-facet.second.rhs,
					facet.second.eqcst),
				CollectFacets::MIN_RHS);
      if (ret.first==-1) return Polytope(_dim, true); /* should not happen */
   }  
   Polytope ret = Polytope(IntervalVector(bbox),std::move(cf));
   ret.state=this->state & pol_state_init; 
   return ret;
}

std::ostream& operator<<(std::ostream& os,
		const Polytope &P) {
   if (P.state[Polytope::EMPTY]) {
      os << "Polytope(" /* << P.state */ << " empty dim " << P._dim << ")" << std::endl;
      return os;
   }
   os << "Polytope(" /* << P.state */ << " bbox " << P._box << ") : " << std::endl;
   os << *P._facets;
   os << "EndPolytope" << std::endl;
   return os;
}

std::vector<IntervalVector> Polytope::vertices() const {
   assert(!state[INVALID]);
   if (this->is_box()) {
      if (_box.is_empty()) return std::vector<IntervalVector>();
      std::vector<IntervalVector> ret;
      Vector a = _box.lb();
      ret.push_back(IntervalVector(a));
      Index i=0;
      while (i<_box.size()) {
         if (_box[i].is_degenerated()) { i++; continue; }
         if (a[i]==_box[i].lb()) {
              a[i]=_box[i].ub();
              ret.push_back(IntervalVector(a));
              i=0; continue;
         }
         a[i]=_box[i].lb(); i++;
      }
      return ret;
   }
   this->build_DDbuildF2V();
   if (state[EMPTY]) return std::vector<IntervalVector>();
   std::vector<IntervalVector> ret;
   for (const DDvertex &vtx : _DDbuildF2V->get_vertices()) {
       ret.push_back(_DDbuildF2V->compute_vertex(vtx.vertex));
   }
   return ret;
}

std::vector<std::vector<Vector>> Polytope::vertices_3Dfacets() const {
   this->build_DDbuildF2V();
   if (state[EMPTY]) return std::vector<std::vector<Vector>>();
   return build_3Dfacets(*_DDbuildF2V);
} 

std::vector<Vector> Polytope::vertices_2Dfacet() const {
   this->build_DDbuildF2V();
   if (state[EMPTY]) return std::vector<Vector>();
   return build_2Dfacet(*_DDbuildF2V);
} 

}

