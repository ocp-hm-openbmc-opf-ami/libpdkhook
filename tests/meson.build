gtest_dep = dependency('gtest', main: false, required: false)
gmock_dep = dependency('gmock', required: false)

if not gtest_dep.found() or not gmock_dep.found()
    cmake = import('cmake')
    opt_var = cmake.subproject_options()
    opt_var.add_cmake_defines({'CMAKE_CXX_FLAGS': '-Wno-pedantic'})
    opt_var.set_override_option('warning_level', '3')
    opt_var.set_override_option('werror', 'true')

    gtest_proj = cmake.subproject('gtest', options: opt_var, required: true)
    if gtest_proj.found()
        gtest_dep = declare_dependency(
            dependencies: [dependency('threads'), gtest_proj.dependency('gtest')]
        )
        gmock_dep = gtest_proj.dependency('gmock')
    else
        assert(
            not get_option('tests').enabled(),
            'Googletest is required if tests are enabled'
        )
    endif
endif

# Production dependencies — redeclared here for test isolation
sdbusplus_test_dep = dependency('sdbusplus')
phosphor_logging_test_dep = dependency('phosphor-logging')
systemd_test_dep = dependency('systemd', required: true)

common_deps = [
    gtest_dep,
    gmock_dep,
    sdbusplus_test_dep,
    phosphor_logging_test_dep,
    systemd_test_dep,
]

common_cpp_args = [
    '-DBOOST_ALL_NO_LIB',
    '-DBOOST_SYSTEM_NO_DEPRECATED',
    '-DBOOST_ERROR_CODE_HEADER_ONLY',
]

common_link_args = [
    '-lcrypt',
    '-lstdc++fs',
]

# test_PDKHooks — unit tests covering PDKHooks.cpp
pdkhooks_exe = executable(
    'test_PDKHooks',
    [
        'test_main.cpp',
        'test_PDKHooks.cpp',
        '../PDKHooks.cpp',
    ],
    include_directories: include_directories('..'),
    dependencies: common_deps,
    cpp_args: common_cpp_args,
    link_args: common_link_args,
)
test('unit_test_PDKHooks', pdkhooks_exe)

# test_PDKHW — unit tests covering PDKHW.cpp
pdkhw_exe = executable(
    'test_PDKHW',
    [
        'test_main.cpp',
        'test_PDKHW.cpp',
        '../PDKHW.cpp',
    ],
    include_directories: include_directories('..'),
    dependencies: common_deps,
    cpp_args: common_cpp_args,
    link_args: common_link_args,
)
test('unit_test_PDKHW', pdkhw_exe)
