#include "tubex_CtcDynBasic.h"

using namespace std;
using namespace ibex;


namespace tubex
{
	CtcDynBasic::CtcDynBasic(tubex::Fnc& fnc, double prec): fnc(fnc), prec(prec)
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

		//check if the gates used to contract are bounded
		for (int i = 0 ; i < x_slice.size(); i++){
			if ((t_propa & FORWARD) && (x_slice[0]->input_gate().is_unbounded()))
				return false;
			else if ((t_propa & BACKWARD) && (x_slice[0]->output_gate().is_unbounded()))
				return false;
		}

		bool fix_point_n;
		bool first_iteration = true;

	do{
		fix_point_n = false;
		double volume = 0.;
		double input_diam=0.;
		double output_diam=0.;
		
		for (int i =0; i<  x_slice.size(); i++){
		  volume+=x_slice[i]->volume();
		  input_diam+=x_slice[i]->input_gate().diam();
		  output_diam+=x_slice[i]->output_gate().diam();
		}
		for (int i = 0 ; i < x_slice.size() ; i++){

			Slice aux_slice_x(*x_slice[i]);
			Slice aux_slice_v(*v_slice[i]);

			double sx;
			/*without polygons*/
			if(m_fast_mode)
				ctc_deriv.set_fast_mode(true);
			if (get_reasoning_slice()){
				do
				{
					sx = aux_slice_x.volume();
					ctc_deriv.contract(aux_slice_x, aux_slice_v,t_propa);
					ctc_fwd(aux_slice_x, aux_slice_v, x_slice, v_slice, i);
				} while ((1-(aux_slice_x.volume()/sx)) > get_prec());
			}
			else{
				ctc_deriv.contract(aux_slice_x, aux_slice_v,t_propa);
				ctc_fwd(aux_slice_x, aux_slice_v, x_slice, v_slice, i);
			}
			

			//			double input_diam = x_slice[i]->input_gate().diam();
			//			double output_diam = x_slice[i]->output_gate().diam();

			/*Replacing the old domains with the new ones*/
			x_slice[i]->set_envelope(aux_slice_x.codomain()); v_slice[i]->set_envelope(aux_slice_v.codomain());
			x_slice[i]->set_input_gate(aux_slice_x.input_gate()); v_slice[i]->set_input_gate(aux_slice_v.input_gate());
			x_slice[i]->set_output_gate(aux_slice_x.output_gate()); v_slice[i]->set_output_gate(aux_slice_v.output_gate());
			//			cout << " volume " << volume << " after " << x_slice[i]->volume() << endl;

		
		}
		double volume_after=0;
		double input_diam_after=0;
		double output_diam_after=0;
		for (int i =0; i<  x_slice.size(); i++){
		  input_diam_after+=x_slice[i]->input_gate().diam();
		  output_diam_after+=x_slice[i]->output_gate().diam();
		  volume_after+=x_slice[i]->volume();
		}
		//		if (volume_after < volume || input_diam_after< input_diam || output_diam_after < output_diam) 
		if (volume_after < volume)
		  fix_point_n = true;



		if ((first_iteration) && !(fix_point_n))
		  
		  //		  { for (int i = 0 ; i < x_slice.size() ; i++){
		      //		      cout << " end " << i << "  " <<  *(x_slice[i]) << endl;
		  
		  return false;
	
	first_iteration = false;
	if (!get_reasoning_slice()) fix_point_n=false;

	} while(fix_point_n);

		return true;
	}

	void CtcDynBasic::ctc_fwd(Slice &x, Slice &v, std::vector<Slice*> x_slice, std::vector<Slice*> v_slice, int pos)
	{
		/*envelope*/
		IntervalVector envelope(x_slice.size()+1);
		envelope[0] = x.domain();

		for (int i = 0 ; i < x_slice.size() ; i++){
			if (i==pos)
				envelope[i+1] = x.codomain();
			else
				envelope[i+1] = x_slice[i]->codomain();
		}
		v.set_envelope(fnc.eval_vector(envelope)[pos]);
	}

	double CtcDynBasic::get_prec()
	{
		return this->prec;
	}

	void CtcDynBasic::set_prec(double prec)
	{
		this->prec = prec;
	}

	bool CtcDynBasic::get_reasoning_slice(){
		return this->m_reasoning_slice;
	}

	void CtcDynBasic::set_reasoning_slice(bool reasoning_slice){
		this->m_reasoning_slice = reasoning_slice;
	}
}



