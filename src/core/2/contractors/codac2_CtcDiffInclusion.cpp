/** 
 *  CtcDiffInclusion class
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac2_CtcDiffInclusion.h"
#include "codac2_IParals.h"
#include "codac2_expIMat.h"

using namespace std;
using namespace ibex;

namespace codac2
{
  CtcDiffInclusion::CtcDiffInclusion(const TFunction& f)
    : _f(f)
  {

  }

  const TFunction& CtcDiffInclusion::f() const
  {
    return _f;
  }

  const IntervalVector CtcDiffInclusion::eval_function(const Interval &tim,
                 const IntervalVector& cdom, const IntervalVector *u) const {
        if (u!=NULL) 
          return _f.eval_vector(tim,cdom,*u); else
          return _f.eval_vector(tim,cdom); 

  }
  const IntervalVector CtcDiffInclusion::eval_function(const Interval &tim,
                 const IParals& cdom, const IntervalVector* u) const {
     return this->eval_function(tim, cdom.box(),u);
  }
  const IntervalVector CtcDiffInclusion::eval_function(double tim,
                 const IntervalVector& cdom, const IntervalVector* u) const {
        const Interval timI(tim);
        return this->eval_function(timI,cdom,u);
  }
  const IntervalVector CtcDiffInclusion::eval_function(double tim,
                 const IParals& cdom, const IntervalVector* u) const {
        const Interval timI(tim);
        return this->eval_function(timI,cdom.box(),u);
  }
  const IntervalVector CtcDiffInclusion::eval_function(double tim,
                 const Vector& cdom, const IntervalVector* u) const {
        const Interval timI(tim);
        const IntervalVector cdomI(cdom);
        return this->eval_function(timI,cdomI,u);
  }


  // basic enclosing for evolution
  IParals CtcDiffInclusion::extend_box_basic(const IParals& frame,
                        const IParals& startIV, const IntervalVector* u,
                        const Interval& tim,
                        double inflation_factor,
			TimePropag t_propa,
                        int nb_tries) const {
      double tstep = (t_propa == TimePropag::FORWARD ? tim.diam() 
			: -tim.diam());
      Interval btime(0.0,tstep);
      if (t_propa == TimePropag::BACKWARD) btime = -btime;
      double starttime = (t_propa == TimePropag::FORWARD ? tim.lb() 
				: tim.ub());
      double endtime = (t_propa == TimePropag::FORWARD ? tim.ub() 
				: tim.lb());
      /* estimation des pentes */
      IntervalVector k1 = this->eval_function(starttime,startIV,u);
      IParals XB1 = sum_tau(startIV,(tstep/2.0)*k1);
      IntervalVector k2 = this->eval_function(tim.mid(), XB1,u);
      XB1 = sum_tau(XB1,(tstep/2.0)* k2);
      IntervalVector k3 = this->eval_function(endtime, XB1,u);
      /* compute approximation of the result */
      IParals Res = sum_tau(startIV,
                        (tstep*0.505)* ((k1|k2) + (k2|k3)));

      Res&=frame;

      if (nb_tries==0) return Res;
      /* inflation */
//      Res.inflate(inflation_factor);
//      Res &= frame;
//      if (nb_tries<3) {
        double ifact = nb_tries==0 ? 1.0 : inflation_factor;
        if (nb_tries==2) ifact *= inflation_factor;
        Res = sum_tau(startIV, (ifact * btime) * 
			(this->eval_function(tim,Res,u)));
        Res&=frame;
 //     } 

      return Res;
  }


  IntervalMatrix CtcDiffInclusion::jacobian(const IParals& codom,
                 const IntervalVector* u,
                 const Interval& tdom,
                 IntervalVector& tvec) const {
        const Function& func = _f.getFunction();
        const IntervalVector& b = codom.box();
        if (u!=NULL) {
          IntervalVector box(b.size()+u->size()+1);
          box[0]=tdom;
          box.put(1,b);
          box.put(1+b.size(),*u);
          IntervalMatrix bjac = func.jacobian(box);
          tvec = bjac.col(0);
          return bjac.cols(1,b.size());
        } else {
          IntervalVector box(b.size()+1);
          box[0]=tdom;
          box.put(1,b);
          IntervalMatrix bjac = func.jacobian(box);
          tvec = bjac.col(0);
          return bjac.cols(1,b.size());
        }
  }


  bool CtcDiffInclusion::compute_step(const IParals& frame,
 				const IntervalVector* u,
                                const IParals& actState,
                                IParals& tauState,
                                IParals& finState,
                                const Interval& timeslice,
			        TimePropag t_propa) const {

       const int dim=frame.get_dim();
       IntervalVector tdiff(dim);
       IntervalMatrix jacM = this->jacobian(tauState,u,timeslice,tdiff);
       if (t_propa == TimePropag::BACKWARD) {
           jacM = -jacM;
           tdiff = -tdiff;
       }
       const Matrix jac = jacM.mid();
       jacM -= jac;
       bool time_dependent = !tdiff.is_zero();

       IntervalMatrix ExpM(dim,dim);
       IntervalMatrix invExpM(dim,dim);
       IntervalMatrix tauExpM(dim,dim);
       IntervalMatrix IExpM(dim,dim);
       IntervalMatrix tauIExpM(dim,dim);
       IntervalMatrix VExpM(dim,dim);
       IntervalMatrix tauVExpM(dim,dim);
       Matrix IntAbs(dim,dim);

       double tsteps = timeslice.diam();

       global_exp(jac,tsteps,true,time_dependent,
                ExpM,invExpM,tauExpM,IExpM,tauIExpM,
                VExpM,tauVExpM,IntAbs);
       /* other variables which needs to be kept */
       Vector cent_tdiff(dim);
       Vector fun_evalc(dim);
       Vector vuncert(dim);
       Vector cent_tauState(dim);

       bool ok=false;
       bool reducing=false;
       int nb_red=0;
       int nb_enl=0;
       while(!ok) {
           /* computing uncert and other values */
           cent_tauState = tauState.mid();
           IParals ctauState = tauState - cent_tauState;
           IntervalVector uncert = jacM * ctauState;
           if (time_dependent) {
              cent_tdiff = tdiff.mid();
              IntervalVector ctdiff = tdiff-cent_tdiff;
              uncert += timeslice.rad() * ctdiff;
           }
           IntervalVector fun_eval =
                  this->eval_function(timeslice.mid(),cent_tauState,u);
           fun_evalc = fun_eval.mid();
           uncert += (fun_eval-fun_evalc);
           if (t_propa == TimePropag::BACKWARD) {
              fun_evalc = -fun_evalc;
           }

//           cout << "uncert : " << uncert << "\n";
           /* now computing the new "tau" states */
           vuncert = IntAbs * uncert.ub();
           // ``derivation'' of the center ( f(cent) * int exp(M\tau) )
           IntervalVector tauCent = tauIExpM * fun_evalc;
           // non-autonomous factor
           if (time_dependent) {
                 tauCent += tauVExpM * cent_tdiff;
           }
           /* evolution of the center */
           for (int i=0;i<dim;i=i+1) (tauCent[i]).inflate(vuncert[i]);

           /* next tauState, do NOT change the underlying matrices */
           IParals ntauState(actState);
//           cout << "avant ctau" << ntauState << "\ncent " << cent_tauState <<
//                      "\ntauExpM " << tauExpM << "\ntauCent " << tauCent << "\n";
           ntauState.ctau_mult_and_add(cent_tauState,tauExpM,tauCent);


           ntauState &= frame;
           if (ntauState.is_subsetFast(tauState)) {
              reducing=true;
//              cout << "plus petit " << nb_red << " " << ntauState << "\t" << tauState << "\n";
              if (tauState.rel_distanceFast(ntauState)<1e-5) ok=true;
                                /* FIXME : select a value for rel_distance */
                                /* maybe dependent on the computation */
              tauState=ntauState;
              nb_red++;
           } else {
	      if (reducing) { break; /* problem FIXME */
              }
              nb_enl++;
              if (nb_enl==narrow_factor) return false;
                        /* FIXME : narrow_factor is arbitrary */
              tauState.inflate_from_baseFast(ntauState,default_inflation_factor);
           }
           if (reducing) {
              /* computing ``arrival'' states */
              IntervalVector evolCenter = IExpM * fun_evalc;
              // non-autonomous factor
              if (time_dependent) {
                  evolCenter += VExpM * cent_tdiff;
              }
              for (int i=0;i<dim;i=i+1) (evolCenter[i]).inflate(vuncert[i]);
              finState = actState;
           }
           if (!ok) {
                /* recompute jacM and tdiff */
                jacM = this->jacobian(tauState,u,timeslice,tdiff);
                if (t_propa == TimePropag::BACKWARD) {
                   jacM = -jacM;
                   tdiff = -tdiff;
                }
                jacM = jacM - jac; /* maybe not centered */
           }
	}
        /* computing ``arrival'' states */
        IntervalVector evolCenter = IExpM * fun_evalc;
        // non-autonomous factor
        if (time_dependent) {
            evolCenter += VExpM * cent_tdiff;
        }
        for (int i=0;i<dim;i=i+1) (evolCenter[i]).inflate(vuncert[i]);
        finState = actState;
        finState.cmult_and_add(cent_tauState,ExpM,invExpM,evolCenter);
        return true;
   }



  void CtcDiffInclusion::contract(Tube<IParals>& x, const Tube<IParals>* u, TimePropag t_propa)
  {
    bool sameslicing=false;
    // Verifying that x and u share exactly the same tdomain and slicing:

    if (u!=nullptr) {
       if (x.tdomain() == u->tdomain()) sameslicing=true;
       else {
//	   std::cout << x.tdomain()->t0_tf() << " " << u->tdomain()->t0_tf() << "\n";
           assert(x.tdomain()->t0_tf().is_subset(u->tdomain()->t0_tf()));
       }
    // Verifying that the provided tubes are consistent with the function
//       std::cout << _f.nb_var() << " " << x.size() << " " << u->size() << "\n";
       assert((size_t)_f.nb_var() == x.size()+u->size());
    } else 
       assert((size_t)_f.nb_var() == x.size());
    assert((size_t)_f.image_dim() == x.size());

    for(auto& sx : x) // sx is a Slice<IParals> of the Tube<IParals> x
    {
      if(sx.is_gate()) // the slice may be on a degenerated temporal domain, i.e. a gate
        continue;
      if (sx.tslice().t0_tf().is_unbounded()) continue;

      // su is a Slice<IParals> of the Tube<IParals> u:
      const shared_ptr<const IParals> du = 
        (u == nullptr ? nullptr : 
            (sameslicing ?
		std::make_shared<const IParals>(
		std::static_pointer_cast<const Slice<IParals>>(sx.tslice().slices().at(u))->codomain()) : 
                std::make_shared<const IParals>(u->eval(sx.tslice().t0_tf()))
            )
        );
//    std::cout << "uslice : " << *du << "\n";
     // const shared_ptr<Slice<IParals>> su = (u==NULL ? NULL : &(sx.tslice().slices().at(u)));
//      const double dt = sx.t0_tf().diam();

//nullptr
       // sx.set(su.codomain());

      // ...
      contract(sx,du,t_propa);

/*
      if(t_propa & TimePropag::FORWARD)
      {
        contract(sx,su,t_progag);
        // Computations related to forward propagation
        // ...
      }

      if(t_propa & TimePropag::BACKWARD)
      {
        contract(sx,su,t_progag);
        // Computations related to backward propagation
        // ...
      }
*/
    }
  }

  void CtcDiffInclusion::contract_from_slice(Tube<IParals>& x, const Tube<IParals>* u, std::shared_ptr<Slice<IParals>>& gate, TimePropag t_propa)
  {
    bool sameslicing=false;
    // Verifying that x and u share exactly the same tdomain and slicing:

    if (u!=nullptr) {
       if (x.tdomain() == u->tdomain()) sameslicing=true;
       else assert(x.tdomain()->t0_tf().is_subset(u->tdomain()->t0_tf()));
    // Verifying that the provided tubes are consistent with the function
       assert((size_t)_f.nb_var() == x.size()+u->size());
    } else 
       assert((size_t)_f.nb_var() == x.size());
    assert((size_t)_f.image_dim() == x.size());

    if (t_propa & TimePropag::FORWARD) {
       std::shared_ptr<Slice<IParals>> sx = gate->next_slice_ptr();
       while (sx!=NULL) {
         if (!sx->is_gate() && !sx->tslice().t0_tf().is_unbounded()) {
            const shared_ptr<const IParals> du = 
              (u == nullptr ? nullptr : 
                (sameslicing ?
    		std::make_shared<const IParals>(
    		std::static_pointer_cast<const Slice<IParals>>(sx->tslice().slices().at(u))->codomain()) : 
                    std::make_shared<const IParals>(u->eval(sx->tslice().t0_tf()))
                )
             );
             contract(*sx,du,TimePropag::FORWARD);
         }
         sx = sx->next_slice_ptr();
       }
    }
    if (t_propa & TimePropag::BACKWARD) {
       std::shared_ptr<Slice<IParals>> sx = gate->prev_slice_ptr();
       while (sx!=NULL) {
         if (!sx->is_gate() && !sx->tslice().t0_tf().is_unbounded()) {
            const shared_ptr<const IParals> du = 
              (u == nullptr ? nullptr : 
                (sameslicing ?
    		std::make_shared<const IParals>(
    		std::static_pointer_cast<const Slice<IParals>>(sx->tslice().slices().at(u))->codomain()) : 
                    std::make_shared<const IParals>(u->eval(sx->tslice().t0_tf()))
                )
             );
             contract(*sx,du,TimePropag::BACKWARD);
         }
         sx = sx->prev_slice_ptr();
       }
    }
  }

  void CtcDiffInclusion::contract(Slice<IParals>& x,
           const Slice<IParals>& u, TimePropag t_propa) {
     const shared_ptr<const IParals> du =
         std::make_shared<const IParals>(u.codomain());
     this->contract(x,du,t_propa);
  }

  void CtcDiffInclusion::contract(Slice<IParals>& x,
           const std::shared_ptr<const IParals>& uDom, TimePropag t_propa)
  {
    if (uDom!=nullptr) {
    // Verifying that the provided slices are consistent with the function
       assert((size_t)_f.nb_var() == x.size()+uDom->size());
    } else 
       assert((size_t)_f.nb_var() == x.size());
    assert((size_t)_f.image_dim() == x.size());
    if (x.is_gate() || x.is_empty()) return;

    Interval timeslice=x.t0_tf();
//   double dt = timeslice.diam();

    // ...
    IParals frame = x.codomain();
    const IntervalVector* uVect = (uDom==nullptr ? nullptr :
		 &(uDom->box()));
    if((t_propa & TimePropag::FORWARD) && !(x.input_gate().is_unbounded()))
    {
      IParals approxIV(frame);
      bool okreduced=false; /* becomes true when we have a contraction */
      double inflation_f = default_inflation_factor; /* initial inflation factor */
      int nb_tries=0;
      IParals g_t0 = x.input_gate();
      IParals g_t1(g_t0); /* start matrices */
      while (true) {
        if (!okreduced) {
	   approxIV = this->extend_box_basic(frame,
			g_t0,uVect,timeslice,inflation_f,
			TimePropag::FORWARD,nb_tries);
        }
        bool safe = compute_step(frame,uVect,
		g_t0,approxIV,g_t1,timeslice,TimePropag::FORWARD);
        if (!safe) {
	   if (nb_tries>2) {
              inflation_f *= 1.5;
           }
        } else {
           okreduced=true;
           if (nb_tries>=5) break;
        }
        nb_tries++;
      }
      frame &= approxIV;  /* ensure contractance */
      x.set(frame);
      if (x.next_slice_ptr() && x.next_slice_ptr()->is_gate()) {
         IParals end = x.next_slice_ptr()->codomain();
	 end &= g_t1; /* ensure contractance */
         x.next_slice_ptr()->set(end);
      }
    }

    if((t_propa & TimePropag::BACKWARD) && (!x.output_gate().is_unbounded()))
    {
      IParals approxIV(frame);
      bool okreduced=false; /* becomes true when we have a contraction */
      double inflation_f = default_inflation_factor; /* initial inflation factor */
      int nb_tries=0;
      IParals g_t1 = x.output_gate();
      IParals g_t0(g_t1); /* start matrices */
      while (true) {
        if (!okreduced) {
	   approxIV = this->extend_box_basic(frame,
			g_t1,uVect,timeslice,inflation_f,
			TimePropag::BACKWARD,nb_tries);
        }
        bool safe = compute_step(frame,uVect,
		g_t1,approxIV,g_t0,timeslice,TimePropag::BACKWARD);
        if (!safe) {
	   if (nb_tries>2) {
              inflation_f *= 1.5;
           }
        } else {
           okreduced=true;
           if (nb_tries>=5) break;
        }
        nb_tries++;
      }
      frame &= approxIV;  /* ensure contractance */
      x.set(frame);
      if (x.prev_slice_ptr() && x.prev_slice_ptr()->is_gate()) {
         IParals beg = x.prev_slice_ptr()->codomain();
	 beg &= g_t0; /* ensure contractance */
         x.prev_slice_ptr()->set(beg);
      }
    }
  }
}
