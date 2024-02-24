#ifndef __CODAC2_ISBASEOF__
#define __CODAC2_ISBASEOF__

// From https://stackoverflow.com/questions/32477691/is-base-of-of-generic-type
#include <type_traits>

namespace detail
{
  template<template<typename> class C>
  struct is_base_of_any_helper
  {
    template<typename T>
    std::true_type operator ()(const C<T>*) const;
    std::false_type operator()(...) const;
  };
}

template<template<typename> class C, typename T>
using is_base_of_any = decltype(detail::is_base_of_any_helper<C>{}(std::declval<const T*>()));

#endif