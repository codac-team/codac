#ifndef __CODAC2_TUPLES__
#define __CODAC2_TUPLES__

#include <tuple>

template<std::size_t... Is, typename Tuple>
bool are_all_members_the_same(std::index_sequence<Is...>, const Tuple& left, const Tuple& right)
{
  return ((std::get<Is>(left).get() == std::get<Is>(right).get()) && ...);
}

template<typename Tuple>
bool are_all_members_the_same(const Tuple&left, const Tuple& right)
{
  return are_all_members_the_same(
    std::make_index_sequence<std::tuple_size<Tuple>::value>(), left, right);
}

#endif