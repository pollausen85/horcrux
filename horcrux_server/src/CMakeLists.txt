cmake_minimum_required(VERSION 3.16)

set(target horcrux_server)

#INCLUDE_DIRECTORIES(${BOOST_LIBRARY_INCLUDES})
#include_directories(asio INTERFACE ${boost_asio_SOURCE_DIR}/include)

# Add source to this project's executable.
add_executable (${target} main.cpp)
target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(${target} PUBLIC cxx_std_20)

target_link_libraries(${target}
  PRIVATE 
  Boost::asio
  Boost::thread
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET horcrux_server PROPERTY CXX_STANDARD 20)
endif()
