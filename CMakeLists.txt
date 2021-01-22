cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

project(Shura)

set(SFML_STATIC_LIBRARIES TRUE)
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory("lib/SFML")
add_subdirectory("lib/nlohmann")

#========== SHURA TARGET ==========
file(GLOB SHURA_SRC "src/*.cpp")
add_executable(Shura "${SHURA_SRC}")
target_compile_options(Shura PRIVATE -Wall)
target_link_libraries(Shura sfml-graphics sfml-window sfml-system sfml-audio sfml-network nlohmann_json::nlohmann_json -pthread)
add_custom_command(
        TARGET Shura POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/data/
                ${CMAKE_CURRENT_BINARY_DIR}/data/)
