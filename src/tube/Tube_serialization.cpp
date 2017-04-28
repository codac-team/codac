/* ============================================================================
 *  tube-lib - Tube class
 * ============================================================================
 *  Copyright : Copyright 2016 Simon Rohou
 *  License   : This program can be distributed under the terms of
 *              the Apache License, Version 2.0. See the file LICENSE.
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2016
 * ---------------------------------------------------------------------------- */

#include "Tube.h"
#include "exceptions/TubeException.h"
#include <iostream>
#include <fstream>
#include <iomanip> // for setprecision()
#ifdef _OPENMP
  #include <omp.h> // for multithreading
#endif

using namespace std;
using namespace ibex;

#define CURRENT_VERSION_NUMBER 1

/*
  Interval binary structure (VERSION 1)
    - format: [char_intv_type][double_lb][double_ub]
    - short_int_intv_type of type IntervalType (see below)
    - in case of unbounded intervals, the two last fields disappear
*/

enum IntervalType { BOUNDED, EMPTY_SET, ALL_REALS, POS_REALS, NEG_REALS };

void serializeInterval(ofstream& binFile, const Interval& intv)
{
  char intv_type;

  if(intv == Interval::EMPTY_SET)
    intv_type = EMPTY_SET;

  else if(intv == Interval::ALL_REALS)
    intv_type = ALL_REALS;

  else if(intv == Interval::POS_REALS)
    intv_type = POS_REALS;

  else if(intv == Interval::NEG_REALS)
    intv_type = NEG_REALS;

  else
    intv_type = BOUNDED;

  binFile.write((const char*)&intv_type, sizeof(char));

  if(intv_type == BOUNDED)
  {
    double lb = intv.lb(), ub = intv.ub();
    binFile.write((const char*)&lb, sizeof(double));
    binFile.write((const char*)&ub, sizeof(double));
  }
}

void deserializeInterval(ifstream& binFile, Interval& intv)
{
  char intv_type;
  binFile.read((char*)&intv_type, sizeof(char));

  switch(intv_type)
  {
    case EMPTY_SET:
      intv = Interval::EMPTY_SET;
      break;

    case ALL_REALS:
      intv = Interval::ALL_REALS;
      break;

    case POS_REALS:
      intv = Interval::POS_REALS;
      break;

    case NEG_REALS:
      intv = Interval::NEG_REALS;
      break;

    case BOUNDED:
      double lb, ub;
      binFile.read((char*)&lb, sizeof(double));
      binFile.read((char*)&ub, sizeof(double));
      intv = Interval(lb, ub);
      break;

    default:
      throw TubeException("Tube::deserializeInterval(...)", "unhandled case");
  }
}

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

bool Tube::serialize(const string& binary_file_name) const
{
  map<double,double> void_real_values;
  return serialize(binary_file_name, void_real_values);
}

bool Tube::serialize(const string& binary_file_name, const map<double,double>& real_values) const
{
  vector<map<double,double> > v_real_values;
  v_real_values.push_back(real_values);
  return serialize(binary_file_name, v_real_values);
}

bool Tube::serialize(const string& binary_file_name, const vector<map<double,double> >& v_real_values) const
{
  ofstream binFile(binary_file_name.c_str(), ios::out | ios::binary);

  if(!binFile.is_open())
    throw TubeException("Tube::serialize(binary_file_name)", "error while writing file \"" + binary_file_name + "\"");

  // Version number for compliance purposes
  char version = CURRENT_VERSION_NUMBER;
  binFile.write((const char*)&version, sizeof(char));

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
    throw TubeException("Tube::serialize(binary_file_name)", "error while opening file \"" + binary_file_name + "\"");

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
    throw TubeException("Tube::Tube(binary_file_name)", "deserialization version number not supported");

  binFile.close();
}