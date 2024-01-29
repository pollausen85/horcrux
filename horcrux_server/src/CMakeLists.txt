cmake_minimum_required(VERSION 3.16)

set(target horcrux_server)

# Add source to this project's executable.
add_executable (${target} main.cpp server.cpp DiskStorer.cpp ${CMAKE_SOURCE_DIR}/common/CRC32.cpp)
target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/common ${nlohmann_json_SOURCE_DIR}/include ${BASE64PP_INCLUDE_DIR})
target_compile_features(${target} PUBLIC cxx_std_20)

target_link_libraries(${target}
  PRIVATE 
  Boost::asio
  Boost::thread
  Boost::uuid
  Boost::crc
  nlohmann_json::nlohmann_json
  base64pp
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET horcrux_server PROPERTY CXX_STANDARD 20)
endif()
