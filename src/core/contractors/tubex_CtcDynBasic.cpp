#include "tubex_CtcDynBasic.h"

using namespace std;
using namespace ibex;


namespace tubex
{
	CtcDynBasic::CtcDynBasic(tubex::Function& fnc, double prec): fnc(fnc), prec(prec)
	{
		/*check input*/
		assert(prec >= 0);
	}

	bool CtcDynBasic::contract(std::vector<Slice*> x_slice, std::vector<Slice*> v_slice, TPropagation t_propa)
	{
		/*check if the domains are the same*/
		Interval to_try(x_slice[0]->domain());
		for (int i = 1 ; i < x_slice.size(); i++)
			assert(to_try == x_slice[i]->domain());

		bool fix_point_n;
		bool first_iteration = true;

	do{
		fix_point_n = false;
		for (int i = 0 ; i < x_slice.size() ; i++){
			Slice aux_slice_x(*x_slice[i]);
			Slice aux_slice_v(*v_slice[i]);

			double sx;
			/*without polygons*/
			if(m_fast_mode)
				ctc_deriv.set_fast_mode(true);
			do
			{
				sx = aux_slice_x.volume();
				ctc_deriv.contract(aux_slice_x, aux_slice_v,t_propa);
				ctc_fwd(aux_slice_x, aux_slice_v, x_slice, v_slice, i);
			} while (sx - aux_slice_x.volume() > get_prec());
			double volume = x_slice[i]->volume();

			/*Replacing the old domains with the new ones*/
			x_slice[i]->set_envelope(aux_slice_x.codomain()); v_slice[i]->set_envelope(aux_slice_v.codomain());
			x_slice[i]->set_input_gate(aux_slice_x.input_gate()); v_slice[i]->set_input_gate(aux_slice_v.input_gate());
			x_slice[i]->set_output_gate(aux_slice_x.output_gate()); v_slice[i]->set_output_gate(aux_slice_v.output_gate());

			if (x_slice[i]->volume() < volume)
				fix_point_n = true;
		}

		if ((first_iteration) && !(fix_point_n))
			return false;
		first_iteration = false;

	} while(fix_point_n);

		return true;
	}


	void CtcDynBasic::ctc_fwd(Slice &x, Slice &v, std::vector<Slice*> x_slice, std::vector<Slice*> v_slice, int pos)
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

	double CtcDynBasic::get_prec()
	{
		return this->prec;
	}

	void CtcDynBasic::set_prec(double prec)
	{
		this->prec = prec;
	}

	void CtcDynBasic::set_reasoning_slice(bool reasoning_slice){
		this->m_reasoning_slice = reasoning_slice;
	}
}



