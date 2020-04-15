/*
 *  CtcBox class
 * ----------------------------------------------------------------------------
 * 	\date       2020
 *  \authors  	Victor Reyes
 */
#include "tubex_CtcDynCidGuess.h"

using namespace std;
using namespace ibex;



namespace tubex
{
	CtcDynCidGuess::CtcDynCidGuess(tubex::Function& fnc, double prec): fnc(fnc), prec(prec)
	{
		assert(prec >= 0);
	}

	bool CtcDynCidGuess::contract(std::vector<Slice*> x_slice, std::vector<Slice*> v_slice, TPropagation t_propa)
	{
		//checks that the domain of each slice is the same.
		Interval to_try(x_slice[0]->domain());

		for (int i = 0 ; i < x_slice.size(); i++){
			assert(to_try == x_slice[i]->domain());
//			if (t_propa & FORWARD)
//				if (x_slice[0]->input_gate().diam() != 0) puntual_gate = false;
		}

		//check if the gates used to contract are bounded
		for (int i = 0 ; i < x_slice.size(); i++){
			if ((t_propa & FORWARD) && (x_slice[0]->input_gate().is_unbounded()))
				return false;
			else if ((t_propa & BACKWARD) && (x_slice[0]->output_gate().is_unbounded()))
				return false;
		}

		bool fix_point_l;
		bool m_iter = true;
		bool first_iteration = true;
		do{

			fix_point_l = false;
			/*with the bounds of each variable compute the impact on the output door*/
			vector<Slice> x_slice_bounds;
			vector<Slice> v_slice_bounds;
//			x_slice_bounds.clear(); v_slice_bounds.clear();

			for (int i = 0 ; i < x_slice.size() ; i++){
				x_slice_bounds.push_back(*x_slice[i]);
				v_slice_bounds.push_back(*v_slice[i]);
			}

			/*Guess procedure, just using the bounds*/
			bool fix_point_n;
			do{
				fix_point_n = false;
				/*work for each dimension*/

				for (int i = 0 ; i < x_slice.size() ;i++){

					std::vector<Interval> x_subslices;
					x_subslices.clear();
					create_slices(x_slice_bounds[i],x_subslices, t_propa);


					Interval hull_input_x = Interval::EMPTY_SET; Interval hull_input_v = Interval::EMPTY_SET;
					Interval hull_output_x = Interval::EMPTY_SET; Interval hull_output_v = Interval::EMPTY_SET;
					Interval hull_codomain_x = Interval::EMPTY_SET; Interval hull_codomain_v = Interval::EMPTY_SET;

					for (int j = 0 ; j < x_subslices.size() ; j++){

						/*Temporal slices on $x$ and $v$*/
						Slice aux_slice_x(x_slice_bounds[i]);
						Slice aux_slice_v(v_slice_bounds[i]);

						if (t_propa & FORWARD)
							aux_slice_x.set_input_gate(x_subslices[j]);
						else if (t_propa & BACKWARD)
							aux_slice_x.set_output_gate(x_subslices[j]);

						/*Fixpoint for each sub-slice at each tube*/
						double sx;
						int max_iterations=0;
						/*without polygons*/
						if (m_fast_mode)
							ctc_deriv.set_fast_mode(true);
						do
						{
							sx = aux_slice_x.volume();
							ctc_deriv.contract(aux_slice_x, aux_slice_v,t_propa);
							ctc_fwd(aux_slice_x, aux_slice_v, x_slice, v_slice, i);
							max_iterations++;
						} while( (sx-aux_slice_x.volume()>0) && (max_iterations<50));

						if (max_iterations < 50) m_iter = false;

						/*The union of the current guess is made.*/
						if (t_propa & BACKWARD){
							hull_input_x |= aux_slice_x.input_gate(); hull_input_v |= aux_slice_v.input_gate();
						}
						else if (t_propa & FORWARD){
							hull_output_x |= aux_slice_x.output_gate(); hull_output_v |=aux_slice_v.output_gate();
						}
						hull_codomain_x |= aux_slice_x.codomain(); hull_codomain_v |= aux_slice_v.codomain();
					}

					double volume = x_slice_bounds[i].volume();

					/*Intersection in all the dimensions*/
					x_slice_bounds[i].set_envelope(hull_codomain_x & x_slice_bounds[i].codomain() );  v_slice_bounds[i].set_envelope(hull_codomain_v & v_slice_bounds[i].codomain());
					if (t_propa & BACKWARD){
						x_slice_bounds[i].set_input_gate(hull_input_x & x_slice_bounds[i].input_gate()); v_slice_bounds[i].set_input_gate(hull_input_v & v_slice_bounds[i].input_gate());
					}
					else if (t_propa & FORWARD){
						x_slice_bounds[i].set_output_gate(hull_output_x & x_slice_bounds[i].output_gate()); v_slice_bounds[i].set_output_gate(hull_output_v & x_slice_bounds[i].output_gate());
					}

					if (volume > x_slice_bounds[i].volume())
						fix_point_n = true;
				}
			} while(fix_point_n);


			/*3B part*/
			for (int i = 0 ; i < x_slice.size() ; i++){
				double aux_envelope = x_slice[i]->codomain().diam();
				Interval remove_ub,remove_lb;
				if (t_propa & FORWARD){
					remove_ub = Interval(x_slice_bounds[i].output_gate().ub(),x_slice[i]->output_gate().ub());
					remove_lb = Interval(x_slice[i]->output_gate().lb(),x_slice_bounds[i].output_gate().lb());
				}
				else if (t_propa & BACKWARD){
					remove_ub = Interval(x_slice_bounds[i].input_gate().ub(),x_slice[i]->input_gate().ub());
					remove_lb = Interval(x_slice[i]->input_gate().lb(),x_slice_bounds[i].input_gate().lb());
				}

				/*Check of the guess*/
				if (remove_ub.diam()>0){
					var3Bcheck(remove_ub,ub,i,x_slice,v_slice,t_propa);
				}
				if (remove_lb.diam()>0){
					var3Bcheck(remove_lb,lb,i,x_slice,v_slice,t_propa);
				}
				/*to update domains*/
				ctc_deriv.contract(*x_slice[i], *v_slice[i],t_propa);
				ctc_fwd(*x_slice[i],*v_slice[i], x_slice, v_slice, i);
				ctc_deriv.contract(*x_slice[i], *v_slice[i],t_propa);

				/*empty test*/
				if (x_slice[i]->is_empty()) return false;
				if (aux_envelope > x_slice[i]->codomain().diam())
					fix_point_l = true;
			}

			if ((first_iteration) && !(fix_point_l))
				return false;

			first_iteration = false;
			if (!m_iter) fix_point_l = false;

		} while (fix_point_l);

		return true;
	}

	void CtcDynCidGuess::ctc_fwd(Slice &x, Slice &v, std::vector<Slice*> x_slice, std::vector<Slice*> v_slice, int pos)
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

	double CtcDynCidGuess::get_prec()
	{
		return this->prec;
	}

	void CtcDynCidGuess::change_prec(double prec){
		this->prec = prec;
	}

	void CtcDynCidGuess::create_slices(Slice& x_slice, std::vector<Interval> & x_slices, TPropagation t_propa)
	{

		/*Varcid in the input gate*/
		if (t_propa & FORWARD){
			x_slices.push_back(Interval(x_slice.input_gate().lb()));
			x_slices.push_back(Interval(x_slice.input_gate().ub()));
		}

		/*Varcid in the output gate*/
		else if (t_propa & BACKWARD){
			x_slices.push_back(Interval(x_slice.output_gate().lb()));
			x_slices.push_back(Interval(x_slice.output_gate().ub()));
		}
	}

	void CtcDynCidGuess::var3Bcheck(ibex::Interval remove_bound ,int bound, int pos ,std::vector<Slice*> & x_slice, std::vector<Slice*> v_slice, TPropagation t_propa)
	{

		ctc_deriv.set_fast_mode(false);
		bool fix_point_n;
		vector<Slice> x_slice_bounds;
		vector<Slice> v_slice_bounds;
		for (int i = 0 ; i < x_slice.size() ; i++){
			x_slice_bounds.push_back(*x_slice[i]);
			v_slice_bounds.push_back(*v_slice[i]);
		}
		if (t_propa & FORWARD)
			x_slice_bounds[pos].set_output_gate(remove_bound);
		else if (t_propa & BACKWARD)
			x_slice_bounds[pos].set_input_gate(remove_bound);

		/*try to remove the complete interval*/
		for (int i = 0 ;  i < x_slice.size() ; i++){
			double sx;
			int max_iterations=0;
			do
			{
				sx = x_slice_bounds[i].volume();
				ctc_deriv.contract(x_slice_bounds[i], v_slice_bounds[i],t_propa);
				ctc_fwd(x_slice_bounds[i], v_slice_bounds[i], x_slice, v_slice, i);
				max_iterations++;
			} while((sx-x_slice_bounds[i].volume()>get_prec()) && (max_iterations < 50));

			/*if something is empty means that we can remove the complete interval*/
			if (x_slice_bounds[i].is_empty()){
				if (t_propa & FORWARD){
					if (bound == ub)
						x_slice[pos]->set_output_gate(Interval(x_slice[pos]->output_gate().lb(),remove_bound.lb()));
					else if (bound == lb)
						x_slice[pos]->set_output_gate(Interval(remove_bound.ub(),x_slice[pos]->output_gate().ub()));
				}
				if (t_propa & BACKWARD){
					if (bound == ub)
						x_slice[pos]->set_input_gate(Interval(x_slice[pos]->input_gate().lb(),remove_bound.lb()));
					else if (bound == lb)
						x_slice[pos]->set_input_gate(Interval(remove_bound.ub(),x_slice[pos]->input_gate().ub()));
				}
				return;
			}
		}

		/*update the guess with x_slice_bounds*/
		if (t_propa & FORWARD){
			if (bound == ub)
				remove_bound = Interval(remove_bound.lb(),x_slice_bounds[pos].output_gate().ub());
			else if (bound == lb)
				remove_bound = Interval(x_slice_bounds[pos].output_gate().lb(),remove_bound.ub());
		}
		else if (t_propa & BACKWARD){
			if (bound == ub)
				remove_bound = Interval(remove_bound.lb(),x_slice_bounds[pos].input_gate().ub());
			else if (bound == lb)
				remove_bound = Interval(x_slice_bounds[pos].input_gate().lb(),remove_bound.ub());
		}

		/*update the bounds*/
		if (t_propa & FORWARD){
			if (bound==ub)
				x_slice[pos]->set_output_gate(Interval(x_slice[pos]->output_gate().lb(),remove_bound.ub()));
			else if (bound==lb)
				x_slice[pos]->set_output_gate(Interval(remove_bound.lb(),x_slice[pos]->output_gate().ub()));
		}
		else if (t_propa & BACKWARD){
			if (bound==ub)
				x_slice[pos]->set_input_gate(Interval(x_slice[pos]->input_gate().lb(),remove_bound.ub()));
			else if (bound==lb)
				x_slice[pos]->set_input_gate(Interval(remove_bound.lb(),x_slice[pos]->input_gate().ub()));
		}
	}
}
