/** 
 *  \file
 *  Serialization tools (tubes)
 * ----------------------------------------------------------------------------
 *  \date       2016
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_SERIALIZ_TUBES_H__
#define __CODAC_SERIALIZ_TUBES_H__

#include <fstream>

namespace codac
{
  #define SERIALIZATION_VERSION 2

  class Tube;
  class TubeVector;

  /// \name Tube
  /// @{

  /**
   * \brief Writes a Tube object into a binary file (version 2)
   * 
   * Tube binary structure: <br>
   *   [short_int_version_number] <br>
   *   [int_nb_slices] <br>
   *   [double_t0] <br>
   *   [double_t1] // time input shared by 1rst and 2nd slices <br>
   *   [double_t2] <br>
   *   ... <br>
   *   [interval_y0] // value of 1rst slice <br>
   *   [interval_y1] <br>
   *   ... <br>
   *   [gate_t0] // value of 1rst gate <br>
   *   [gate_t1] <br>
   *   ...
   *
   * \param bin_file binary file (ofstream object)
   * \param tube Tube object to be serialized
   * \param version_number optional version number for tests purposes (backwards compatibility)
   */
  void serialize_Tube(std::ofstream& bin_file, const Tube& tube, int version_number = SERIALIZATION_VERSION);

  /**
   * \brief Creates a Tube object from a binary file.
   *
   * The binary file has to be written by the serialize_Tube() function.
   *
   * \param bin_file binary file (ifstream object)
   * \param tube Tube object to be deserialized
   */
  void deserialize_Tube(std::ifstream& bin_file, Tube *&tube);

  /// @}
  /// \name TubeVector
  /// @{

  /**
   * \brief Writes a TubeVector object into a binary file
   * 
   * TubeVector binary structure: <br>
   *   [short_int_size] <br>
   *   [Tube_1] <br>
   *   ... <br>
   *   [Tube_n]
   *
   * \param bin_file binary file (ofstream object)
   * \param tube TubeVector object to be serialized
   * \param version_number optional version number for tests purposes (backwards compatibility)
   */
  void serialize_TubeVector(std::ofstream& bin_file, const TubeVector& tube, int version_number = SERIALIZATION_VERSION);

  /**
   * \brief Creates a TubeVector object from a binary file.
   *
   * The binary file has to be written by the serialize_TubeVector() function.
   *
   * \param bin_file binary file (ifstream object)
   * \param tube TubeVector object to be deserialized
   */
  void deserialize_TubeVector(std::ifstream& bin_file, TubeVector *&tube);
  
  /// @}
}

#endif