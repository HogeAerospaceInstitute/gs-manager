cmake_minimum_required(VERSION 3.13)
project(ground_station_mgr)

file(GLOB SRC "src/*.cpp" "gsm_task.h")

add_library(ground_station_mgr SHARED ${SRC})
target_link_libraries(ground_station_mgr PRIVATE sdrpp_core /usr/local/lib/libcurl.a /usr/local/lib64/libssl.a /usr/local/lib64/libcrypto.a)
set_target_properties(ground_station_mgr PROPERTIES PREFIX "")

target_include_directories(ground_station_mgr PRIVATE "src/")
target_include_directories(ground_station_mgr PRIVATE "src/spdlog")
target_include_directories(ground_station_mgr PRIVATE "/usr/local/include/curl")

if (MSVC)
    target_compile_options(ground_station_mgr PRIVATE /O2 /Ob2 /std:c++17 /EHsc)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(ground_station_mgr PRIVATE -O3 -std=c++17 -Wno-unused-command-line-argument -undefined dynamic_lookup)
else ()
    target_compile_options(ground_station_mgr PRIVATE -O3 -std=c++17)
endif ()

# Install directives
install(TARGETS ground_station_mgr DESTINATION lib/sdrpp/plugins)
