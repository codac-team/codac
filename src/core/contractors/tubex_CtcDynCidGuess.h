/*
 *  DynCidGuess class
 * ----------------------------------------------------------------------------
 * 	\date       2020
 *  \authors  	Victor Reyes, Gilles Trombettoni
 */

#ifndef __TUBEX_CTCDYNCIDGUESS_H__
#define __TUBEX_CTCDYNCIDGUESS_H__

#include "tubex_Ctc.h"
#include "tubex_Slice.h"
#include "ibex_Function.h"
#include "tubex_CtcDeriv.h"
#include <vector>
#include <cmath>
#include <ctime>


namespace tubex
{
	class CtcDynCidGuess : public Ctc{

	enum {lb,ub};

	public:
		/*
		 *
		 */
		CtcDynCidGuess(tubex::Function& fnc, double prec=0);
		/*
		 *
		 */
		bool contract(std::vector<Slice*> x_slice, std::vector<Slice*> v_slice, TPropagation t_propa);
		/*
		 *
		 */
		void ctc_fwd(Slice &x, Slice &v, std::vector<Slice*> x_slice, std::vector<Slice*> v_slice, int pos);
		/*
		 *
		 */
		void create_slices(Slice & x_slice, std::vector<ibex::Interval> & slices, TPropagation t_propa);
		/*
		 *
		 */
		double get_prec();
		/*
		 *
		 */
		void change_prec(double prec);
		/*
		 *
		 */
		void var3Bcheck(ibex::Interval remove_ub,int bound, int pos ,std::vector<Slice*> & x_slice,std::vector<Slice*> v_slice,TPropagation t_propa);



	private:
		double prec;
		tubex::Function& fnc;
		CtcDeriv ctc_deriv;
	};
}

#endif /* SRC_CORE_CONTRACTORS_TUBEX_CTC3BGUESS_H_ */
