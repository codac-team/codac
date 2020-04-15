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
#include "tubex_ContractorNetwork.h"
#include "tubex_Figure.h" // for add_suffix

using namespace std;

namespace tubex
{
  void ContractorNetwork::set_name(Domain dom, const string& name)
  {
    Domain *dom_ptr = add_dom(dom);
    dom_ptr->set_name(name);
  }
  
  void ContractorNetwork::set_name(const Contractor& ctc, const string& name)
  {
    bool contractor_found = false;
    for(const auto& added_ctc: m_v_ctc)
      if(added_ctc->comes_from(ctc))
      {
        added_ctc->set_name(name);
        contractor_found = true;
      }

    assert(contractor_found);
  }

  int ContractorNetwork::print_dot_graph(const string& cn_name, const string& prog) const
  {
    ofstream dot_file;
    dot_file.open(cn_name + ".dot");

    dot_file << "graph graphname {" << endl;
    dot_file << "  overlap=\"prism4000\"" << endl;
    dot_file << "  splines=\"compound\"" << endl;

    dot_file << endl << "  // Domains nodes" << endl;
    for(const auto dom : m_v_domains)
      dot_file << "  " << Figure::add_suffix("dom",dom->id()) << " [shape=box, label=\"" << dom->dom_name(m_v_domains) << "\"];" << endl;

    dot_file << endl << "  // Contractors nodes" << endl;
    for(const auto ctc : m_v_ctc)
    {
      dot_file << "  " << Figure::add_suffix("ctc",ctc->id())
               // Node style:
               << " [shape=circle, "
               << "label=\"" << ctc->name() << "\"];" << endl;
    }

    dot_file << endl << "  // Relations" << endl;
    for(const auto ctc : m_v_ctc)
      for(const auto dom : m_v_domains)
        if(find(dom->contractors().begin(), dom->contractors().end(), ctc) != dom->contractors().end())
          dot_file << "  " << Figure::add_suffix("ctc",ctc->id()) << " -- " << Figure::add_suffix("dom",dom->id()) << ";" << endl;

    // Subgraph for clustering components of a same vector
    for(const auto dom : m_v_domains)
    {
      if(dom->type() == Domain::Type::INTERVAL_VECTOR)
      {
        dot_file << endl;
        dot_file << "  subgraph cluster_" << Figure::add_suffix("dom",dom->id()) << " {" << endl;
        dot_file << "    color=\"#006680\";" << endl << "    ";

        // Adding the main vector
        dot_file << Figure::add_suffix("dom",dom->id()) + "; ";

        // Adding its components
        Domain *one_component = NULL;
        for(const auto dom_i : m_v_domains) // todo: a fast get_components method
          if(dom_i->is_component_of(*dom))
          {
            one_component = dom_i;
            dot_file << Figure::add_suffix("dom",dom_i->id()) + "; ";
          }

        // Adding their component-contractor
        if(one_component != NULL) // todo: transform it as an assert
        for(const auto ctc : m_v_ctc)
          if(ctc->type() == Contractor::Type::COMPONENT)
            for(const auto& dom : ctc->domains())
              if(dom == one_component)
              {
                dot_file << Figure::add_suffix("ctc",ctc->id()) + "; ";
                break;
              }

        dot_file << endl << "  }" << endl;
      }
    }

    // Subgraph for aliases (contractors EQUALITY)
    /*for(const auto ctc : m_v_ctc)
    {
      if(ctc->type() == Contractor::Type::EQUALITY)
      {
        dot_file << endl;
        dot_file << "  " << Figure::add_suffix("subgraph cluster_ctc_equal",ctc->id()) << " {" << endl;
        dot_file << "  color=\"#006680\";" << endl;
        dot_file << "  label=\"" + Figure::add_suffix("ctc_equal",ctc->id()) + "\";" << endl;
        dot_file << "    ";
        for(const auto dom : ctc->domains())
          dot_file << Figure::add_suffix("dom",dom->id()) + "; ";
        dot_file << endl << "  }" << endl;
      }
    }*/

    dot_file << "}" << endl;

    dot_file.close();

    return system((("dot2tex --crop --texmode=math --autosize --prog=" + prog + " -ftikz ") + cn_name + ".dot > "
                   + cn_name + ".tex ; pdflatex " + cn_name + ".tex > /dev/null").c_str());    
  }
}