# type_list

type_list is a single header C++ library for type lists for C++11 and above.

It features type trait like meta functions for any type list like type, i.e.
any type that only takes template type parameters (std::variant, std::tuple,
etc.). The meta functions either have an inner typedef called type or a static
constexpr member called value, depending on whether the function returns a type
or a value. Meta functions returning a type have a shorthand alias with a '_t'
suffix. From C++14 onwards, meta functions returning a value also have a
shorthand alias with a  '_v' suffix.

## Installation

Just clone the repo and add the include directory to your build
system's include path.

## API

Name            | Description
--------------- | ---------------------------------------------------
``TypeList<Ts...>`` | a "concrete" type list template with elements ``Ts``

### Value Meta Functions

Name                  | Description
--------------------- | ----------------------------------------------------------------------------
``list_size<List>``   | returns the number of elements in ``List``
``contains<T, List>`` | returns true if ``T`` is an element of ``List``
``index_of<T, List>`` | returns the index of the first occurrence ``T`` in ``List``, or ``npos`` if ``T`` is not in ``List``

### Type Meta Functions

Name                     | Description
------------------------ | ----------------------------------------------------------------------
``head<List>``               | returns the first element in ``List``
``tail<List>``               | returns ``List`` with the first element removed
``first<List>``              | returns the first element in ``List``
``last<List>``               | returns the last element in ``List``
``type_at<N, List>``         | returns the ``N``-th type in ``List``
``push_front<T, List>``      | returns ``List`` with ``T`` added at the front
``push_back<T, List>``       | returns ``List`` with ``T`` added at the back
``pop_front<List>``          | returns ``List`` with the first element removed
``pop_back<List>``           | returns ``List`` with the last element removed
``apply<F, List>``           | returns ``F<Ts...>`` where ``Ts`` are the elements of ``List``
``extract<Class>``           | return the template parameters of ``Class`` as a type_list
``for_each<F, List>``        | returns List with each element ``T`` replaced by ``F<T>``
``transform<F, List>``       | returns List with each element ``T`` replaced by ``F<T>::type``
``filter<Predicate, List>``  | returns ``List`` filtered with ``Predicate``
``remove_duplicates<List>``  | removes duplicate occurrences in ``List``
``merge<Lists...>``          | returns the elements of ``Lists`` concatenated into a single type list

## Examples

```cpp
#include "type_list/type_list.hpp"

namespace tl = type_list;

using List = tl::TypeList<unsigned, int, long long, int, unsigned short>;

template<typename T>
struct Identity{
  using type = T;
};

template<typename T>
struct AlwaysTrue : std::true_type{};

template<typename T>
struct AlwaysFalse : std::false_type{};

// list_size
static_assert(tl::list_size_v<List> == 5);

// contains
static_assert(tl::contains_v<int, List>);
static_assert(not tl::contains_v<char, List>);

// index_of
static_assert(tl::index_of_v<int, List> == 1);
static_assert(tl::index_of_v<char, List> == tl::npos);

// first
static_assert(std::is_same_v<tl::first_t<List>, unsigned>);

// last
static_assert(std::is_same_v<tl::last_t<List>, unsigned short>);

// head
static_assert(std::is_same_v<tl::head_t<List>, unsigned>);

// tail
static_assert(std::is_same_v<tl::tail_t<List>,
                             tl::TypeList<int, long long, int, unsigned short>>);

// type_at
static_assert(std::is_same_v<tl::type_at_t<3, List>, int>);
// compilation error: static_assert(std::is_same_v<tl::type_at_t<5, List>, int>);

// push_front
static_assert(std::is_same_v<tl::push_front_t<char, List>,
                             tl::TypeList<char, unsigned, int, long long,
                                          int, unsigned short>>);

// push_back
static_assert(std::is_same_v<tl::push_back_t<char, List>,
                             tl::TypeList<unsigned, int, long long, int, 
                                          unsigned short, char>>);

// pop_front
static_assert(std::is_same_v<tl::pop_front_t<List>,
                             tl::TypeList<int, long long, int, unsigned short>>);

static_assert(std::is_same_v<tl::pop_front_t<tl::TypeList<>>, tl::TypeList<>>);

// pop_back
static_assert(std::is_same_v<tl::pop_back_t<List>,
                             tl::TypeList<unsigned, int, long long, int>>);

static_assert(std::is_same_v<tl::pop_back_t<tl::TypeList<>>, tl::TypeList<>>);

// apply
static_assert(std::is_same_v<tl::apply_t<std::variant, List>, 
                             std::variant<unsigned, int, long long,
                                          int, unsigned short>>);

// extract
static_assert(std::is_same_v<tl::extract_t<std::variant<unsigned, int,
                                                        long long, int,
                                                        unsigned short>>,
                             List>);

// for_each
static_assert(std::is_same_v<tl::for_each_t<Identity, List>, 
                             tl::TypeList<Identity<unsigned>, 
                                          Identity<int>, 
                                          Identity<long long>, 
                                          Identity<int>, 
                                          Identity<unsigned short>>>);

// transform
static_assert(std::is_same_v<tl::transform_t<Identity, List>, List>);
static_assert(std::is_same_v<tl::transform_t<std::make_signed, List>,
                             tl::TypeList<int, int, long long, int, short>>);

// filter
static_assert(std::is_same_v<filter_t<AlwaysFalse, List>, tl::TypeList<>>);
static_assert(std::is_same_v<filter_t<AlwaysTrue, List>, List>);
static_assert(std::is_same_v<filter_t<std::is_signed, List>,
                             tl::TypeList<int, long long, int>>);

// remove duplicates
static_assert(std::is_same_v<remove_duplicates_t<List>, 
                             tl::TypeList<unsigned, int, long long, unsigned short>>);

// merge
static_assert(std::is_same_v<merge_t<>, tl::TypeList<>>);

static_assert(std::is_same_v<merge_t<List, List>, 
                             tl::TypeList<unsigned, int, long long, int, 
                                          unsigned short, unsigned, int, 
                                          long long, int, unsigned short>>);

static_assert(std::is_same_v<merge_t<std::variant<char,double>, List>, 
                             std::variant<char, double, unsigned, int, 
                                          long long, int, unsigned short>>);
```

## Building Doxygen Docs

Building the html and man page documentation can be done with meson, or by hand.

### With meson

1. Ensure that doxygen is in your path and cd into the repository.
2. setup the build directory: ``meson setup build``
3. compile the docs target: ``meson compile docs -C build``
4. the documentation is now available in the folder ``build/docs``.

### By hand

1. use the provided Doxyfile with doxygen. The environment variable ``SRCDIR`` must
be set to root of this repository,
e.g. ``SRCDIR=path/to/repo doxygen path/to/repo/Doxyfile``.
4. the documentation is now available in the folder ``path/to/repo/docs``.
