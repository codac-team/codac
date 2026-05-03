/**
 * \file codac2_Polytope.h
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
#include <initializer_list>

#include "codac2_Index.h"
#include "codac2_Matrix.h"
#include "codac2_Vector.h"
#include "codac2_Row.h"
#include "codac2_IntervalRow.h"
#include "codac2_IntervalVector.h"
#include "codac2_IntervalMatrix.h"
#include "codac2_BoolInterval.h"
#include "codac2_Polytope_util.h"
#include "codac2_Facet.h"
#include "codac2_Polytope_dd.h"
#include "codac2_Parallelepiped.h"
#include "codac2_Zonotope.h"

namespace codac2 {
  /**
   * \class Polytope
   *  Represents a bounded convex polytope as a set of constraints
   * and a bounding box (the bounding box is part of the constraints)  */
  class Polytope 
  {
    public:
        /**
         *  basic constuctor
         */ 
        Polytope();

        /**
         *  basic constuctor with a dimension, full
         */ 
        explicit Polytope(Index dim);

        /**
         *  basic constuctor with a dimension, empty or full
         */ 
        explicit Polytope(Index dim, bool empty);

        /**
         *  Constructs a bounded polytope representing a box
         * from a bounded interval vector
         */
        explicit Polytope(const IntervalVector &box);

        /**
         *  Copy (no minimization)
         */
        Polytope(const Polytope &P);

//        Polytope(Polytope &&P) = default;

        /** 
         *  Constructs a polytope from a set of vertices
         * \param vertices the vertices
         */
        explicit Polytope(const std::vector<Vector> &vertices);

        /** 
         *  Constructs a polytope from a set of vertices as intervalboxes
         * \param vertices the vertices
         */
        explicit Polytope(const std::vector<IntervalVector> &vertices);

        /** 
         *  Constructs a polytope from a set of vertices (IntervalVector)
         * and a set of linear forms, described as a CollectFacets
         * \param vertices the vertices
         * \param facetsform the facets description (the CollectFacets)
         */
        Polytope(const std::vector<IntervalVector> &vertices, 
		const CollectFacets &facetsform);

        /**
         *  from a parallelepiped
         * \param par the parallelepiped
         */
        explicit Polytope(const Parallelepiped &par);

        /**
         *  from a zonotope
         * \param zon the zonotope
         */
        explicit Polytope(const Zonotope &zon);

        /**
         *  Constructs a polytope from a box and a CollectFacets which
	 * will be moved in the polytope. Perform a "box extraction" and a 
         * renumbering on the collection.
         * \param box the box
         * \param facets the collection of facets 
         */
        Polytope(IntervalVector &&box, CollectFacets &&facets);

        /**
         *  Constructs a polytope from a CollectFacets which
	 * will be moved in the polytope. Perform a "box extraction" and a 
         * renumbering on the collection.
         * \param facets the collection of facets 
         */
        Polytope(CollectFacets &&facets);

        /**
         *  Constructs a polytope from a bounding box and 
         * a set of constraints (inequalities)
 	 * 
         */
        Polytope(const IntervalVector &box, 
                 const std::vector<std::pair<Row,double>> &facets, 
	         bool minimize=false);

        /** 
         *  build a polytope from an ine File (cddlib format)
         * \param filename the file */
	static Polytope from_ineFile(const char *filename);
        /** 
         *  build a polytope from an ext File (cddlib format)
         * \param filename the file */
	static Polytope from_extFile(const char *filename);

        /**  convex union of polytopes
         *  use vertices to join the polytopes, as such the result may be
         *  too large
         *  \param lst a non-empty initializer_list of valid polytopes of 
	 *  same dimension
         *  \return the polytope */
        static Polytope union_of_polytopes(std::initializer_list<Polytope> lst);

        /**  convex union of polytopes
         *  use vertices to join the polytopes, as such the result may be
         *  too large
         *  \param lst a non-empty initializer_list of valid polytopes of 
	 *  same dimension
         *  \return the polytope */
        static Polytope union_of_polytopes(const std::vector<Polytope> &lst);

        /**
         *  Destructor
         */
        virtual ~Polytope() = default;

        /**
         *  copy assignment operator 
         * \param P copy
         */
        Polytope &operator=(const Polytope &P);
     
        Polytope &operator=(Polytope &&P) = default;


        /***************** ACCESS **************************/
        /**
         *  Get the dimension of the space 
         * 
         * \return the dimension of the space
         */
        Index dim() const;
        Index size() const;

        /**
         *  Get the dimension of the polytope (ie the dimension
         * of the space minus the equalities). -1 means that the polytope
         * is empty.
         * 
         * \return the dimension of the polytope.
         */
//        Index dim_polytope() const;

        /**
         *  Returns true if the polytope is a box (i.e. there are no
         *  other facets).
         *
         *  \return true if the polytope is a box, false otherwise.
         */
        bool is_box() const;

        /**
         *  Get the number of constraints
         * 
         * \return the number of facets (including equalities)
         */
        Index nb_facets() const;

        /**
         *  Get the number of equality constraints
         * 
         * \return the number of equalities
         */
        Index nb_eq_facets() const;

        /**
         *  is empty
         */
        bool is_empty(bool check=true) const;

        /**
         *  has a flat dimension
         */
        bool is_flat() const;

        /**  ``component'' : interval of the bounding box 
         * (only const !!!). Update the bbox if needed.
         * \return enclosing of the component
         */
        const Interval& operator[](Index i) const;

        /**
         * ``middle'' of the bbox
         * \return the middle of the box
         */
        Vector mid() const;

        /**
         * point inside the polytope using the vertices
         * \return a barycenter of the middle of the vertices
         */
        Vector mid_in() const;

        /** 
         *  bound a constraint (fast), either by a "close" one (+box)
         * or by the vertices if they are computed.
         * no update or LP are done 
         * return an interval I : row X is guaranteed to be less than I.ub()
         * and I.lb gives an indication of the precision of the computation
         * (i.e. if diam(I) is low, a constraint close to row was used
         * \param base the constraint, expressed as a FacetBase
         * \return the interval I */
        Interval fast_bound(const FacetBase &base) const;

        /** 
         *  bound a constraint, using DD
         * \param r the row
         * \return the max
         */
        virtual double bound_row(const Row &r) const;
  
#if 0
        /**  ``distance'' from the satisfaction of a constraint 
         *  ie the inflation of the rhs needed to satisfy the constraint
         *  \param fc the constraint
         *  \return an interval
         */
        double distance_cst (const Facet &fc) const;

        /**
         *  relationships with a box (fast check)
         * \param p the box
         * \return polytope_inclrel checking inclusion and intersection */
        Facet_::polytope_inclrel relation_Box(const IntervalVector& p) const;
#endif

        /**
         *  Checks whether the polytope contains a given point, or
         *  includes a box
         *
         * \param p The point or box to check, enclosed in an IntervalVector.
         * \return BoolInterval indicating possible containment.
         */
        BoolInterval contains(const IntervalVector& p) const;

        /**
         *  Checks whether the interior of the polytope contains a given
         * point, or includes a box
         *
         * \param p The point or box to check, enclosed in an IntervalVector.
         * \return BoolInterval indicating possible containment.
         */
        BoolInterval interior_contains(const IntervalVector& p) const;

        /**
         *  Checks inclusion in another polytope
         * \param P the polytope
         * \return true if inclusion is guaranteed
         */
        virtual bool is_subset(const Polytope &P) const;

        /**
         *  Checks enclosure of another polytope
         * \param P the polytope
         * \return true if (*this) encloses P
         */
        bool is_superset(const Polytope &P) const;

        /**
         *  Checks inclusion in the interior of another polytope
         * \param P the polytope
         * \return true if inclusion is guaranteed
         */
        virtual bool is_interior_subset(const Polytope &P) const;

        /**  intersects a box 
         * \param x the box (IntervalVector) 
         * \return false if the intersection is guaranteed to be empty
         */
        bool intersects(const IntervalVector& x) const;
        
        /**  intersects a polytope
         * \param p the polytope
         * \return false if the intersection is guaranteed to be empty
         */
        bool intersects(const Polytope &p) const;

        /** test if it is disjoint with a polytope
         * \param p the polytope
         * \return true if the intersection is guaranteed to be empty
         */
        bool is_disjoint(const Polytope &p) const;

        /** compute the box hull of B diff (*this)
         *  \param b a box, contracted.
         */
        void contract_out_Box(IntervalVector &b) const;

        /** minimize the constraints, removing (possibly) redundant
         *  constraints.
         */
        virtual void minimize_constraints() const;


        /************* Box access *************************/
 
        /**
         *  get the current bounding box of the polytope
         * (note: it may not be tightest bounding box, it is the 
         *  bounding box used for approximations results) 
         *
         * \return The current bounding box
         */
        const IntervalVector &box(bool tight=true) const;

        /**
         *  test if bounding box is included
         * \return true if the polytope is included in x
         */
        bool box_is_subset(const IntervalVector& x) const;

        /** 
         *  tests if the polytope is bisectable 
	 * (i.e. its bounding box is bisectable).
         * do not update the box
         * \return true if the bbox is bisectable
         */
        bool is_bisectable() const;


        /************* Modification **********************/
        /* keeping the current polytope                  */

        /**  set to empty */
        void set_empty();

        /**  set to (singleton) 0^dim */
        virtual void clear();

        /**  add a inequality (pair row X <= rhs)
         *  do basic checks, but do not minimize the system
         *  \param facet the constraint
         *  \param tolerance tolerance for redundancy checking (CLP only)
         *  \return false if the (basic) checks showed the constraint to
         *  be redundant (or inside tolerance), true if added */
        bool add_constraint(const std::pair<Row,double>& facet,
		double tolerance=0.0);

        /**  add a inequality with intervalVector (cst X <= rhs)
         *  using the bounding box
         *  do basic checks, but do not minimize the system
         *  \param cst the row constraint
         *  \param rhs the rhs
         *  \param tolerance tolerance for redundancy checking (CLP only)
         *  \return false if the (basic) checks showed the constraint to
         *  be redundant (or inside tolerance), true if added */
        bool add_constraint(const IntervalRow &cst, double rhs,
		double tolerance=0.0);

        /**  two inequalities with intervalVector (cst X in rhs)
         *  using the bounding box
         *  do basic checks, but do not minimize the system. Also,
         *  should not be used for equalities (cst and rhs punctual)
         *  \param cst the row constraint
         *  \param rhs the rhs
         *  \param tolerance tolerance for redundancy checking (CLP only)
         *  \return pair of booleans (one for each constraints possibly added */
        std::pair<bool,bool> add_constraint_band(const IntervalRow &cst,
		 const Interval &rhs, double tolerance=0.0);
        std::pair<bool,bool> add_constraint_band(const Row &cst,
		 const Interval &rhs, double tolerance=0.0);

        /**  add a equality (pair row X = rhs)
         *  do basic checks, but do not minimize the system
         *  \param facet the constraint
         *  \return -1 if empty, 0 no change (not probable), 1 change */
        int add_equality(const std::pair<Row,double>& facet);

        /**  intersect with a box.
         *  \param b the box
         *  \return 0 if nothing done, 1 changed made, -1 results is empty */
        int meet_with_box(const IntervalVector &b);

        /**  intersect with a box.
         *  \param b the box
         *  \return *this */
        Polytope &operator&=(const IntervalVector &b);

        /**  intersect with a polytope.
         *  \param P the polytope
         *  \return 0 if nothing done, 1 changed made, -1 results is empty */
        int meet_with_polytope(const Polytope &P);

        /**  intersect with a polytope.
         *  \param P the polytope
         *  \return *this */
        Polytope &operator&=(const Polytope &P);

        /**  union with a polytope
         *  \param P the polytope
         *  \return 0 if nothing done, 1 changed made, -1 results is empty */
        int join_with_polytope(const Polytope &P);

        /**  union with a polytope
         *  \param P the polytope
         *  \return *this */
        Polytope &operator|=(const Polytope &P);

        /**  inflation by a cube, keeping the shape (not optimal)
         *  this <- this + [-rad,rad]^d
         *  \param rad radius of the box
         *  \return *this (keep minimized, except rounding)
         */
        Polytope &inflate(double rad);

        /**  inflation with a box, keeping the shape (not optimal)
         *  this <- this + box
         *  \param box the box
         *  \return *this (keep minimized, except rounding)
         */
        Polytope &inflate(const IntervalVector& box);

        /**  inflation by a ball, keeping the shape (not optimal)
         *  this <- this + B_d(rad) (note: vector norm is computed on double)
         *  \param rad radius of the ball
         *  \return *this (keep minimized, except rounding)
         */
        Polytope &inflate_ball(double rad);

        /**  expansion of a dimension (not optimal)
         *  equivalent to inflation by a box 0,...,[-rad,rad],...0
         *  \param dm index of the dimension
         *  \param rad radius 
         *  \return *this (keep minimized, except rounding) */
        Polytope &unflat(Index dm, double rad);

        /**  centered homothety (optimal if c is punctual)
         *  x <- [c] + delta*(x-[c]) ( or (1-delta)[c] + delta*x )
         *  note : delta must be > 0
         *  \param c ``center''
         *  \param delta expansion
         *  \return *this
         */
        Polytope &homothety(const IntervalVector &c, double delta);

        /* creating a new polytope                  */

        /**  return a polytope intersected with an hyperplan
	 * in a specific coordinate (rebuild the constraints without
         * any dependence on this coordinate in a new polytope)
         *  \param dm index of the coordinate
         *  \param x the value of the coordinate
         *  \return the polytope */
        Polytope meet_with_hyperplane(Index dm, double x) const;

        /**  reverse affine transformation 
	 *  compute { x' in Box s.t. ([M] x' + [P]) in the initial polytope }
         *  The bounding box is needed to handle interval in [M].
         *  \param M non-empty matrix
         *  \param P non-empty vector 
         *  \param bbox bouding-box of the new polytope
         *  \return a new polytope */
        Polytope reverse_affine_transform(const IntervalMatrix &M,
		const IntervalVector &P, const IntervalVector &bbox) const;

        /**  affine transformation 
	 *  compute { [B] cap ([M] x + [P]) s.t. x in the initial polytope }
         *  If [M] is bijective, use inverse to compute the inverse of M
         *  If [M] is full-rank and injective, use the
         *  LU decomposition of transpose(M) to compute
         *  the left inverse of M and the kernel of M.
         *  Otherwise, call direct-affine-transform.
	 *  if use_direct is true, use both methods if possible
         *  \param M matrix
         *  \param P non-empty vector 
	 *  \param B box limiting the result
	 *  \param use_direct use direct_affine_transform in addition of
	 *  the other method
         *  \return a new polytope */
        Polytope affine_transform(const IntervalMatrix &M,
			const IntervalVector &P, const IntervalVector &B,
			bool use_direct=false) const;

        /** affine transformation 
	 *  compute { [M] x + [P] s.t x in the initial polytope }
         *  Generate IntervalVector from the vertices, but not the
         *  the vertices of the IntervalVector themselves (hence not
         *  optimal if [M] and [P] are not punctual). As it uses vertices,
         *  it may be expensive. Use affine_transform to avoid use of
	 *  vertices (for injective transformations).
         *  \param M non-empty matrix
         *  \param P non-empty vector 
         *  \return a new polytope */
        Polytope direct_affine_transform(const IntervalMatrix &M,
		const IntervalVector &P) const;

        /** time-elapse transformation
	 *  compute { x + t [P] s.t x in the initial polytope and t in T }
         *  Generate IntervalVector from the vertices, but not the
         *  the vertices of the IntervalVector themselves (hence not
         *  optimal if [P] is not punctual). As it uses vertices,
         *  it may be expensive. An inexpensive but less optimal approach is
         *  to use inflate(T*P).
         *  \param P non-empty vector
         *  \param T non-empty interval
         *  \return a new polytope */
        Polytope time_elapse(const IntervalVector &P, const Interval &T) const;

        /**  sum of two polytopes, based on the sum of vertices
         *  (expensive computations)
         * \param p1 first polytope 
         * \param p2 second polytope 
         * \return a new polytope */
        friend Polytope operator+ (const Polytope &p1, const Polytope &p2);

        /** unary minus 
         *  \return the negation of the current polytope */
        Polytope operator- () const;
  

        /*********** Printing and other ``global access'' *********/
        /**
         *  output the bbox and the set of facets 
         * \param os the output stream
         * \param P the polytope
         * \return the stream
         */
        friend std::ostream& operator<<(std::ostream& os,
			const Polytope &P);

        /**
         *  Return the set of facets, as a map of pairs
         * FacetBase/FacetRhs. Note: calling any method
         * of the polytope (even const methods) can change the map. The 
         * bounding box is not included in the map (use box()). For
         * empty polytopes, the set is empty.
         * 
         * \return the facets
         */
        const CollectFacets::mapType &facets() const;

        /**
         *  Computes a set of vertices enclosing the polytope
         * 
         * \return set of vertices, as IntervalVectors
         */
        std::vector<IntervalVector> vertices() const;

        /**
         *  Computes the set of 3D facets
         * 
         * \return set of set of vertices, as Vectors
         */
        std::vector<std::vector<Vector>> vertices_3Dfacets() const;

        /**
         *  Computes the vertices as a 2D facet
         * 
         * \return set of set of vertices, as Vectors 
	 *  (mid of ``real'' vertices'')
         */
        std::vector<Vector> vertices_2Dfacet() const;


             

   protected:
      Index _dim;                      /* dimension */
      mutable IntervalVector _box;     /* bounding box */
      mutable std::shared_ptr<CollectFacets> _facets; 
	/* "native" formulation , may be shared by other formulations */
      mutable std::unique_ptr<DDbuildF2V> _DDbuildF2V; 
		/* DDbuildF2V formulation, if used */
      mutable std::unique_ptr<DDbuildV2F> _DDbuildV2F; 
		/* DDbuildV2F formulation, generally not used */

        /**  state of the polytope */
        enum POLSTATE {
            EMPTY,       /* is empty */
            NOTEMPTY,    /* is NOT empty */
            MINIMIZED,   /* minimized : redundant constraints removed */
            BOXUPDATED,  /* box is minimal */
            F2VFORM,     /* has an up-to-date F2V form */
            V2FFORM,     /* has an up-to-date V2F form */
            INVALID,     /* not initialised */
            SIZE_POLSTATE
        };
        using pol_state = std::bitset<SIZE_POLSTATE>;
        static constexpr pol_state pol_state_empty = 
			1<<EMPTY | 1<<MINIMIZED | 1<<BOXUPDATED;
        static constexpr pol_state pol_state_init = 
			1<<NOTEMPTY | 1<<MINIMIZED | 1<<BOXUPDATED;
        
      mutable pol_state state = pol_state_init;
      virtual void update_box() const;
      virtual bool check_empty() const;
      virtual void set_empty_private() const;
      void build_DDbuildF2V() const;

};


inline const IntervalVector &Polytope::box(bool tight) const { 
      if (tight) this->update_box();
      return _box; 
}
inline Index Polytope::dim() const { return _dim; }
inline Index Polytope::size() const { return _dim; }
inline bool Polytope::is_empty(bool check) const { 
	if (!check) return state[EMPTY]; 
	return this->check_empty(); }
inline bool Polytope::is_flat() const { return state[EMPTY] ||
		_box.is_degenerated() || (_facets && _facets->nbeqfcts()>0); }
inline bool Polytope::is_box() const {
    return (!_facets || _facets->nbfcts()==0);
}
inline Index Polytope::nb_facets() const { 
 	if (state[EMPTY]) return -1;
        if (!_facets) return (2*_dim);
        return (2*_dim + _facets->nbfcts());
     }

inline Index Polytope::nb_eq_facets() const { 
 	if (state[EMPTY]) return -1;
        Index cnt=0;
        for (Index i=0;i<_dim;i++) 
           if (_box[i].is_degenerated()) cnt++;
        return (cnt + _facets->nbeqfcts());
     }

inline void Polytope::set_empty_private() const {
   state = pol_state_empty;
   _box.set_empty();
   _DDbuildF2V=nullptr;
   _DDbuildV2F=nullptr;
   _facets=nullptr;
}


inline void Polytope::set_empty() {
}

inline const CollectFacets::mapType &Polytope::facets() const 
{ return this->_facets->get_map(); }

std::ostream& operator<<(std::ostream& os, const Polytope &P);
Polytope operator+ (const Polytope &p1, const Polytope &p2);

}
