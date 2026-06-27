# Unit tests for range-base-lib (QTest based).
#
# Enabled by configuring the project with -DRANGE_BUILD_TESTS=ON.
# Run with `ctest` from the build directory.

find_package(Qt6 REQUIRED COMPONENTS Test)

set(CMAKE_AUTOMOC ON)

set(range_base_lib_tests
    tst_rvector
    tst_r3vector
    tst_rmatrix
    tst_version
    tst_limit_box
    tst_statistics
    tst_uvector
)

foreach(test_name IN LISTS range_base_lib_tests)
    qt_add_executable(${test_name} ${test_name}.cpp)
    target_link_libraries(${test_name}
        PRIVATE
            range-base-lib
            Qt6::Test
    )
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()
