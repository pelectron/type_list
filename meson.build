project(
  'type_list',
  'cpp',
  version: '1.1',
  default_options: ['cpp_std=c++11'],
  meson_version: '>=1.1',
)

inc = include_directories('include')

type_list_dep = declare_dependency(include_directories: inc)

if (get_option('tests').enabled())
  test(
    'tl',
    executable('tl_test', sources: ['tests/test.cpp'], dependencies: type_list_dep),
  )
endif

doxygen = find_program('doxygen', required: false)
should_build_docs = get_option('docs')
if (should_build_docs.enabled() and not doxygen.found())
  warning('cannot build docs without doxygen')
endif

man_files = [
  'docs/man/man3/npos.3',
  'docs/man/man3/pop_back_t.3',
  'docs/man/man3/type_list.hpp.3',
  'docs/man/man3/type_list_TypeList.3',
  'docs/man/man3/type_list_apply.3',
  'docs/man/man3/type_list_contains.3',
  'docs/man/man3/type_list_extract.3',
  'docs/man/man3/type_list_for_each.3',
  'docs/man/man3/type_list_head.3',
  'docs/man/man3/type_list_index_of.3',
  'docs/man/man3/type_list_list_size.3',
  'docs/man/man3/type_list_merge.3',
  'docs/man/man3/type_list_pop_back.3',
  'docs/man/man3/type_list_push_front.3',
  'docs/man/man3/type_list_remove_duplicates.3',
  'docs/man/man3/type_list_tail.3',
  'docs/man/man3/type_list_transform.3',
  'docs/man/man3/type_list_type_at.3',
]

if (doxygen.found() and not should_build_docs.disabled())
  doxyfile = meson.project_source_root() / 'Doxyfile'
  docs = run_target(
    'docs',
    command: [doxygen, doxyfile],
    env: {'SRCDIR': meson.project_source_root()},
  )
endif

install_headers('include/type_list/type_list.hpp', subdir: 'type_list')
