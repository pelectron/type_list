// clang-format off
/**
 * @file type_list/type_list.hpp
 * @version 1.0
 * @copyright Boost Software License - Version 1.0
 * @brief A single header C++ library for type lists.
 *
 * @mainpage
 *
 * type_list is a single header C++ library for type lists for C++11 and above.
 *
 * It features type trait like meta functions for any type list like type, i.e.
 * any type that only takes template type parameters. The meta functions
 * either have an inner typedef called type or a static constexpr member called
 * value, depending on whether the function returns a type or a value. Meta
 * functions returning a type have a shorthand alias with a '_t' suffix. From
 * C++14 onwards, meta functions returning a value also have a shorthand alias
 * with a  '_v' suffix.
 *
 * @section types Types
 *
 * Name | Description
 * ---- | -----------
 * ``TypeList<Ts...>`` | a "concrete" type list template with ``Ts`` as elements
 *
 * @section functions Meta functions
 *
 * @subsection values-functions Value Meta Functions
 *
 * Name                  | Description
 * --------------------- | ----------------------------------------------------------------------------------------------------
 * ``list_size<List>``   | returns the number of elements in ``List``
 * ``contains<T, List>`` | returns true if ``T`` is an element of ``List``
 * ``index_of<T, List>`` | returns the index of the first occurrence ``T`` in ``List``, or ``npos`` if ``T`` is not in ``List``
 *
 * @subsection type-functions Type Meta Functions
 *
 * Name                         | Description
 * ---------------------------- | ----------------------------------------------------------------------
 * ``head<List>``               | returns the first element in ``List``
 * ``tail<List>``               | returns ``List`` with the first element removed
 * ``first<List>``              | returns the first element in ``List``
 * ``last<List>``               | returns the last element in ``List``
 * ``type_at<N, List>``         | returns the ``N``-th type in ``List``
 * ``push_front<T, List>``      | returns ``List`` with ``T`` added at the front
 * ``push_back<T, List>``       | returns ``List`` with ``T`` added at the back
 * ``pop_front<List>``          | returns ``List`` with the first element removed
 * ``pop_back<List>``           | returns ``List`` with the last element removed
 * ``apply<F, List>``           | returns ``F<Ts...>`` where ``Ts`` are the elements of ``List``
 * ``extract<Class>``           | return the template parameters of ``Class`` as a type_list
 * ``for_each<F, List>``        | returns List with each element ``T`` replaced by ``F<T>``
 * ``transform<F, List>``       | returns List with each element ``T`` replaced by ``F<T>::type``
 * ``filter<Predicate, List>``  | returns ``List`` filtered with ``Predicate``
 * ``remove_duplicates<List>``  | removes duplicate occurrences in ``List``
 * ``merge<Lists...>``          | returns the elements of ``Lists`` concatenated into a single type list
 *
 */

#ifndef TYPE_LIST_HPP
#define TYPE_LIST_HPP

#include <cstddef>
#include <limits>
#include <type_traits>

#if __cplusplus >= 201703L
#define TYPELIST_INLINE inline
#else
#define TYPELIST_INLINE static
#endif

namespace type_list {

/// @brief constant for indicating an invalid index.
TYPELIST_INLINE constexpr size_t npos = std::numeric_limits<size_t>::max();

/**
 * @brief a concrete type list type.
 * @tparam Ts the elements of the type list
 */
template <class... Ts> struct TypeList {};

template <class... Ts>
constexpr bool operator==(TypeList<Ts...>, TypeList<Ts...>) {
  return true;
}

template <class... Ts, class... Us>
constexpr bool operator==(TypeList<Ts...>, TypeList<Us...>) {
  return false;
}

template <class... Ts>
constexpr bool operator!=(TypeList<Ts...>, TypeList<Ts...>) {
  return false;
}

template <class... Ts, class... Us>
constexpr bool operator!=(TypeList<Ts...>, TypeList<Us...>) {
  return true;
}

namespace {

template <size_t N, class T, class... Ts> struct index_of_impl;

template <size_t N, class T, class T1, class... Ts>
struct index_of_impl<N, T, T1, Ts...> : index_of_impl<N + 1, T, Ts...> {};

template <size_t N, class T, class... Ts> struct index_of_impl<N, T, T, Ts...> {
  static constexpr size_t value = N;
};

template <size_t N, class T> struct index_of_impl<N, T> {
  static constexpr size_t value = npos;
};

template <size_t N, class T, class... Ts>
struct type_at_impl : type_at_impl<N - 1, Ts...> {};

template <class T, class... Ts> struct type_at_impl<0, T, Ts...> {
  using type = T;
};

template<bool B,class...Ts>
struct disjunction_helper;

template<>
struct disjunction_helper<false> : std::false_type{};

template<typename...Ts>
struct disjunction_helper<true,Ts...> : std::true_type{};

template<class T,class...Ts>
struct disjunction_helper<false, T, Ts...> : disjunction_helper<T::value, Ts...>{};


template<class...Ts> struct disjunction : disjunction_helper<false, Ts...>{};
  
} // namespace

/**
 * @brief get the first element of List.
 *
 * @tparam List
 */
template <class List> struct head;

template <class T, template <class...> class List, class... Ts>
struct head<List<T, Ts...> > {
  using type = T;
};

template <class List> using head_t = typename head<List>::type;

/**
 * @brief returns List with the first element removed.
 *
 * @tparam List a type list
 */
template <class List> struct tail;

template <template <class...> class List> struct tail<List<> > {
  using type = List<>;
};

template <class T, template <class...> class List, class... Ts>
struct tail<List<T, Ts...> > {
  using type = List<Ts...>;
};

template <class List> using tail_t = typename tail<List>::type;

/**
 * @brief get the number of elements in a type list.
 *
 * @tparam List
 */
template <class List> struct list_size;

template <template <class...> class List, class... Ts>
struct list_size<List<Ts...> > {
  static constexpr size_t value = sizeof...(Ts);
};

/**
 * @brief get the index of the first occurrence T in List, or
 * npos if T is not in the List.
 *
 * @tparam T the type to search for
 * @tparam List the type list to search
 */
template <class T, class List> struct index_of;

template <class T, template <class...> class List, class... Ts>
struct index_of<T, List<Ts...> > : index_of_impl<0, T, Ts...> {};

/**
 * @brief adds an element into the type list from the front.
 *
 * @tparam T the type to add
 * @tparam List the list to add onto
 */
template <class T, class List> struct push_front;

template <class T, template <class...> class List, class... Ts>
struct push_front<T, List<Ts...> > {
  using type = TypeList<T, Ts...>;
};

template <class T, class List>
using push_front_t = typename push_front<T, List>::type;

/**
 * @brief pop the first type from the list and return the shortend list.
 * @example pop_front<type_list<int,char> >::type == type_list<char>
 * @tparam List list pop the front of.
 */
template <class List> struct pop_front;

template <class T, template <class...> class List, class... Ts>
struct pop_front<List<T, Ts...> > {
  using type = List<Ts...>;
};

template <template <class...> class List> struct pop_front<List<> > {
  using type = List<>;
};

template <class List> using pop_front_t = typename pop_front<List>::type;

/**
 * @brief get N-th type in type_list List.
 *
 * @tparam N index of type
 * @tparam List
 */
template <size_t N, class List> struct type_at;

template <size_t N, template <class...> class List, class... Ts>
struct type_at<N, List<Ts...> > : type_at_impl<N, Ts...> {};

template <size_t N, class List>
using type_at_t = typename type_at<N, List>::type;

template <class List> using first_t = head_t<List>;
template <class List>
using last_t = type_at_t<list_size<List>::value - 1, List>;

/**
 * @brief add T to end of List.
 * @example push_back<double, type_list<int,char> >::type == type_list<int,
 * char, double>
 * @tparam List list to pop the front of.
 */
template <class T, class List> struct push_back;

template <class T, template <class...> class List, class... Ts>
struct push_back<T, List<Ts...> > {
  using type = List<Ts..., T>;
};

/**
 * @brief append T to the end of List.
 * @example push_back_t<double, type_list<int,char> > ==
 * type_list<int,char,double>
 * @tparam T
 * @tparam List
 */
template <class T, class List>
using push_back_t = typename push_back<T, List>::type;

namespace {
template <class List, size_t I> struct pop_back_impl;

template <class T, template <class...> class List, class... Ts, size_t I>
struct pop_back_impl<List<T, Ts...>, I> {
  using type =
      push_front_t<T, typename pop_back_impl<List<Ts...>, I - 1>::type>;
};

template <template <class...> class List, class T>
struct pop_back_impl<List<T>, 1> {
  using type = List<>;
};

template <template <class...> class List> struct pop_back_impl<List<>, 0> {
  using type = List<>;
};
} // namespace

/**
 * @brief returns List with the last element removed.
 *
 * @tparam List
 */

template <class List>
struct pop_back : pop_back_impl<List, list_size<List>::value> {};

/**
 * @brief pop last element of list and return the new list.
 *
 * @tparam List
 */
template <class List> using pop_back_t = typename pop_back<List>::type;

/**
 * @brief applies the List to F, i.e. returns F<Ts...> where Ts are the elements
 * of List.
 *
 * @tparam F the type to apply to
 * @tparam List the list to apply
 */
template <template <class...> class F, class List> struct apply;

template <template <class...> class F, template <class...> class List,
          class... Ts>
struct apply<F, List<Ts...> > {
  using type = F<Ts...>;
};

template <template <class...> class F, class List>
using apply_t = typename apply<F, List>::type;

/**
 * @brief return the template parameters of Class as a type_list.
 */
template <class Class> struct extract;

template <template <class...> class Class, class... Ts>
struct extract<Class<Ts...> > {
  using type = TypeList<Ts...>;
};

template <class Class> using extract_t = typename extract<Class>::type;

/**
 * @brief replaces T with F<T> for each T in List.
 *
 * @tparam List the type list to iterate
 */
template <template <class> class F, class List> struct for_each;

template <template <class> class F, template <class...> class List, class... Ts>
struct for_each<F, List<Ts...> > {
  using type = List<F<Ts>...>;
};

template <template <class> class F, class List>
using for_each_t = typename for_each<F, List>::type;

/**
 * @brief replaces T with F<T>::type for each T in List.
 *
 * @tparam F a type trait like type
 * @tparam List a type list
 */
template <template <class> class F, class List> struct transform;

template <template <class> class F, template <class...> class List, class... Ts>
struct transform<F, List<Ts...> > {
  using type = List<typename F<Ts>::type...>;
};

template <template <class> class F, class List>
using transform_t = typename transform<F, List>::type;

/**
 * @brief merges the Lists into a single type list.
 *
 * @tparam Lists the type lists to merge
 */
template <class... Lists> struct merge;

template <template <class...> class List1, template <class...> class List2,
          class... Ts, class... Us, class... Lists>
struct merge<List1<Ts...>, List2<Us...>, Lists...> {
  using type = typename merge<List1<Ts..., Us...>, Lists...>::type;
};

template <template <class...> class List1, template <class...> class List2,
          class... Ts, class... Us>
struct merge<List1<Ts...>, List2<Us...> > {
  using type = List1<Ts..., Us...>;
};

template <template <class...> class List, class... Ts>
struct merge<List<Ts...> > {
  using type = List<Ts...>;
};

template <> struct merge<> {
  using type = TypeList<>;
};

template <class... Lists> using merge_t = typename merge<Lists...>::type;

template <class FilteredList, class ToFilterList,
          template <class> class Predicate>
struct filter_impl;

template <template <class...> class List1, template <class...> class List2,
          class... Filtered, class ToFilter, class... ToBeFiltered,
          template <class> class Predicate>
struct filter_impl<List1<Filtered...>, List2<ToFilter, ToBeFiltered...>,
                   Predicate> {
  using type = typename std::conditional<
      Predicate<ToFilter>::value,
      typename filter_impl<List2<Filtered..., ToFilter>, List2<ToBeFiltered...>,
                           Predicate>::type,
      typename filter_impl<List2<Filtered...>, List2<ToBeFiltered...>,
                           Predicate>::type>::type;
};

template <template <class...> class List1, template <class...> class List2,
          class... Filtered, template <class> class Predicate>
struct filter_impl<List1<Filtered...>, List2<>, Predicate> {
  using type = List2<Filtered...>;
};

/**
 * @brief filters List with Predicate.
 *
 * @tparam List
 * @tparam Predicate a type trait like type which contains a boolean member
 * value.
 * @example
 *  ```{.cpp}
 *  static_assert(filter<std::is_signed, type_list<int,unsigned, long, unsigned
 * short> >::type{} == type_list<int,long>{});
 *  ```
 */
template <template <class> class Predicate, class List>
struct filter : filter_impl<TypeList<>, List, Predicate> {};

template <template <class> class Predicate, class List>
using filter_t = typename filter<Predicate, List>::type;

/**
 * @brief returns true if T is an element of List.
 *
 * @tparam T the type to search for
 * @tparam List the type list
 */
template <class T, class List> struct contains;

template <class T, template <class...> class List, class... Ts>
struct contains<T, List<Ts...> > : disjunction<std::is_same<T, Ts>...> {};

namespace {
template <class FilteredList, class ToFilterList> struct remove_duplicates_impl;

template <template <class...> class List1, template <class...> class List2,
          class... Filtered, class ToFilter, class... ToBeFiltered>
struct remove_duplicates_impl<List1<Filtered...>,
                              List2<ToFilter, ToBeFiltered...> > {
  using type = typename std::conditional<
      contains<ToFilter, List1<Filtered...> >::value,
      typename remove_duplicates_impl<List2<Filtered...>,
                                      List2<ToBeFiltered...> >::type,
      typename remove_duplicates_impl<List2<Filtered..., ToFilter>,
                                      List2<ToBeFiltered...> >::type>::type;
};

template <template <class...> class List1, template <class...> class List2,
          class... Filtered>
struct remove_duplicates_impl<List1<Filtered...>, List2<> > {
  using type = List2<Filtered...>;
};
} // namespace

/**
 * @brief removes duplicate occurences in List
 *
 * @tparam List a type list
 */
template <class List>
struct remove_duplicates : remove_duplicates_impl<TypeList<>, List> {};

template <class List>
using remove_duplicates_t = typename remove_duplicates<List>::type;

#if __cplusplus >= 201402L
template <class List>
TYPELIST_INLINE constexpr size_t list_size_v = list_size<List>::value;

template <class T, class List>
TYPELIST_INLINE constexpr size_t index_of_v = index_of<T, List>::value;

template <class T, class List>
TYPELIST_INLINE constexpr bool contains_v = contains<T, List>::value;
#endif
} // namespace type_list
#endif
// clang-format on
