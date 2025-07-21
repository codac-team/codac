/** 
 *  \file codac2_serialize.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <fstream>
#include "codac2_Vector.h"
#include "codac2_SampledTraj.h"

namespace codac2
{
  // Trivial types (int, float, double...)

    template <typename T>
      requires (std::is_trivially_copyable_v<T>)
    void serialize(std::ofstream& f, const T& x)
    {
      f.write(reinterpret_cast<const char*>(&x), sizeof(T));
    }

    template <typename T>
      requires (std::is_trivially_copyable_v<T>)
    void deserialize(std::ifstream& f, T& x)
    {
      f.read(reinterpret_cast<char*>(&x), sizeof(T));
    }

  // Vectors and matrices

    template<typename T,int R=-1,int C=-1>
    void serialize(std::ofstream& f, const Eigen::Matrix<T,R,C>& x)
    {
      Index r = x.rows(), c = x.cols();
      f.write(reinterpret_cast<const char*>(&r), sizeof(Index));
      f.write(reinterpret_cast<const char*>(&c), sizeof(Index));
      for(Index i = 0 ; i < r ; i++)
        for(Index j = 0 ; j < c ; j++)
          serialize(f,x(i,j));
    }

    template<typename T,int R=-1,int C=-1>
    void deserialize(std::ifstream& f, Eigen::Matrix<T,R,C>& x)
    {
      Index r, c;
      f.read(reinterpret_cast<char*>(&r), sizeof(Index));
      f.read(reinterpret_cast<char*>(&c), sizeof(Index));

      if constexpr(R == -1 && C == -1)
        x.resize(r,c);
      else if constexpr(R == -1 || C == -1)
        x.resize(std::max(r,c));
      else
        assert_release(R == r && C == c);

      for(Index i = 0 ; i < r ; i++)
        for(Index j = 0 ; j < c ; j++)
          deserialize(f,x(i,j));
    }

  // SampledTraj<T>

    template <typename T>
    void serialize(std::ofstream& f, const SampledTraj<T>& x)
    {
      Index size = x.nb_samples();
      f.write(reinterpret_cast<const char*>(&size), sizeof(size));

      for(const auto& [ti, xi] : x)
      {
        serialize(f, ti);
        serialize(f, xi);
      }
    }

    template <typename T>
    void deserialize(std::ifstream& f, SampledTraj<T>& x)
    {
      Index size;
      x.clear();
      f.read(reinterpret_cast<char*>(&size), sizeof(size));

      for(Index i = 0 ; i < size ; i++)
      {
        double ti; deserialize(f,ti);
        T xi; deserialize(f,xi);
        x.set(ti,xi);
      }
    }
}