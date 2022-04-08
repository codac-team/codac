//============================================================================
//                                 P Y I B E X
// File        : Union of separators with bounding box optimization
// Author      : Benoit Desrochers, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : May 14, 2012
//============================================================================

#ifndef __IBEX_SEP_UNION_BBOX__
#define __IBEX_SEP_UNION_BBOX__

#include "ibex_Sep.h"
#include "ibex_Array.h"

namespace codac {

/**
 * \ingroup iset
 *
 * \brief Union of separators
 *
 * SepInter(s1,...sn) generates a separator for the set
 *
 *             S1 \cup ... \cup Sn
 *
 * where Si is the set associated to si.
 *
 * For a box [x], SepInter performs:
 *
 *       x_out := s_1^{out}([x]) \cup ... \cup s_n^{out}([x]) and
 *       x_in  := s_1^{in}([x])  \cap ... \cap s_n^{in}([x])
 *
 * where s_i^{in} (resp. s_i^{out}) denotes the inner (resp. outer) contraction
 * of s_i.
 */
class SepUnionBbox : public ibex::Sep {
public:

	/**
     * \brief Create the union of all the separators in the list.
	 */
    SepUnionBbox(const ibex::Array<Sep>& list, std::vector<ibex::IntervalVector>&);


    /**
     * \brief Separate a box.
     */
   virtual void separate(ibex::IntervalVector &x_in, ibex::IntervalVector &x_out);

    /**
     * \brief The list of sub-separators.
	 */
    ibex::Array<Sep> list;

		/**
		 * \bried Bounding box used for each separator
		*/
		std::vector<ibex::IntervalVector> bbox;

};

} // end namespace ibex

#endif // __IBEX_SEP_UNION_BBOX__
