cmake_minimum_required(VERSION 3.8)
project(nmp25)

enable_testing()
find_package(Threads REQUIRED)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

foreach(IDX RANGE 1 3 1)
	add_executable(Question-${IDX}-out Q${IDX}/Question-${IDX}.cc)
	target_link_libraries(Question-${IDX}-out Threads::Threads)
	add_test(question-${IDX} Question-${IDX}-out)
endforeach()

find_package(GTest QUIET)
if(NOT GTest_FOUND)
	include(FetchContent)
	FetchContent_Declare(
		googletest
		URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
	)
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
	FetchContent_MakeAvailable(googletest)
endif()
add_executable(Question-2-test Q2/Question-2.test.cc)
target_link_libraries(Question-2-test PRIVATE GTest::gtest GTest::gtest_main Threads::Threads)
add_test(question-2-tests Question-2-test)
