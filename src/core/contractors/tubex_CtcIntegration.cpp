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
	CtcIntegration::CtcIntegration(tubex::Fnc& fnc, Ctc* slice_ctr): fnc(fnc), slice_ctr(slice_ctr),finaltime(-1)
	{

	}

	void CtcIntegration::contract(TubeVector& x, TubeVector& v, double time_dom, TPropagation t_propa, Ptype p_type)
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

		CtcPicard ctc_picard;
		ctc_picard.set_picard_subslices(10); //todo: setter for nb of subslices

		/*todo: how to start from any point inside the tube for picard?*/
		if (m_slice_picard_mode){
			if ((time_dom == x.domain().lb()) || (time_dom == x.domain().ub()))
				m_slice_picard_mode = true;
			else
				m_slice_picard_mode = false;
		}
		std::vector<Interval> idiff_values;
		if (p_type & integrodiff){
			for (int i = 0 ; i < x.nb_slices();i++)
				idiff_values.push_back(Interval(0.,0.));
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
			for (int i = 0 ; i < x_slice.size() ; i++){
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

				if(p_type & integrodiff){
					if (contract_idiff(x_slice,v_slice,x,nb_slices,idiff_values,t_propa)){
						if (t_propa & FORWARD)
							finaltime = x_slice[0]->domain().lb();
						else if (t_propa & BACKWARD)
							finaltime = x_slice[0]->domain().ub();
						if (m_incremental_mode)
							return;
					}
				}

				else if(dynamic_cast <CtcDynCid*> (slice_ctr)){
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

	void CtcIntegration::contract(TubeVector& x, double time_dom, TPropagation t_propa)
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
			IntervalVector envelope(x_slice.size()+1);
			envelope[0] = x_slice[0]->domain();
			for (int j = 0 ; j < x_slice.size() ; j++)
				envelope[j+1] = x_slice[j]->codomain();
			envelope = fnc.eval_vector(envelope);
			for (int j = 0 ; j < x_slice.size() ; j++)
				v_slice[j]->set_envelope(envelope[j]);

			for (int i = 0 ; i < x.size() ; i++)
				x_slice[i] = x_slice[i]->next_slice();
		}

		contract(x,v,time_dom,t_propa);
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

	std::pair<int,std::pair<double,double>> CtcIntegration::bisection_guess(TubeVector x, TubeVector v, Ctc* slice_ctr, tubex::Function& fnc, int variant){

		//variant 0 -> return immediately as soon as we find a potential gate
		//variant 1 -> return the largest gate in a slice
		//variant 2 -> return the largest gate in the complete tube

		/*variable - time of bisection - bisection point*/
		pair <int, pair <double,double> > bisection;

		double t_bisection;  //time of bisection (in t)
		double x_bisection; // value of bisection (in x)

		/*init pair*/
		bisection = make_pair(-1,make_pair(-1,-1));

		/*check if everything is ok*/
		assert(x.size() == v.size());
		assert(x.domain() == v.domain());
		assert(TubeVector::same_slicing(x, v));

		/*init all the tubes*/
		vector<Slice*> x_slice;
		vector<Slice*> v_slice;
		/*auxiliar tubes*/
		vector<Slice*> aux_x_slice; TubeVector aux_x = x;
		vector<Slice*> aux_v_slice; TubeVector aux_v = v;


		double max_diameter = -1;
		double gate_diam;
		for (int it = 0 ; it < 2 ; it++){
			//clean
			x_slice.clear(); v_slice.clear();
			aux_x_slice.clear(); aux_v_slice.clear();
			/*push slices for forward phase*/
			//for forward
			TPropagation t_propa;
			if (it == 0){
				t_propa = FORWARD;
				for (int i = 0 ; i < x.size() ; i++){
					x_slice.push_back(x[i].first_slice()); aux_x_slice.push_back(aux_x[i].first_slice());
					v_slice.push_back(v[i].first_slice()); aux_v_slice.push_back(aux_v[i].first_slice());
				}
			}
			//for backward
			else{
				t_propa = BACKWARD;
				for (int i = 0 ; i < x.size() ; i++){
					x_slice.push_back(x[i].last_slice()); aux_x_slice.push_back(aux_x[i].last_slice());
					v_slice.push_back(v[i].last_slice()); aux_v_slice.push_back(aux_v[i].last_slice());
				}
			}

			while (x_slice[0] != NULL){
				for (int i = 0 ; i < x.size() ; i++){
					if (t_propa & FORWARD){
						x_bisection = aux_x_slice[i]->output_gate().mid();
						t_bisection = aux_x_slice[i]->domain().ub();
						gate_diam = aux_x_slice[i]->output_gate().diam();
						aux_x_slice[i]->set_output_gate(x_bisection);
					}
					else if (t_propa & BACKWARD){
						x_bisection = aux_x_slice[i]->input_gate().mid();
						t_bisection = aux_x_slice[i]->domain().lb();
						gate_diam = aux_x_slice[i]->input_gate().diam();
						aux_x_slice[i]->set_input_gate(x_bisection);
					}
					if(dynamic_cast <CtcDynCid*> (slice_ctr)){
						CtcDynCid * cid = dynamic_cast <CtcDynCid*> (slice_ctr);
						cid->contract(aux_x_slice,aux_v_slice,t_propa);
					}

					else if(dynamic_cast <CtcDynCidGuess*> (slice_ctr)){
						CtcDynCidGuess * cidguess = dynamic_cast <CtcDynCidGuess*> (slice_ctr);
						cidguess->contract(aux_x_slice,aux_v_slice,t_propa);
					}
					else if(dynamic_cast <CtcDynBasic*> (slice_ctr)){
						CtcDynBasic * basic = dynamic_cast <CtcDynBasic*> (slice_ctr);
						basic->contract(aux_x_slice,aux_v_slice,t_propa);
					}

					for (int k = 0 ; k < aux_x_slice.size() ; k++){
						if (aux_x_slice[k]->is_empty()){
							if (variant == 0){
								bisection.first = i;
								bisection.second.first = t_bisection;
								bisection.second.second = x_bisection;
								return bisection;
							}
							else {
								if (gate_diam > max_diameter){
									bisection.first = i;
									bisection.second.first = t_bisection;
									bisection.second.second = x_bisection;
									max_diameter = gate_diam;
									break;
								}
							}
						}
					}
					//restore values for x and v
					for (int k = 0 ; k < aux_x_slice.size() ; k++){
						aux_x_slice[k]->set_envelope(x_slice[k]->codomain()); aux_v_slice[k]->set_envelope(v_slice[k]->codomain());
						aux_x_slice[k]->set_input_gate(x_slice[k]->input_gate()); aux_v_slice[k]->set_input_gate(v_slice[k]->input_gate());
						aux_x_slice[k]->set_output_gate(x_slice[k]->output_gate()); aux_v_slice[k]->set_output_gate(v_slice[k]->output_gate());
					}
				}

				if (variant == 1){
					if (bisection.first != -1)
						return bisection;
				}

				if (t_propa & FORWARD){
					for (int i = 0 ; i < x.size() ; i++){
						x_slice[i] = x_slice[i]->next_slice(); aux_x_slice[i] = aux_x_slice[i]->next_slice();
						v_slice[i] = v_slice[i]->next_slice(); aux_v_slice[i] = aux_v_slice[i]->next_slice();
					}
				}
				else if (t_propa & BACKWARD){
					for (int i = 0 ; i < x.size() ; i++){
						x_slice[i] = x_slice[i]->prev_slice(); aux_x_slice[i] = aux_x_slice[i]->prev_slice();
						v_slice[i] = v_slice[i]->prev_slice(); aux_v_slice[i] = aux_v_slice[i]->prev_slice();
					}
				}
			}
		}
		/*if variant 2 is selected, it will return here*/
		return bisection;
	}

	bool CtcIntegration::contract_idiff(std::vector<Slice*> x_slice, std::vector<Slice*> v_slice,TubeVector x,int id, std::vector<Interval> idiff_values,TPropagation t_propa){

		Interval to_try(x_slice[0]->domain());
		for (int i = 1 ; i < x_slice.size(); i++)
			assert(to_try == x_slice[i]->domain());

		bool fix_point_n;
		bool first_iteration = true;
		CtcDeriv ctc_deriv;
		ctc_deriv.set_fast_mode(true);
		do{
			fix_point_n = false;

			for (int i = 0 ; i < x_slice.size() ; i++){
//				TubeVector aux_vector_x = x;
				/*Fixpoint for each sub-slice at each tube*/
				double sx;
				double aux_envelope = x_slice[i]->codomain().diam();
				/*without polygons*/

				ctc_deriv.contract(*x_slice[i], *v_slice[i],t_propa);
				if (t_propa & FORWARD){
					Interval aux_codomain;
					/*todo: how to make this general?*/
					Interval integral_value = -5. * x.integral(x_slice[0]->domain().ub())[i];
					IntervalVector envelope(x_slice.size());
					for (int j = 0 ; j < x_slice.size() ; j++)
						envelope[j] = x_slice[j]->codomain();
					aux_codomain = fnc.eval_vector(envelope)[i]+ integral_value;
//					if (id > 0 ) aux_codomain+=idiff_values[id-1];
					v_slice[i]->set_envelope(aux_codomain);
					idiff_values[id]=integral_value;
					ctc_deriv.contract(*x_slice[i], *v_slice[i],t_propa);
				}
				if (x_slice[i]->is_empty()) return false;
			}
			if ((first_iteration) && !(fix_point_n))
				return false;

			first_iteration = false;

			/*For 1-dimensional problems it is not necessary to repeat the process*/
			if (x_slice.size() == 1)
				fix_point_n=false;
		} while(fix_point_n);

		return true;
	}

}
