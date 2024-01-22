cmake_minimum_required(VERSION 3.16)
project(horcrux)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_ESTENSION OFF)
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)

set(BOOST_INCLUDE_LIBRARIES asio thread regex system uuid date_time)
set(BOOST_ENABLE_CMAKE ON)

include(FetchContent)
FetchContent_Declare(
  Boost
  GIT_REPOSITORY https://github.com/boostorg/boost.git
  GIT_PROGRESS TRUE
  GIT_TAG boost-1.84.0
)
FetchContent_MakeAvailable(Boost)

FetchContent_Declare(
    json 
    GIT_REPOSITORY https://github.com/nlohmann/json
    GIT_TAG v3.11.2)
FetchContent_MakeAvailable(json)

FetchContent_Declare(
    base64pp
    GIT_REPOSITORY https://github.com/matheusgomes28/base64pp
    GIT_TAG v0.2.0-rc0)
FetchContent_MakeAvailable(base64pp)

add_subdirectory(horcrux_server/src)
add_subdirectory(horcrux_client/src)
