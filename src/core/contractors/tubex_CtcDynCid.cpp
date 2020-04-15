/*
 *  CtcDynCid Class
 * ----------------------------------------------------------------------------
 * 	\date       2020
 *  \authors  	Victor Reyes, Gilles Trombettoni
 */

#include "tubex_CtcDynCid.h"

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

		bool fix_point_n;
		bool first_iteration = true;

		//check if the gates used to contract are bounded
		for (int i = 0 ; i < x_slice.size(); i++){
			if ((t_propa & FORWARD) && (x_slice[0]->input_gate().is_unbounded()))
				return false;
			else if ((t_propa & BACKWARD) && (x_slice[0]->output_gate().is_unbounded()))
				return false;
		}


		do{
			fix_point_n = false;
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

//					/*If the tube is unbounded, then the algorithm stops*/
//					if (v_slice[i]->codomain().is_unbounded())
//						return false;

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

					} while(sx - aux_slice_x.volume() > get_prec());

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

				if (aux_envelope > x_slice[i]->codomain().diam())
					fix_point_n = true;
				/*empty test (Bertrand)*/
				if (x_slice[i]->is_empty()) return false;
			}
			/*Incrementality test (Bertrand)*/
			if ((first_iteration) && !(fix_point_n))
				return false;

			first_iteration = false;

			/*For 1-dimensional problems it is not necessary to repeat the process*/
			if (x_slice.size() == 1)
				fix_point_n=false;
		} while(fix_point_n);

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

	double CtcDynCid::get_scid()
	{
		return this->scid;
	}

	double CtcDynCid::get_prec()
	{
		return this->prec;
	}

	void CtcDynCid::set_scid(int scid)
	{
		this->scid = scid;
	}

	void CtcDynCid::set_prec(double prec)
	{
		this->prec = prec;
	}

	void PropagationEngine(){

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
