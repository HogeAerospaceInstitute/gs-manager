cmake_minimum_required(VERSION 3.13)
project(sddc_source)

if (MSVC)
    # What the fuck?
    file(GLOB SRC "src/*.cpp" "src/*.c" "src/libsddc/libsddc/*.c" "src/libsddc/libsddc/*.cpp" "src/libsddc/Core/*.c" "src/libsddc/Core/*.cpp" "src/libsddc/Core/radio/*.cpp" "src/libsddc/Core/pffft/*.c" "src/libsddc/Core/pffft/*.cpp" "src/libsddc/Core/arch/win32/*.c" "src/libsddc/Core/arch/win32/*.cpp" "src/libsddc/Core/arch/win32/CyAPI/*.cpp")
else (MSVC)
    file(GLOB SRC "src/*.cpp" "src/*.c" "src/libsddc/libsddc/*.c" "src/libsddc/libsddc/*.cpp" "src/libsddc/Core/*.c" "src/libsddc/Core/*.cpp" "src/libsddc/Core/radio/*.cpp" "src/libsddc/Core/pffft/*.c" "src/libsddc/Core/pffft/*.cpp" "src/libsddc/Core/arch/linux/*.c" "src/libsddc/Core/arch/linux/*.cpp")
endif ()

add_library(sddc_source SHARED ${SRC})
target_link_libraries(sddc_source PRIVATE sdrpp_core)
set_target_properties(sddc_source PROPERTIES PREFIX "")

target_include_directories(sddc_source PRIVATE "src/" "src/libsddc/" "src/libsddc/Core/" "src/libsddc/Core/pffft/" "src/libsddc/libsddc/")

if (MSVC)
    target_compile_options(sddc_source PRIVATE /O2 /Ob2 /std:c++17 /EHsc)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(sddc_source PRIVATE -O3 -std=c++17 -Wno-unused-command-line-argument -undefined dynamic_lookup)
else ()
    target_compile_options(sddc_source PRIVATE -O3 -std=c++17)
endif ()

if (MSVC)
    # Lib path
    target_link_directories(sddc_source PRIVATE "C:/Program Files/PothosSDR/lib/")

    # Misc headers
    target_include_directories(sddc_source PRIVATE "C:/Program Files/PothosSDR/include/libusb-1.0/")

    target_link_libraries(sddc_source PRIVATE libusb-1.0)
    target_link_libraries(sddc_source PRIVATE Setupapi.lib)
else (MSVC)
    find_package(PkgConfig)

    pkg_check_modules(LIBUSB REQUIRED libusb-1.0)

    target_include_directories(sddc_source PRIVATE ${LIBUSB_INCLUDE_DIRS})
    target_link_directories(sddc_source PRIVATE ${LIBUSB_LIBRARY_DIRS})
    target_link_libraries(sddc_source PRIVATE ${LIBUSB_LIBRARIES})
endif ()

# Install directives
install(TARGETS sddc_source DESTINATION lib/sdrpp/plugins)