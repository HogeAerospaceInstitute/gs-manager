cmake_minimum_required(VERSION 3.13)
project(ground_station_mgr)

file(GLOB SRC "src/*.cpp" "gsm_task.h")

add_library(ground_station_mgr SHARED ${SRC})
target_link_libraries(ground_station_mgr PRIVATE sdrpp_core curl ssl crypto)
set_target_properties(ground_station_mgr PROPERTIES PREFIX "")

target_include_directories(ground_station_mgr PRIVATE "src/")
target_include_directories(ground_station_mgr PRIVATE "src/spdlog")
target_include_directories(ground_station_mgr PRIVATE "/usr/local/include/curl")
target_include_directories(ground_station_mgr PRIVATE "../recorder/src")

if (MSVC)
    target_compile_options(ground_station_mgr PRIVATE /O2 /Ob2 /std:c++17 /EHsc)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(ground_station_mgr PRIVATE -O3 -std=c++17 -Wno-unused-command-line-argument -undefined dynamic_lookup)
else ()
    target_compile_options(ground_station_mgr PRIVATE -Og -std=c++17 -Wall)
endif ()

# Install directives
install(TARGETS ground_station_mgr DESTINATION lib/sdrpp/plugins)
