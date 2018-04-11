/* ============================================================================
 *  tubex-lib - Serialization tools
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2016
 * ---------------------------------------------------------------------------- */

#include "tubex_TubeSerialization.h"
#include "tubex_IntervalSerialization.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  void serializeTube(ofstream& bin_file, const Tube& tube, int version_number)
  {
    if(!bin_file.is_open())
      throw Exception("serializeTube()", "ofstream& bin_file not open");

    // Version number for compliance purposes
    bin_file.write((const char*)&version_number, sizeof(int));

    switch(version_number)
    {
      case 1:
        throw Exception("serializeTube()", "serialization version no more supported");
        break;

      case 2:
      {
        // Slices number
        int slices_number = tube.nbSlices();
        bin_file.write((const char*)&slices_number, sizeof(int));

        TubeSlice *slice;

        // Domains
        double lb;
        slice = tube.getFirstSlice();
        while(slice != NULL)
        {
          lb = slice->domain().lb();
          bin_file.write((const char*)&lb, sizeof(double));
          slice = slice->nextSlice();
        }
        lb = tube.domain().ub();
        bin_file.write((const char*)&lb, sizeof(double));

        // Codomains
        slice = tube.getFirstSlice();
        while(slice != NULL)
        {
          serializeInterval(bin_file, slice->codomain());
          slice = slice->nextSlice();
        }

        // Gates

        int nb_gates = 0;
        slice = tube.getFirstSlice();
        while(slice != NULL)
        {
          if(slice->m_input_gate != NULL)
            nb_gates++;
          slice = slice->nextSlice();
        }
        if(tube.getLastSlice()->m_output_gate != NULL)
          nb_gates++;
        bin_file.write((const char*)&nb_gates, sizeof(int));

        slice = tube.getFirstSlice();
        while(slice != NULL)
        {
          if(slice->m_input_gate != NULL)
          {
            double t = slice->domain().lb();
            bin_file.write((const char*)&t, sizeof(double));
            serializeInterval(bin_file, *slice->m_input_gate);
          }
          slice = slice->nextSlice();
        }
        
        if(tube.getLastSlice()->m_output_gate != NULL)
        {
          double t = tube.domain().ub();
          bin_file.write((const char*)&t, sizeof(double));
          serializeInterval(bin_file, *tube.getLastSlice()->m_output_gate);
        }

        break;
      }

      default:
        throw Exception("serializeTube()", "unhandled case");
    }
  }

  void deserializeTube(ifstream& bin_file, Tube& tube)
  {
    if(!bin_file.is_open())
      throw Exception("deserializeTube()", "ifstream& bin_file not open");

    if(tube.nbSlices() != 1)
      throw Exception("deserializeTube()", "tube already defined");

    // Version number for compliance purposes
    int version_number;
    bin_file.read((char*)&version_number, sizeof(int));

    switch(version_number)
    {
      case 1:
        throw Exception("deserializeTube()", "serialization version no more supported");
        break;

      case 2:
      {
        // Slices number
        int slices_number;
        bin_file.read((char*)&slices_number, sizeof(int));

        // Domains
        double domain_lb;
        bin_file.read((char*)&domain_lb, sizeof(double));

        vector<double> v_domain_bounds;
        for(int i = 1 ; i < slices_number ; i++)
        {
          double bound;
          bin_file.read((char*)&bound, sizeof(double));
          v_domain_bounds.push_back(bound);
        }

        double domain_ub;
        bin_file.read((char*)&domain_ub, sizeof(double));

        // Creating tube
        tube.m_domain = Interval(domain_lb, domain_ub);
        tube.getFirstSlice()->m_domain = Interval(domain_lb, domain_ub);
        tube.sample(v_domain_bounds);

        // Codomains
        for(int i = 0 ; i < slices_number ; i++)
        {
          Interval slice_value;
          deserializeInterval(bin_file, slice_value);
          tube.set(slice_value, i);
        }

        // Gates

        int gates_number;
        bin_file.read((char*)&gates_number, sizeof(int));

        for(int i = 0 ; i < gates_number ; i++)
        {
          double t;
          bin_file.read((char*)&t, sizeof(double));
          Interval gate;
          deserializeInterval(bin_file, gate);
          tube.set(gate, t);
        }

        break;
      }

      default:
        throw Exception("deserializeTube()", "deserialization version number not supported");
    }
  }
}