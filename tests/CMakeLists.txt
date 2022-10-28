cmake_minimum_required(VERSION 3.16)

enable_testing()

add_executable(test_smem)
target_link_libraries(test_smem gtest_main smem)

target_sources(test_smem PRIVATE test.cpp)
add_test(NAME test_smem COMMAND test_smem)

include(GoogleTest)
gtest_discover_tests(test_smem)

if(WIN32)
  target_link_options(test_smem PRIVATE /NODEFAULTLIB:libcmt.lib /NODEFAULTLIB:libcmtd.lib /NODEFAULTLIB:msvcrtd.lib)
endif()
set_target_properties(test_smem PROPERTIES FOLDER "test")
set_target_properties(gtest PROPERTIES FOLDER "test/gtest")
set_target_properties(gtest_main PROPERTIES FOLDER "test/gtest")
