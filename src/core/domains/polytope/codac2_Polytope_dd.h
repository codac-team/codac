/**
 * \file codac2_Polytope_dd.h implementation of the dd algorithms
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Damien Massé
 *  \copyright  Copyright 2025
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include <vector>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <ctime>
#include <cmath>
#include <bitset>
#include <memory>
#include <map>
#include <forward_list>
#include <set>
#include <unordered_set>
#include <stdexcept>

#include "codac2_Index.h"
#include "codac2_Vector.h"
#include "codac2_Row.h"
#include "codac2_Facet.h"


namespace codac2 {


/** \brief multiply a vector by a power of 2 (without rounding)
 *  such that 2^(-10) <= norm(vec) <= 2^10
 *  \param vec the vector (modified)
 *  \return the multiplying factor */
inline double reduce_vector_without_rounding(IntervalVector &vec) {
      int fx;
      if (frexp(vec[0].mag(),&fx)==0.0) fx=INT_MIN;
      for (Index i=1;i<vec.size();i++) {
         int fx2;
         if (frexp(vec[i].mag(),&fx2)==0.0) continue;
         if (fx2>fx) fx=fx2;
      }
      assert_release (fx!=INT_MIN);
      if (std::abs(fx)<10) return 1.0;
      double mult=exp2(-fx);
      for (Index i=0;i<vec.size();i++) {
//           vec[i] = Interval(ldexp(vec[i].lb(),-fx),ldexp(vec[i].ub(),-fx));
         vec[i] *= mult;
      }
      return mult;
}

/** \brief multiply a row by a power of 2 (without rounding)
 *  such that 2^(-10) <= norm(vec) <= 2^10
 *  \param vec the row (modified)
 *  \return the multiplying factor */
inline double reduce_row_without_rounding(Row &vec) {
      int fx;
      if (frexp(vec[0],&fx)==0.0) fx=INT_MIN;
      for (Index i=1;i<vec.size();i++) {
         int fx2;
         if (frexp(vec[i],&fx2)==0.0) continue;
         if (fx2>fx) fx=fx2;
      }
      assert_release (fx!=INT_MIN);
      if (std::abs(fx)<10) return 1.0;
      double mult=exp2(-fx);
      for (Index i=0;i<vec.size();i++) {
//           vec[i] = ldexp(vec[i],-fx);
         vec[i] *= mult;
      }
      return mult;
}


/** \brief structure for vertices in BuildF2V */
struct DDvertex {
   Index Id;
   IntervalVector vertex;
   Interval lambda;
   std::vector<Index> fcts; 
	/* note : double redirection here, a ``real'' facet may have several
           Ids */
   
   enum ddvertexstatus {
     VERTEXNEW, /* created */
     VERTEXLT, /* < (GT,LT) => new vertex */
     VERTEXON, /* <,=,> (GT,LE) => for now, equivalent to LT */
     VERTEXGE, /* = and possible small >
		  keep the vertex. no change with (GT,GE) */
     VERTEXGT, /* vertex GT, untreated and not in stack */
     VERTEXSTACK, /* vertex in stack for later treatment */
     VERTEXREM /* vertex GT treated, to be removed */
   };
   ddvertexstatus status;

   std::vector<std::forward_list<DDvertex>::iterator> links;
 
   DDvertex(const IntervalVector &v, Index Id) : Id(Id), 
	 vertex(v), status(VERTEXNEW) {
	reduce_vector();
   }

   DDvertex(IntervalVector &&v, Index Id, 
		std::vector<std::forward_list<DDvertex>::iterator> &nlinks) :
	 Id(Id), 
	 vertex(v), status(VERTEXNEW) {
         this->links.swap(nlinks);
	 reduce_vector();
   }

   inline void reduce_vector() {
      reduce_vector_without_rounding(vertex);
   }

   bool addLnk(const std::forward_list<DDvertex>::iterator &a,
		 bool check=false) {
      if (check) {
         for (auto b : links) {
            if (a==b) return false;
         } 
      }
      this->links.push_back(a);
      return true;
   }

//   void addLnkVertex(const std::forward_list<DDvertex>::iterator &V1);

   void removeLnk(const std::forward_list<DDvertex>::iterator &a) {
      for (Index i=0;i<(Index)links.size();i++) {
         if (links[i]==a) { 
	    links[i]=links[links.size()-1]; 
	    links.pop_back();
            return;
         }
      }
   }

   void changeLnk(const std::forward_list<DDvertex>::iterator &a,
		const std::forward_list<DDvertex>::iterator &b) {
      for (Index i=0;i<(Index)links.size();i++) {
         if (links[i]==a) { links[i]=b; return; }
      }
      /* should not happen */
      links.push_back(b);
   }

   bool addFct(Index a) {
      auto rev_it = this->fcts.rbegin();
      while (rev_it != this->fcts.rend() && (*rev_it)>a) rev_it++;
      if (rev_it!=this->fcts.rend() && (*rev_it)==a) return false;
      this->fcts.insert(rev_it.base(),a);
      return true;
   }
 
};

/** \brief Structure to build an enclosing set of IntervalVector
 *  from a set of facets */
class DDbuildF2V {
  public:
     /* create the build with a collection of facet, but
        use only the equality facets */
     DDbuildF2V(Index dim, const IntervalVector &box,
 		std::shared_ptr<CollectFacets> facetsptr,
		bool include_box=true);
     
     /* add an inequality facet in the build */
     /* return 1 : changed, 0 : not changed, -1 : empty */
     int add_facet(Index id);
     int add_facet(CollectFacets::mapCIterator itFacet);

     int add_constraint_box(const IntervalVector &box);
     int add_bound_var(Index c, bool mx, double rhs);

     IntervalVector compute_vertex(const IntervalVector &vect) const;
     Index get_dim() const;
     Index get_fdim() const;

     bool is_empty() const;
     const IntervalMatrix &get_M_EQ() const;

     /* return a set of the Id of (detected as) redundant facets ,
        and remove these facets of the representation */
     std::set<Index> redundantFacets();

     /* update reffacets following a renumbering */
     void update_renumber(const std::vector<Index> &ref);

     /* return the bounding box of all vertices */
     IntervalVector build_bbox() const;

     const std::vector<IntervalVector> &get_lines() const;
     const std::forward_list<DDvertex> &get_vertices() const;
     const std::vector<Index> &get_reffacets() const;

     friend std::ostream& operator<<(std::ostream& os, const DDbuildF2V& build);

  private:
     int add_facet(Index idFacet, const IntervalRow &facet);
     bool addDDlink(const std::forward_list<DDvertex>::iterator& V1,
   		      const std::forward_list<DDvertex>::iterator& V2);

     std::forward_list<DDvertex>::iterator addDDvertex(const IntervalVector &v);
     std::forward_list<DDvertex>::iterator addDDvertex(IntervalVector &&v);
/*     std::map<Index,DDvertex>::iterator addVertexSon(const DDvertex &p1,
		 DDvertex &p2, double rhs, Index idFacet);  */
/*     void removeVertex(Index Id, bool removeLnks); */

     Index dim;
     Index fdim; /* dimension - nb independant equalities constraints */
     IntervalVector bbox;

     std::shared_ptr<CollectFacets> facetsptr;

     /* if fdim<dim :
        constraint a X <= b becomes (-b 0) + (a M_EQ) 
        vertex : (c Y) becomes X = M_EQ (1 Y/c) 
        ray : (0 Y) becomes a ray X = M_EQ (0 Y) ?
        lines : (0 Y) becomes a line X = M_EQ (0 Y) ?
     */
     IntervalMatrix M_EQ;

     std::vector<IntervalVector> lines; /* lines (for unbounded polyhedron) */
               /* rays can be handled as classical vertices */

     std::forward_list<DDvertex> vertices;
#if 0
     std::forward_list<DDlink> total_links;
#endif
     std::vector<Index> reffacets; /* indirection for facets */

     bool empty, flat;
     Index nbIn;
     double tolerance = 1e-9;
};

inline Index DDbuildF2V::get_dim() const { return this->dim; }
inline Index DDbuildF2V::get_fdim() const { return this->fdim; }
inline bool DDbuildF2V::is_empty() const { return this->empty; }
inline const IntervalMatrix &DDbuildF2V::get_M_EQ() const { return this->M_EQ; }
inline const std::vector<IntervalVector> &DDbuildF2V::get_lines() const 
	{ return this->lines; }
inline const std::forward_list<DDvertex> &DDbuildF2V::get_vertices() const 
	{ return this->vertices; }
inline const std::vector<Index> &DDbuildF2V::get_reffacets() const 
	{ return this->reffacets; }


/** \brief structure for facets in BuildF2V */
struct DDfacet {
   CollectFacets::mapIterator facetIt;
   double lambda;
   
   enum ddfacetstatus {
     FACETNEW,
     FACETIN,
     FACETON,
     FACETOUT,
     FACETREM
   };
   ddfacetstatus status;

   std::vector<std::forward_list<DDfacet>::iterator> links;
   std::vector<Index> vtx;
 
   DDfacet(CollectFacets::mapIterator facetIt,
		std::vector<std::forward_list<DDfacet>::iterator> &nlinks,
		std::vector<Index> &nvtx) : facetIt(facetIt), status(FACETNEW) {
         this->links.swap(nlinks);
         this->vtx.swap(nvtx);
   }

   bool addLnk(std::forward_list<DDfacet>::iterator a, bool check=false) {
      if (check) {
         for (auto b : links) {
            if (a==b) return false;
         } 
      }
      this->links.push_back(a);
      return true;
   }

   void removeLnk(const std::forward_list<DDfacet>::iterator &a) {
      for (Index i=0;i<(Index)links.size();i++) {
         if (links[i]==a) { 
	    links[i]=links[links.size()-1]; 
	    links.pop_back();
            return;
         }
      }
   }

   void changeLnk(const std::forward_list<DDfacet>::iterator &a,
		const std::forward_list<DDfacet>::iterator &b) {
      for (Index i=0;i<(Index)links.size();i++) {
         if (links[i]==a) { links[i]=b; return; }
      }
      /* should not happen */
      links.push_back(b);
   }

   void addVtx(Index a) {
      auto it = this->vtx.begin();
      while (it != this->vtx.end() && (*it)<a) it++;
      if (it!=this->vtx.end() && (*it)==a) return;
      this->vtx.insert(it,a);
   }

};

/** \brief construction of an (approximate) set of facets from a
 *  a set of vertices */
class DDbuildV2F {
  public:
     DDbuildV2F(Index idVertex, const Vector &vertex);
   
     int add_vertex(Index idVertex, const Vector& vertex);
     int add_vertex(Index idVertex, const IntervalVector& vertex);

     std::shared_ptr<CollectFacets> getFacets();

     friend std::ostream& operator<<(std::ostream& os, const DDbuildV2F& build);


  private:
     /* can NOT return facets.end() */
     std::forward_list<DDfacet>::iterator addDDfacet(
		const CollectFacets::mapIterator &it,
		std::vector<std::forward_list<DDfacet>::iterator> &links,
		std::vector<Index> &vtx);
     /* can return facets.end() if insertion failed */
     std::forward_list<DDfacet>::iterator addDDfacet(
		std::pair<Row,double> &&row_rhs,
		std::vector<std::forward_list<DDfacet>::iterator> &links,
		std::vector<Index> &vtx);
     /* can return facets.end() if insertion failed */
     std::forward_list<DDfacet>::iterator addDDfacet(
 		const std::pair<Row,double> &row_rhs,
		std::vector<std::forward_list<DDfacet>::iterator> &links,
		std::vector<Index> &vtx);
     /* can return facets.end() if insertion failed */
     std::forward_list<DDfacet>::iterator
	 addFacetSon(const std::forward_list<DDfacet>::iterator &It1, 
		std::forward_list<DDfacet>::iterator &It2, Index idVertex); 
     void removeFacet(std::forward_list<DDfacet>::iterator It,
			 bool removeLnks);

     inline std::pair<Row,double> reduce_facet(const Row &row, double rhs) {
        std::pair<Row,double> ret(row,rhs);
        double mult = reduce_row_without_rounding(ret.first);
        ret.second *= mult;
        return ret;
     }
     
     std::shared_ptr<CollectFacets> facetsptr;
     std::forward_list<DDfacet> facets;
     std::vector<Index> idVertices; /* list of vertices entered, useful as
                long as there are eqfacets */
     Index nbIn=0;
     double tolerance =1e-8;
     
};

inline std::shared_ptr<CollectFacets> DDbuildV2F::getFacets() { return facetsptr; }

}

