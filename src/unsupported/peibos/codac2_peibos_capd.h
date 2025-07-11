/** 
 *  \file codac2_peibos_capd.h
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_peibos.h"

#include <codac2_capd.h>

#include <capd/poincare/lib.h>


namespace codac2
{
  class IMapWrapper
  {
    public:
      IMapWrapper(const string& s);

      void setParameter(const string& name, double value);
      void setParameter(const string& name, Interval value);

      capd::IMap gamma;
  };

  vector<Parallelepiped> PEIBOS(const IMapWrapper& i_map_wrapper, double tf, const AnalyticFunction<VectorType>& psi_0, const vector<vector<int>>& generators , double epsilon, bool verbose = false);
  vector<Parallelepiped> PEIBOS(const IMapWrapper& i_map_wrapper, double tf, const AnalyticFunction<VectorType>& psi_0, const vector<vector<int>>& generators , double epsilon, const Vector& offset, bool verbose = false);

  map<double,vector<Parallelepiped>> PEIBOS(const IMapWrapper& i_map_wrapper, double tf, double dt, const AnalyticFunction<VectorType>& psi_0, const vector<vector<int>>& generators , double epsilon, bool verbose = false);
  map<double,vector<Parallelepiped>> PEIBOS(const IMapWrapper& i_map_wrapper, double tf, double dt, const AnalyticFunction<VectorType>& psi_0, const vector<vector<int>>& generators , double epsilon, const Vector& offset, bool verbose = false);
}