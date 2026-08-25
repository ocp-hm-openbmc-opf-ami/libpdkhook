project(
  'libpdkhook',
  'cpp',
  default_options: [
    'warning_level=3',
    'werror=true',
    'cpp_std=c++23'
  ],
  version: '1.0',
  meson_version: '>=1.1.1',
)

if meson.is_subproject() or get_option('tests').enabled()
  patch_prog = find_program('patch', required: false)
  if patch_prog.found()
    run_command(
      patch_prog, '-p1', '--forward', '--reject-file=-',
      '-i', meson.current_source_dir() / 'patches/0001-fix-boost-asio-compatibility.patch',
      check: false,
    )
  endif
endif

deps = [
  dependency('systemd'),
  dependency('sdbusplus'),
  dependency('phosphor-logging'),
]

libpdkhook_src = [
    'PDKHooks.cpp',
    'PDKHW.cpp',
]

# Shared library target
pdkhook_lib = shared_library(
  'pdkhook',
  libpdkhook_src,
  dependencies: [deps],
  install: true,
  version: '1.0.0',
  soversion: '1'
)

if meson.is_subproject()
  inc_dir = include_directories('.')
  libpdkhook_dep = declare_dependency(
    include_directories: inc_dir,
    link_with: pdkhook_lib
  )
endif

tests_opt = get_option('tests')
if tests_opt.enabled()
    subdir('tests')
endif

