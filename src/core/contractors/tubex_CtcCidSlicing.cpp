/*
 *  CtcBox class
 * ----------------------------------------------------------------------------
 * 	\date       2020
 *  \authors  	Victor Reyes, Gilles Trombettoni
 */

#include "tubex_CtcCidSlicing.h"

using namespace std;
using namespace ibex;


namespace tubex
{
	CtcCidSlicing::CtcCidSlicing(ibex::Fnc& fnc,int scid, double prec): fnc(fnc), scid(scid), prec(prec)
	{
		/*check inputs*/
		assert(scid > 0.);
		assert(prec >= 0);
	}

	void CtcCidSlicing::contract(TubeVector& x, TubeVector& v, TPropagation t_propa, bool m_report)
	{
		/*check if everything is ok*/
		assert(x.size() == v.size());
		assert(x.domain() == v.domain());
		assert(TubeVector::same_slicing(x, v));

		double old_volume = x.volume();

		/*cpu time measurement*/
		clock_t tStart = clock();

		/*init all the tubes*/
		vector<Slice*> x_slice;
		vector<Slice*> v_slice;

		if (t_propa & FORWARD){
			for (int i = 0 ; i < x.size() ; i++){
				x_slice.push_back(x[i].first_slice());
				v_slice.push_back(v[i].first_slice());
			}
		}
		else if (t_propa & BACKWARD){
			for (int i = 0 ; i < x.size() ; i++){
				x_slice.push_back(x[i].last_slice());
				v_slice.push_back(v[i].last_slice());
			}
		}

		/*Defining the sub-contractor Ctc_Derive*/
		CtcDeriv ctc_deriv;
		int no_contraction = 0 ;
		/*for each tube, go all over the slices*/
		while(x_slice[0] != NULL){
			/*iteration step, made for each subslice at each tube x*/
			bool fix_point_n;
			bool first_iteration = true;
			do{
				fix_point_n=false;
				for (int i = 0 ; i < x.size() ; i++){
					std::vector<ibex::Interval> x_subslices;
					x_subslices.clear();

					/*create the sub-slices*/
					create_subslices(*x_slice[i],x_subslices, t_propa);

					/*For each slice on $t$ compute the corresponding the hull */
					Interval hull_input_x = Interval::EMPTY_SET; Interval hull_input_v = Interval::EMPTY_SET;
					Interval hull_output_x = Interval::EMPTY_SET; Interval hull_output_v = Interval::EMPTY_SET;
					Interval hull_codomain_x = Interval::EMPTY_SET; Interval hull_codomain_v = Interval::EMPTY_SET;


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
						ctc_deriv.set_fast_mode(true);
						do
						{
							sx = aux_slice_x.volume();
							ctc_deriv.contract(aux_slice_x, aux_slice_v,t_propa);
							ctc_bwd(aux_slice_x, aux_slice_v, x_slice, v_slice, i);

						} while(sx-aux_slice_x.volume()>get_prec());

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

					if ( aux_envelope > x_slice[i]->codomain().diam())
						fix_point_n = true;
					// empty TESt (BN)
					if (x_slice[i]->is_empty()) return;
				
				}
				// Incrementality test (BN)
				if ((first_iteration) && !(fix_point_n))
				  return;
				
				first_iteration= false;
			

				/*is not necessary for 1-dimensional problems*/
				if (x.size() == 1)
				  fix_point_n=false;
			} while(fix_point_n);


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
		}
		if (m_report)
			report(tStart,x,old_volume);
	}


	void CtcCidSlicing::ctc_bwd(Slice &x, Slice &v, std::vector<Slice*> x_slice, std::vector<Slice*> v_slice, int pos)
	{
		/*envelope*/
		IntervalVector envelope(x_slice.size());
		for (int i = 0 ; i < x_slice.size() ; i++){
			if (i==pos)
				envelope[i] = x.codomain();
			else
				envelope[i] = x_slice[i]->codomain();
		}
		v.set_envelope(fnc.eval_vector(envelope)[pos]);
	}

	double CtcCidSlicing::get_scid()
	{
		return this->scid;
	}

	double CtcCidSlicing::get_prec()
	{
		return this->prec;
	}

	void CtcCidSlicing::set_scid(int scid)
	{
		this->scid = scid;
	}

	void CtcCidSlicing::set_prec(double prec)
	{
		this->prec = prec;
	}

	void CtcCidSlicing::create_subslices(Slice& x_slice, std::vector<ibex::Interval> & x_slices, TPropagation t_propa)
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
//			x_slices.push_back(Interval(x_slice.output_gate().lb()));
//			x_slices.push_back(Interval(x_slice.output_gate().ub()));
			double size_interval = x_slice.output_gate().diam()/get_scid();
			for (int i = 0 ; i < get_scid() ;i++){
				x_slices.push_back(Interval(x_slice.output_gate().lb()+i*size_interval,x_slice.output_gate().lb()+size_interval*(i+1)));
			}
		}
	}

	void CtcCidSlicing::report(clock_t tStart,TubeVector& x,double old_volume)
	{

		cout <<endl<< "----------Results for: " <<	dynamic_cast <ibex::Function&>(fnc)<<"----------"<<endl << endl;
		/*CidSlicing does nothing, */
		if (old_volume == x.volume()){
			cout << "\033[1;31mNo contraction made by CidSlicing!\033[0m\n";
			printf("CPU Time spent by CidSlicing: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
		}
		/*CidSlicing contracts the tube*/
		else{
			double doors_size = 0 ;
			int nb_doors = 0;
			for (int i = 0 ; i < x.size() ; i++){
				Slice* x_slice = x[i].first_slice();
				for (int j = 0 ; j < x[i].nb_slices() ; j++){
					doors_size +=x_slice->output_gate().diam();
					nb_doors++;
					x_slice = x_slice->next_slice();
				}
			}
			cout << "\033[1;31mContraction successful!  -  CidSlicing\033[0m\n";
			printf("CPU Time spent by CidSlicing: %.3f (s)\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
			printf("Old Volume: %.7f\n", old_volume);
			printf("New Volume: %.7f\n", x.volume());
			printf("Average size of doors: %f\n\n", (double)doors_size/nb_doors);
		}
	}
}

