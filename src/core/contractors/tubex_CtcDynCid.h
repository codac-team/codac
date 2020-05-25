/*
 *  CtcDynCid class
 * ----------------------------------------------------------------------------
 * 	\date       2020
 *  \authors  	Victor Reyes, Gilles Trombettoni
 */

#ifndef __TUBEX_CTCDYNCID_H__
#define __TUBEX_CTCDYNCID_H__

#include "tubex_Ctc.h"
#include "tubex_Slice.h"
#include "tubex_CtcDeriv.h"
#include <vector>


namespace tubex
{

	class CtcDynCid : public Ctc{

	public:
		/*
		 * CtcDynCid is a contractor applied at the Slice level, based
		 * on the Cid-consistency used in continuous domains. It divides the input (or output) gate
		 * on a given dimension of x by several subintervals of equal width (the number is
		 * defined by the variable $s_{cid}$). Then two contractors are applied: $C_{Deriv}$
		 * and C_{fwd}.
		 */
		CtcDynCid(tubex::Fnc& fnc,int scid=8, double prec=0.);
		/*
		 * This method performs a contraction at the Slice level.
		 * Note that the timesteps between the Tubes of x and v must be identically the same.
		 */
		bool contract(std::vector<Slice*> x_slice, std::vector<Slice*> v_slice, TPropagation t_propa);
		/*
		 * creates a certain number of subslices to be treated
		 */
		void create_subslices(Slice & x_slice, std::vector<ibex::Interval> & slices, TPropagation t_propa);

		/*
		 * ctc_fwd manages to make an evaluation of the current Slices in order to contract and update v
		 */
		void ctc_fwd(Slice &x, Slice &v, std::vector<Slice*> x_slice, std::vector<Slice*> v_slice, int pos);
		void ctc_fwd(Slice &x, Slice &v, std::vector<Slice> x_slice, std::vector<Slice> v_slice, int pos);
		/*
		 * used to obtain the number of scid subslices.
		 */
		double get_scid();
		/*
		 *  used to obtain the current precision of the iterative method.
		 */
		double get_prec();
		/*
		 * used to obtain what kind of propagation is going to be used-> 0: atomic (simple),
		 *  1: complete (stronger, but slower)
		 */
		int get_propagation_engine();
		/*
		 * changes the value of scid
		 */
		void set_scid(int scid);
		/*
		 * changes the propagator
		 */
		void set_propagation_engine(int engine);
		/*
		 * changes the value of the precision
		*/
		void set_prec(double prec);
		/*
		 * todo: add comments
		*/
		void FullPropagationEngine(std::vector<Slice*> x_slice, std::vector<Slice*> v_slice, TPropagation t_propa);
		/*
		 * todo: add comments
		*/
		void AtomicPropagationEngine(std::vector<Slice*> x_slice, std::vector<Slice*> v_slice, TPropagation t_propa);
	private:
		int scid;
		double prec;
		tubex::Fnc& fnc;
		CtcDeriv ctc_deriv;
		int engine = 0;  //by default the propagation engine is atomic (faster)
	};
}

#endif /* SRC_CORE_CONTRACTORS_TUBEX_CTCDYNCID_H_ */
