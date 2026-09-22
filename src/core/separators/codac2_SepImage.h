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
#include "codac2_OctaSym.h"
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
  /**
   * \brief A separator on the direct image of a set by a function. It relies on a boundary approach
   */
  class SepImage : public Sep<SepImage>
  {
    public:

      /**
       * \brief Constructor of the separator
       * 
       * \param f The analytic function \f$\mathbf{f}:\mathbb{R}^n\rightarrow\mathbb{R}^p,p\geq n\f$ 
       * \param psi_0 The transformation function \f$\psi_0:\mathbb{R}^m\rightarrow\mathbb{R}^n\f$ to construct the atlas
       * \param Sigma The set of symmetry operators \f$\sigma\f$ to construct the atlas
       * \param epsilon The maximum diameter of the boxes to split \f$[-1,1]^m\f$ before computing the parallelepiped inclusions
       * \param ctc_init The contractor on the initial set
       * \param epsilon_pave An optionnal parameter to custom the resolution of the paving step
       */
      template<typename C>
        requires IsCtcBaseOrPtr<C,IntervalVector>
      SepImage(const AnalyticFunction<VectorType>& f, const AnalyticFunction<VectorType>& psi_0, const std::vector<OctaSym>& Sigma, double epsilon, const C& ctc_init, double epsilon_pave = -1)
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

        // Determination of the inner connected susets
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
        // This code comes from the example coloration.py, 
        // TODO : factorize it by allowing to pass a list of ConnectedSubset as argument
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

                // there used to be a copy x_in = h there, but didn't behave as exepected

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

      /**
       * \brief Constructor of the separator
       * 
       * \param f The analytic function \f$\mathbf{f}:\mathbb{R}^n\rightarrow\mathbb{R}^p,p\geq n\f$ 
       * \param psi_0 The transformation function \f$\psi_0:\mathbb{R}^m\rightarrow\mathbb{R}^n\f$ to construct the atlas
       * \param Sigma The set of symmetry operators \f$\sigma\f$ to construct the atlas
       * \param epsilon The maximum diameter of the boxes to split \f$[-1,1]^m\f$ before computing the parallelepiped inclusions
       * \param ctc_init A shared pointer to the contractor on the initial set
       * \param epsilon_pave An optionnal parameter to custom the resolution of the paving step
       */
      template<typename C>
        requires IsCtcBaseOrPtr<C,IntervalVector>
      SepImage(const AnalyticFunction<VectorType>& f, const AnalyticFunction<VectorType>& psi_0, const std::vector<OctaSym>& Sigma, double epsilon, const std::shared_ptr<C>& ctc_init, double epsilon_pave = -1)
        : SepImage(f, psi_0, Sigma, epsilon, *ctc_init, epsilon_pave)
      { }
      
      /**
       * \brief Separates the box.
       * 
       * \param x The box to separate.
       * 
       * \return A box pair according to separator arithmetics.
       */
      BoxPair separate(const IntervalVector& x) const;

    protected:

      Collection<SepBase> _sep;
  };
}