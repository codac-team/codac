/*
 *  CtcDynCid Class
 * ----------------------------------------------------------------------------
 * 	\date       2020
 *  \authors  	Victor Reyes, Gilles Trombettoni
 */

#include "tubex_CtcDynCid.h"
#include <deque>


using namespace std;
using namespace ibex;


namespace tubex
{
	CtcDynCid::CtcDynCid(tubex::Function& fnc,int scid, double prec): fnc(fnc), scid(scid), prec(prec)
	{
		/*check inputs*/
		assert(scid > 0.);
		assert(prec >= 0);
	}

	bool CtcDynCid::contract(std::vector<Slice*> x_slice, std::vector<Slice*> v_slice, TPropagation t_propa)
	{
		//checks that the domain of each slice is the same.
		Interval to_try(x_slice[0]->domain());
		for (int i = 1 ; i < x_slice.size(); i++)
			assert(to_try == x_slice[i]->domain());

		//check if the gates used to contract are bounded
		for (int i = 0 ; i < x_slice.size(); i++){
			if ((t_propa & FORWARD) && (x_slice[0]->input_gate().is_unbounded()))
				return false;
			else if ((t_propa & BACKWARD) && (x_slice[0]->output_gate().is_unbounded()))
				return false;
		}
		//polygone method setter
		if(m_fast_mode)
			ctc_deriv.set_fast_mode(true);

		//save volume before contraction
		double volume_old = 0;
		for (int i = 0; i < x_slice.size() ; i++)
			volume_old = volume_old + x_slice[i]->volume();

		/*propagation engine*/
		bool fix_point;
		do{
			fix_point = false;
			double volume_1 = 0; double volume_2 = 0;
			for (int i = 0; i < x_slice.size() ; i++)
				volume_1 = volume_1 + x_slice[i]->volume();

			if (get_propagation_engine() == 0)
				AtomicPropagationEngine(x_slice,v_slice,t_propa);
			else if (get_propagation_engine() == 1)
				FullPropagationEngine(x_slice,v_slice,t_propa);
			for (int i = 0; i < x_slice.size() ; i++)
				volume_2= volume_2 + x_slice[i]->volume();
			if (1-(volume_2/volume_1) > get_prec()) fix_point = true;
		} while (fix_point);

		/*empty test*/
		for (int i = 0; i < x_slice.size() ; i++)
			if (x_slice[i]->is_empty()) return false;

		/*incrementality test*/
		double volume_new = 0;
		for (int i = 0; i < x_slice.size() ; i++)
			volume_new = volume_new + x_slice[i]->volume();

		if (volume_old == volume_new) return false;

		return true;
	}

	void CtcDynCid::ctc_fwd(Slice &x, Slice &v, std::vector<Slice*> x_slice, std::vector<Slice*> v_slice, int pos)
	{
		/*envelope*/
		IntervalVector envelope(x_slice.size());
		for (int i = 0 ; i < x_slice.size() ; i++){
			if (i==pos)
				envelope[i] = x.codomain();
			else
				envelope[i] = x_slice[i]->codomain();
		}
		v.set_envelope(fnc.eval_slice(x.domain(),envelope)[pos]);
	}


	void CtcDynCid::ctc_fwd(Slice &x, Slice &v, std::vector<Slice> x_slice, std::vector<Slice> v_slice, int pos)
	{
		/*envelope*/
		IntervalVector envelope(x_slice.size());
		for (int i = 0 ; i < x_slice.size() ; i++){
			if (i==pos)
				envelope[i] = x.codomain();
			else
				envelope[i] = x_slice[i].codomain();
		}

		v.set_envelope(fnc.eval_slice(x.domain(),envelope)[pos]);
	}

	double CtcDynCid::get_scid()
	{
		return this->scid;
	}

	double CtcDynCid::get_prec()
	{
		return this->prec;
	}

	int CtcDynCid::get_propagation_engine(){
		return this->engine;
	}

	void CtcDynCid::set_scid(int scid)
	{
		this->scid = scid;
	}

	void CtcDynCid::set_prec(double prec)
	{
		this->prec = prec;
	}

	void CtcDynCid::set_propagation_engine(int engine){
		this->engine = engine;
		if (this->engine == 0) this->set_prec(0);      //difficult to check, as the contraction is more volatile.
		else if (this->engine == 1) this->set_prec(0.01); // todo: seems a good tradeoff between cpu-time and contraction?
	}



	void CtcDynCid::FullPropagationEngine(std::vector<Slice*> x_slice, std::vector<Slice*> v_slice, TPropagation t_propa){

		/*create the contractor queue: format contraint - variable, 0: for ctc_deriv, 1 for fwd*/
		std::deque< vector<int> > contractorQ;

		vector<int> ctr_var;
		for (int i = 0 ; i < 2 ; i++) ctr_var.push_back(-1);

		/*Initialization contractor array - bool*/
		vector<bool > isPresent;

		for (int i = 0 ; i < x_slice.size() ; i++){
			isPresent.push_back(false);
		}

		/*going throw all the variables*/
		for (int i = 0 ; i < x_slice.size() ; i++){

			std::vector<ibex::Interval> x_subslices;
			x_subslices.clear();
			/*create the sub-slices*/
			create_subslices(*x_slice[i],x_subslices, t_propa);

			/*Hull for each dimension on x and v*/
			vector<Interval> hull_input_x; vector<Interval> hull_input_v;
			vector<Interval> hull_output_x; vector<Interval> hull_output_v;
			vector<Interval> hull_codomain_x; vector<Interval> hull_codomain_v;


			/*Initiliazation*/
			for (int j = 0 ; j < x_slice.size() ; j++){
				hull_input_x.push_back(Interval::EMPTY_SET); hull_input_v.push_back(Interval::EMPTY_SET);
				hull_output_x.push_back(Interval::EMPTY_SET); hull_output_v.push_back(Interval::EMPTY_SET);
				hull_codomain_x.push_back(Interval::EMPTY_SET); hull_codomain_v.push_back(Interval::EMPTY_SET);
			}

			for (int k = 0 ; k < x_subslices.size() ; k++){

				/*restore with the current domains*/
				vector<Slice> aux_slice_x; aux_slice_x.clear();
				vector<Slice> aux_slice_v; aux_slice_v.clear();

				for (int j = 0 ; j < x_slice.size() ; j++){
					aux_slice_x.push_back(*x_slice[j]);
					aux_slice_v.push_back(*v_slice[j]);
				}

				/*Set the gate depending on the direction of the contraction*/
				if (t_propa & FORWARD)
					aux_slice_x[i].set_input_gate(x_subslices[k]);
				else if (t_propa & BACKWARD)
					aux_slice_x[i].set_output_gate(x_subslices[k]);

				/*push the first element to the contractor queue*/
				ctr_var[0] = 0; ctr_var[1] = i;
				contractorQ.push_back(ctr_var);

				/*FIFO queue*/
				do{
					/*get what contractor should be called*/
					int contractor = contractorQ.front()[0];
					/*get the variable that is going to be contracted*/
					int variable = contractorQ.front()[1];
					isPresent[variable] = false;
					/*pop the first element*/
					contractorQ.pop_front();
					/*contract*/
					if (contractor == 0 ){ //call ctc_deriv
						/*save the corresponding domain*/
						double size_x = aux_slice_x[variable].codomain().diam();
						ctc_deriv.contract(aux_slice_x[variable],aux_slice_v[variable],t_propa);

						if ((1-(aux_slice_x[variable].codomain().diam()/size_x)) > this->get_prec()){
							ctr_var[0] = 1; ctr_var[1] = variable;
							contractorQ.push_front(ctr_var);
						}
					}
					else if (contractor == 1){ //call ctc_fwd
						/*save the corresponding domain*/
						double size_v = aux_slice_v[variable].codomain().diam();
						ctc_fwd(aux_slice_x[variable], aux_slice_v[variable], aux_slice_x, aux_slice_v, variable);

						/*add the contraints not included in isPresent*/
						if ((1-(aux_slice_v[variable].codomain().diam()/size_v)) > this->get_prec()){
							for (int j = 0 ; j < x_slice.size() ; j++ ){
								if ((!isPresent[j]) && (j!=variable)){
									ctr_var[0] = 1; ctr_var[1] = j;
									contractorQ.push_front(ctr_var);
									isPresent[j] = true;
								}
							}
							ctr_var[0] = 0; ctr_var[1] = variable;
							contractorQ.push_front(ctr_var);
						}
					}

				} while (contractorQ.size() > 0);

				/*The union of the current Slice is made*/
				for (int j = 0 ; j < x_slice.size() ; j++){
					hull_input_x[j] |= aux_slice_x[j].input_gate(); hull_input_v[j] |= aux_slice_v[j].input_gate();
					hull_output_x[j] |= aux_slice_x[j].output_gate(); hull_output_v[j] |= aux_slice_v[j].output_gate();
					hull_codomain_x[j] |= aux_slice_x[j].codomain(); hull_codomain_v[j] |= aux_slice_v[j].codomain();
				}
			}

			/*replacing the old domains*/
			for (int j = 0 ; j < x_slice.size() ; j++){
				x_slice[j]->set_envelope(x_slice[j]->codomain() & hull_codomain_x[j]); v_slice[j]->set_envelope(v_slice[j]->codomain() & hull_codomain_v[j]);
				x_slice[j]->set_input_gate(x_slice[j]->input_gate() & hull_input_x[j]); v_slice[j]->set_input_gate(v_slice[j]->input_gate() & hull_input_v[j]);
				x_slice[j]->set_output_gate(x_slice[j]->output_gate() & hull_output_x[j]); v_slice[j]->set_output_gate(v_slice[j]->output_gate() & hull_output_v[j]);
			}
		}
	}

	void CtcDynCid::AtomicPropagationEngine(std::vector<Slice*> x_slice, std::vector<Slice*> v_slice, TPropagation t_propa){

		for (int i = 0 ; i < x_slice.size() ; i++){

			std::vector<ibex::Interval> x_subslices;
			x_subslices.clear();

			/*create the sub-slices*/
			create_subslices(*x_slice[i],x_subslices, t_propa);

			/*For each slice on $t$ compute the corresponding the hull */
			Interval hull_input_x = Interval::EMPTY_SET; Interval hull_input_v = Interval::EMPTY_SET;
			Interval hull_output_x = Interval::EMPTY_SET; Interval hull_output_v = Interval::EMPTY_SET;
			Interval hull_codomain_x = Interval::EMPTY_SET; Interval hull_codomain_v = Interval::EMPTY_SET;

			/*treat each subslice, make the hull and then intersect*/
			for (int j = 0 ; j < x_subslices.size() ; j++){

				/*Temporal slices on $x$ and $v$*/
				Slice aux_slice_x(*x_slice[i]);
				Slice aux_slice_v(*v_slice[i]);

				if (t_propa & FORWARD)
					aux_slice_x.set_input_gate(x_subslices[j]);
				else if (t_propa & BACKWARD)
					aux_slice_x.set_output_gate(x_subslices[j]);

				/*Fixpoint for each sub-slice at each tube*/
				double sx;

				/*without polygons*/
				if(m_fast_mode)
					ctc_deriv.set_fast_mode(true);

				do
				{
					sx = aux_slice_x.volume();
					ctc_deriv.contract(aux_slice_x, aux_slice_v,t_propa);
					ctc_fwd(aux_slice_x, aux_slice_v, x_slice, v_slice, i);

				} while((1-(aux_slice_x.volume()/sx)) > get_prec());

				/*The union of the current Slice is made.*/
				hull_input_x |= aux_slice_x.input_gate(); hull_input_v |= aux_slice_v.input_gate();
				hull_output_x |= aux_slice_x.output_gate(); hull_output_v |= aux_slice_v.output_gate();
				hull_codomain_x |= aux_slice_x.codomain(); hull_codomain_v |= aux_slice_v.codomain();
			}

			double aux_envelope = x_slice[i]->codomain().diam();

			/*Replacing the old domains with the new ones*/

			x_slice[i]->set_envelope(hull_codomain_x); v_slice[i]->set_envelope(hull_codomain_v);
			x_slice[i]->set_input_gate(hull_input_x); v_slice[i]->set_input_gate(hull_input_v);
			x_slice[i]->set_output_gate(hull_output_x); v_slice[i]->set_output_gate(hull_output_v);
		}
	}

	void CtcDynCid::create_subslices(Slice& x_slice, std::vector<ibex::Interval> & x_slices, TPropagation t_propa)
	{
		/*Varcid in the input gate*/
		if (t_propa & FORWARD){
			double size_interval = x_slice.input_gate().diam()/get_scid();
			for (int i = 0 ; i < get_scid() ;i++){
				x_slices.push_back(Interval(x_slice.input_gate().lb()+i*size_interval,x_slice.input_gate().lb()+size_interval*(i+1)));
			}
		}

		/*Varcid in the output gate*/
		else if (t_propa & BACKWARD){
			double size_interval = x_slice.output_gate().diam()/get_scid();
			for (int i = 0 ; i < get_scid() ;i++){
				x_slices.push_back(Interval(x_slice.output_gate().lb()+i*size_interval,x_slice.output_gate().lb()+size_interval*(i+1)));
			}
		}
	}
}
