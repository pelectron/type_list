#include "type_list/type_list.hpp"

using namespace type_list;

// clang-format off
static_assert(TypeList<int, double>{} == TypeList<int, double>{}, "");
static_assert(not(TypeList<int, double>{} != TypeList<int, double>{}), "");
static_assert(TypeList<int, int>{} != TypeList<int, double>{}, "");
static_assert(not(TypeList<int, int>{} == TypeList<int, double>{}), "");

static_assert(std::is_same<head_t<TypeList<int> >, int>::value, "");
static_assert(std::is_same<head_t<TypeList<int, double, char> >, int>::value,
              "");

static_assert(tail_t<TypeList<> >{} == TypeList<>{}, "");
static_assert(tail_t<TypeList<int> >{} == TypeList<>{}, "");
static_assert(tail_t<TypeList<int, double, char> >{} == TypeList<double, char>{},
              "");

static_assert(list_size<TypeList<> >::value == 0, "");
static_assert(list_size<TypeList<int> >::value == 1, "");
static_assert(list_size<TypeList<int, char> >::value == 2, "");

static_assert(index_of<int, TypeList<> >::value == npos, "");
static_assert(index_of<int, TypeList<int> >::value == 0, "");
static_assert(index_of<int, TypeList<int, int> >::value == 0, "");
static_assert(index_of<int, TypeList<int, char> >::value == 0, "");
static_assert(index_of<int, TypeList<char, int, char> >::value == 1, "");
static_assert(index_of<int, TypeList<char, double, int> >::value == 2, "");

static_assert(push_front_t<int, TypeList<> >{} == TypeList<int>{}, "");
static_assert(push_front_t<int, TypeList<char> >{} == TypeList<int, char>{}, "");
static_assert(push_front_t<int, TypeList<char, double> >{} ==
                  TypeList<int, char, double>{},
              "");

static_assert(pop_front_t<TypeList<> >{} == TypeList<>{}, "");
static_assert(pop_front_t<TypeList<char> >{} == TypeList<>{}, "");
static_assert(pop_front_t<TypeList<char, double> >{} == TypeList<double>{}, "");
static_assert(pop_front_t<TypeList<char, double, int> >{} ==
                  TypeList<double, int>{},
              "");

static_assert(std::is_same<type_at_t<0, TypeList<int> >, int>::value, "");
static_assert(
    std::is_same<type_at_t<0, TypeList<int, double, char> >, int>::value, "");
static_assert(
    std::is_same<type_at_t<1, TypeList<int, double, char> >, double>::value, "");
static_assert(
    std::is_same<type_at_t<2, TypeList<int, double, char> >, char>::value, "");

static_assert(push_back_t<int, TypeList<> >{} == TypeList<int>{}, "");
static_assert(push_back_t<int, TypeList<char> >{} == TypeList<char, int>{}, "");
static_assert(push_back_t<int, TypeList<char, double> >{} ==
                  TypeList<char, double, int>{},
              "");

static_assert(pop_back_t<TypeList<> >{} == TypeList<>{}, "");
static_assert(pop_back_t<TypeList<char> >{} == TypeList<>{}, "");
static_assert(pop_back_t<TypeList<char, double> >{} == TypeList<char>{}, "");
static_assert(pop_front_t<TypeList<char, double, int> >{} ==
                  TypeList<double, int>{},
              "");

template<class> struct Unary{};
template <class...> struct other_list {};

static_assert(
    std::is_same<other_list<>, apply_t<other_list, TypeList<> >>::value, "");
static_assert(
    std::is_same<other_list<int>, apply_t<other_list, TypeList<int> >>::value,
    "");
static_assert(std::is_same<other_list<int, char>,
                           apply_t<other_list, TypeList<int, char> >>::value,
              "");
static_assert(apply_t<TypeList, other_list<int, char> >{} ==
                  TypeList<int, char>{},
              "");
static_assert(extract_t<other_list<> >{} == TypeList<>{}, "");
static_assert(extract_t<other_list<int> >{} == TypeList<int>{}, "");
static_assert(extract_t<other_list<int, char> >{} == TypeList<int, char>{}, "");

static_assert(for_each_t<Unary, TypeList<> >{} == TypeList<>{}, "");
static_assert(for_each_t<Unary, TypeList<int> >{} ==
                  TypeList<Unary<int> >{},
              "");
static_assert(for_each_t<Unary, TypeList<int, char> >{} ==
                  TypeList<Unary<int>, Unary<char> >{},
              "");

static_assert(transform_t<std::make_signed, TypeList<> >{} == TypeList<>{}, "");
static_assert(transform_t<std::make_signed, TypeList<unsigned> >{} ==
                  TypeList<int>{},
              "");
static_assert(
    transform_t<std::make_signed, TypeList<unsigned, unsigned long> >{} ==
        TypeList<int, long>{},
    "");

static_assert(merge_t<>{} == TypeList<>{}, "");
static_assert(merge_t<TypeList<> >{} == TypeList<>{}, "");
static_assert(merge_t<TypeList<>, TypeList<> >{} == TypeList<>{}, "");
static_assert(merge_t<TypeList<>, TypeList<>, TypeList<> >{} == TypeList<>{},
              "");
static_assert(merge_t<TypeList<>, other_list<> >{} == TypeList<>{}, "");
static_assert(merge_t<TypeList<>, other_list<>, TypeList<> >{} == TypeList<>{},
              "");
static_assert(merge_t<TypeList<int>, other_list<> >{} == TypeList<int>{}, "");
static_assert(merge_t<TypeList<>, other_list<int> >{} == TypeList<int>{}, "");
static_assert(merge_t<TypeList<int>, other_list<char> >{} ==
                  TypeList<int, char>{},
              "");
static_assert(merge_t<TypeList<int, double>, other_list<char> >{} ==
                  TypeList<int, double, char>{},
              "");
static_assert(merge_t<TypeList<int, double>, other_list<char, long> >{} ==
                  TypeList<int, double, char, long>{},
              "");
static_assert(merge_t<TypeList<int>, other_list<>, TypeList<> >{} ==
                  TypeList<int>{},
              "");
static_assert(merge_t<TypeList<>, other_list<int>, TypeList<> >{} ==
                  TypeList<int>{},
              "");
static_assert(merge_t<TypeList<int>, other_list<char>, TypeList<> >{} ==
                  TypeList<int, char>{},
              "");
static_assert(merge_t<TypeList<int, double>, other_list<char>, TypeList<> >{} ==
                  TypeList<int, double, char>{},
              "");
static_assert(merge_t<TypeList<int>, other_list<>, TypeList<char> >{} ==
                  TypeList<int, char>{},
              "");
static_assert(merge_t<TypeList<>, other_list<int>, TypeList<char> >{} ==
                  TypeList<int, char>{},
              "");
static_assert(merge_t<TypeList<int>, other_list<char>, TypeList<char> >{} ==
                  TypeList<int, char, char>{},
              "");
static_assert(
    merge_t<TypeList<int, double>, other_list<char>, TypeList<char> >{} ==
        TypeList<int, double, char, char>{},
    "");

static_assert(filter_t<std::is_signed, TypeList<> >{} == TypeList<>{}, "");
static_assert(filter_t<std::is_signed, TypeList<unsigned> >{} == TypeList<>{},
              "");
static_assert(filter_t<std::is_signed, TypeList<unsigned, unsigned long> >{} ==
                  TypeList<>{},
              "");
static_assert(filter_t<std::is_signed,
                       TypeList<unsigned, unsigned long, unsigned char> >{} ==
                  TypeList<>{},
              "");
static_assert(filter_t<std::is_signed, TypeList<int> >{} == TypeList<int>{}, "");
static_assert(filter_t<std::is_signed, TypeList<int, long> >{} ==
                  TypeList<int, long>{},
              "");
static_assert(filter_t<std::is_signed, TypeList<int, long, signed char> >{} ==
                  TypeList<int, long, signed char>{},
              "");
static_assert(
    filter_t<std::is_signed, TypeList<unsigned, int, unsigned long, long,
                                      unsigned char, signed char> >{} ==
        TypeList<int, long, signed char>{},
    "");

static_assert(contains<int, TypeList<> >::value == false, "");
static_assert(contains<int, TypeList<char> >::value == false, "");
static_assert(contains<int, TypeList<char, int> >::value, "");
static_assert(contains<int, TypeList<int, char> >::value, "");
static_assert(contains<int, TypeList<int, char, int> >::value, "");

static_assert(remove_duplicates_t<TypeList<> >{} == TypeList<>{}, "");
static_assert(remove_duplicates_t<TypeList<int> >{} == TypeList<int>{}, "");
static_assert(remove_duplicates_t<TypeList<int, int> >{} == TypeList<int>{}, "");
static_assert(remove_duplicates_t<TypeList<int, int, int> >{} == TypeList<int>{},
              "");
static_assert(remove_duplicates_t<TypeList<int, int, char, double, char, int,
                                           double, char> >() ==
                  TypeList<int, char, double>(),
              "");
// clang-format on

int main() { return 0; }
