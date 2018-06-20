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
#include "tubex_Exception.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  /*
    Tube binary files structure (VERSION 2)
      - minimal storage
      - format: [char_version_number]
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

  void serializeTube(ofstream& bin_file, const TubeVector& tube, int version_number)
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
        slice = tube.getFirstSlice();
        serializeInterval(bin_file, slice->inputGate());
        while(slice != NULL)
        {
          serializeInterval(bin_file, slice->outputGate());
          slice = slice->nextSlice();
        }

        break;
      }

      default:
        throw Exception("serializeTube()", "unhandled case");
    }
  }

  void deserializeTube(ifstream& bin_file, TubeVector& tube)
  {
    if(!bin_file.is_open())
      throw Exception("deserializeTube()", "ifstream& bin_file not open");

    if(tube.nbSlices() != 0)
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
          TubeSlice *slice = new TubeSlice(Interval(lb, ub), 0); // todo: set dim
          slice->setTubeReference(&tube);
          tube.m_v_slices.push_back(slice);
          lb = ub;

          if(prev_slice != NULL)
            TubeSlice::chainSlices(prev_slice, slice);
          prev_slice = slice;
        }

        // Codomains
        TubeSlice *slice = tube.getFirstSlice();
        while(slice != NULL)
        {
          IntervalVector slice_value(1);
          deserializeInterval(bin_file, slice_value);
          slice->set(slice_value);
          slice = slice->nextSlice();
        }

        // Gates

        int gates_number = slices_number + 1;
        for(int i = 0 ; i < gates_number ; i++)
        {
          IntervalVector gate(1);
          deserializeInterval(bin_file, gate);

          double t;
          if(i < gates_number - 1)
            t = tube.getSlice(i)->domain().lb();
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