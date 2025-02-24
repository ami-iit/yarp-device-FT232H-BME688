
# libftd2xx CMake file
# If the library is already built and exists as a .so file
add_library(mpsse SHARED IMPORTED)

# Specify the path to the precompiled library
set_target_properties(mpsse PROPERTIES
    IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/extern/libmpsse/libmpsse.so
)

# Specify include directories
target_include_directories(mpsse INTERFACE
    ${CMAKE_SOURCE_DIR}/extern/libmpsse/include
)


# Link the shared library libftd2xx.so
target_link_libraries(mpsse INTERFACE ftd2xx)  # Linking to the precompiled ftd2xx shared library

# Optional: Set C++ language explicitly if needed
set_target_properties(mpsse PROPERTIES LINKER_LANGUAGE CXX)
