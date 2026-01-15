cmake_minimum_required(VERSION 3.20)
project(storage_types)
set(GOOGLETEST_VERSION 1.16.0)
set(CMAKE_CXX_STANDARD 20)

enable_testing()

add_subdirectory(googletest)

add_library(storage_types
        src/storage_types.cpp
        src/Package.cpp
)

target_include_directories(storage_types
        PUBLIC include
)

add_executable(storage_types_tests
        test/section_1_tests.cpp
)

target_link_libraries(storage_types_tests
        PRIVATE
        storage_types
        gtest
        gtest_main
)

add_test(NAME storage_types_tests COMMAND storage_types_tests)
