# libftd2xx CMake file
# If the library is already built and exists as a .so file
add_library(ftd2xx SHARED IMPORTED)

# Specify the path to the precompiled library
set_target_properties(ftd2xx PROPERTIES
    IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/extern/libftd2xx/libftd2xx.so
)

# Specify include directories
target_include_directories(ftd2xx INTERFACE
    ${CMAKE_SOURCE_DIR}/extern/libftd2xx/include
)

set_target_properties(ftd2xx PROPERTIES LINKER_LANGUAGE CXX)
