/** 
 *  \file
 *  ContractorNetwork class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_CONTRACTORNETWORK__
#define __CODAC2_CONTRACTORNETWORK__

#include <list>
#include <vector>
#include <memory>
#include <codac2_DomainNode.h>
#include <codac2_ContractorNode.h>

namespace codac2
{
  class CtcCN;
  class PropagationSpanBase;

  /**
   * \brief Builds a graph of contractors and domains, allowing constraint propagation
   * under the form of domains contractions.
   */
  class ContractorNetwork
  {
    public:

      /**
       * \brief Creates an empty graph.
       */
      ContractorNetwork();

      /**
       * \brief Creates a graph made of a list of contractors nodes.
       * The domains nodes are extracted from the contractors nodes and connected to this CN.
       * 
       * \param l a list of pointers to contractors nodes
       */
      ContractorNetwork(std::initializer_list<std::shared_ptr<ContractorNodeBase>> l);

      /**
       * \brief Adds a contractor node to the graph.
       * The related domains are extracted from the contractor node and connected to this CN.
       * 
       * \param ctc a pointer to the contractor node to be added
       */
      void add(const std::shared_ptr<ContractorNodeBase>& ctc);
      void add(const std::vector<std::shared_ptr<ContractorNodeBase>>& v_ctc);
      double contract(bool verbose = true, bool verbose_ctc_calls = false);
      void reset_variables();
      void force_fixed_point(bool force = true);
      void trigger_all_contractors();

    protected:

      void merge_into_stack(std::shared_ptr<ContractorNodeBase> ctc);

      friend class CtcCN;
      void get_views_from_cn(std::list<std::shared_ptr<PropagationSpanBase>>& l);

      std::vector<std::shared_ptr<ContractorNodeBase>> _v_ctc;
      std::vector<std::shared_ptr<DomainNodeBase>> _v_domains;
      std::list<std::shared_ptr<ContractorNodeBase>> _stack;

      bool _force_fixed_point = false;
  };
}

#endif