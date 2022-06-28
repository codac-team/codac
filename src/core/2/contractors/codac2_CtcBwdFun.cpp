/** 
 *  CtcBwdFun class
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Damien Massé 
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <vector>
#include "codac_Function.h"
#include "codac_DynCtc.h"
#include "codac2_SliceVector.h"
#include "codac2_IParals.h"
#include "codac2_expIMat.h"
#include "codac2_CtcBwdFun.h"

using namespace std;
using namespace ibex;

namespace codac2
{
  CtcBwdFun::CtcBwdFun(const Function& f)
    : _f(f)
  {

  }

  const Function& CtcBwdFun::f() const
  {
    return _f;
  }

  const IntervalVector CtcBwdFun::eval_function(const IntervalVector& cdom,
			 const IntervalVector *u) const {
        if (u!=NULL) {
          IntervalVector V = cart_prod(cdom,*u);
          return _f.eval_vector(V);
        } else
          return _f.eval_vector(cdom); 

  }
  const IntervalVector CtcBwdFun::eval_function(const IParals& cdom,
			 const IntervalVector* u)
		 const {
     return this->eval_function(cdom.box(),u);
  }
  const IntervalVector CtcBwdFun::eval_function(const IntervalVector& cdom,
         const std::vector<std::pair<const IntervalVector*,
                                     const IntervalVector>>& rhs) const {
      assert (rhs.size()>0);
      IntervalVector Res(rhs.size()*_f.image_dim());
      for (unsigned int i=0;i<rhs.size();i++) {
         const std::pair<const IntervalVector*, const IntervalVector>& p = rhs[i];
         Res.put(i*_f.image_dim(), this->eval_function(cdom,p.first)-p.second);
      }
      return Res;
  }

  const IntervalVector CtcBwdFun::eval_function(const IParals& cdom,
         const std::vector<std::pair<const IntervalVector*,
                                     const IntervalVector>>& rhs) const {
        return this->eval_function(cdom.box(), rhs);
  }


  IntervalMatrix CtcBwdFun::jacobian(const IParals& codom,
                 const IntervalVector* u) const {
        const IntervalVector& b = codom.box();
        if (u!=NULL) {
          IntervalVector V = cart_prod(b,*u);
          IntervalMatrix bjac = _f.jacobian(V);
          return bjac.cols(0,b.size()-1);
        } else {
          return _f.jacobian(b);
        }
  }

  IntervalMatrix CtcBwdFun::jacobian(const IParals& codom,
         const std::vector<std::pair<const IntervalVector*, 
                                     const IntervalVector>>& rhs) const {
       assert (rhs.size()>0);
       IntervalMatrix Res(rhs.size()*_f.image_dim(),codom.get_dim());
       for (unsigned int i=0;i<rhs.size();i++) {
          const std::pair<const IntervalVector*, 
				const IntervalVector>& p = rhs[i];
          
          Res.put(i*_f.image_dim(),0,
		 this->jacobian(codom,p.first));
       }
       return Res;
  }
 
  Vector CtcBwdFun::newton_Approx(const IParals& codom, const Matrix& mjac,
         const IntervalVector* u, const IntervalVector& rs) const {
       Vector md = codom.mid();
       IntervalMatrix JRes(1,mjac.nb_cols()), E(mjac.nb_cols(),1);
       std::vector<int> r = construct_free_set(mjac, JRes, E, NULL);
       Matrix S(r.size(),mjac.nb_rows(),0.0);
       for (unsigned int i=0;i<r.size();i++) {
          assert(r[i]>=0);
          S[i][r[i]] = 1.0;
       }
       IntervalMatrix Et=E.transpose();
       IntervalMatrix ES = Et*S;
       for (int i=0;i<3;i++) {
          IntervalVector val = (this->eval_function(md,u)-rs).mid();
          Vector md2 = (md - ES*val).mid();
          if (!codom.contains(md2)) break;
          md = md2;
       } 
       return md;
  }

        
  Vector CtcBwdFun::newton_Approx(const IParals& codom, const Matrix& mjac,
         const std::vector<std::pair<const IntervalVector*,
                                     const IntervalVector>>& rhs) const {
      Vector md = codom.mid();
      IntervalMatrix JRes(1,mjac.nb_cols()), E(mjac.nb_cols(),1);
      std::vector<int> r = construct_free_set(mjac, JRes, E, NULL);
      Matrix S(r.size(),mjac.nb_rows(),0.0);
      for (unsigned int i=0;i<r.size();i++) {
         assert(r[i]>=0);
         S[i][r[i]] = 1.0;
      }
      IntervalMatrix Et=E.transpose();
      IntervalMatrix ES = Et*S;
      for (int i=0;i<3;i++) {
         IntervalVector val = (this->eval_function(md,rhs)).mid();
         Vector md2 = (md - ES*val).mid();
         if (!codom.contains(md2)) break;
         md = md2;
      } 
      return md;
  }


  void CtcBwdFun::contract(IParals& val, const IntervalVector* u,
		const IntervalVector& rs) const {
       for (int i=0;i<3;i++) {
          IntervalMatrix jac = this->jacobian(val,u);
          Matrix mjac = jac.mid();
          Vector md = this->newton_Approx(val,mjac,u,rs);
          IntervalVector r = rs-this->eval_function(md,u);
          if (!val.meetLM(jac,md,r,true)) break;
       }
  }

  void CtcBwdFun::contract(IParals& val,
         const std::vector<std::pair<const IntervalVector*,
                                     const IntervalVector>>& rhs) const {
       for (int i=0;i<3;i++) {
          IntervalMatrix jac = this->jacobian(val,rhs);
          Matrix mjac = jac.mid();
          Vector md = this->newton_Approx(val,mjac,rhs);
          IntervalVector rs = -this->eval_function(md,rhs);
          if (!val.meetLM(jac,md,rs,true)) {
               break;
          }
       }
  }

  void CtcBwdFun::contract(SliceVector& x, const IntervalVector* u, 
             const IntervalVector& rs) const {
       IParals val = x.codomainI();
       this->contract(val,u,rs);
       x.set(val);
  }
  void CtcBwdFun::contract(SliceVector& x,
         const std::vector<std::pair<const IntervalVector*,
                                     const IntervalVector>>& rhs) const {
       IParals val = x.codomainI();
       this->contract(val,rhs);
       x.set(val);
  }
}
