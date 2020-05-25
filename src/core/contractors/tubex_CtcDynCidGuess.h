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
		 * todo: add comments
		 */
		CtcDynCidGuess(tubex::Fnc& fnc, double prec=0.05);
		/*
		 * todo: add comments
		 */
		bool contract(std::vector<Slice*> x_slice, std::vector<Slice*> v_slice, TPropagation t_propa);
		/*
		 * todo: add comments
		 */
		void ctc_fwd(Slice &x, Slice &v, std::vector<Slice*> x_slice, std::vector<Slice*> v_slice, int pos);
		/*
		 * todo: add comments
		 */
		void ctc_fwd(Slice &x, Slice &v, std::vector<Slice> x_slice, std::vector<Slice> v_slice, int pos);
		/*
		 * todo: add comments
		 */
		void create_slices(Slice & x_slice, std::vector<ibex::Interval> & slices, TPropagation t_propa);
		/*
		 * todo: add comments
		 */
		double get_prec();
		/*
		 * todo: put comments
		 */
		int get_propagation_engine();
		bool get_max_it();
		/*
		 * todo: add comments
		 */
		void set_prec(double prec);
		/*
		 * todo: add comments
		 */
		void set_propagation_engine(int engine);
		void set_max_it(bool max_it);
		/*
		 * todo: add comments
		 */
		void var3Bcheck(ibex::Interval remove_ub,int bound, int pos ,std::vector<Slice*> & x_slice,std::vector<Slice*> v_slice,TPropagation t_propa);
		/*
		 * todo: add comments
		*/
		void FullPropagationEngine(std::vector<Slice> & x_slice, std::vector<Slice> & v_slice, TPropagation t_propa);
		/*
		 * todo: add comments
		*/
		void AtomicPropagationEngine(std::vector<Slice> & x_slice, std::vector<Slice> & v_slice, TPropagation t_propa);

		void create_corners(std::vector<Slice> x_slices, std::vector< std::vector<double> > & points, TPropagation t_propa);
		std::vector<std::vector<double>> cart_product (const std::vector<std::vector<double>>& v);

		void set_s_corn(int s_strategy);
		int get_s_corn();

		void set_dpolicy(int d_policy);
		int get_dpolicy();

		void set_variant(int variation);

	private:
		double prec;
		tubex::Fnc& fnc;
		CtcDeriv ctc_deriv;
		int engine = 0;  //by default the propagation engine is atomic (faster)
		int s_strategy = 0 ;
		bool max_it = false;
		int d_policy = 0; // 0: nothing , 1: small , 2:big
	};
}

#endif /* SRC_CORE_CONTRACTORS_TUBEX_CTC3BGUESS_H_ */
