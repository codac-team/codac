/*
 *  CtcIntegration class
 * ----------------------------------------------------------------------------
 * 	\date       2020
 *  \authors  	Victor Reyes, Gilles Trombettoni
 */

#ifndef __TUBEX_CTCINTEGRATION_H__
#define __TUBEX_CTCINTEGRATION_H__

#include "tubex_Ctc.h"
#include "tubex_Slice.h"
#include "tubex_CtcPicard.h"
#include <vector>
#include <ctime>
#include <utility>


namespace tubex
{

	enum Ptype
	{
	  ode = 0x01,
	  integrodiff = 0x02
	};
	class CtcIntegration : public Ctc{

	public:

		/*
		 * CtcIntegration is a contractor that works at the Tube level. It requires as input an evolution function and a Slice contractor.
		 * Currently, the slice contractors that can be used are: CtcDynCid and CtcDynCidGuess
		 */
		CtcIntegration(tubex::Fnc& fnc, Ctc* slice_ctr);
		/*
		 * This method performs a contraction for the TubeVector x.
		 * Note that the timesteps between the Tubes of x must be identically the same.
		 */
		void contract(TubeVector& x, TubeVector& v, double time_dom, TPropagation t_propa, Ptype p_type=ode);
		/*
		 * Same as above, but v is computed inside.
		 */
		void contract(TubeVector& x, double time_dom, TPropagation t_propa);
		/*
		 * ctc_fwd manages to make an evaluation of the current Slice in order to contract and update v
		 */
		void ctc_fwd(Slice &x, Slice &v, std::vector<Slice*> x_slice, std::vector<Slice*> v_slice, int pos);
		/*
		 * returns the final time reached during contraction
		 */
		double get_finaltime();

		/*
		 * If true, it perform the picard method at the slice level.
		 */

		void set_picard_mode(bool slice_picard = false);
		/*
		 * todo: add comments
		 */
		bool contract_idiff(std::vector<Slice*> x_slice, std::vector<Slice*> v_slice,TubeVector x, int id, std::vector<ibex::Interval> idiff_values,TPropagation t_propa);
		/*
		 * If true, it contracts the tube until no contraction is obtained at the slice level.
		 */
		void set_incremental_mode(bool incremental_mode = true);

		/*temporal function*/
		std::pair<int,std::pair<double,double>> bisection_guess(TubeVector x, TubeVector v, Ctc* slice_ctr, tubex::Function& fnc, int variant);

	private:
		bool m_incremental_mode = true;
		Ctc* slice_ctr;
		tubex::Fnc& fnc;
		double finaltime;
		bool m_slice_picard_mode = false;
	};
}

#endif /* SRC_CORE_CONTRACTORS_TUBEX_CTCINTEGRATION_H_ */
