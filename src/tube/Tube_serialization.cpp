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
    - format: [short_int_intv_type][double_lb][double_ub]
    - short_int_intv_type of type IntervalType (see below)
    - in case of unbounded intervals, the two last fields disappear
*/

enum IntervalType { BOUNDED, EMPTY_SET, ALL_REALS, POS_REALS, NEG_REALS };

void serializeInterval(ofstream& binFile, const Interval& intv)
{
  short int intv_type;

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

  binFile.write((const char*)&intv_type, sizeof(short int));

  if(intv_type == BOUNDED)
  {
    double lb = intv.lb(), ub = intv.ub();
    binFile.write((const char*)&lb, sizeof(double));
    binFile.write((const char*)&ub, sizeof(double));
  }
}

void deserializeInterval(ifstream& binFile, Interval& intv)
{
  short int intv_type;
  binFile.read((char*)&intv_type, sizeof(short int));

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
        cout << "Tube::deserializeInterval(...) unhandled case" << endl;
        return;
  }
}

/*
  Tube binary files structure (VERSION 1)
    - minimal storage
    - format: [short_int_version_number]
              [int_slices_number]
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

bool Tube::serialize(const string& binary_file_name) const
{
  map<double,double> void_real_values;
  return serialize(binary_file_name, void_real_values);
}

bool Tube::serialize(const string& binary_file_name, const map<double,double>& real_values) const
{
  ofstream binFile(binary_file_name.c_str(), ios::out | ios::binary);

  if(!binFile.is_open())
  {
    cout << "Tube::serialize(binary_file_name) error while writing file:"
         << "\"" << binary_file_name << "\"" << endl;
    return false;
  }

  // Version number for compliance purposes
  short int version = CURRENT_VERSION_NUMBER;
  binFile.write((const char*)&version, sizeof(short int));

  // Slices number
  int slices_number = size();
  binFile.write((const char*)&slices_number, sizeof(int));

  // Domain
  serializeInterval(binFile, domain());

  // Slices
  for(int i = 0 ; i < slices_number ; i++)
    serializeInterval(binFile, (*this)[i]);

  // Optional real values
  int number_real_values = real_values.size();
  binFile.write((const char*)&number_real_values, sizeof(int));
  typename map<double,double>::const_iterator it;
  for(it = real_values.begin(); it != real_values.end(); it++)
  {
    double real_t = it->first, real_y = it->second;
    binFile.write((const char*)&real_t, sizeof(double));
    binFile.write((const char*)&real_y, sizeof(double));
  }

  binFile.close();
  return true;
}

void Tube::deserialize(const string& binary_file_name, map<double,double>& real_values)
{
  ifstream binFile(binary_file_name.c_str(), ios::in | ios::binary);

  if(!binFile.is_open())
  {
    cout << "Tube::Tube(binary_file_name) error while opening file:"
         << "\"" << binary_file_name << "\"" << endl;
    return;
  }

  // Version number for compliance purposes
  short int version_number;
  binFile.read((char*)&version_number, sizeof(short int));

  if(version_number == 1)
  {
    // Slices number
    int slices_number;
    binFile.read((char*)&slices_number, sizeof(int));

    // Domain
    Interval domain;
    deserializeInterval(binFile, domain);
    createFromSpecifications(domain, domain.diam() / slices_number);

    // Slices
    for(int i = 0 ; i < slices_number ; i++)
    {
      Interval slice_value;
      deserializeInterval(binFile, slice_value);
      set(slice_value, i);
    }

    // Optional real values
    int number_real_values;
    binFile.read((char*)&number_real_values, sizeof(int));
    
    for(int i = 0 ; i < number_real_values ; i++)
    {
      double real_t, real_y;
      binFile.read((char*)&real_t, sizeof(double));
      binFile.read((char*)&real_y, sizeof(double));
      real_values[real_t] = real_y;
    }
  }

  else
  {
    cout << "Tube::Tube(binary_file_name) deserialization version number "
         << version_number << " not supported." << endl;
    return;
  }

  binFile.close();
}