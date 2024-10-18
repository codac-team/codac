/** 
 *  \file
 *  Ctc Python binding (doc)
 *  Originated from the former pyIbex library (Benoît Desrochers)
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Benoît Desrochers
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

const char* DOCS_CTC_TYPE=
R"_doc(Contractor abscract class
All contractor need to inherit from this class and define the method *contract*
and initilize the Ctc class with the dimension of the contractor
i.e. the dimension of the box passed to contract function.

In python you can define yout own contractor
Example:
   >>> #  contractor on a 2 dimensionnal box
   >>> class myCtc(Ctc):
   >>> def __init__(self):
   >>>     Ctc.__init__(self, 2) # init Ctc abstract class
   >>>
   >>> def contract(self, box):
   >>>     return box
   >>>     pass

You can also make the intersection / union of contractors using & and |

Nota: 
  Since version 1.8.0 of pyibex, custom contractor must return the 
  contracted box at the end.  

Example:
  >>> ctc1 = CtcXXX
  >>> ctc2 = CtcXXX
  >>> # intersection
  >>> ctc3 = ctc1 & ctc2 # intersection
  >>> ctc4 = ctc1 | ctc3 & ctc2 # combinaison

)_doc";

const char* DOC_CTC_CONTRACT=
R"_doc(Contract the box (in-place)
Args:
    x (IntervalVector) : box to be contracted
)_doc";

const char* DOC_CTC_NB_VAR=
R"_doc(Return the number of variables this contractor works with.
)_doc";

const char* DOC_CTC_OR=
R"_doc(Union of contractors
)_doc";

const char* DOC_CTC_AND=
R"_doc(Intersection of contractors)_doc";

const char* DOC_CMPOP_TYPE=
R"_doc(Comparaison operators
  + LT  <
  + LEQ <=
  + EQ  =
  + GEQ >=
  + GT  >
)_doc";

const char* DOC_CTCUNION_TYPE=
R"_doc(Union of contractors
For a box [x] the union of {c_0,...c_n} performs
:math:`c_1([x]) \cup ... \cup c_n([x])`

Args:
  list<Ctc> list of contractors
Return:
  CtcUnion :
)_doc";

const char* DOC_CTCCOMPO_TYPE=
R"_doc(Intersection (composition) of contractors
For a box [x] the composition of {c_0,...c_n} performs
c_n(...(c_1(c_0([x])))).

Args:
  list<Ctc> list of contractors
Return:
  CtcCompo :
)_doc";

const char* DOC_CTCFWDBWD_TYPE=
R"_doc(Forward-backward contractor (HC4Revise).

Can be initialized with:
    + a function ( constraint f(x) cmp 0) default cmp = EQ
    + a function and a interval/box  f(x) in itv

Args:
    f (Function) : Function used for the Forward-backward
    op (CmpOp): comparaison operator
    itv_y (Interval): for constraint f(x) in itv_y
    box_y (IntervalVector): for constraint f(x) in box_y

Return:
    CtcFwdBwd: a Forward-backward contractor

Example:
    >>> f = Function("x","y", "(sin(x)^2 + 3*y)")
    >>> # Constaint f(x) <= O
    >>> ctc = CtcFwdBwd(f, LEQ)
    >>> # Constaint f(x) <= [0, 1]
    >>> ctc = CtcFwdBwd(f, Interval(0, 1))

)_doc";

const char* DOC_CTCINVERSE_TYPE=
R"_doc(Image of a contractor by a function in a forward-backaward manner
[x] = [x] \cap f^{-1}( ctc( f([x]) ) )

Args:
    ctc (Ctc): contrctor to use
    f (Function): function to use
Return:
    CtcInverse : contractor
)_doc";

const char* DOC_CTCNOTIN_TYPE=
R"_doc(Contractor for f(x) not-in y
wrt to f, y can be an Interval or a box.
Note:
    Only Interval and IntervalVector types are supported for now.
)_doc";

const char* DOC_CTCFIXPOINT_TYPE=
R"_doc(Fix point of a contractor
When the Hausdorff distance between
two iterations is less than ratio*diameter
the fix-point is considered to be reached.
Args:
    ctc  (Ctc): a contractor
    ratio (double) : stop ratio between two iterationl
)_doc";

const char* DOC_CTCQINTER_TYPE=
R"_doc(Exact relaxed intersection of a list of contractors
Args:
    list (list<Ctc>) : list of contractors
    q (int) : The number of contractors which have to intersect the result
)_doc";

const char* DOC_CTCQINTERPROJF_TYPE=
R"_doc(Exact relaxed intersection of a list of contractors using projection algorithm
Args:
    list (list<Ctc>) : list of contractors
    q (int) : The number of contractors which have to intersect the result
)_doc";

const char* DOC_CTCHULL_TYPE=
R"_doc(Compute the outer contraction from a separator using a paver.
  It returns the union of all inner and underterminated boxes.
Args:
    sep (Sep) : the separator to use
    eps (double) : accuracy of the approximation used by the paver
)_doc";

const char* DOC_CTCEXISTS_TYPE=
R"_doc(Projection of a contractor C assosiated with a constraint  such as:
  +  C_{exists}(x) <=> \exists y in [y_init] , C(x, y)
Args:
    C (Ctc) : the contractor to use
    eps (double) : accuracy of the approximation used by the paver
)_doc";


const char* DOC_CTCFORALL_TYPE=
R"_doc(Projection of a contractor C assosiated with a constraint  such as:
  +  C_{forall} <=> \forall y in [y_init] , C(x, y)
Args:
    C (Ctc) : the contractor to use
    eps (double) : accuracy of the approximation used by the paver
)_doc";

const char* DOC_CTCPOLAR_TYPE=
R"_doc(Polar contractor
Contraction associated with the change between Cartesian to Polar coordinates.
    x = \rho cos(\theta) and y = \rho sin(\theta)
)_doc";

const char* DOC_CTCPOLAR_CONTRACT1=
R"_doc(contract the input box
Args:
    box (IntervalVector): input box ([x],[y],[\rho],[\theta])
)_doc";

const char* DOC_CTCPOLAR_CONTRACT2=
R"_doc(contract the input intervals
Args:
    x (Interval): [x]
    x (Interval): [y]
    \rho (Interval): [\rho]
    \theta (Interval): [\theta]
)_doc";