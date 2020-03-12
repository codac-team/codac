/*
 *  3BGuess class
 * ----------------------------------------------------------------------------
 * 	\date       2020
 *  \authors  	Victor Reyes, Gilles Trombettoni
 */

#ifndef __TUBEX_CTC3BGUESS_H__
#define __TUBEX_CTC3BGUESS_H__

#include "tubex_Ctc.h"
#include "tubex_Slice.h"
#include "ibex_Function.h"
#include "tubex_CtcDeriv.h"
#include <vector>
#include <time.h>


namespace tubex
{
	class Ctc3BGuess : public Ctc{
		enum {lb,ub};

	public:

		Ctc3BGuess(ibex::Fnc& fnc,int bisections=20, double prec=1e-7);

		void contract(TubeVector& x, TubeVector& v, TPropagation t_propa = FORWARD | BACKWARD, bool report=true);

		void ctc_bwd(Slice &x, Slice &v, std::vector<Slice*> x_slice, std::vector<Slice*> v_slice, int pos);

		int get_bisections();

		double get_prec();

		void create_slices(Slice & x_slice, std::vector<ibex::Interval> & slices, TPropagation t_propa);

		void change_bisections(int bisections);

		void change_prec(double prec);

		void var3Bcheck(ibex::Interval remove_ub,int bound, int pos ,std::vector<Slice*> & x_slice,std::vector<Slice*> v_slice,TPropagation t_propa);

		void report(clock_t tStart,TubeVector& x, double old_volume);

	private:
		int bisections;
		double prec;
		ibex::Fnc& fnc;
	};
}

#endif /* SRC_CORE_CONTRACTORS_TUBEX_CTC3BGUESS_H_ */
