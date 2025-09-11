/** 
 *  \file codac2_serialization.h
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

    /**
     * \brief Writes the binary representation of a trivially
     * copyable object to the given output stream.
     *
     * Binary structure: <br>
     *   [raw memory of x]
     *
     * \param f output stream
     * \param x object/variable to be serialized
     */
    template <typename T>
      requires (std::is_trivially_copyable_v<T>)
    inline void serialize(std::ostream& f, const T& x)
    {
      f.write(reinterpret_cast<const char*>(&x), sizeof(T));
    }

    /**
     * \brief Reads the binary representation of a trivially
     * copyable object from the given input stream.
     *
     * Binary structure: <br>
     *   [raw memory of x]
     *
     * \param f input stream
     * \param x object to be deserialized
     */
    template <typename T>
      requires (std::is_trivially_copyable_v<T>)
    inline void deserialize(std::istream& f, T& x)
    {
      f.read(reinterpret_cast<char*>(&x), sizeof(T));
    }

  // Interval

    /**
     * \brief Writes the binary representation of an
     * ``Interval`` object to the given output stream.
     * 
     * Interval binary structure: <br>
     *   [double_lb][double_ub]
     *
     * \param f output stream
     * \param x ``Interval`` object to be serialized
     */
    inline void serialize(std::ostream& f, const Interval& x)
    {
      serialize(f,x.lb());
      serialize(f,x.ub());
    }

    /**
     * \brief Creates an ``Interval`` object from the binary
     * representation given in an input stream.
     *
     * Interval binary structure: <br>
     *   [double_lb][double_ub]
     *
     * \param f input stream
     * \param x ``Interval`` object to be deserialized
     */
    inline void deserialize(std::istream& f, Interval& x)
    {
      double lb, ub;
      deserialize(f,lb);
      deserialize(f,ub);
      x = Interval(lb,ub);
    }

  // Vectors and matrix structures (real or interval objects)

    /**
     * \brief Writes the binary representation of an ``Eigen::Matrix``
     * to the given output stream. The structure can be a matrix, a row, a vector, 
     * with ``double`` or ``Interval`` components.
     *
     * Matrix binary structure: <br>
     *   [Index_rows][Index_cols][element_00][element_01]...[element_rc]
     *
     * \param f output stream
     * \param x matrix structure to be serialized
     */
    template<typename T,int R=-1,int C=-1>
    inline void serialize(std::ostream& f, const Eigen::Matrix<T,R,C>& x)
    {
      Index r = x.rows(), c = x.cols();
      f.write(reinterpret_cast<const char*>(&r), sizeof(Index));
      f.write(reinterpret_cast<const char*>(&c), sizeof(Index));
      for(Index i = 0 ; i < r ; i++)
        for(Index j = 0 ; j < c ; j++)
          serialize(f,x(i,j));
    }

    /**
     * \brief Reads the binary representation of an ``Eigen::Matrix``
     * from the given input stream. The structure can be a matrix, a row, a vector, 
     * with ``double`` or ``Interval`` components.
     *
     * Matrix binary structure: <br>
     *   [Index_rows][Index_cols][element_00][element_01]...[element_rc]
     *
     * \param f input stream
     * \param x matrix structure to be deserialized
     */
    template<typename T,int R=-1,int C=-1>
    inline void deserialize(std::istream& f, Eigen::Matrix<T,R,C>& x)
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

    /**
     * \brief Writes the binary representation of a ``SampledTraj``
     * object to the given output stream.
     *
     * SampledTraj binary structure: <br>
     *   [nb_samples][t0][x0][t1][x1]...[tn][xn]
     *
     * \param f output stream
     * \param x ``SampledTraj`` object to be serialized
     */
    template <typename T>
    inline void serialize(std::ostream& f, const SampledTraj<T>& x)
    {
      Index size = x.nb_samples();
      f.write(reinterpret_cast<const char*>(&size), sizeof(size));

      for(const auto& [ti, xi] : x)
      {
        serialize(f, ti);
        serialize(f, xi);
      }
    }

    /**
     * \brief Reads the binary representation of a ``SampledTraj``
     * object from the given input stream.
     *
     * SampledTraj binary structure: <br>
     *   [nb_samples][t0][x0][t1][x1]...[tn][xn]
     *
     * \param f input stream
     * \param x ``SampledTraj`` object to be deserialized
     */
    template <typename T>
    inline void deserialize(std::istream& f, SampledTraj<T>& x)
    {
      Index size;
      x.clear();
      f.read(reinterpret_cast<char*>(&size), sizeof(size));

      for(Index i = 0 ; i < size ; i++)
      {
        double ti; deserialize(f,ti);
        T xi; deserialize(f,xi);
        x.set(xi,ti);
      }
    }
}