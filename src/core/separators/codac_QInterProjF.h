//============================================================================
//                                  I B E X
// File        : Q-intersection contracto
// Author      : Clément Carbonnel, Benoit Desrochers
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 30, 2012
// Last Update : Apr 30, 2012
//============================================================================

#ifndef __IBEX_CTC_Q_INTER_2_H__
#define __IBEX_CTC_Q_INTER_2_H__

#include "ibex_Ctc.h"
#include "ibex_Sep.h"
#include "ibex_Array.h"
#include "ibex_IntervalMatrix.h"

using ibex::IntervalVector;
using ibex::IntervalMatrix;
using ibex::Array;
using ibex::Ctc;
using ibex::Sep;
using ibex::Interval;



namespace codac {


IntervalVector qinter_projf(const Array<IntervalVector>& _boxes, int q);

/**
 * \ingroup contractor
 * \brief Q-intersection contractor.
 *
 */
class CtcQInterProjF : public Ctc {
public:
	/**
	 * \brief q-intersection on a list of contractors.
	 *
	 * The list itself is not kept by reference.
	 * \param list of contractor to use
	 * \param q number of allowed outlier
	 */
	CtcQInterProjF(const Array<Ctc>& list, int q);

	/**
	 * \brief Contract the box.
	 * 
	 * \param box: box to be contracted
	 */
	virtual void contract(IntervalVector& box);

	/**
	 * List of contractors
	 */
	Array<Ctc> list;

	/**
	 * \brief The number of contractors we have to intersect the
	 * result.
	 */
	int q;

protected:
	/**
	 * @brief store boxes for each contraction
	 * 
	 */
	IntervalMatrix boxes; 
};


class SepQInterProjF : public Sep {
public:
	/**
	 * \brief q-intersection on a list of separators.
	 *
	 * \param list : list of separators
	 * 				 The list itself is not kept by reference.
	 * \param q : the nunmber of constrains that can be relaxed
	 */
    SepQInterProjF(const Array<Sep>& list, int q = 0);


  	/**
     * @brief Separate a box. 
     * @see ibex::Sep
     * @param xin the n-dimensional box \f$[\mathbf{x}_{\textrm{in}}]\f$ to be inner-contracted
     * @param xout the n-dimensional box \f$[\mathbf{x}_{\textrm{out}}]\f$ to be outer-contracted
     */
  	virtual void separate(IntervalVector& xin, IntervalVector& xout);


	/**
	 * @brief Set the the number of allowed out object
	 * 
	 * @param q 
	 */
	void set_q(int q);

	/**
	 * \brief return the number of allowed outliers 
	 * \return the number of allowed outliers
	 */
	int get_q();

protected:

	/**
	 * \brief list of separators
	 */
	Array<Sep> list;

	/**
	 * \brief boxes_in : stores in boxes for each contraction
	 */
	IntervalMatrix boxes_in;
	/**
	 * \brief boxes_out : stores in boxes for each contraction
	 */
	IntervalMatrix boxes_out;

	/**
	 * \brief The number of contractors we have to intersect the
	 * result.
	 */
	int q;

};

/* ============================================================================
 	 	 	 	 	 	 	 inline implementation
  ============================================================================*/

inline CtcQInterProjF::CtcQInterProjF(const Array<Ctc>& list, int q) :
		Ctc(list), list(list), q(q), boxes(list.size(), nb_var) { }

inline SepQInterProjF::SepQInterProjF(const Array<Sep>& list, int q) :
		Sep(list[0].nb_var),
		list(list),
		boxes_in(list.size(), list[0].nb_var),
		boxes_out(list.size(), list[0].nb_var)
	{ this->set_q(q); }


inline void SepQInterProjF::set_q(int q){
	assert (q >= 0 || q < list.size());
	this->q = q;
}

inline int SepQInterProjF::get_q(){ return q; }



} // end namespace pyibex
#endif // __IBEX_CTC_Q_INTER_2_H__
