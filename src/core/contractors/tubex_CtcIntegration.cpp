/*
 *  CtcIntegration Class
 * ----------------------------------------------------------------------------
 * 	\date       2020
 *  \authors  	Victor Reyes, Gilles Trombettoni
 */

#include "tubex_CtcIntegration.h"
#include "tubex_CtcDynBasic.h"
#include "tubex_CtcDynCid.h"
#include "tubex_CtcDynCidGuess.h"

using namespace std;
using namespace ibex;


namespace tubex
{
	CtcIntegration::CtcIntegration(tubex::Function& fnc, Ctc* slice_ctr): fnc(fnc), slice_ctr(slice_ctr),finaltime(-1)
	{

	}

//	void CtcIntegration
//	{
//
//		/*check if everything is ok*/
//		assert(x.size() == v.size());
//		assert(x.domain() == v.domain());
//		assert(TubeVector::same_slicing(x, v));
//
//		/*cpu time measurement*/
//		clock_t tStart = clock();
//
//		/*init all the tubes*/
//		vector<Slice*> x_slice;
//		vector<Slice*> v_slice;
//
//		/*set where to start with the contraction in the dom=[t0,tf]*/
//		/*if the contraction is from t=t0*/
//		if (time_dom <= x.domain().lb()){
//			for (int i = 0 ; i < x.size() ; i++){
//				x_slice.push_back(x[i].first_slice());
//				v_slice.push_back(v[i].first_slice());
//			}
//		}
//
//		/*if the contraction is from t=tf*/
//		else if (time_dom >= x.domain().ub()){
//			for (int i = 0 ; i < x.size() ; i++){
//				x_slice.push_back(x[i].last_slice());
//				v_slice.push_back(v[i].last_slice());
//			}
//		}
//
//		/*else the time is inside the interval [t0,tf]*/
//		else{
//			for (int i = 0 ; i < x.size() ; i++){
//				x_slice.push_back(x[i].slice(time_dom));
//				v_slice.push_back(v[i].slice(time_dom));
//			}
//			if (t_propa & FORWARD){
//				for (int i = 0 ; i < x.size() ; i++){
//					x_slice[i]=x_slice[i]->next_slice();
//					v_slice[i]=v_slice[i]->next_slice();
//				}
//			}
//		}
//
//		/*for each tube, go all over the slices*/
//		while(x_slice[0] != NULL){
//
//			/*todo: checking if correct..*/
//			for (int i = 0 ; i < v_slice.size() ; i++){
//				if (v_slice[i]->codomain().is_unbounded())
//					return;
//			}
//
//			if(dynamic_cast <CtcDynCid*> (slice_ctr)){
//				CtcDynCid * cid = dynamic_cast <CtcDynCid*> (slice_ctr);
//				if (!cid->contract(x_slice,v_slice,t_propa)){
//					if (t_propa & FORWARD)
//						finaltime = x_slice[0]->domain().lb();
//					else if (t_propa & BACKWARD)
//						finaltime = x_slice[0]->domain().ub();
//					if (m_incremental_mode)
//						return;
//				}
//			}
//
//			else if(dynamic_cast <CtcDynCidGuess*> (slice_ctr)){
//				CtcDynCidGuess * cidguess = dynamic_cast <CtcDynCidGuess*> (slice_ctr);
//				if (!cidguess->contract(x_slice,v_slice,t_propa)){
//					if (t_propa & FORWARD)
//						finaltime = x_slice[0]->domain().lb();
//					else if (t_propa & BACKWARD)
//						finaltime = x_slice[0]->domain().ub();
//					if (m_incremental_mode)
//						return;
//				}
//			}
//			else if(dynamic_cast <CtcDynBasic*> (slice_ctr)){
//				CtcDynBasic * basic = dynamic_cast <CtcDynBasic*> (slice_ctr);
//				if (!basic->contract(x_slice,v_slice,t_propa)){
//					if (t_propa & FORWARD)
//						finaltime = x_slice[0]->domain().lb();
//					else if (t_propa & BACKWARD)
//						finaltime = x_slice[0]->domain().ub();
//					if (m_incremental_mode)
//						return;
//				}
//			}
//			else{
//				cout << "ERROR: this sub-contractor is not handled by CtcIntegration" << endl;
//				return;
//			}
//
//			/*continue with the next slice*/
//			if (t_propa & FORWARD){
//				for (int i = 0 ; i < x.size() ; i++){
//					x_slice[i] = x_slice[i]->next_slice();
//					v_slice[i] = v_slice[i]->next_slice();
//				}
//			}
//			else if (t_propa & BACKWARD){
//				for (int i = 0 ; i < x.size() ; i++){
//					x_slice[i] = x_slice[i]->prev_slice();
//					v_slice[i] = v_slice[i]->prev_slice();
//				}
//			}
//		}
//		if (t_propa & FORWARD)
//			finaltime = x.domain().ub();
//		else if (t_propa & BACKWARD)
//			finaltime = x.domain().lb();
//	}
//
//	void CtcIntegration::contract(TubeVector& x, double time_dom, TPropagation t_propa, bool m_report)
//	{
//		/*v is computed*/
//		TubeVector v=x;
//		vector<Slice*> x_slice;
//		vector<Slice*> v_slice;
//
//		for (int i = 0 ; i < x.size() ; i++){
//			x_slice.push_back(x[i].first_slice());
//			v_slice.push_back(v[i].first_slice());
//		}
//		while(x_slice[0] != NULL){
//			IntervalVector envelope(x_slice.size());
//			for (int j = 0 ; j < x_slice.size() ; j++)
//				envelope[j] = x_slice[j]->codomain();
//			envelope = fnc.eval_slice(x_slice[0]->domain(),envelope);
//			for (int j = 0 ; j < x_slice.size() ; j++)
//				v_slice[j]->set_envelope(envelope[j]);
//
//			for (int i = 0 ; i < x.size() ; i++)
//				x_slice[i] = x_slice[i]->next_slice();
//		}
//
//		contract(x,v,time_dom,t_propa,m_report);
//	}


	void CtcIntegration::contract(TubeVector& x, TubeVector& v, double time_dom, TPropagation t_propa, bool m_report)
	{

		/*check if everything is ok*/
		assert(x.size() == v.size());
		assert(x.domain() == v.domain());
		assert(TubeVector::same_slicing(x, v));

		/*cpu time measurement*/
		clock_t tStart = clock();

		/*init all the tubes*/
		vector<Slice*> x_slice;
		vector<Slice*> v_slice;



		/*set where to start with the contraction in the dom=[t0,tf]*/
		/*if the contraction is from t=t0*/
		if (time_dom <= x.domain().lb()){
			for (int i = 0 ; i < x.size() ; i++){
				x_slice.push_back(x[i].first_slice());
				v_slice.push_back(v[i].first_slice());
			}
		}

		/*if the contraction is from t=tf*/
		else if (time_dom >= x.domain().ub()){
			for (int i = 0 ; i < x.size() ; i++){
				x_slice.push_back(x[i].last_slice());
				v_slice.push_back(v[i].last_slice());
			}
		}

		/*else the time is inside the interval [t0,tf]*/
		else{
			for (int i = 0 ; i < x.size() ; i++){
				x_slice.push_back(x[i].slice(time_dom));
				v_slice.push_back(v[i].slice(time_dom));
			}
			if (t_propa & FORWARD){
				for (int i = 0 ; i < x.size() ; i++){
					x_slice[i]=x_slice[i]->next_slice();
					v_slice[i]=v_slice[i]->next_slice();
				}
			}
		}

		/*counter for slices*/
		int nb_slices;
		if (t_propa & FORWARD) nb_slices = 0;
		else if (t_propa & BACKWARD) nb_slices = x.nb_slices()-1;
		//	cout << x << "  " << nb_slices << " volume " << x.volume() << endl;
		CtcPicard ctc_picard;
		//		ctc_picard.set_picard_subslices(500); //todo: setter for nb of subslices
		ctc_picard.set_picard_subslices(10);
		

		/*todo: how to start from any point inside the tube for picard?*/
		if (m_slice_picard_mode){
			if ((time_dom == x.domain().lb()) || (time_dom == x.domain().ub()))
				m_slice_picard_mode = true;
			else
				m_slice_picard_mode = false;
		}

		/*for each tube, go all over the slices*/
		while(x_slice[0] != NULL){

			/*if something is unbounded return*/
			if (m_slice_picard_mode){
				for (int i = 0 ; i < v_slice.size() ; i++){
					if (v_slice[i]->codomain().is_unbounded()){

					  ctc_picard.contract_picard_slice(fnc,x,nb_slices,t_propa);

					  v=fnc.eval_vector(x);
					  v_slice.clear();
					  for (int i = 0 ; i < x.size() ; i++)
					    v_slice.push_back(v[i].slice(nb_slices));
					  break;
					}
				}
			}

			bool contract_slice = true;
			for (int i = 0 ; i < v_slice.size() ; i++){
				if (v_slice[i]->codomain().is_unbounded()){
					if (m_incremental_mode)
						return;
					else{
						contract_slice = false;
						break;
					}
				}
			}

			if (contract_slice){
				if(dynamic_cast <CtcDynCid*> (slice_ctr)){
					CtcDynCid * cid = dynamic_cast <CtcDynCid*> (slice_ctr);
					if (!cid->contract(x_slice,v_slice,t_propa)){
						if (t_propa & FORWARD)
							finaltime = x_slice[0]->domain().lb();
						else if (t_propa & BACKWARD)
							finaltime = x_slice[0]->domain().ub();
						if (m_incremental_mode)
							return;
					}
				}

				else if(dynamic_cast <CtcDynCidGuess*> (slice_ctr)){
					CtcDynCidGuess * cidguess = dynamic_cast <CtcDynCidGuess*> (slice_ctr);
					if (!cidguess->contract(x_slice,v_slice,t_propa)){
						if (t_propa & FORWARD)
							finaltime = x_slice[0]->domain().lb();
						else if (t_propa & BACKWARD)
							finaltime = x_slice[0]->domain().ub();
						if (m_incremental_mode)
							return;
					}
				}
				else if(dynamic_cast <CtcDynBasic*> (slice_ctr)){
					CtcDynBasic * basic = dynamic_cast <CtcDynBasic*> (slice_ctr);
					if (!basic->contract(x_slice,v_slice,t_propa)){
						if (t_propa & FORWARD)
							finaltime = x_slice[0]->domain().lb();
						else if (t_propa & BACKWARD)
							finaltime = x_slice[0]->domain().ub();
						if (m_incremental_mode)
							return;
					}
				}
				else{
					cout << "ERROR: this sub-contractor is not handled by CtcIntegration" << endl;
					return;
				}
			}
			/*continue with the next slice*/
			if (t_propa & FORWARD){
				for (int i = 0 ; i < x.size() ; i++){
					x_slice[i] = x_slice[i]->next_slice();
					v_slice[i] = v_slice[i]->next_slice();
				}
			}
			else if (t_propa & BACKWARD){
				for (int i = 0 ; i < x.size() ; i++){
					x_slice[i] = x_slice[i]->prev_slice();
					v_slice[i] = v_slice[i]->prev_slice();
				}
			}
			//for picard_slice
			if (t_propa & FORWARD) nb_slices++;
			else if (t_propa & BACKWARD) nb_slices--;
		}


		if (t_propa & FORWARD)
			finaltime = x.domain().ub();
		else if (t_propa & BACKWARD)
			finaltime = x.domain().lb();
	}

	void CtcIntegration::contract(TubeVector& x, double time_dom, TPropagation t_propa, bool m_report)
	{
		/*v is computed*/
		TubeVector v=x;
		vector<Slice*> x_slice;
		vector<Slice*> v_slice;

		for (int i = 0 ; i < x.size() ; i++){
			x_slice.push_back(x[i].first_slice());
			v_slice.push_back(v[i].first_slice());
		}
		while(x_slice[0] != NULL){
			IntervalVector envelope(x_slice.size());
			for (int j = 0 ; j < x_slice.size() ; j++)
				envelope[j] = x_slice[j]->codomain();
			envelope = fnc.eval_slice(x_slice[0]->domain(),envelope);
			for (int j = 0 ; j < x_slice.size() ; j++)
				v_slice[j]->set_envelope(envelope[j]);

			for (int i = 0 ; i < x.size() ; i++)
				x_slice[i] = x_slice[i]->next_slice();
		}

		contract(x,v,time_dom,t_propa,m_report);
	}

	double CtcIntegration::get_finaltime()
	{
		return this->finaltime;
	}

	void CtcIntegration::set_incremental_mode(bool incremental_mode){
		this->m_incremental_mode = incremental_mode;
	}

	void CtcIntegration::set_picard_mode(bool slice_picard_mode){
		this->m_slice_picard_mode = slice_picard_mode;
	}

	
	void CtcIntegration::report(clock_t tStart,TubeVector& x,double old_volume)
	{
//		cout <<endl<< "----------Results for: " <<	dynamic_cast <ibex::Function&>(fnc)<<"----------"<<endl << endl;
//		/*CidSlicing does nothing, */
//		if (old_volume == x.volume()){
//			cout << "\033[1;31mNo contraction made by 3BGuess!\033[0m\n";
//			printf("CPU Time spent by 3BGuess: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
//		}
//		/*CidSlicing contracts the tube*/
//		else{
//			double doors_size = 0 ;
//			int nb_doors = 0;
//			for (int i = 0 ; i < x.size() ; i++){
//				Slice* x_slice = x[i].first_slice();
//				for (int j = 0 ; j < x[i].nb_slices() ; j++){
//					doors_size +=x_slice->output_gate().diam();
//					nb_doors++;
//					x_slice = x_slice->next_slice();
//				}
//			}
//			cout << "\033[1;31mContraction successful!  -  3BGuess\033[0m\n";
//			printf("CPU Time spent by 3BGuess: %.5f (s)\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
//			printf("Old Volume: %.7f\n", old_volume);
//			printf("New Volume: %.7f\n", x.volume());
//			printf("Average size of doors: %f\n\n", (double)doors_size/nb_doors);
}


}
