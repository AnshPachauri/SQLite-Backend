cmake_minimum_required(VERSION 3.16)
project(sqlite_clone CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(dbcore src/pager.cpp)
target_include_directories(dbcore PUBLIC include)

add_executable(db src/main.cpp)
target_link_libraries(db dbcore)

enable_testing()
add_executable(test_pager tests/test_pager.cpp)
target_link_libraries(test_pager dbcore)
add_test(NAME PagerTests COMMAND test_pager)