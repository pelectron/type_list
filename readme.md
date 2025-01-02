# type_list

type_list is a header only C++ library for type lists written in C++11.

type_list is a single header C++ library for type lists for C++11 and above.

It features type trait like meta functions for any type list like type, i.e.
any type that only takes template type parameters. The meta functions
either have an inner typedef called type or a static constexpr member called
value, depending on whether the function returns a type or a value. Meta
functions returning a type have a shorthand alias with a '_t' suffix. From
C++14 onwards, meta functions returning a value also have a shorthand alias
with a  '_v' suffix.

## Types

Name            | Description
--------------- | ---------------------------------------------------
``TypeList<Ts...>`` | a "concrete" type list template with ``Ts`` as elements

### Value Meta Functions

Name                  | Description
--------------------- | ----------------------------------------------------------------------------------------------------
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

TODO

## Installation

None to do, just clone the repo and add the include directory to your build systems include path.

## Building Doxygen Docs

Building the html and man page documentation can be done with meson, or by hand.

### With meson

1. setup the build directory: ``meson setup build``
2. compile the docs target: ``meson compile docs -C build``
3. the documentation is now available in the folder build/docs.

### By hand

1. use the provided Doxyfile with doxygen. the environment variable INPUT must be set to ``include/type_list/type_list.hpp``, i.e. ``INPUT=include/type_list/type_list.hpp doxygen Doxyfile``.
