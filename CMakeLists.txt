cmake_minimum_required(VERSION 3.20)
project(NetSim)
set(GOOGLETEST_VERSION 1.16.0)
set(CMAKE_CXX_STANDARD 20)

enable_testing()

add_subdirectory(googletest)

add_library(NetSim
        src/storage_types.cpp
        src/Package.cpp
        src/nodes.cpp
        src/helpers.cpp
        src/factory.cpp
        src/reports.cpp


)

target_include_directories(NetSim
        PUBLIC include
)

add_executable(NetSim_tests
        test/section_1_tests.cpp
        test/section_2_tests.cpp
        test/section_4_tests.cpp
)

target_link_libraries(NetSim_tests
        PRIVATE
        NetSim
        gtest
        gtest_main
)

add_test(NAME NetSim_tests COMMAND NetSim_tests)
