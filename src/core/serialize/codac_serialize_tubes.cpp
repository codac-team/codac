/** 
 *  Serialization tools (tubes)
 * ----------------------------------------------------------------------------
 *  \date       2016
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac_serialize_tubes.h"
#include "codac_serialize_intervals.h"
#include "codac_Exception.h"
#include "codac_Tube.h"
#include "codac_TubeVector.h"
#include "codac_Slice.h"

using namespace std;
using namespace ibex;

namespace codac
{
  void serialize_Tube(ofstream& bin_file, const Tube& tube, int version_number)
  {
    if(!bin_file.is_open())
      throw Exception(__func__, "ofstream& bin_file not open");

    switch(version_number)
    {
      case 1:
        throw Exception(__func__, "serialization version no more supported");
        break;

      case 2:
      {
        // Version number for compliance purposes
        bin_file.write((const char*)&version_number, sizeof(short int));

        // Slices number
        int slices_number = tube.nb_slices();
        bin_file.write((const char*)&slices_number, sizeof(int));

        // Domains
        double lb;
        for(const Slice *s = tube.first_slice() ; s ; s = s->next_slice())
        {
          lb = s->tdomain().lb();
          bin_file.write((const char*)&lb, sizeof(double));
        }
        lb = tube.tdomain().ub();
        bin_file.write((const char*)&lb, sizeof(double));

        // Codomains
        for(const Slice *s = tube.first_slice() ; s ; s = s->next_slice())
          serialize_Interval(bin_file, s->codomain());

        // Gates
        serialize_Interval(bin_file, tube.first_slice()->input_gate());
        for(const Slice *s = tube.first_slice() ; s ; s = s->next_slice())
          serialize_Interval(bin_file, s->output_gate());

        break;
      }

      default:
        throw Exception(__func__, "unhandled case");
    }
  }

  void deserialize_Tube(ifstream& bin_file, Tube *&tube)
  {
    if(!bin_file.is_open())
      throw Exception(__func__, "ifstream& bin_file not open");

    // Version number for compliance purposes
    short int version_number;
    bin_file.read((char*)&version_number, sizeof(short int));

    switch(version_number)
    {
      case 1:
        throw Exception(__func__, "serialization version no more supported");
        break;

      case 2:
      {
        tube = new Tube();

        // Slices number
        int slices_number;
        bin_file.read((char*)&slices_number, sizeof(int));

        if(slices_number < 1)
          throw Exception(__func__, "wrong slices number");

        // Creating slices
        double lb;
        bin_file.read((char*)&lb, sizeof(double));
        Interval tube_tdomain(lb);

        Slice *prev_slice = nullptr, *slice = nullptr;
        for(int k = 0 ; k < slices_number ; k++)
        {
          double ub;
          bin_file.read((char*)&ub, sizeof(double));
          tube_tdomain |= Interval(lb, ub);

          if(slice == nullptr)
          {
            slice = new Slice(Interval(lb, ub));
            tube->m_first_slice = slice;
          }

          else
          {
            slice->m_next_slice = new Slice(Interval(lb, ub));
            slice = slice->next_slice();
          }

          if(prev_slice)
          {
            delete slice->m_input_gate;
            slice->m_input_gate = nullptr;
            Slice::chain_slices(prev_slice, slice);
          }

          prev_slice = slice;
          lb = ub;
        }

        // Codomains
        for(Slice *s = tube->first_slice() ; s ; s = s->next_slice())
        {
          Interval slice_value;
          deserialize_Interval(bin_file, slice_value);
          s->set(slice_value);
        }

        // Domain
        tube_tdomain |= lb;
        tube->m_tdomain = tube_tdomain; // redundant information for fast access

        // Gates
        Interval gate;
        deserialize_Interval(bin_file, gate);
        tube->first_slice()->set_input_gate(gate);
        for(Slice *s = tube->first_slice() ; s ; s = s->next_slice())
        {
          deserialize_Interval(bin_file, gate);
          s->set_output_gate(gate);
        }
        
        break;
      }

      default:
        throw Exception(__func__, "deserialization version number not supported");
    }
  }

  void serialize_TubeVector(ofstream& bin_file, const TubeVector& tube, int version_number)
  {
    if(!bin_file.is_open())
      throw Exception(__func__, "ofstream& bin_file not open");

    short int size = tube.size();
    bin_file.write((const char*)&size, sizeof(short int));
    for(int i = 0 ; i < size ; i++)
      serialize_Tube(bin_file, tube[i], version_number);
  }

  void deserialize_TubeVector(ifstream& bin_file, TubeVector *&tube)
  {
    if(!bin_file.is_open())
      throw Exception(__func__, "ifstream& bin_file not open");

    tube = new TubeVector();

    short int size;
    bin_file.read((char*)&size, sizeof(short int));
    
    tube->m_n = size;
    tube->m_v_tubes = new Tube[size];
    
    for(int i = 0 ; i < size ; i++)
    {
      Tube *ptr;
      deserialize_Tube(bin_file, ptr);
      (*tube)[i] = *ptr;
      delete ptr;
    }
  }
}