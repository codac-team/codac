/** 
 *  \file codac2_TypeInfo.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

namespace codac2
{
  template<typename T>
  struct is_interval_based : std::false_type {};

  template<typename T>
  constexpr bool is_interval_based_v = is_interval_based<T>::value;

  template<typename T>
  struct is_ctc : std::false_type {};

  template<typename T>
  constexpr bool is_ctc_v = is_ctc<T>::value;

  template<typename T>
  struct is_sep : std::false_type {};

  template<typename T>
  constexpr bool is_sep_v = is_sep<T>::value;
}