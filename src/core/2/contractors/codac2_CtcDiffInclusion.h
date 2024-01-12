/** 
 *  \file
 *  CtcDiffInclusion class
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Damien Massé
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_CTCDIFFINCLUSION_H__
#define __CODAC2_CTCDIFFINCLUSION_H__

#include "codac_TFunction.h"
#include "codac_DynCtc.h"
#include "codac2_Tube.h"
#include "codac2_Slice.h"
#include "codac2_IParals.h"

namespace codac2
{
  using codac::TFunction;
  using codac::TimePropag;

  /**
   * \class CtcDiffInclusion
   * \brief ...
   */
  template
  <typename _TpP>
  class CtcDiffInclusion
  {
    public:

      CtcDiffInclusion(const TFunction& t);

      /** evaluate function on box + time interval
       */
      const codac::IntervalVector eval_function(const Interval &tim,
                 const codac::IntervalVector& cdom, const codac::IntervalVector* u) const;
      const codac::IntervalVector eval_function(const Interval &tim,
                 const _TpP& cdom, const codac::IntervalVector* u) const;
      /** evaluate function on box + time
       */
      const codac::IntervalVector eval_function(double tim,
                 const codac::IntervalVector& cdom, const codac::IntervalVector* u) const;
      const codac::IntervalVector eval_function(double tim,
                 const _TpP& cdom, const codac::IntervalVector* u) const;
      /** evaluate function on point + time
       */
      const codac::IntervalVector eval_function(double tim,
                 const Vector& cdom, const codac::IntervalVector* u) const;

      IntervalMatrix jacobian(const _TpP& codom,
                 const codac::IntervalVector* u,
                 const Interval& tdom,
                 codac::IntervalVector& tvec) const;



      _TpP extend_box_basic(const _TpP& frame,
                        const _TpP& startIV,
                        const codac::IntervalVector* u,
                        const Interval& tim,
                        double inflation_factor,
                        TimePropag t_propa,
                        int nb_tries) const;

      bool compute_step(const _TpP& frame,
                                const codac::IntervalVector* u,
                                const _TpP& actState,
                                _TpP& tauState,
                                _TpP& finState,
                                const Interval& timeslice,
                                TimePropag t_propa) const;


      void contract(Tube<_TpP>& x, const Tube<_TpP>* u, TimePropag t_propa = TimePropag::FORWARD | TimePropag::BACKWARD);
      void contract_from_slice(Tube<_TpP>& x, const Tube<_TpP>* u, std::shared_ptr<Slice<_TpP>>& gate, TimePropag t_propa = TimePropag::FORWARD | TimePropag::BACKWARD);
      void contract(Slice<_TpP>& x, const Slice<_TpP>& u, TimePropag t_propa = TimePropag::FORWARD | TimePropag::BACKWARD);
      void contract(Slice<_TpP>& x, const std::shared_ptr<const _TpP>& uDom, TimePropag t_propa = TimePropag::FORWARD | TimePropag::BACKWARD);

      const TFunction& f() const;

    protected:

      //friend class Slice; // to be removed
      const TFunction _f;

      /** inflation factor in extend_box_basic */
      constexpr static double default_inflation_factor = 1.1; 
      constexpr static int narrow_factor = 3;


  };
}


/** 
 * CtcDiffInclusion code
 */

#include "codac2_CtcDiffInclusion.h"

using namespace std;
using namespace ibex;

namespace codac2
{
  template<typename _TpP>
  CtcDiffInclusion<_TpP>::CtcDiffInclusion(const TFunction& f)
    : _f(f)
  {

  }

  template<typename _TpP>
  const TFunction& CtcDiffInclusion<_TpP>::f() const
  {
    return _f;
  }

  template<typename _TpP>
  const IntervalVector CtcDiffInclusion<_TpP>::eval_function(const Interval &tim,
                 const IntervalVector& cdom, const IntervalVector *u) const {
        if (u!=NULL) 
          return _f.eval_vector(tim,cdom,*u); else
          return _f.eval_vector(tim,cdom); 

  }
  template<typename _TpP>
  const IntervalVector CtcDiffInclusion<_TpP>::eval_function(const Interval &tim,
                 const _TpP& cdom, const IntervalVector* u) const {
     return this->eval_function(tim, cdom.box(),u);
  }
  template<typename _TpP>
  const IntervalVector CtcDiffInclusion<_TpP>::eval_function(double tim,
                 const IntervalVector& cdom, const IntervalVector* u) const {
        const Interval timI(tim);
        return this->eval_function(timI,cdom,u);
  }
  template<typename _TpP>
  const IntervalVector CtcDiffInclusion<_TpP>::eval_function(double tim,
                 const _TpP& cdom, const IntervalVector* u) const {
        const Interval timI(tim);
        return this->eval_function(timI,cdom.box(),u);
  }
  template<typename _TpP>
  const IntervalVector CtcDiffInclusion<_TpP>::eval_function(double tim,
                 const Vector& cdom, const IntervalVector* u) const {
        const Interval timI(tim);
        const IntervalVector cdomI(cdom);
        return this->eval_function(timI,cdomI,u);
  }


  // basic enclosing for evolution
  template<typename _TpP>
  _TpP CtcDiffInclusion<_TpP>::extend_box_basic(const _TpP& frame,
                        const _TpP& startIV, const IntervalVector* u,
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
      _TpP XB1 = sum_tau(startIV,(tstep/2.0)*k1);
      IntervalVector k2 = this->eval_function(tim.mid(), XB1,u);
      XB1 = sum_tau(XB1,(tstep/2.0)* k2);
      IntervalVector k3 = this->eval_function(endtime, XB1,u);
      /* compute approximation of the result */
      _TpP Res = sum_tau(startIV,
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


  template<typename _TpP>
  IntervalMatrix CtcDiffInclusion<_TpP>::jacobian(const _TpP& codom,
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


  template<typename _TpP>
  bool CtcDiffInclusion<_TpP>::compute_step(const _TpP& frame,
 				const IntervalVector* u,
                                const _TpP& actState,
                                _TpP& tauState,
                                _TpP& finState,
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
//       std::cout << "jacobian : " << jac << "\n";
//       std::cout << "ExpM : " << ExpM << "\n";
//       std::cout << "IExpM : " << IExpM << "\n";
//       std::cout << "IntAbs : " << IntAbs << "\n";
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
           _TpP ctauState = tauState - cent_tauState;
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
           _TpP ntauState(actState);
//           cout << "avant ctau" << ntauState << "\ncent " << cent_tauState <<
//                      "\ntauExpM " << tauExpM << "\ntauCent " << tauCent << "\n";
           ntauState.ctau_mult_and_add(cent_tauState,tauExpM,tauCent);

//           cout << "ntauState" << ntauState << "\ntauState " << tauState <<
//                      "\n";


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
//        std::cout << "evolCenter " << evolCenter << "\n";
//        std::cout << "actState " << actState << "\n";
        finState.cmult_and_add(cent_tauState,ExpM,invExpM,evolCenter);
 //       std::cout << "final after cmult_and_add " << finState << "\n";
        return true;
   }




  template<typename _TpP>
  void CtcDiffInclusion<_TpP>::contract(Slice<_TpP>& x,
           const std::shared_ptr<const _TpP>& uDom, TimePropag t_propa)
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
    _TpP frame = x.codomain();
    const IntervalVector* uVect = (uDom==nullptr ? nullptr :
		 &(uDom->box()));
    if((t_propa & TimePropag::FORWARD) && !(x.input_gate().is_unbounded()))
    {
      _TpP approxIV(frame);
      bool okreduced=false; /* becomes true when we have a contraction */
      double inflation_f = default_inflation_factor; /* initial inflation factor */
      int nb_tries=0;
      _TpP g_t0 = x.input_gate();
      _TpP g_t1(g_t0); /* start matrices */
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
      if (x.next_slice_ptr() && x.next_slice_ptr()->is_gate()) {
         _TpP end = x.next_slice_ptr()->codomain();
	 end &= g_t1; /* ensure contractance */
         x.next_slice_ptr()->set(end);
      }
      x.set(frame); /* first the gate, then the frame... */
    }

    if((t_propa & TimePropag::BACKWARD) && (!x.output_gate().is_unbounded()))
    {
      _TpP approxIV(frame);
      bool okreduced=false; /* becomes true when we have a contraction */
      double inflation_f = default_inflation_factor; /* initial inflation factor */
      int nb_tries=0;
      _TpP g_t1 = x.output_gate();
      _TpP g_t0(g_t1); /* start matrices */
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
         _TpP beg = x.prev_slice_ptr()->codomain();
	 beg &= g_t0; /* ensure contractance */
         x.prev_slice_ptr()->set(beg);
      }
    }
  }

  template<typename _TpP>
  void CtcDiffInclusion<_TpP>::contract_from_slice(Tube<_TpP>& x, const Tube<_TpP>* u, std::shared_ptr<Slice<_TpP>>& gate, TimePropag t_propa)
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
       std::shared_ptr<Slice<_TpP>> sx = gate->next_slice_ptr();
       while (sx!=NULL) {
         if (!sx->is_gate() && !sx->tslice().t0_tf().is_unbounded()) {
            const shared_ptr<const _TpP> du = 
              (u == nullptr ? nullptr : 
                (sameslicing ?
    		std::make_shared<const _TpP>(
    		std::static_pointer_cast<const Slice<_TpP>>(sx->tslice().slices().at(u))->codomain()) : 
                    std::make_shared<const _TpP>(u->eval(sx->tslice().t0_tf()))
                )
             );
             contract(*sx,du,TimePropag::FORWARD);
         }
         sx = sx->next_slice_ptr();
       }
    }
    if (t_propa & TimePropag::BACKWARD) {
       std::shared_ptr<Slice<_TpP>> sx = gate->prev_slice_ptr();
       while (sx!=NULL) {
         if (!sx->is_gate() && !sx->tslice().t0_tf().is_unbounded()) {
            const shared_ptr<const _TpP> du = 
              (u == nullptr ? nullptr : 
                (sameslicing ?
    		std::make_shared<const _TpP>(
    		std::static_pointer_cast<const Slice<_TpP>>(sx->tslice().slices().at(u))->codomain()) : 
                    std::make_shared<const _TpP>(u->eval(sx->tslice().t0_tf()))
                )
             );
             contract(*sx,du,TimePropag::BACKWARD);
         }
         sx = sx->prev_slice_ptr();
       }
    }
  }

  template<typename _TpP>
  void CtcDiffInclusion<_TpP>::contract(Slice<_TpP>& x,
           const Slice<_TpP>& u, TimePropag t_propa) {
     const shared_ptr<const _TpP> du =
         std::make_shared<const _TpP>(u.codomain());
     this->contract(x,du,t_propa);
  }

  template<typename _TpP>
  void CtcDiffInclusion<_TpP>::contract(Tube<_TpP>& x, const Tube<_TpP>* u, TimePropag t_propa)
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

    for(auto& sx : x) // sx is a Slice<_TpP> of the Tube<_TpP> x
    {
      if(sx.is_gate()) // the slice may be on a degenerated temporal domain, i.e. a gate
        continue;
      if (sx.tslice().t0_tf().is_unbounded()) continue;

      // su is a Slice<_TpP> of the Tube<_TpP> u:
      const shared_ptr<const _TpP> du = 
        (u == nullptr ? nullptr : 
            (sameslicing ?
		std::make_shared<const _TpP>(
		std::static_pointer_cast<const Slice<_TpP>>(sx.tslice().slices().at(u))->codomain()) : 
                std::make_shared<const _TpP>(u->eval(sx.tslice().t0_tf()))
            )
        );
//    std::cout << "uslice : " << *du << "\n";
     // const shared_ptr<Slice<_TpP>> su = (u==NULL ? NULL : &(sx.tslice().slices().at(u)));
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
}

#endif
