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

#include "tubex_TubeVectorSerialization.h"
#include "tubex_IntervalSerialization.h"
#include "tubex_Exception.h"
#include "tubex_DimensionException.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  /*
    Tube binary files structure (VERSION 2)
      - minimal storage
      - format: [char_version_number]
                [int_dim]
                [int_nb_slices]
                [double_t0]
                [double_t1] // time input shared by 1rst and 2nd slices
                [double_t2]
                ...
                [interval_y0] // value of 1rst slice
                [interval_y0]
                ...
                [gate_t0] // value of 1rst gate
                [gate_t1]
                ...
  */

  void serialize_tubevector(ofstream& bin_file, const TubeVector& tube, int version_number)
  {
    if(!bin_file.is_open())
      throw Exception("serializeTube()", "ofstream& bin_file not open");

    // Version number for compliance purposes
    bin_file.write((const char*)&version_number, sizeof(char));

    switch(version_number)
    {
      case 1:
        throw Exception("serializeTube()", "serialization version no more supported");
        break;

      case 2:
      {
        // Slices number
        int dim = tube.dim();
        bin_file.write((const char*)&dim, sizeof(int));

        // Slices number
        int slices_number = tube.nb_slices();
        bin_file.write((const char*)&slices_number, sizeof(int));

        const TubeSlice *slice;

        // Domains
        double lb;
        slice = tube.get_first_slice();
        while(slice != NULL)
        {
          lb = slice->domain().lb();
          bin_file.write((const char*)&lb, sizeof(double));
          slice = slice->next_slice();
        }
        lb = tube.domain().ub();
        bin_file.write((const char*)&lb, sizeof(double));

        // Codomains
        slice = tube.get_first_slice();
        while(slice != NULL)
        {
          serialize_intervalvector(bin_file, slice->codomain());
          slice = slice->next_slice();
        }

        // Gates
        slice = tube.get_first_slice();
        serialize_intervalvector(bin_file, slice->input_gate());
        while(slice != NULL)
        {
          serialize_intervalvector(bin_file, slice->output_gate());
          slice = slice->next_slice();
        }

        break;
      }

      default:
        throw Exception("serializeTube()", "unhandled case");
    }
  }

  void deserialize_tubevector(ifstream& bin_file, TubeVector& tube)
  {
    if(!bin_file.is_open())
      throw Exception("deserializeTube()", "ifstream& bin_file not open");

    if(tube.nb_slices() != 0)
      throw Exception("deserializeTube()", "tube's already defined");

    // Version number for compliance purposes
    char version_number;
    bin_file.read((char*)&version_number, sizeof(char));

    switch(version_number)
    {
      case 1:
        throw Exception("deserializeTube()", "serialization version no more supported");
        break;

      case 2:
      {
        // Dimension
        int dim;
        bin_file.read((char*)&dim, sizeof(int));
        DimensionException::check(dim);

        // Slices number
        int slices_number;
        bin_file.read((char*)&slices_number, sizeof(int));

        if(slices_number < 1)
          throw Exception("deserializeTube()", "wrong slices number");

        // Creating slices
        double lb;
        bin_file.read((char*)&lb, sizeof(double));

        TubeSlice *prev_slice = NULL;
        vector<double> v_domain_bounds;
        for(int i = 0 ; i < slices_number ; i++)
        {
          double ub;
          bin_file.read((char*)&ub, sizeof(double));
          TubeSlice *slice = new TubeSlice(Interval(lb, ub), dim);
          slice->set_tube_ref(&tube);
          tube.m_v_slices.push_back(slice);
          lb = ub;

          if(prev_slice != NULL)
          {
            delete slice->m_input_gate;
            slice->m_input_gate = NULL;
            TubeSlice::chain_slices(prev_slice, slice);
          }
          prev_slice = slice;
        }

        // Codomains
        TubeSlice *slice = tube.get_first_slice();
        while(slice != NULL)
        {
          IntervalVector slice_value(dim);
          deserialize_intervalvector(bin_file, slice_value);
          slice->set(slice_value);
          slice = slice->next_slice();
        }

        // Gates

        int gates_number = slices_number + 1;
        for(int i = 0 ; i < gates_number ; i++)
        {
          IntervalVector gate(dim);
          deserialize_intervalvector(bin_file, gate);

          double t;
          if(i < gates_number - 1)
            t = tube.get_slice(i)->domain().lb();
          else
            t = tube.domain().ub();

          tube.set(gate, t);
        }

        break;
      }

      default:
        throw Exception("deserializeTube()", "deserialization version number not supported");
    }
  }
}