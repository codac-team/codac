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