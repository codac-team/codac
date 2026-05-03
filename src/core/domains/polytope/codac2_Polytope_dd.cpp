/**
 * \file codac2_Polytope_dd.cpp implementation of the dd algorithm
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
#include <list>
#include <queue>
#include <utility>
#include <tuple>

#include "codac2_Index.h"
#include "codac2_Vector.h"
#include "codac2_Row.h"
#include "codac2_Facet.h" 
#include "codac2_Polytope_dd.h"
#include "codac2_IntvFullPivLU.h"
#include "codac2_PosetMax.h"

#undef DEBUG_CODAC2_DD

namespace codac2 {


/********************/
/*** DDbuildF2V *****/
/********************/

DDbuildF2V::DDbuildF2V(Index dim, const IntervalVector &box,
	std::shared_ptr<CollectFacets> facetsptr, bool include_box) : dim(dim),
		fdim(dim), bbox(box), facetsptr(facetsptr),
		empty(false), flat(false),
		nbIn(0) {
   int nb_eq_dim= (facetsptr==nullptr ? 0 : facetsptr->nbeqfcts());
   if (include_box) {
      for (int i=0;i<dim;i++) {
         if (bbox[i].is_degenerated()) nb_eq_dim++;
      }
   }
   if (nb_eq_dim>0) {
      /* managing the equalities */
      /* we first check the existence of a solution, and get a finite
         number of idependant equalities */
      Matrix MatEQ = Matrix::Zero(nb_eq_dim,dim);
      IntervalVector RhsEQ = 
			IntervalVector::Zero(nb_eq_dim);
      int base=0; 
      if (include_box) {
        for (int i=0;i<dim;i++) {
           if (!bbox[i].is_degenerated()) continue;
           MatEQ(base,i)=1.0;
           RhsEQ[base]=box[i];
           base++;
        }
      }
      if (facetsptr!=nullptr) {
        for (Index i=0;i<(Index)facetsptr->nbeqfcts();i++) {
            MatEQ.row(base+i) = facetsptr->get_eqFacet(i)->first.row;
            RhsEQ[base+i] = facetsptr->get_eqFacet(i)->second.rhs;
        }
      }
      IntvFullPivLU LUdec(MatEQ);
#if 0
      if (LUdec.is_surjective)!=BoolInterval::TRUE) {
         /* TODO : check emptiness */
         /* IntervalMatrix eqsolve = LUdec.solve(RhsEQ);
         if (eqsolve.is_empty()) { empty=true; return; }  */
			/* FIXME : NOT CORRECT, as solve DOES NOT
			   guarantee the emptiness of solutions. We need
			   to use e.g. a bounding box... wait for 
			   pull request */
      } else 
#endif
      {
         this->fdim = this->dim - nb_eq_dim;
         this->M_EQ = IntervalMatrix::Zero(this->dim,this->fdim+1);
         this->M_EQ.col(0) = LUdec.solve(RhsEQ);
         this->M_EQ.rightCols(this->fdim) = LUdec.kernel();
	 this->flat=true;
      }
#ifdef DEBUG_CODAC2_DD
      std::cout << "M_EQ : " << M_EQ << std::endl;
#endif
   } 
   /* initial vertice :
      (1 0 0 ... ), + rays */
   IntervalVector v = IntervalVector::Zero(this->fdim+1);
   v[0]=1.0;
   this->addDDvertex(v);
   for (int i=0;i<fdim;i++) {
       IntervalVector line = IntervalVector::Zero(this->fdim+1);
       line[i+1]=1.0;
       lines.push_back(line);
   }
   if (include_box) this->add_constraint_box(bbox);
}

int DDbuildF2V::add_bound_var(Index c, bool mx, double rhs) {
   if (empty) return -1;
   if (mx && rhs==+oo) return 0;
   if (!mx && rhs==-oo) return 0;
   IntervalRow facet = IntervalRow::Zero(this->fdim+1);
   facet[0] = -rhs;
   if (flat) {
      facet += this->M_EQ.row(c);
   } else {
      facet[c+1] = 1.0;
   }
   if (!mx) facet=-facet;
   Index idFacet = -c-1-(mx ? 0 : this->dim);
   return this->add_facet(idFacet,facet);
}

int DDbuildF2V::add_constraint_box(const IntervalVector &box) {
   int ret=0;
   for (Index i=0;i<box.size();i++) {
      if (box[i].is_degenerated()) continue;
      int ret1=this->add_bound_var(i,true,box[i].ub());
      if (ret1==-1) return -1;
      if (ret1==1) ret=1;
   }
   for (Index i=0;i<box.size();i++) {
      if (box[i].is_degenerated()) continue;
      int ret1=this->add_bound_var(i,false,box[i].lb());
      if (ret1==-1) return -1;
      if (ret1==1) ret=1;
   }
   return ret;
}

std::forward_list<DDvertex>::iterator DDbuildF2V::addDDvertex(const IntervalVector &v) {
    nbIn++;
    vertices.emplace_front(DDvertex(v,nbIn));
    return vertices.begin();
}
std::forward_list<DDvertex>::iterator  DDbuildF2V::addDDvertex(IntervalVector &&v) {
    nbIn++;
    vertices.emplace_front(DDvertex(v,nbIn));
    return vertices.begin();
}

inline std::vector<Index> fctSon(Index fct,const std::vector<Index> fcts1,
		const std::vector<Index> fcts2) {
    std::vector<Index> nfcts;
    Index a1=0; Index a2=0;
    bool idFacetAdded=(fct<0);
    while (a1<(Index)fcts1.size() && a2<(Index)fcts2.size()) {
       if (fcts1[a1]<fcts2[a2]) {
          a1++; if (a1==(Index)fcts1.size()) break;
       } else if (fcts2[a2]<fcts1[a1]) {
          a2++; if (a2==(Index)fcts2.size()) break;
       } else {
         if (fcts1[a1]>fct && !idFacetAdded) {
            nfcts.push_back(fct);
            idFacetAdded=true;
         }
         nfcts.push_back(fcts1[a1]);
         a1++; a2++;
       }
    }
    if (!idFacetAdded) nfcts.push_back(fct);

    return nfcts;
}

#if 0
/* p1 has lambda>rhs, p2 has lambda<rhs ;
   p1 is outside the new constraint, p2 is inside.
   For inequalities, we need to get "as much" p1 as possible, 
   that is maximize |lambda2| and minimize |lambda1| */
Index DDbuildF2V::addVertexSon(const DDvertex &p1, DDvertex &p2, double rhs,
        Index idFacet) {
    Interval lambdaP1=p1.lambda.lb();
    Interval lambdaP2=p2.lambda.lb();
    IntervalVector newV = ((lambdaP1-rhs)*p2.vertex
          + (rhs-lambdaP2)*p1.vertex)/(lambdaP1-lambdaP2);
#if 0
    Interval ratio2=1.0/(1.0-p1.lambda/p2.lambda); /* maximize ratio */
    ratio2 &= Interval(0.0,1.0);
    IntervalVector newV = ratio2.ub()*p1.vertex + 
		(Interval(1.0)-ratio2.ub())*p2.vertex;
#endif
    /* computation of facets */
    std::vector<Index> nfcts;
    Index a1=0; Index a2=0;
    bool idFacetAdded=false;
    while (a1<(Index)p1.fcts.size() && a2<(Index)p2.fcts.size()) {
       if (p1.fcts[a1]<p2.fcts[a2]) {
          a1++; if (a1==(Index)p1.fcts.size()) break;
       } else if (p2.fcts[a2]<p1.fcts[a1]) {
          a2++; if (a2==(Index)p2.fcts.size()) break;
       } else {
         if (p1.fcts[a1]>idFacet && !idFacetAdded) {
            nfcts.push_back(idFacet);
            idFacetAdded=true;
         }
         nfcts.push_back(p1.fcts[a1]);
         a1++; a2++;
       }
    }
    if (!idFacetAdded) nfcts.push_back(idFacet);
    std::set<Index> lnks;
    lnks.insert(p2.Id);
    Index newId=this->addDDvertex(newV,lnks,nfcts);
    p2.replaceLnks(p1.Id,newId);
    return newId;
}
#endif

IntervalVector DDbuildF2V::compute_vertex(const IntervalVector &vect) const {
   if (!flat) {
     if (vect[0]==0.0) return vect.tail(this->fdim);
     return vect.tail(this->fdim)/vect[0];
   }
   if (vect[0]==0.0) return this->M_EQ*vect;
   return this->M_EQ*(vect/vect[0]);
}


struct CmpDD {
   bool operator()(const std::forward_list<DDvertex>::iterator &a,
		const std::forward_list<DDvertex>::iterator &b) {
       return (b->vertex[0].lb()*a->lambda.lb()<=a->vertex[0].lb()*b->lambda.lb());
   }
};

bool DDbuildF2V::addDDlink(const std::forward_list<DDvertex>::iterator& V1,
                      const std::forward_list<DDvertex>::iterator& V2) {
      if (V1->addLnk(V2,true)) return V2->addLnk(V1,false);
      return false;
//    return (V1->addLnk(V2,true) && V2->addLnk(V1,false));
}

int DDbuildF2V::add_facet(Index idFacet, const IntervalRow &facet) {
   /*  consider lines */
   Index bestline=-1;
   double maxmig=0.0;
   for (Index i=0;i<(Index)lines.size();i++) {
      Interval u = facet.dot(lines[i]);
      if (u.contains(0.0)) continue;
      double mg=u.mig();
      if (mg>maxmig) {
         bestline=i;
         maxmig=mg;
      }
   }  
   if (bestline!=-1) {
      Index codeFacet = reffacets.size();
      reffacets.push_back(idFacet);
      if (bestline!=(Index)lines.size()-1) {
	lines[bestline].swap(lines.back());
      }
      IntervalVector& bstline = lines.back();
      Interval ubest = facet.dot(bstline);
      /* traitement des lines */
      for (Index i=0;i<(Index)lines.size()-1;i++) {
         Interval u = facet.dot(lines[i]);
         lines[i] = ubest*lines[i] - u*bstline;
         reduce_vector_without_rounding(lines[i]);
      }
      /* traitement des sommets */
      for (DDvertex &v : vertices) {
         double lower =  facet.dot(v.vertex).lb();
         if (ubest.lb()>0.0) {
            if (lower>0.0) 
              v.vertex = ubest.ub()*v.vertex - lower*bstline;
            else
	      v.vertex = ubest.lb()*v.vertex - lower*bstline;
	 } else  {
            if (lower>0.0) 
              v.vertex = -ubest.lb()*v.vertex + lower*bstline;
            else
	      v.vertex = -ubest.ub()*v.vertex + lower*bstline;
         }
	 v.reduce_vector();
         v.addFct(codeFacet);
      }
      /* ajout du nouveau sommet et des liens */
      std::forward_list<DDvertex>::iterator itVertex = vertices.begin();
      std::forward_list<DDvertex>::iterator newVertex = 
		this->addDDvertex(ubest.lb()<0.0 ? bstline : (-bstline));
      for (;itVertex!=vertices.end();++itVertex) {
          this->addDDlink(itVertex,newVertex);
      }
      for (Index a=0;a<codeFacet;a++) {
          newVertex->addFct(a);
      }
      /* ce nouveau sommet a une face spéciale, "négation" de la contrainte,
         de code 0 */
      newVertex->addFct(reffacets.size());
      reffacets.push_back(0);
      /* suppression de la ligne */
      this->lines.pop_back();
      return 1;
   }
   /* end of line treatments , the following 
      is when no line is available */
   int nbGT=0;
   int nbGE=0;
   int nbTot=0;
   bool notempty=false;
   for (DDvertex &vt : vertices) {
      vt.lambda = facet.dot(vt.vertex);
      notempty |= (vt.lambda.lb()<=0.0);
      nbTot++;
      if (vt.lambda.ub()<0.0) {
         vt.status = DDvertex::VERTEXLT;
         continue;
      }
      if (vt.lambda.lb()<0.0) {
         vt.status = DDvertex::VERTEXON;
         continue;
      }
      nbGE++;
      if (vt.lambda.lb()<this->tolerance) {
         vt.status = DDvertex::VERTEXGE;
         continue;
      }
      nbGT++;
      vt.status=DDvertex::VERTEXGT;
   }
   if (!notempty) { this->empty=true; vertices.clear(); return -1; }
   if (nbGT==0) return 0;

   std::forward_list<DDvertex>::iterator itVertex = vertices.begin();
   std::priority_queue<std::forward_list<DDvertex>::iterator,
		std::vector<std::forward_list<DDvertex>::iterator>,
		CmpDD> stack; /* note : absolutely not clear that we need
				 a priority queue */
   Index refFacet=-1;
   while (itVertex!=vertices.end()) {
//       bool inBoundary;
       std::forward_list<DDvertex>::iterator actVertex;
       if (stack.size()>0) {
          actVertex = stack.top();
          stack.pop();
          if (actVertex->status!=DDvertex::VERTEXSTACK) continue;
//          inBoundary=true;
       } else {
          actVertex=itVertex;
          ++itVertex;
          if (actVertex->status!=DDvertex::VERTEXGT) continue;
          refFacet = reffacets.size(); /* new facet identification */
          reffacets.push_back(idFacet);
//          inBoundary=false;
       }
#ifdef DEBUG_CODAC2_DD
       std::cout << " elimination sommet : " << actVertex->Id << ", " << actVertex->links.size() << "liens \n";
#endif
       actVertex->status = DDvertex::VERTEXREM;
       std::vector<std::pair<std::forward_list<DDvertex>::iterator,
			std::vector<Index>>>
		adjacentVertices;
       for (std::forward_list<DDvertex>::iterator lnk : actVertex->links) {
           DDvertex &destlink = *lnk;
#ifdef DEBUG_CODAC2_DD
       std::cout << " lien : " << destlink.Id << "\n";
#endif
           if (destlink.status==DDvertex::VERTEXGT) {
               destlink.status=DDvertex::VERTEXSTACK;
               stack.push(lnk);
           }
           if (destlink.status==DDvertex::VERTEXGE ||
               destlink.status==DDvertex::VERTEXSTACK) {
#ifdef DEBUG_CODAC2_DD
	       std::cout << " GE ou STACK " << destlink.lambda << "\n";
#endif
               /* add the facet to the vertex, if it does not already exists */
	       adjacentVertices.push_back(std::pair(lnk,fctSon(-1,lnk->fcts,actVertex->fcts)));
               destlink.addFct(refFacet);
	       destlink.removeLnk(actVertex);
	       continue;
           }
           assert(destlink.status!=DDvertex::VERTEXREM);
			 /* should not happen */
           IntervalVector partV=destlink.vertex;
           if (destlink.status==DDvertex::VERTEXON) {
             IntervalRow x1(facet);
             MulOp::bwd(Interval(-oo,0.0),x1,partV);
             if (partV.is_empty()) {
#ifdef DEBUG_CODAC2_DD
	       std::cout << " Sommet conserve " << facet << " " << destlink.vertex << " " << destlink.lambda << "\n";
#endif
	       adjacentVertices.push_back(std::pair(lnk,fctSon(-1,lnk->fcts,actVertex->fcts)));
               destlink.addFct(refFacet);
	       destlink.removeLnk(actVertex);
	       continue;
             }
           }
           if (-destlink.lambda.lb()/actVertex->lambda.lb()<tolerance) {
#ifdef DEBUG_CODAC2_DD
	       std::cout << " Extension sommet " << -destlink.lambda.lb()/actVertex->lambda.lb() << "\n";
#endif
	      /* extend the vertex, and add fct */
	      adjacentVertices.push_back(std::pair(lnk,fctSon(-1,lnk->fcts,actVertex->fcts)));
              destlink.vertex |=
                 (partV - 
		(destlink.lambda.lb()/actVertex->lambda)*actVertex->vertex);
              destlink.addFct(refFacet);
	      destlink.removeLnk(actVertex);
	      continue;
           }
	   /* creation of a new vertex */
	   IntervalVector newV = -destlink.lambda.lb()*actVertex->vertex
		      + actVertex->lambda.lb()*partV;
           assert(!newV.is_empty());
           std::forward_list<DDvertex>::iterator newVx = 
		     this->addDDvertex(newV);
#ifdef DEBUG_CODAC2_DD
       std::cout << " creation new V " << newV << "id : " << newVx->Id << " coef " << (-destlink.lambda.lb()/actVertex->lambda.lb()) << "\n";
#endif
	   newVx->lambda=0.0;
	   destlink.changeLnk(actVertex,newVx);
	   newVx->addLnk(lnk,false);
	   newVx->fcts = fctSon(-1,destlink.fcts,actVertex->fcts);
	   adjacentVertices.push_back(std::pair(newVx,newVx->fcts));
           newVx->addFct(refFacet);
           newVx->status=DDvertex::VERTEXGE;
       }
#ifdef DEBUG_CODAC2_DD
       std::cout << " fin traitement liens : " << adjacentVertices.size() << "\n";
#endif
       PosetMaxSet<std::pair<std::forward_list<DDvertex>::iterator,
			std::forward_list<DDvertex>::iterator>> maxset;
       for (Index i=0;i<=(Index)adjacentVertices.size()-1;i++) {
#ifdef DEBUG_CODAC2_DD
         std::cout << " sommet : " << adjacentVertices[i].first->Id << " : ";
         for (Index u : adjacentVertices[i].second) {
            std::cout << u << ",";
         }        
         std::cout << std::endl;
#endif
         for (Index j=i+1;j<(Index)adjacentVertices.size();j++) {
              PosetElem<std::pair<std::forward_list<DDvertex>::iterator,
			std::forward_list<DDvertex>::iterator>>
			actuel(std::pair(adjacentVertices[i].first,
			       adjacentVertices[j].first),
			       adjacentVertices[i].second,
			       adjacentVertices[j].second,refFacet);
	      if (actuel.elems.size()<fdim-2-lines.size()) continue;
	      maxset.addElement(std::move(actuel));
          }
       }
#ifdef DEBUG_CODAC2_DD
       std::cout << " boucle maxset : " << actVertex->Id << "\n";
#endif
       for (const PosetElem<std::pair<std::forward_list<DDvertex>::iterator,
			std::forward_list<DDvertex>::iterator>>
			 &pm : maxset) {
#ifdef DEBUG_CODAC2_DD
	      if (pm.elems.size()<fdim-2-lines.size()) {
           std::cout << " maxset strange : " << pm.a.first->Id << " " << pm.a.second->Id << " " << pm.elems.size() <<  std::endl;
            }
		std::cout << pm.a.first->Id << " " << pm.a.second->Id << "\n";
#endif
	    [[maybe_unused]] bool res = this->addDDlink(pm.a.first,pm.a.second);
#ifdef DEBUG_CODAC2_DD
            std::cout << (res ? "   added " : "   already here ") << "\n";
#endif

       }
   }
       
   vertices.remove_if([](const DDvertex &v) 
			{ return v.status==DDvertex::VERTEXREM; });
   return 1;
}


int DDbuildF2V::add_facet(Index id) {
   return this->add_facet((*facetsptr)[id]);
}


int DDbuildF2V::add_facet(CollectFacets::mapCIterator itFacet) {
   Index idFacet = itFacet->second.Id;
   if (empty) return 0;
   if (itFacet->second.eqcst)
	throw std::domain_error("add_facet with an equality facet.");
   /* the first step is to take the equalities constraints into account */
   IntervalRow facet = IntervalRow::Zero(this->fdim+1);
   facet[0] = -itFacet->second.rhs;
   if (flat) {
      facet += itFacet->first.row * this->M_EQ;
   } else {
      facet.tail(this->fdim) = itFacet->first.row;
   }
   return this->add_facet(idFacet,facet);
}

/* detection of "irredundant facets" */
std::set<Index> DDbuildF2V::redundantFacets() {
   std::map<Index,std::vector<Index>> mapping;
   std::set<Index> resultat; /* set of existing facets */
   for (Index i = -2*dim;i<0;i++) {
      resultat.insert(i);
   }
   for (const auto &u : facetsptr->get_map()) {
      resultat.insert(u.second.Id);
   } 
   for (const DDvertex &vt : vertices) { 
		/* vt.Id is in decreasing order , therefore we consider
		   the negation of vt.Id to be in increasing order */
      Index id = vt.Id;
      for (Index ft : vt.fcts) {
          std::vector<Index> &elem = mapping[ft];
          elem.push_back(-id); /* in decreasing order */
      }
   }
   PosetMaxSet<Index> facetElems;
   for (std::pair<const Index,std::vector<Index>> &p : mapping) {
//       std::cout << "insert " << p.first << ":" << reffacets[p.first] << " "  << p.second.size() << " : ";
//       for (Index q : p.second) std::cout << q << "\n";
       facetElems.addElement(p.first,p.second); /* swap p.second */
   }
   /* to get the result of facetElems, we store booleans for each facets */
   std::vector<bool> redundant(reffacets.size(),true);
   for (const PosetElem<Index> &elm : facetElems) {
        redundant[elm.a]=false;
        resultat.erase(reffacets[elm.a]); 
		/* note : this approach (fill resultat then remove
		   irredundant facets) is ok when one facet may "appear"
                   several times */
   }
   /* renumbering reffacets */
   std::vector<Index> new_reffacets;
   Index nb_irredundant=0;
   for (unsigned int i=0;i<reffacets.size();i++) {
       if (redundant[i]) { reffacets[i]=-1; continue; }
       new_reffacets.push_back(reffacets[i]);
       reffacets[i]=nb_irredundant;
       nb_irredundant++;
   } 
   /* renumbering the index of vertices */
   for (DDvertex &vt : vertices) { 
      unsigned int j=0;
      for (unsigned int i=0;i<vt.fcts.size();i++) {
         if (reffacets[vt.fcts[i]]<0) continue;
         vt.fcts[j]=reffacets[vt.fcts[i]];
         j++;
      }
      vt.fcts.resize(j);
   }
   reffacets = std::move(new_reffacets);
   return resultat;
}

/* update reffacets following a renumbering */
void DDbuildF2V::update_renumber(const std::vector<Index> &ret) {
   for (Index &a : reffacets) {
      if (a<=0) continue;
      if (ret[a-1]==-1) {
         a=0;
      } else { 
         a=ret[a-1];
      }
   }
}

/* build the bbox of all vertices */
IntervalVector DDbuildF2V::build_bbox() const {
   IntervalVector b = IntervalVector::empty(dim);
   for (const DDvertex &a : vertices) {
      b |= this->compute_vertex(a.vertex);
   }
   return b;
}


/* normalisation for printing of vertice... good idea ? */
IntervalVector normalise(const IntervalVector &v) {
   IntervalVector res(v);
   if (res[0].lb()<=0.0) return res;
   return res/res[0].lb();
}

std::ostream& operator<<(std::ostream& os, const DDbuildF2V& build) {
   os << "DDbuildF2V (" << /*build.vertices.size() <<*/ " vtx) : " << std::endl;
   os << "   " << build.lines.size() << " lines : " << std::endl;
   for (const auto &l : build.lines) {
     os << " " << l << std::endl;
   }
   Index nbvx=0, nbrays=0;
   for (const auto &vt : build.vertices) {
      if (vt.vertex[0]!=0) nbvx++; else nbrays++;
      os << " " << vt.Id << " : " << normalise(vt.vertex) << std::endl;
      os << " fct : (";
      for (const auto &a : vt.fcts) {
         os << a << ":" << build.reffacets[a] << ",";
      }
      os << ")" << std::endl;
      os << " lnks: (";
      for (const auto &a : vt.links) {
         os << a->Id << ",";
      }
      os << ")" << std::endl;
   }
   os << "endDDbuildF2V ( "<< nbrays << " rays, " << nbvx << " vertices, " << (nbrays+nbvx) << " total)" << std::endl;
   return os;
}

/*****************************/
/*** DDbuildF2V::EqFacet *****/
/*****************************/
/* management of the equality constraints for DDbuildF2V */
#if 0
void DDbuildF2V::EqFacet::adapt_eqconstraint(const IntervalVector &bbox,
                                Row &cst, Interval &rhs) const {
           Interval alpha(cst[dim]); cst[dim]=0.0;
           IntervalRow newcst = cst + alpha*this->eqcst;
           cst = newcst.mid();
           rhs += (newcst-cst).dot(bbox)-alpha*valcst;
}
void DDbuildF2V::EqFacet::adapt_ineqconstraint(IntervalRow &cst, double &rhs)
	 const {
           Interval alpha(cst[dim]); cst[dim]=0.0;
           cst = cst + alpha*this->eqcst;
           rhs = (rhs-alpha*valcst).ub();
}
DDbuildF2V::EqFacet 
	DDbuildF2V::EqFacet::build_EqFacet(const IntervalVector &bbox,
                        const Row &cst, Interval &rhs) {
            /* identification of the dimension */
            Index dim = -1;
            double val=0;
            for (Index i=0;i<cst.size();i++) {
                double v2 = std::abs(cst[i]);
                if (v2>val) {
                   dim=i;
                   val = v2;
                }
            }
            if (dim==-1) throw 
			std::domain_error("zero vector in build_EqFacet");
            IntervalRow newcst(cst);
            val=cst[dim];
            newcst[dim]=0.0;
            newcst = newcst/val;
            rhs = rhs/val - (newcst-newcst.mid()).dot(bbox);
            return EqFacet(dim,-newcst.mid(),rhs);
}

void DDbuildF2V::EqFacet::adapt_box(IntervalVector &bbox) const {
    Interval eval = this->eqcst.dot(bbox) + valcst;
    if ((bbox[this->dim] & eval).is_empty()) {
       bbox.set_empty();
    }
    bbox[this->dim]=0;
}
#endif

/*****************************/
/*** DDbuildV2F::EqFacet *****/
/*****************************/

DDbuildV2F::DDbuildV2F(Index idVertex, const Vector &vertex) :
	facetsptr(std::make_shared<CollectFacets>()) {
   Row r = Row::Zero(vertex.size());
   for (Index i=0;i<vertex.size();i++) {
      r[i]=1.0;
      facetsptr->insert_facet(r,vertex[i],true);
      r[i]=0.0;
   }
   idVertices.push_back(idVertex);
}

std::forward_list<DDfacet>::iterator DDbuildV2F::addFacetSon(
		const std::forward_list<DDfacet>::iterator &It1, 
                std::forward_list<DDfacet>::iterator &It2, Index idVertex) {
    const CollectFacets::mapIterator &p1 = It1->facetIt;
    const CollectFacets::mapIterator &p2 = It2->facetIt;
#ifdef DEBUG_CODAC2_DD
    std::cout << " p1.facet.row " <<  p1->first.row << std::endl;
    std::cout << " p2.facet.row " <<  p2->first.row << std::endl;
#endif
    Row newV = - It2->lambda*p1->first.row + It1->lambda*p2->first.row;
    double newRhs = - It2->lambda*p1->second.rhs + It1->lambda*p2->second.rhs;
#ifdef DEBUG_CODAC2_DD
    std::cout << " newV " << newV << std::endl;
    std::cout << " newRhs " << newRhs << std::endl;
#endif
    /* computation of vertices */
    std::vector<Index> nvtx;
    Index a1=0; Index a2=0;
    bool idVertexAdded=false;
    while (a1<(Index)It1->vtx.size() && a2<(Index)It2->vtx.size()) {
       if (It1->vtx[a1]<It2->vtx[a2]) {
          a1++; if (a1==(Index)It1->vtx.size()) break;
       } else if (It2->vtx[a2]<It1->vtx[a1]) {
          a2++; if (a2==(Index)It2->vtx.size()) break;
       } else {
         if (It1->vtx[a1]>idVertex && !idVertexAdded) {
            nvtx.push_back(idVertex);
            idVertexAdded=true;
         }
         nvtx.push_back(It1->vtx[a1]);
         a1++; a2++;
       }
    }
    if (!idVertexAdded) nvtx.push_back(idVertex);
    std::vector<std::forward_list<DDfacet>::iterator> lnks;
    lnks.push_back(It2);
    std::forward_list<DDfacet>::iterator
	 newF=this->addDDfacet(reduce_facet(newV,newRhs),lnks,nvtx); 
    if (newF!=facets.end()) It2->changeLnk(It1,newF);
	else It2->removeLnk(It1);
    return newF;
}

int DDbuildV2F::add_vertex(Index idVertex, const Vector& vertex) {
    return this->add_vertex(idVertex,IntervalVector(vertex));
}

int DDbuildV2F::add_vertex(Index idVertex, const IntervalVector& vertex) {
   int cnt=0;
   /* first check equalities */
   if (facetsptr->nbeqfcts()>0) {
      Index eqViolated=-1;
      CollectFacets::mapIterator eqViolatedIt= facetsptr->endFacet();
      Interval delta;
      for (Index i=facetsptr->nbeqfcts()-1;i>=0;i--) {
          CollectFacets::mapIterator fcIt = facetsptr->get_eqFacet(i);
          Interval calc = fcIt->first.row.dot(vertex)-fcIt->second.rhs;
          if (calc.mig()<this->tolerance) continue;
          if (eqViolated==-1) { 
	     eqViolated=i; 
	     delta=calc; 
 	     eqViolatedIt = fcIt;
	     continue;
          } else {
             double correction = -(calc/delta).mid();
 	     bool success;
             std::tie(fcIt,success) = facetsptr->change_eqFacet(i,
		  fcIt->first.row + correction*eqViolatedIt->first.row,
		  fcIt->second.rhs + correction*eqViolatedIt->second.rhs);
	     /* if success is false, the eqFacet index may have changed */
             if (!success && 
		 eqViolated==facetsptr->nbeqfcts()) eqViolated=i; 
          }
      }
      if (eqViolated!=-1) {
         if (facets.empty()) {
            this->nbIn=0;
            std::vector<std::forward_list<DDfacet>::iterator> lnk;
            std::vector<Index> vtx = { idVertex };
            Interval nrhsI = eqViolatedIt->first.row.dot(vertex);
            double nrhs = delta.lb()>0.0 ? nrhsI.ub() : nrhsI.lb();
	    CollectFacets::mapIterator newIt =
		facetsptr->dissociate_eqFacet(eqViolated,nrhs);
	    assert_release(newIt!=facetsptr->endFacet());
            if (delta.lb()>0.0) {
               auto a = this->addDDfacet(eqViolatedIt,lnk,vtx);
               vtx = idVertices;
               lnk = { a };
               auto b = this->addDDfacet(newIt,lnk,vtx);
	       a->addLnk(b,false);
            } else {
               auto a = this->addDDfacet(newIt,lnk,vtx);
               vtx = idVertices;
               lnk = { a };
               auto b = this->addDDfacet(eqViolatedIt,lnk,vtx);
	       a->addLnk(b,false);
           }
           cnt=2;
         } else {
           /* add the remaining inequality, linked with all existing facets */
           std::vector<std::forward_list<DDfacet>::iterator> lnk;
           for (std::forward_list<DDfacet>::iterator it =
			facets.begin(); it!=facets.end(); ++it) {
                 it->status=DDfacet::FACETOUT; /* to remove FACETNEW */
		 lnk.push_back(it);
           } 
           Row saveEqrow = eqViolatedIt->first.row;
           double saveRhs = eqViolatedIt->second.rhs;
	   CollectFacets::mapIterator nIt =
		facetsptr->dissociate_eqFacet(eqViolated,
			(delta.lb()>0.0 ? +oo : -oo));
//	   std::cout << "dissociated : " << (delta.lb()>0.0 ? +oo : -oo) <<
//                  " : " << (nIt == facetsptr->endFacet()) << "\n";
           eqViolatedIt=nIt; /* in the following, the case nIt==facets.end()
			        is not treated, as it should not happen */
           std::vector<Index> vtx = idVertices;
           
           std::forward_list<DDfacet>::iterator u = 
		(eqViolatedIt != facetsptr->endFacet() ? 
			this->addDDfacet(eqViolatedIt,lnk,vtx) :
			facets.end());
           /* change all existing facets to take the new vertex into account */
           /* no facet should be removed, theoretically, but we
	    * nevertheless consider this possibility */
           std::forward_list<DDfacet>::iterator it_f=facets.before_begin();
           std::forward_list<DDfacet>::iterator nxt = facets.begin();
           while (nxt!=facets.end()) {
               DDfacet &f = (*nxt);
               if (f.status!=DDfacet::FACETOUT) { it_f=nxt; ++nxt; continue; }
               Interval q = f.facetIt->second.rhs-
				f.facetIt->first.row.dot(vertex);
	       double correction = (q/delta).mid();
               if (u!=facets.end()) f.addLnk(u);
               f.addVtx(idVertex);
 	       bool success;
               std::tie(f.facetIt, success) =
		   facetsptr->change_ineqFacet(f.facetIt->second.Id,
			    f.facetIt->first.row + correction*saveEqrow,
			    f.facetIt->second.rhs + correction*saveRhs);
	       if (!success) { this->removeFacet(nxt,true); 
			       facets.erase_after(it_f); 
			       nxt=it_f; ++nxt; continue; }
               it_f=nxt; ++nxt;
           }
           cnt=(u!=facets.end() ? 1 : 0);
         }
         if (facetsptr->nbeqfcts()>0) idVertices.push_back(idVertex);
         return cnt;
      } else idVertices.push_back(idVertex); /* and follows */
   }
   /* no equalities removed */
   for (DDfacet &d : facets) {
       if (d.status==DDfacet::FACETREM) continue;
       Interval calc = d.facetIt->first.row.dot(vertex)-d.facetIt->second.rhs;
       d.lambda = calc.lb();
       if (calc.mig()<this->tolerance) { d.status=DDfacet::FACETON; }
       else if (calc.lb()>0.0) { d.status=DDfacet::FACETOUT; }
       else { d.status=DDfacet::FACETIN; }
   } 
   for (std::forward_list<DDfacet>::iterator it = facets.begin();
                  it!=facets.end();++it) {
     DDfacet &d = (*it);
     if (d.status==DDfacet::FACETON) {
        d.addVtx(idVertex);
        continue;
     }
     if (d.status==DDfacet::FACETIN || d.status==DDfacet::FACETNEW) continue;
     if (d.links.empty()) { /* not supposed to happen */
        /* very specific case of a 1-dimension polyhedra */
        d.vtx.clear();
        d.vtx.push_back(idVertex);
        facetsptr->change_ineqFacet_rhs(d.facetIt, 
			d.facetIt->first.row.dot(vertex).ub());
        continue;
     }
     for (std::forward_list<DDfacet>::iterator idL : d.links) {
        if (idL==facets.end()) continue; /* can it happen ? */
        DDfacet &d2 = (*idL);
        if (d2.status==DDfacet::FACETON) {
           d2.removeLnk(it);
           continue;
        }
        if (d2.status!=DDfacet::FACETIN) { 
	   continue; /* do nothing for out */
        }
#ifdef DEBUG_CODAC2_DD
        std::cout << "    addFacet " << d.facetIt->second.Id << " " << d2.facetIt->second.Id << " " << d.status << " " << d2.status << std::endl;
        std::cout << "    (" << d.lambda << " " << d2.lambda << ")    " << std::endl;
#endif
        [[maybe_unused]]  std::forward_list<DDfacet>::iterator 
		newIt = this->addFacetSon(it,idL,idVertex);
#ifdef DEBUG_CODAC2_DD
        if (newIt==facets.end()) 
	  std::cout << " ... FAILED! " << std::endl;
        else 
          std::cout << " ... done " << newIt->facetIt->second.Id << std::endl;
#endif
        cnt++;
     }
     facetsptr->removeFacetById(it->facetIt->second.Id);
     this->removeFacet(it,false);
   }
   /*construction of new links for the eq facets */
   PosetMaxSet<std::pair<std::forward_list<DDfacet>::iterator,
			std::forward_list<DDfacet>::iterator>> maxset;
   for (std::forward_list<DDfacet>::iterator it = facets.begin();
                  it!=facets.end();++it) {
      DDfacet &d = (*it);
      if (d.status==DDfacet::FACETON || d.status==DDfacet::FACETNEW) {
         for (std::forward_list<DDfacet>::iterator it2 = next(it);
		it2!=facets.end(); ++it2) {
            DDfacet &d2 = (*it2);
            if (d2.status==DDfacet::FACETON || d2.status==DDfacet::FACETNEW) {
               PosetElem<std::pair<std::forward_list<DDfacet>::iterator,
                        std::forward_list<DDfacet>::iterator>>
			 actuel(std::pair(it,it2),d.vtx,d2.vtx);
	       maxset.addElement(std::move(actuel));
            }
         }
      }
   }
   for (const PosetElem<std::pair<std::forward_list<DDfacet>::iterator,
			std::forward_list<DDfacet>::iterator>> &pm : maxset) {
//      std::cout << " maxset : " << pm.a << " " << pm.b << std::endl;
      pm.a.first->addLnk(pm.a.second);
      pm.a.second->addLnk(pm.a.first);
   }
   facets.remove_if([](const DDfacet &v) 
			{ return v.status==DDfacet::FACETREM; });
   facetsptr->renumber();
   return cnt;
}

std::forward_list<DDfacet>::iterator 
	DDbuildV2F::addDDfacet(const CollectFacets::mapIterator &it,
		 std::vector<std::forward_list<DDfacet>::iterator> &links,
		 std::vector<Index> &vtx) {
    facets.emplace_front(it,links,vtx);
    return facets.begin();
}

std::forward_list<DDfacet>::iterator 
	DDbuildV2F::addDDfacet(const std::pair<Row, double> &row_rhs,
		 std::vector<std::forward_list<DDfacet>::iterator> &links,
		 std::vector<Index> &vtx) {
    std::pair<Index,bool> result =
	 facetsptr->insert_facet(row_rhs.first,row_rhs.second,false);
    if (!result.second) {
       return facets.end();
    }
    facets.emplace_front((*facetsptr)[result.first-1],links,vtx);
    return facets.begin();
}

std::forward_list<DDfacet>::iterator 
	DDbuildV2F::addDDfacet(std::pair<Row,double> &&row_rhs,
		 std::vector<std::forward_list<DDfacet>::iterator> &links,
		 std::vector<Index> &vtx) {
    std::pair<Index,bool> result  =
	 facetsptr->insert_facet(row_rhs.first,row_rhs.second,false);
    if (!result.second) {
       return facets.end();
    }
    facets.emplace_front((*facetsptr)[result.first-1],links,vtx);
    return facets.begin();
}

void DDbuildV2F::removeFacet(std::forward_list<DDfacet>::iterator It,
		 bool removeLnks) {
   DDfacet &d = (*It);
   if (removeLnks) {
      for (std::forward_list<DDfacet>::iterator it : d.links) {
         it->removeLnk(It);
      }
   }
   d.facetIt=facetsptr->endFacet();
   d.status=DDfacet::FACETREM;
}

std::ostream& operator<<(std::ostream& os, const DDbuildV2F& build) {
   os << "DDbuildV2F (" << build.facetsptr->nbeqfcts() << " eq, " <<
		build.facetsptr->nbfcts() << " total fcts) : " << std::endl;
   for (Index i=0;i<build.facetsptr->nbeqfcts();++i) {
      const Facet &eqf = (*build.facetsptr->get_eqFacet(i));
      os << eqf.first.get_row() << " = " << eqf.second.get_rhs() << std::endl;
   }
   for (const auto &ifacet : build.facets) {
      auto &ieqf = *(ifacet.facetIt);
      os << ieqf.second.get_Id() << " : " << ieqf.first.get_row() << " <= " << ieqf.second.get_rhs() << std::endl;
      os << " vtx: (";
      for (Index a : ifacet.vtx) {
         os << a << ",";
      }
      os << ")" << std::endl;
      os << " lnks: (";
      for (std::forward_list<DDfacet>::iterator a : ifacet.links) {
         os << a->facetIt->second.get_Id() << ",";
      }
      os << ")" << std::endl;
   }
   os << "endDDbuildV2F" << std::endl;
   return os;
}

}

