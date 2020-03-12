/*
 *  CtcCidSlicing class
 * ----------------------------------------------------------------------------
 * 	\date       2020
 *  \authors  	Victor Reyes, Gilles Trombettoni
 */

#ifndef __TUBEX_CTCCIDSLICING_H__
#define __TUBEX_CTCCIDSLICING_H__

#include "tubex_Ctc.h"
#include "tubex_Slice.h"
#include "ibex_Function.h"
#include "tubex_CtcDeriv.h"
#include <vector>
#include <time.h>

namespace tubex
{

	class CtcCidSlicing : public Ctc{

	public:
		/*
		 * CtcCidSlicing correspond to a generic contractor applied at the Slice level. It is based
		 * on the famous Cid-consistency used in continuous domains. It divides one slice
		 * in the corresponding x coordinate by several subslices of equal width (the number is
		 * defined by the variable $s_{cid}$). Then two contractors are applied: $C_{Deriv}$
		 * and C_{fwd}.
		 * After all the s_cid are treated, the Hull is applied and intersected with the
		 * corresponding slice. By default it uses scid=8 and prec=1e-7
		 */
		CtcCidSlicing(ibex::Fnc& fnc,int scid=8, double prec=0.);
		/*
		 * This method performs a contraction for the TubeVector x.
		 * Note that the timesteps between the Tubes of x must be identically the same.
		 */
		void contract(TubeVector& x, TubeVector& v, TPropagation t_propa, bool report=false);
		/*
		 * ctc_fwd_slices manages to make an evaluation of the current Slice in order to contract and update v
		 */
		void ctc_bwd(Slice &x, Slice &v, std::vector<Slice*> x_slice, std::vector<Slice*> v_slice, int pos);
		/*
		 * used to obtain the number of scid subslices.
		 */
		double get_scid();
		/*
		 *  used to obtain the current precision of the iterative method.
		 */
		double get_prec();
		/*
		 * changes the value of scid
		 */
		void set_scid(int scid);
		/*
		 * changes the value of the precision
		 */
		void set_prec(double prec);
		/*
		 * creates a certain number of subslices to be treated
		 */
		void create_subslices(Slice & x_slice, std::vector<ibex::Interval> & slices, TPropagation t_propa);

		void report(clock_t tStart,TubeVector& x, double old_volume);

	private:
		int scid;
		double prec;
		ibex::Fnc& fnc;
	};
}

#endif /* SRC_CORE_CONTRACTORS_TUBEX_CTCCIDSLICING_H_ */
