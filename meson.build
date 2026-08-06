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
