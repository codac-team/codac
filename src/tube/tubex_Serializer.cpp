/* ============================================================================
 *  tubex-lib - Tube class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2016
 * ---------------------------------------------------------------------------- */

#include "tubex_Serialization.h"
#include "tubex_Tube.h"
#include "exceptions/tubex_Exception.h"
#include <iomanip> // for setprecision()
#ifdef _OPENMP
  #include <omp.h> // for multithreading
#endif

using namespace std;
using namespace ibex;

namespace tubex
{
  /*
    Tube binary files structure (VERSION 1)
      - minimal storage
      - format: [char_version_number]
                [double_dt_spec]
                [Interval_domain]
                [Interval_slice1]
                [Interval_slice2]
                ...
                [int_number_real_values]
                [double_value1_t]
                [double_value1_y]
                [double_value2_t]
                ...
  */

  Tube::Tube(const string& binary_file_name)
  {
    map<double,double> void_map;
    deserialize(binary_file_name, void_map);
  }

  Tube::Tube(const string& binary_file_name, map<double,double> &real_values)
  {
    deserialize(binary_file_name, real_values);
  }

  Tube::Tube(const string& binary_file_name, vector<map<double,double> > &v_real_values)
  {
    deserialize(binary_file_name, v_real_values);
  }

  bool Tube::serialize(const string& binary_file_name, int version_number) const
  {
    map<double,double> void_real_values;
    return serialize(binary_file_name, void_real_values, version_number);
  }

  bool Tube::serialize(const string& binary_file_name, const map<double,double>& real_values, int version_number) const
  {
    vector<map<double,double> > v_real_values;
    v_real_values.push_back(real_values);
    return serialize(binary_file_name, v_real_values, version_number);
  }

  bool Tube::serialize(const string& binary_file_name, const vector<map<double,double> >& v_real_values, int version_number) const
  {
    ofstream binFile(binary_file_name.c_str(), ios::out | ios::binary);

    if(!binFile.is_open())
      throw Exception("Tube::serialize(binary_file_name)", "error while writing file \"" + binary_file_name + "\"");

    // Version number for compliance purposes
    binFile.write((const char*)&version_number, sizeof(char));

    switch(version_number)
    {
      case 1:
      {
        // Timestep
        double dt_specs = m_dt_specifications;
        binFile.write((const char*)&dt_specs, sizeof(double));

        // Domain
        serializeInterval(binFile, domain());

        // Slices
        for(int i = 0 ; i < size() ; i++)
          serializeInterval(binFile, (*this)[i]);

        // Optional real values
        int number_maps = v_real_values.size();
        binFile.write((const char*)&number_maps, sizeof(int));

        for(int i = 0 ; i < v_real_values.size() ; i++)
        {
          int number_real_values = v_real_values[i].size();
          binFile.write((const char*)&number_real_values, sizeof(int));
          typename map<double,double>::const_iterator it;
          for(it = v_real_values[i].begin(); it != v_real_values[i].end(); it++)
          {
            double real_t = it->first, real_y = it->second;
            binFile.write((const char*)&real_t, sizeof(double));
            binFile.write((const char*)&real_y, sizeof(double));
          }
        }

        break;
      }

      default:
        throw Exception("Tube::serialize()", "unhandled case");
    }

    binFile.close();
    return true;
  }

  void Tube::deserialize(const string& binary_file_name, map<double,double>& real_values)
  {
    vector<map<double,double> > v_real_values;
    deserialize(binary_file_name, v_real_values);
    real_values = v_real_values[0];
  }

  void Tube::deserialize(const string& binary_file_name, vector<map<double,double> >& v_real_values)
  {
    ifstream binFile(binary_file_name.c_str(), ios::in | ios::binary);

    if(!binFile.is_open())
      throw Exception("Tube::serialize(binary_file_name)", "error while opening file \"" + binary_file_name + "\"");

    // Version number for compliance purposes
    char version_number;
    binFile.read((char*)&version_number, sizeof(char));

    if(version_number == 1)
    {
      // Timestep
      double dt_specs;
      binFile.read((char*)&dt_specs, sizeof(double));

      // Domain
      Interval domain;
      deserializeInterval(binFile, domain);
      createFromSpecifications(domain, dt_specs);

      // Slices
      for(int i = 0 ; i < size() ; i++)
      {
        Interval slice_value;
        deserializeInterval(binFile, slice_value);
        set(slice_value, i);
      }

      // Optional real values
      int number_maps;
      binFile.read((char*)&number_maps, sizeof(int));
      for(int j = 0 ; j < number_maps ; j++)
      {
        map<double,double> real_values;
        int number_real_values;
        binFile.read((char*)&number_real_values, sizeof(int));

        for(int i = 0 ; i < number_real_values ; i++)
        {
          double real_t, real_y;
          binFile.read((char*)&real_t, sizeof(double));
          binFile.read((char*)&real_y, sizeof(double));
          real_values[real_t] = real_y;
        }

        v_real_values.push_back(real_values);
      }
    }

    else
      throw Exception("Tube::Tube(binary_file_name)", "deserialization version number not supported");

    binFile.close();
  }
}