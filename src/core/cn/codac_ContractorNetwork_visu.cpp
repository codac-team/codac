/** 
 *  ContractorNetwork class : export to DOT language
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <iostream>
#include <fstream>
#include "codac_Tools.h"
#include "codac_ContractorNetwork.h"
#include "codac_Exception.h"

using namespace std;
using namespace ibex;

namespace codac
{
  // Public methods

    // Visualization

    void ContractorNetwork::set_name(Domain dom, const string& name)
    {
      // DomainHashcode hash(dom);
      // if(m_map_domains.find(hash) == m_map_domains.end())
      //   throw Exception(__func__, "domain cannot be found in CN");
      //   
      // m_map_domains[hash]->set_name(name);
      add_dom(dom)->set_name(name);
    }
    
    void ContractorNetwork::set_name(Ctc& ctc, const string& name)
    {
      bool contractor_found = false;

      for(auto& added_ctc : m_map_ctc)
        if(added_ctc.second->type() == Contractor::Type::T_IBEX && &added_ctc.second->ibex_ctc() == &ctc)
        {
          added_ctc.second->set_name(name);
          contractor_found = true;
        }

      if(!contractor_found)
        throw Exception(__func__, "contractor cannot be found in CN");
    }
    
    void ContractorNetwork::set_name(DynCtc& ctc, const string& name)
    {
      bool contractor_found = false;

      for(auto& added_ctc : m_map_ctc)
        if(added_ctc.second->type() == Contractor::Type::T_CODAC && &added_ctc.second->codac_ctc() == &ctc)
        {
          added_ctc.second->set_name(name);
            contractor_found = true;
        }

      if(!contractor_found)
        throw Exception(__func__, "contractor cannot be found in CN");
    }

    int ContractorNetwork::print_dot_graph(const string& cn_name, const string& layer_model) const
    {
      if(m_map_domains.size() > 100 || m_map_ctc.size() > 100)
        cout << "Warning: important number of domains/contractors in the graph, may not be able to generate the diagram." << endl;

      ofstream dot_file;
      dot_file.open(cn_name + ".dot");

      dot_file << "graph graphname {" << endl;
      dot_file << "  overlap=\"prism\"" << endl;
      dot_file << "  splines=\"compound\"" << endl;

      dot_file << endl << "  // Domains nodes" << endl;
      for(const auto& dom : m_map_domains)
        dot_file << "  " << ("dom" + std::to_string(dom.second->id())) << " [shape=box, label=\"" << dom.second->dom_name(m_map_domains) << "\"];" << endl;

      dot_file << endl << "  // Contractors nodes" << endl;
      for(auto& ctc : m_map_ctc)
      {
        dot_file << "  " << ("ctc" + std::to_string(ctc.second->id()))
                 // Node style:
                 << " [shape=circle, "
                 << "label=\"" << ctc.second->name() << "\"];" << endl;
      }

      dot_file << endl << "  // Relations" << endl;
      for(auto& ctc : m_map_ctc)
        for(const auto& dom : m_map_domains)
          if(find(dom.second->contractors().begin(), dom.second->contractors().end(), ctc.second) != dom.second->contractors().end())
            dot_file << "  " << ("ctc" + std::to_string(ctc.second->id())) << " -- " << ("dom" + std::to_string(dom.second->id())) << ";" << endl;

      // Subgraph for clustering components of a same vector
      for(const auto& dom : m_map_domains)
      {
        if(dom.second->type() == Domain::Type::T_INTERVAL_VECTOR)
        {
          dot_file << endl;
          dot_file << "  subgraph cluster_" << ("dom" + std::to_string(dom.second->id())) << " {" << endl;
          dot_file << "    color=\"#006680\";" << endl << "    ";

          // Adding the main vector
          dot_file << ("dom" + std::to_string(dom.second->id())) + "; ";

          // Adding its components
          Domain *one_component = nullptr;
          for(const auto& dom_i : m_map_domains) // todo: a fast get_components method
            if(dom_i.second->is_component_of(*dom.second))
            {
              one_component = dom_i.second;
              dot_file << ("dom" + std::to_string(dom_i.second->id())) + "; ";
            }

          // Adding their component-contractor
          if(one_component) // todo: transform it as an assert
          for(auto& ctc : m_map_ctc)
            if(ctc.second->type() == Contractor::Type::T_COMPONENT)
              for(const auto& dom_i : ctc.second->domains())
                if(dom_i == one_component)
                {
                  dot_file << ("ctc" + std::to_string(ctc.second->id())) + "; ";
                  break;
                }

          dot_file << endl << "  }" << endl;
        }
      }

      // Subgraphs for tubes and their slices
      for(const auto& dom : m_map_domains)
      {
        if(dom.second->type() == Domain::Type::T_TUBE)
        {
          dot_file << endl;
          dot_file << "  " << ("subgraph cluster_tube" + std::to_string(dom.second->id())) << " {" << endl;
          dot_file << "    color=\"#BA4E00\";" << endl;
          dot_file << "    ";

          // Looking for all domains and contractors exclusively related to this tube
          for(const auto& ctc : dom.second->contractors())
          {
            for(const auto& dom_i : ctc->domains())
            {
              if(dom_i != dom.second && dom_i->type() != Domain::Type::T_SLICE)
                break; // we are not dealing with the slice-component contractor

              // At this point we are dealing with either the tube or its slices
              if(dom_i->type() == Domain::Type::T_SLICE)
              {
                for(const auto& ctc_dom_i : dom_i->contractors())
                  if(ctc_dom_i->type() == Contractor::Type::T_COMPONENT
                    && ctc_dom_i->domains().size() == 2
                    && ((ctc_dom_i->domains()[0] == dom_i && ctc_dom_i->domains()[1]->type() == Domain::Type::T_SLICE) ||
                        (ctc_dom_i->domains()[1] == dom_i && ctc_dom_i->domains()[0]->type() == Domain::Type::T_SLICE)))
                dot_file << ("ctc" + std::to_string(ctc_dom_i->id()) + "; "); // component contractor linking slices
              }

              dot_file << ("dom" + std::to_string(dom_i->id()) + "; ");
              dot_file << ("ctc" + std::to_string(ctc->id()) + "; ");
            }
          }

          dot_file << endl << "  }" << endl;
        }
      }

      dot_file << "}" << endl;

      dot_file.close();

      return 0;
      //return system((("dot2tex --crop --texmode=math --autosize --prog=" + layer_model + " -ftikz ") + cn_name + ".dot > "
      //               + cn_name + ".tex ; pdflatex " + cn_name + ".tex > /dev/null").c_str());    
    }

    ostream& operator<<(ostream& str, const ContractorNetwork& cn)
    {
      str << cn.nb_ctc() << " contractors\n";
      str << cn.nb_dom() << " domains:\n";
      for(const auto& dom : cn.m_map_domains)
        str << *dom.second << endl;
      return str;
    }
}