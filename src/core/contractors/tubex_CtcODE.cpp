/*
 *  CtcDynCid Class
 * ----------------------------------------------------------------------------
 * 	\date       2020
 *  \authors  	Victor Reyes, Gilles Trombettoni
 */

#include "tubex_CtcODE.h"

using namespace std;
using namespace ibex;


namespace tubex
{	//ctc_integration, observations->input_times
	CtcODE::CtcODE(std::vector<double> input_times, CtcIntegration ctc_integration): input_times(input_times), ctc_integration(ctc_integration)
	{
		/*check input, size must be greater than 0*/
		assert(input_times.size() > 0.);
		/*check if it is sorted?*/

	}

	void CtcODE::contract(TubeVector& x, TubeVector& v)
	{

		/*FORWARD Phase*/
		for (int i = 0 ; i < input_times.size() ; i++){
			ctc_integration.contract(x,v,input_times[i],FORWARD);
			int ii = 0;
			for (int j = i+1 ; j < input_times.size() ; j++){
				if (ctc_integration.get_finaltime() > input_times[j])
					ii++;
			}
			i+=ii;
		}

		/*BACKWARD Phase*/
		for (int i = input_times.size()-1 ; i >= 0 ; i--){
			ctc_integration.contract(x,v,input_times[i],BACKWARD);
			int ii = 0;
			for (int j = i ; j >=0 ; j--){
				if (ctc_integration.get_finaltime() < input_times[j])
					ii--;
			}
			i+=ii;
		}
	}
}
