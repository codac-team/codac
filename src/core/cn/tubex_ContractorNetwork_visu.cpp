/** 
 *  ContractorNetwork class : export to DOT language
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <iostream>
#include <fstream>
#include "tubex_Tools.h"
#include "tubex_ContractorNetwork.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  // Public methods

    // Visualization

    void ContractorNetwork::set_name(Domain dom, const string& name)
    {
      Domain *dom_ptr = add_dom(dom);
      dom_ptr->set_name(name);
    }
    
    void ContractorNetwork::set_name(Ctc& ctc, const string& name)
    {
      #ifndef NDEBUG
        bool contractor_found = false;
      #endif

      for(const auto& added_ctc: m_v_ctc)
        if(added_ctc->type() == Contractor::Type::T_IBEX && &added_ctc->ibex_ctc() == &ctc)
        {
          added_ctc->set_name(name);
          #ifndef NDEBUG
            contractor_found = true;
          #endif
        }

      assert(contractor_found);
    }
    
    void ContractorNetwork::set_name(DynCtc& ctc, const string& name)
    {
      #ifndef NDEBUG
        bool contractor_found = false;
      #endif

      for(const auto& added_ctc: m_v_ctc)
        if(added_ctc->type() == Contractor::Type::T_TUBEX && &added_ctc->tubex_ctc() == &ctc)
        {
          added_ctc->set_name(name);
          #ifndef NDEBUG
            contractor_found = true;
          #endif
        }

      assert(contractor_found);
    }

    int ContractorNetwork::print_dot_graph(const string& cn_name, const string& layer_model) const
    {
      if(m_v_domains.size() > 100 || m_v_ctc.size() > 100)
        cout << "Warning: important number of domains/contractors in the graph, may not be able to generate the diagram." << endl;

      ofstream dot_file;
      dot_file.open(cn_name + ".dot");

      dot_file << "graph graphname {" << endl;
      dot_file << "  overlap=\"prism\"" << endl;
      dot_file << "  splines=\"compound\"" << endl;

      dot_file << endl << "  // Domains nodes" << endl;
      for(const auto dom : m_v_domains)
        dot_file << "  " << Tools::add_int("dom",dom->id()) << " [shape=box, label=\"" << dom->dom_name(m_v_domains) << "\"];" << endl;

      dot_file << endl << "  // Contractors nodes" << endl;
      for(const auto ctc : m_v_ctc)
      {
        dot_file << "  " << Tools::add_int("ctc",ctc->id())
                 // Node style:
                 << " [shape=circle, "
                 << "label=\"" << ctc->name() << "\"];" << endl;
      }

      dot_file << endl << "  // Relations" << endl;
      for(const auto ctc : m_v_ctc)
        for(const auto dom : m_v_domains)
          if(find(dom->contractors().begin(), dom->contractors().end(), ctc) != dom->contractors().end())
            dot_file << "  " << Tools::add_int("ctc",ctc->id()) << " -- " << Tools::add_int("dom",dom->id()) << ";" << endl;

      // Subgraph for clustering components of a same vector
      for(const auto dom : m_v_domains)
      {
        if(dom->type() == Domain::Type::T_INTERVAL_VECTOR)
        {
          dot_file << endl;
          dot_file << "  subgraph cluster_" << Tools::add_int("dom",dom->id()) << " {" << endl;
          dot_file << "    color=\"#006680\";" << endl << "    ";

          // Adding the main vector
          dot_file << Tools::add_int("dom",dom->id()) + "; ";

          // Adding its components
          Domain *one_component = NULL;
          for(const auto dom_i : m_v_domains) // todo: a fast get_components method
            if(dom_i->is_component_of(*dom))
            {
              one_component = dom_i;
              dot_file << Tools::add_int("dom",dom_i->id()) + "; ";
            }

          // Adding their component-contractor
          if(one_component != NULL) // todo: transform it as an assert
          for(const auto ctc : m_v_ctc)
            if(ctc->type() == Contractor::Type::T_COMPONENT)
              for(const auto& dom : ctc->domains())
                if(dom == one_component)
                {
                  dot_file << Tools::add_int("ctc",ctc->id()) + "; ";
                  break;
                }

          dot_file << endl << "  }" << endl;
        }
      }

      // Subgraphs for tubes and their slices
      for(const auto dom : m_v_domains)
      {
        if(dom->type() == Domain::Type::T_TUBE)
        {
          dot_file << endl;
          dot_file << "  " << Tools::add_int("subgraph cluster_tube",dom->id()) << " {" << endl;
          dot_file << "    color=\"#BA4E00\";" << endl;
          dot_file << "    ";

          // Looking for all domains and contractors exclusively related to this tube
          for(const auto& ctc : dom->contractors())
          {
            for(const auto& dom_i : ctc->domains())
            {
              if(dom_i != dom && dom_i->type() != Domain::Type::T_SLICE)
                break; // we are not dealing with the slice-component contractor

              // At this point we are dealing with either the tube or its slices
              if(dom_i->type() == Domain::Type::T_SLICE)
              {
                for(const auto& ctc_dom_i : dom_i->contractors())
                  if(ctc_dom_i->type() == Contractor::Type::T_COMPONENT
                    && ctc_dom_i->domains().size() == 2
                    && ((ctc_dom_i->domains()[0] == dom_i && ctc_dom_i->domains()[1]->type() == Domain::Type::T_SLICE) ||
                        (ctc_dom_i->domains()[1] == dom_i && ctc_dom_i->domains()[0]->type() == Domain::Type::T_SLICE)))
                dot_file << Tools::add_int("ctc",ctc_dom_i->id()) + "; "; // component contractor linking slices
              }

              dot_file << Tools::add_int("dom",dom_i->id()) + "; ";
              dot_file << Tools::add_int("ctc",ctc->id()) + "; ";
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
      for(const auto& dom : cn.m_v_domains)
        str << *dom << endl;
      return str;
    }
}