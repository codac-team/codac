//============================================================================
//                                P Y I B E X
// File        : pyIbex_doc_Interval.h
// Author      : Benoit Desrochers, Gilles Charbert
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Jul 06, 2016
// Comments    : Mainly taken from ibex_Interval.h source file
//============================================================================

const char* DOCS_INTERVAL_TYPE =
R"doc_itv(An Interval represents a closed sub set of R

The docs string is taken from ibex_Interval.h source file
For more information read doc from http://www.ibex-lib.org/

Examples:
    >>> a = Interval(1,2)
    >>> a = Interval(2)
    >>> a = Interval.ALL_REALS
    >>> b = Interval(a)

Warning:
    For C++ user, recall that the assigment operator in python copies only references
    and not the object. a and b are referencing the same object. To make a copy used:

    For instance:
        >>> a = Interval(4)
        >>> b = a
        >>> b = a.copy()
        >>> b = Interval(a)

It is posible to acess ub and lower bound with lb() and ub() but also with
an array like syntax.

Example:
    >>> a = Interval(1,4)
    >>> a.lb()
    1
    >>> a[0]
    1
Note:
    Because intervals are immutable object, it is not possible to set directly
    the upper / lower bounds of the Interval object

)doc_itv";


const char* DOCS_INTERVAL_INFLATE =
R"doc_itv(Add [-rad,+rad] to self.
Returns:
     Return self.
)doc_itv";

const char* DOCS_INTERVAL_MID =
R"doc_itv(Returns the midpoint of self.

  The return point is guaranteed to be included in self
  but not necessarily to be the closest floating point
  from the real midpoint.
  Cases are:

    * \emptyset  -> Quiet NaN
    * [-oo, +oo] -> midP = 0.0
    * [-oo, b]   -> midP = -MAXREAL
    * [a, +oo]   -> midP = MAXREAL
    * [a, b]     -> midP ~ a + .5*(b-a)
)doc_itv";

const char* DOCS_INTERVAL_RAD =R"doc_itv(
Return the diameter of self.
By convention, 0 if self is empty.

)doc_itv";

const char* DOCS_INTERVAL_DIAM =R"doc_itv(
Return the diameter of self.
By convention, 0 if self is empty.

)doc_itv";

const char* DOCS_INTERVAL_MIG =R"doc_itv(
Returns the mignitude of self:

  * +(lower bound)  if self > 0
  * -(upper bound) if self < 0
  * 0 otherwise

)doc_itv";


const char* DOCS_INTERVAL_MAG =
R"doc_itv(Returns the magnitude of self

  mag(self)=max(``|lower bound|``, ``|upper bound|``).

)doc_itv";

const char* DOCS_INTERVAL_IS_SUBSET =
R"doc_itv(True iff this interval is a subset of \a x.

Note:
    Always return true if self is empty.

)doc_itv";

const char* DOCS_INTERVAL_IS_STRICT_SUBSET =
R"doc_itv(True iff this interval is a subset of x and not x itself.

Note:
    In particular, (-oo,oo) is not a strict subset of (-oo,oo)
    and the empty set is not a strict subset of the empty set although
    in both cases, the first is inside the interior of the second.

)doc_itv";

const char* DOCS_INTERVAL_IS_INTERIOR_SUBSET=
R"doc_itv(Returns:
bool: True iff this interval is in the interior of `x`.

Note:
    In particular, (-oo,oo) is in the interior of (-oo,oo)
    and the empty set is in the interior of the empty set.

Note:
    Always return true if self is empty.

)doc_itv";

const char* DOCS_INTERVAL_IS_STRICT_INTERIOR_SUBSET =
R"doc_itv(Returns:
  bool: True iff this interval is in the interior of \a x and different from x.

Note:
    In particular, (-oo,oo) is not "strictly" in the interior of (-oo,oo)
    and the empty set is not "strictly" in the interior of the empty set.
  )doc_itv";

const char* DOCS_INTERVAL_IS_SUPERSET =
R"doc_itv(True iff this interval is a superset of \a x.
Note:
    Always return true if x is empty.
  )doc_itv";

const char* DOCS_INTERVAL_IS_STRICT_SUPERSET =
R"doc_itv(True iff this interval is a superset of \a x different from x.

See:
    #is_strict_subset(const Interval&) const.
  )doc_itv";

const char* DOCS_INTERVAL_CONTAINS =
R"doc_itv(True iff self contains \a d.

Note: d can also be an "open bound", i.e., infinity.
    So this function is not restricted to a set-membership
    interpretation.
  )doc_itv";

const char* DOCS_INTERVAL_INTERIOR_CONTAINS =
R"doc_itv(True iff the interior of self contains \a d.
)doc_itv";

const char* DOCS_INTERVAL_INTERSECTS =
R"doc_itv(True iff self and \a x intersect.
)doc_itv";

const char* DOCS_INTERVAL_OVERLAPS =
R"doc_itv(True iff self and \a x intersect and the intersection
  has a non-null volume.
  Equivalently, some interior points (of this or x) must belong
  to the intersection.
)doc_itv";

const char* DOCS_INTERVAL_IS_DISJOINT =
R"doc_itv(True iff self and \a x do not intersect.
)doc_itv";

const char* DOCS_INTERVAL_IS_EMPTY =
R"doc_itv(True iff self is empty.
)doc_itv";

const char* DOCS_INTERVAL_IS_DEGENERATED =
R"doc_itv(True iff self is degenerated.

An interval is degenerated if it is of the form [a, a]

Note:
    An empty interval is considered here as degenerated.

)doc_itv";

const char* DOCS_INTERVAL_IS_UNBOUNDED =
R"doc_itv(Returns:
     True if one bound of self is infinite.
Note:
    An empty interval is always bounded.
)doc_itv";

const char* DOCS_INTERVAL_IS_BISECTABLE =
R"doc_itv(True iff self can be bisected into two non-degenerated intervals.

Examples of non bisectable intervals are [0,next_float(0)] or [DBL_MAX,+oo).

)doc_itv";

const char* DOCS_INTERVAL_REL_DISTANCE =
R"doc_itv(Relative Hausdorff distance between self and x.

The relative distance is basically distance(x)/diam(self).

See:
  #ibex::distance (const ibex::Interval &x1, const ibex::Interval &x2).

)doc_itv";

const char* DOCS_INTERVAL_COMPLEMENTARY =
R"doc_itv(Returns:
    The complementary of x.
)doc_itv";

const char* DOCS_INTERVAL_DIFF =
R"doc_itv(Returns:
   x\y
)doc_itv";

const char* DOCS_INTERVAL_BISECT =R"doc_itv(Bisect self into two subintervals.

The Interval musts be bisectable i.e. is_bisectable() must be true.

Args:
    ratio (float): says where to split (in [0, 1] , 0.5=middle)
Returns:
    list (itv1, itv2): two sub intervals
)doc_itv";
