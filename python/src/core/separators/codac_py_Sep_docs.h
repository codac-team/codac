/** 
 *  \file
 *  Sep Python binding (doc)
 *  Originated from the former pyIbex library (Benoît Desrochers)
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Benoît Desrochers
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

const char* __DOC_SEP_TYPE=
R"_docs(Separator interface.
A separator is an operator that performs two independent and
complementary contractions. The separator is associated with
a set (noted S) and the first contraction (called "inner")
removes points inside S. The second contraction (called
"outer") removes points outside S.
In summary:
Given a box [x], the separator produces two sub-boxes
[x_in] and [x_out] that verify:

    + ([x] \\cap [x_in]) \\subset S
    + ([x] \\cap [x_out]) \\cap S = \\emptyset

For efficiency reasons, the separate(...) function takes only
two input-output arguments, x_in and x_out, each containing
initially a copy of the box [x].
A separator can also be viewed a as pair of contractors.

See Also:
    pyibex.SepCtcPair.

See Also:
    L. Jaulin and B. Desrochers (2014). "Introduction to the
    Algebra of Separators with Application to Path Planning".
    Engineering Applications of Artificial Intelligence volume 33, pp. 141-147.

All separator need to inherit from this class and define the method *separate*
and initilize the Sep class with the dimension of the separator
i.e. the dimension of the box passed to separate function.

In python you can define yout own separator

Example:
    >>> #  separator on a 2 dimensionnal boxes
    >>> class mySep(Sep):
    >>>     def __init__(self):
    >>>         Sep.__init__(self, 2) # init Sep abstract class
    >>>     def separate(self, box_in, box_out):
    >>>         return box_in, box_out
    >>>         pass

You can also make the intersection / union / restriction of separators using &, | and ~

Nota: 
  Since version 1.8.0 of pyibex, custom separator must return the 
  contracted boxes at the end.  

Example:
    >>> sep1 = SepXXX
    >>> sep2 = SepXXX
    >>> # intersection
    >>> sep3 = sep1 & sep2 # intersection
    >>> sep4 = sep1 | sep3 & sep2 # combinaison
    >>> sep5 =   ~sep4 | spe1

)_docs";

const char* __DOC_SEP_SEPARATE=R"_docs(
Separate a box in two sub-boxes.

Args:
    x_in  (IntervalVector): As input: the initial box. As output: result of the first ("inner") contraction
    x_out (IntervalVector): As input: the initial box. As output: the result of the second ("outer") contraction

Note:
    Precondition: x_in and x_out must be the same boxes.

)_docs";

const char* __DOC_SEP_NB_VAR=R"_docs(
The number of variables this separator works with.
)_docs";


const char* __DOC_SEP_OR=R"_docs(
Union a two separators

Example:
    >>> sep = sep1 | sep2
)_docs";

const char* __DOC_SEP_AND=R"_docs(
Intersection of two separators

Example:
    >>> sep = sep1 & sep2
)_docs";

const char* __DOC_SEP_INVERSE=R"_docs(
Inverse of a separator (operator not)

Example:
    >>> sep = ~sep1
)_docs";

const char* __DOC_SEP_SEPUNION=R"_docs(
Union of separators

Args:
    list<Sep>: list of separators
)_docs";

const char* __DOC_SEP_SEPINTER=R"_docs(
Intersection of separators

Args:
    list<Sep>: list of separators
)_docs";

const char* __DOC_SEP_SEPCTCPAIR=R"_docs(
  Build a separator with a pair of complementary contractors

Args:
    ctc_in (Ctc): inner contractor
    ctc_out (Ctc): outer contractor

Example:
    >>> f = Function("x[2]",  "x[0] + 2*x[1]")
    >>> ctc_out = CtcFwdBwd(f, LEQ)
    >>> ctc_in = CtcFwdBwd(f, GEQ)
    >>> sep = SepCtcPair(ctc_in, ctc_out)
)_docs";

const char* __DOC_SEP_SEPFWDBWD=R"_docs(
This separator applies inner and outer forward-backward
contractors to separate a box w.r.t. a constraint
:math:`f(x) in [y]`.

Args:
    f (Function): Function used for the Forward-backward
    op (CmpOp): comparaison operator
    itv_y (Interval): for constraint f(x) in itv_y
    box_y (IntervalVector): for constraint f(x) in box_y

Example:
    >>> # disk of raduis 2
    >>> f = Function("x", "y", "x^2+y^2 - 4")
    >>> sep = SepFwdBwd(f, LEQ)
)_docs";

const char* __DOC_SEP_SEPNOT=
R"_docs(Separator for :math:`f(x) \notin [y]`

See Also:
    pyibex.CtCNotIn
)_docs";

const char* __DOC_SEP_SEPQINTER=
R"_docs(
Exact relaxed intersection of a list of separators

The complexity of this algorithm is exponential wrt the dimension of the
input box.

Args:
    list (list<Ctc>) : list of separators
    q (int) : The number of separators which have to intersect the result

)_docs";

const char* __DOC_SEP_SEPQINTERPROJF=
R"_docs(
Approximation of the relaxed intersection.

Compute the approximation of the relaxed intersection of a list of separators
using a projection algorithm.

Args:
    list (list<Ctc>) : list of separators
    q (int) : The number of separators which have to intersect the result

)_docs";

const char* __DOC_SEP_SEPINVERSE=
R"_docs(Image of a separator by a function in a forward-backward manner

Args:
    sep (Sep) : Separator to use
    f (Function): Function to use

See Also
    pyibex.CtcInverse

)_docs";


const char* __DOC_SEP_SEPTRANSFORM=
R"_docs(Image of a separator by a function in a forward-backward manner with
  the analytical expression of f and f^{-1}
  It will return f(S) = { f o S_in o f^{-1}, f o S_out o f^{-1}}

Args:
    sep (Sep) : Separator to use
    ffwd (Function): forward expression of f
    fbwd (Function): reverse expression of f

See Also
    pyibex.CtcInverse, pyibex.SepInverse
)_docs";


const char* __DOC_SEP_SEPPROJ=
R"_docs(Projection of separator using the complementarity of the inner and outer
  contractor of the separator.
  Given a separator S and a box y_init, the projection of S is:
  ProjS(x)  <=> \exists y in [y_init] , S(x,y)
Args:
  sep (Sep): Separator to use
  y_init (IntervalVector): inital box for y

)_docs";
const char* __DOC_SEP_SEPCTCPAIRPROJ=
R"_docs(Projection of separator using the ibex implementation with CtcExist and ctcForAll
  ProjS = { CtcForAll(ctc_in), CtcExist(ctc_out)}
  Given a separator S={Sin, Sout} and a box y_init, the projection of ProjS is:
  ProjS = {ProjS_in, ProjS_out}, with
  + ProjS_out  <=> \exists y in [y_init] , Sout(x, y)
  + ProjS_in  <=> \forall y in [y_init], Sin(x, y)
Args:
  sep (Sep): Separator to use
  y_init (IntervalVector): inital box for y
)_docs";
