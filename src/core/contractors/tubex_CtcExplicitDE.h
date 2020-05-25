/*
 *  CtcODE class
 * ----------------------------------------------------------------------------
 * 	\date       2020
 *  \authors  	Victor Reyes, Gilles Trombettoni
 */

#ifndef __TUBEX_CTCEXPLICITDE_H__
#define __TUBEX_CTCEXPLICITDE_H__

#include "tubex_CtcIntegration.h"
#include "tubex_Ctc.h"
#include <vector>

namespace tubex
{

	class CtcExplicitDE : public Ctc{

	public:

		CtcExplicitDE(std::vector<double> input_times, CtcIntegration ctc_integration);
		/*
		 * This method performs a contraction for the TubeVector x.
		 * Note that the timesteps between the Tubes of x must be identically the same.
		 */
		void contract(TubeVector& x, TubeVector& v);

	private:

		std::vector<double> input_times;
		CtcIntegration ctc_integration;
	};
}

#endif /* SRC_CORE_CONTRACTORS_TUBEX_CTCODE_H_ */
