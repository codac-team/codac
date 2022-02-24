//============================================================================
//                                P Y I B E X
// File        : pyIbex_doc_IntervalVector.h
// Author      : Benoit Desrochers, Gilles Charbert
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Jul 06, 2016
// Comments    : Mainly taken from ibex_IntervalVector.h source file
//============================================================================

const char* DOCS_INTERVALVECTOR_TYPE =
R"doc_itv(An IntervalVector is a cartesian product of Intervals
The docs string is taken from ibex_IntervalVector.h source file
For more information read doc from http://www.ibex-lib.org/

An IntervalVector can be initialized with:

    + a int (i.e. size) only ( all elements are initialized to [-oo,oo])
    + a int (i.e. size) and a value for all elements
    + another IntervalVector (make a copy)
    + a list of bounds [[lb_1, ub_1], [...], [lb_n, ub_n]]
    + a tuple of intervals

Example:
    >>> a = IntervalVector(2)
    ([ ENTIRE ] ; [ ENTIRE ])
    >>> a = IntervalVector(2, Interval(1,5))
    ([1, 5] ; [1, 5])
    >>> a = IntervalVector(2, [1,5])
    ([1, 5] ; [1, 5])
    >>> a = IntervalVector([[1,2], [3,4]])
    ([1, 2] ; [3, 4])
    >>> a = IntervalVector( (Interval(2,4), Interval(5,6) ) )
    ([2, 4] ; [5, 6])

)doc_itv";

// const char* DOCS_INTERVALVECTOR_CONSTRUCT_01 =
// R"doc_itv(Construct an IntervalVector of dimension dim
// Args:
//     dim (int) : dimension of the IntervalVector.
// Note:
//    All elements are initialized to [-oo, +oo]
// Example:
//     a = IntervalVector(2)
//     >>> ([ ENTIRE ] ; [ ENTIRE ])
// )doc_itv";
//
//
// const char* DOCS_INTERVALVECTOR_CONSTRUCT_02 =
// R"doc_itv(Construct an IntervalVector of dimension dim
// Args:
//     dim (int) : dimension of the IntervalVector.
//     itv (Interval) : Initial
// Note:
//    All elements are initialized to [-oo, +oo]
// Example:
//     a = IntervalVector(2)
//     >>> ([ ENTIRE ] ; [ ENTIRE ])
// )doc_itv";

const char* DOCS_INTERVALVECTOR_EMPTY =
R"_docs(Create [empty; ...; empty]

  Create an empty IntervalVector of dimension \a n
  (all the components being empty Intervals)

  Args:
    n (int): dimension > 0

)_docs";


const char* DOCS_INTERVALVECTOR_SET_EMPTY =
R"_docs(Set this IntervalVector to the empty IntervalVector
    The dimension remains the same.
)_docs";

const char* DOCS_INTERVALVECTOR_CLEAR =
R"_docs(Set all the elements to 0 (even if empty).
Note:
    Emptiness is "overridden".
)_docs";

const char* DOCS_INTERVALVECTOR_INIT =
R"_docs(Set all the elements to x (even if empty).
Args:
    x (Interval) : value for elements
Note:
    Emptiness is "overridden".
)_docs";

const char* DOCS_INTERVALVECTOR_INFLATE =
R"_docs(Add [-rad,+rad] to all the components of self.

Args:
    rad (float): radius to use
Returns:
    self
)_docs";

const char* DOCS_INTERVALVECTOR_RESIZE =
R"_docs(Resize this IntervalVector.
If the size is increased, the existing components are not
modified and the new ones are set to (-inf,+inf), even if
(self) is the empty Interval (however, in this case, the status of
(self) remains "empty").

Args:
  n (int) : size of the new vector

)_docs";

const char* DOCS_INTERVALVECTOR_SUBVECTOR =
R"_docs(Return a subvector
Return:
    [ (self)[start_index]; ...; (self)[end_index] ].

Args:
  start_index (int): first index
  end_index (int): last index

Note:
  self must not be empty
)_docs";

const char* DOCS_INTERVALVECTOR_PUT =
R"_docs(Put a subvector into self at a given position.
Args:
  start_index - the position where the subvector
  subvec (IntervalVector): the subvector
Note:
    (self) must not be empty
)_docs";



const char * DOCS_INTERVALVECTOR_SIZE=
R"_docs(Returns:
    The dimension (number of components)
)_docs";

const char * DOCS_INTERVALVECTOR_LB=
R"_docs(Return the lower bound vector
Note:
   (self) must be nonempty
)_docs";

const char * DOCS_INTERVALVECTOR_UB=
R"_docs(Return the upper bound vector
Note:
   (self) must be nonempty
  /
)_docs";

const char * DOCS_INTERVALVECTOR_MID=
R"_docs(Return the midpoint
Note:
    (self) must be nonempty
)_docs";

const char * DOCS_INTERVALVECTOR_MIG=
R"_docs(Return the mignitude vector.
Note:
    (self) must be nonempty
)_docs";

const char * DOCS_INTERVALVECTOR_MAG=
R"_docs(Return the magnitude vector.
Note:
   (self) must be nonempty
)_docs";


const char* DOCS_INTERVALVECTOR_IS_EMPTY=
R"_docs(Return true iff this IntervalVector is empty
)_docs";

const char* DOCS_INTERVALVECTOR_IS_FLAT=
R"_docs(Return true iff this IntervalVector is flat.

   An IntervalVector is "flat" if the radius is 0 on at least one dimension
   An empty interval vector is considered as flat.

)_docs";

const char* DOCS_INTERVALVECTOR_IS_UNBOUNDED=
R"_docs(Return True iff this interval vector contains an infinite bound.

Note:
    An empty interval vector is always bounded.
)_docs";

const char* DOCS_INTERVALVECTOR_IS_SUBSET=
R"_docs(Return True iff this interval vector is a subset of \a x.
Dimension of \a x must be equal to the dimension of this vector.
Note:
    Always return true if this interval vector is empty.
See Also:
    #Interval.is_subset.
)_docs";

const char* DOCS_INTERVALVECTOR_IS_STRICT_SUBSET=
R"_docs(Return Return True iff this interval vector is inside the interior of \a x.
Dimension of \a x must be equal to the dimension of this vector.
Note:
    return true if this interval vector is empty and \a x not.
See Also:
    Interval.is_strict_subset.
)_docs";

const char* DOCS_INTERVALVECTOR_IS_INTERIOR_SUBSET=
R"_docs(Return True iff this interval vector is a subset of the interior of \a x.
Dimension of \a x must be equal to the dimension of this vector.
Note:
     Always return true if this interval vector is empty.
See Also:
    Interval.is_interior_subse
 )_docs";


const char* DOCS_INTERVALVECTOR_IS_STRICT_INTERIOR_SUBSET=
R"_docs(Return True iff this interval vector is a subset of the interior of \a x and different from x.
Dimension of \a x must be equal to the dimension of this vector.
Args:
    x (IntervalVector): to be compared
Note:
    Always return true if this interval vector is empty.
Sea Also:
    Interval.is_interior_subset.
)_docs";


const char* DOCS_INTERVALVECTOR_IS_SUPERSET=
R"_docs(Return True iff this interval vector is a superset of \a x.
Dimension of \a x must be equal to the dimension of this vector.
Args:
    x (IntervalVector): to be compared
Note:
    Always return true if \a x is empty.
See Also:
    Interval.is_superset.
)_docs";

const char* DOCS_INTERVALVECTOR_IS_STRICT_SUPERSET=
R"_docs(Return True iff \a x is inside the interior of (self).
Dimension of \a x must be equal to the dimension of this vector.
Args:
    x (IntervalVector): to be compared
Note:
    return true if x is empty and not (self).
See Also:
    Interval.is_strict_superset.
)_docs";

const char* DOCS_INTERVALVECTOR_CONTAINS=
R"_docs(True iff this interval vector contains \a x.
Dimension of \a x must be equal to the dimension of (self).
Args:
    x (IntervalVector): to be compared
See Also:
    pyibex.pyibex.Interval.contains.
)_docs";

const char* DOCS_INTERVALVECTOR_INTERIOR_CONTAINS=
R"_docs(True iff \a x is in the interior of this interval vector
Dimension of \a x must be equal to the dimension of (self).
Args:
    x (IntervalVector): to be compared
See Also:
    pyibex.pyibex.Interval.strictly_contain : same for Interval
)_docs";

const char* DOCS_INTERVALVECTOR_INTERSECTS=
R"_docs(True iff this interval vector intersects \a x.
Dimension of \a x must be equal to the dimension of (self).
Args:
    x (IntervalVector): to be compared
See Also:
    Interval.intersects.
)_docs";

const char* DOCS_INTERVALVECTOR_OVERLAPS=
R"_docs(True iff this interval vector intersects \a x and the intersection has a non-null volume
Dimension of \a x must be equal to the dimension of (self).
Args:
    x (IntervalVector): to be compared
See Also:
    Interval.strictly_intersects.
)_docs";

const char* DOCS_INTERVALVECTOR_IS_DISJOINT=
R"_docs(True iff this interval vector does not intersect \a x.
Dimension of \a x must be equal to the dimension of (self).
Args:
    x (IntervalVector): to be compared
See Also:
    Interval.is_disjoint.
)_docs";

const char* DOCS_INTERVALVECTOR_IS_ZERO=
R"_docs(Returns:
    bool : True iff self is a vector of zeros.
)_docs";

const char* DOCS_INTERVALVECTOR_IS_BISECTABLE=
R"_docs(Returns:
    bool : True iff self can be bisected along one dimension.
See Also:
     Interval.is_bisectable.
)_docs";

const char* DOCS_INTERVALVECTOR_RAD=
R"_docs(Returns:
    list : Vector of radii.
)_docs";

const char* DOCS_INTERVALVECTOR_DIAM=
R"_docs(Returns the vector of diameters.
)_docs";

const char* DOCS_INTERVALVECTOR_EXTR_DIAM_INDEX=
R"_docs(Return the index of a component with minimal/maximal diameter.
Args:
    min (bool) : true => minimal diameter
Raises:
    InvalidIntervalVectorOp if the IntervalVector is empty.
)_docs";

const char* DOCS_INTERVALVECTOR_SORT_INDICES=
R"_docs(Return the indices of all the components, sorted by increasing/decreasing diameter.
Returns:
  list : list of indices sorted by increasing/decreasing diameter.
Args:
  min (bool) : true => first indice corresponds to smallest diameter
)_docs";

const char* DOCS_INTERVALVECTOR_MAX_DIAM=
R"_docs(Return the maximal diameter among all the components.
Raises:
    InvalidIntervalVectorOp if the IntervalVector is empty.
)_docs";

const char* DOCS_INTERVALVECTOR_MIN_DIAM=
R"_docs(Return the minimal diameter among all the components.
Raises:
    InvalidIntervalVectorOp if the IntervalVector is empty
)_docs";

const char* DOCS_INTERVALVECTOR_VOLUME=
R"_docs(Return the volume of this interval vector.
Note:
    Return \c POS_INFINITY if the vector is unbounded and not flat.
Note:
    Return 0 if the vector is flat and not unbounded.
Warning:
    If the interval vector is both flat and unbounded, the result is undefined.
See Also:
    flat()
    unbounded()
)_docs";

const char* DOCS_INTERVALVECTOR_PERIMETER=
R"_docs(Return the perimeter of this interval vector.
Note:
    Return \c POS_INFINITY if unbounded.
)_docs";

const char* DOCS_INTERVALVECTOR_REL_DISTANCE=
R"_docs(Return the relative distance with x.
Args:
    x (IntervalVector)
Returns:
    \f$\displaystyle \max_{i=1..n} rel\_distance([this]_i, x_i)/diam([this]_i)\f$.
Sea also:
    distance
    Interval.rel_distance
)_docs";


const char* DOCS_INTERVALVECTOR_DIFF =
R"_docs(Return self \\ y (set difference).

Return:
    list<IntervalVector> : the difference under the form of a union of
    non-overlapping IntervalVectors. The lenght of the list is <= 2*n
Note:
    If the difference is empty, result is an array of one element set
    to the empty box. It is <b>not</b> a zero-sized array containing no element.
Args:
    y (IntervalVector) : to be compared
)_docs";

const char* DOCS_INTERVALVECTOR_COMPLEMENTARY =
R"_docs(Return the complementary of self.
Store the complementary under the form of a union of non-overlapping IntervalVectors,
into \a result, and return the size of the union.

If (self) is the empty set with n components, the complementary of (self) is
the n-dimensional box (-oo,oo)x...(-oo,oo).

If the complementary is empty, \a result is an array of one element set to the empty box.
It is **not** a zero-sized array containing no element.

Return:
    list<IntervalVector> : complementaries boxes.
Example:
    >>> a = IntervalVector([[2,3], [-2, 4]])
    ([2, 3] ; [-2, 4])
    >>> a.complementary()
    [([-inf, 2] ; [ ENTIRE ]),
     ([3, inf] ; [ ENTIRE ]),
     ([2, 3] ; [-inf, -2]),
     ([2, 3] ; [4, inf])]

)_docs";

const char* DOCS_INTERVALVECTOR_BISECT =
R"_docs(Bisect the box
The box is bisected along the dimension \a i
and with a ratio \a ratio. If (self)[i] is the interval [a,a+d]:

    + The first box of the result is (self)[0]x...x(self)[i-1]x[a+ratio*d]x...
    + The second box is (self)[0]x...x(self)[i-1]x[a+ratio*d,a+d]x...

Args:
    i (int) : dimension to besect along
    radio (float) : ratio Default  is 0.5. Need to belong to [0, 1]

Return:
    list<IntervalVector> : two sub intervalvectors.

)_docs";


const char* DOCS_INTERVALVECTOR_TOLIST=
R"_docs(Convert an IntervalVector  into a list

Return:
  list<double>: [x1_lb, x1_ub, x2_lb, x2_ub, ...., ]
)_docs";
