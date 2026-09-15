/** 
 *  \file codac2_SepImage.h
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Maël Godard
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Sep.h"
#include "codac2_Collection.h"
#include "codac2_Paving.h"
#include "codac2_PavingNode.h"
#include "codac2_Subpaving.h"
#include "codac2_AnalyticFunction.h"
#include "codac2_peibos.h"
#include "codac2_CtcUnion.h"
#include "codac2_CtcWrapper.h"
#include "codac2_CtcIdentity.h"
#include "codac2_SepCtcPair.h"
#include "codac2_SepInverse.h"
#include "codac2_SepWrapper.h"
#include "codac2_SepInter.h"
#include "codac2_pave.h"
#include <codac2_hull.h>

namespace codac2
{
  class SepImage : public Sep<SepImage>
  {
    public:

      SepImage(AnalyticFunction<VectorType>& f, AnalyticFunction<VectorType>& psi_0, std::vector<OctaSym>& Sigma, double epsilon, CtcBase<IntervalVector>& ctc_init, double epsilon_pave = -1)
        :Sep<SepImage>(f.output_size())
      {
        // Using PEIBOS to get the boundary of the image set
        int dim = f.output_size();
        auto v_par = PEIBOS(f,psi_0,Sigma,epsilon);

        IntervalVector X0 (dim);
        ctc_init.contract(X0);

        // Contractor on the boundary
        CtcUnion ctc_union (dim);
        for (const auto& par : v_par)
          ctc_union |= CtcWrapper(par);

        SepCtcPair sep_boundary(CtcIdentity(dim), ctc_union);

        IntervalVector Y0 (dim);
        ctc_union.contract(Y0);
        Y0.inflate(Y0.min_diam()/10.);

        // paving resolution, can be tuned
        if (epsilon_pave == -1)
          epsilon_pave = Y0.min_diam()/50.;

        // Initial paving, with only an outer aproximation
        PavingInOut p = pave(Y0,sep_boundary,epsilon_pave);

        // Determination of the inner connected subsets
        auto v_cs = p.connected_subsets(PavingInOut::outer_complem);
        std::list<std::shared_ptr<PavingInOut::ConnectedSubset_>> cs_to_color;
        for (const auto& cs: v_cs)
        {
          IntervalVector max_box = IntervalVector::empty(dim);
          for (const auto& box : cs.boxes())
            if (box.volume()>max_box.volume())
              max_box=box;

          CtcInverse ctc_inv (f, max_box.mid());
          IntervalVector X0_copy (X0);
          bool could_contract = false;
          
          ctc_inv.contract(X0_copy);
          ctc_init.contract(X0_copy);

          if (X0_copy != X0)
          {
            could_contract = true;
            if (!X0_copy.is_empty())
              cs_to_color.push_back(std::make_shared<PavingInOut::ConnectedSubset_>(cs));
          }

          // if the CtcInverse was not able to conclude directly, we subdivide the initial box

          if (!could_contract)
          {
            std::vector<IntervalVector> boxes;
            split(X0_copy,0.125,boxes);
            for (auto box: boxes)
            {
              IntervalVector box_copy (box); 
              ctc_inv.contract(box);
              ctc_init.contract(box);
              if (box != box_copy)
                {
                  could_contract = true;
                  if (!box.is_empty())
                  {
                    cs_to_color.push_back(std::make_shared<PavingInOut::ConnectedSubset_>(cs));
                    break;
                  }
                }
            }
          }

          if (!could_contract)
            printf("warning, the SepImage was not able to conclude on the inner regions\n");
        }

        // Turning the previously identified connected subsets as inner (displayed green)
        auto visitor = [&](std::shared_ptr<PavingNode<PavingInOut>> n)
          {
            std::list<IntervalVector> boxes_to_color;
            for (const auto& cs : cs_to_color)
              for (const auto& box : cs->boxes())
                boxes_to_color.push_back(box);

            IntervalVector h = n->hull();
            IntervalVector u = n->unknown();

            for (const auto& bi : boxes_to_color)
            {
              IntervalVector bih = bi & h;
              if (!bih.is_empty())
              {
                if (std::get<0>(n->boxes()).is_empty())
                { 
                  n->set_boxes(std::make_tuple(std::get<1>(n->boxes()),std::get<0>(n->boxes())));
                  return false;
                }

                std::list<IntervalVector> lbi;

                for (const auto& bj : boxes_to_color)
                {
                  IntervalVector bjh = bj & h;
                  if (!bjh.is_empty())
                    lbi.push_back(bjh);
                }

                IntervalVector prev_x_in = IntervalVector::empty(dim);

                while (h != prev_x_in)
                {
                  prev_x_in = IntervalVector(h);

                  for (const auto& li : lbi)
                  {
                    std::list<IntervalVector> d = h.diff(li);

                    if (!d.empty())
                    {
                      
                      IntervalVector hi = IntervalVector::empty(dim);
                      for (const auto& di : d)
                        hi |= (h & di);
                      h &= hi;

                    }
                  }
                }
                
                n->set_boxes(std::make_tuple(hull(lbi)|u, h));
                return true;
              }
            }
            return true;
          };

        // Use the visitor to color the inner regions
        p.tree()->visit(visitor);

        // Create a separator on this paving
        SepWrapper<PavingInOut> sep_paving(p);

        // By construction, we know that we have the whole image set.
        // What is outside of the paving box Y0 is for sure ouside of it.
        SepInter sep_inter = sep_paving&Y0;

        _sep.push_back(sep_inter);
      }
      
      BoxPair separate(const IntervalVector& x) const;

    protected:

      Collection<SepBase> _sep;
  };
}