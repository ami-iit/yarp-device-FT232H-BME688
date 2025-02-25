# SPDX-FileCopyrightText: 2023 Istituto Italiano di Tecnologia (IIT)
# SPDX-License-Identifier: BSD-3-Clause

yarp_prepare_plugin(ft232h_bme688
  CATEGORY device
  TYPE FT232H_BME688_Driver
  INCLUDE FT232H_BME688_Driver.h
  INTERNAL ON
)

macro(YARP_ADD_PLUGIN_LOCAL _library_name)
  if(YARP_FORCE_DYNAMIC_PLUGINS OR BUILD_SHARED_LIBS)
      set(_library_type "SHARED")
  else()
      set(_library_type "STATIC")
  endif()

  # Each library can contain more than one plugin, if this is the case, then the
  # library must be added once per each plugin to the YARP_BUNDLE_OWNERS
  # variable in order to be able to match plugin and owner library later.
  get_property(devs GLOBAL PROPERTY YARP_BUNDLE_PLUGINS)
  get_property(owners GLOBAL PROPERTY YARP_BUNDLE_OWNERS)
  list(LENGTH devs devs_len)
  list(LENGTH owners owners_len)
  math(EXPR owned_devs "${devs_len} - ${owners_len}")
  foreach(_unused RANGE 1 ${owned_devs})
    set_property(GLOBAL APPEND PROPERTY YARP_BUNDLE_OWNERS ${_library_name})
  endforeach()

  # The user is adding a bone-fide plugin library.  We add it,
  # while inserting any generated source code needed for initialization.
  add_library(${_library_name} ${_library_type})
  target_sources(${_library_name} PRIVATE ${ARGN})

  get_property(srcs DIRECTORY PROPERTY YARP_BUNDLE_CODE)
  target_sources(${_library_name} PRIVATE ${srcs})

  if(YARP_FORCE_DYNAMIC_PLUGINS OR BUILD_SHARED_LIBS)
    # Do not add the "lib" prefix to dynamic plugin libraries.
    # This simplifies search on different platforms and makes it easier
    # to distinguish them from linkable libraries.
    set_property(TARGET ${_library_name} PROPERTY PREFIX "")
  else()
    # Compile static plugins with -DYARP_STATIC_PLUGIN
    set_property(TARGET ${_library_name} APPEND PROPERTY COMPILE_DEFINITIONS YARP_STATIC_PLUGIN)
  endif()

  # Add the library to the list of plugin libraries.
  set_property(GLOBAL APPEND PROPERTY YARP_BUNDLE_LIBS ${_library_name})
  # Reset the list of generated source code and link libraries to empty.
  set_property(DIRECTORY PROPERTY YARP_BUNDLE_CODE)

  get_property(_ini_file_set DIRECTORY PROPERTY YARP_BUNDLE_INI SET)
  if(_ini_file_set)
    get_property(_ini_files DIRECTORY PROPERTY YARP_BUNDLE_INI)
    foreach(_ini_file IN LISTS _ini_files)
      string(MAKE_C_IDENTIFIER "${_ini_file}" _ini_id)
      get_property(_ini_file_content DIRECTORY PROPERTY YARP_BUNDLE_INI_CONTENT_${_ini_id})
      string(REPLACE "\@YARPPLUG_LIBRARY\@" "${_library_name}" _ini_file_content "${_ini_file_content}")
      file(WRITE "${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${_ini_file}" "${_ini_file_content}")
      # Reset the .ini file content property
      set_property(DIRECTORY PROPERTY YARP_BUNDLE_INI_CONTENT_${_ini_id})
      set_property(TARGET ${_library_name} APPEND PROPERTY YARP_INI_FILES "${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${_ini_file}")
    endforeach()
    # Reset .ini file name property
    set_property(DIRECTORY PROPERTY YARP_BUNDLE_INI)

  endif()
endmacro()



if(NOT SKIP_ft232h_bme688)
    yarp_add_plugin_local(yarp_ft232h_bme688)

  target_sources(yarp_ft232h_bme688
    PRIVATE
    FT232H_i2c.cpp
    FT232H_i2c.h
    BME688.cpp
    BME688.h
    FT232H_BME688_Driver.cpp
    FT232H_BME688_Driver.h
  )

# Specify include directories
#target_include_directories(yarp_ft232h_bme688 PRIVATE
##    ${CMAKE_SOURCE_DIR}/extern/libftd2xx/include
 #   ${CMAKE_SOURCE_DIR}/extern/libmpsse/include
#)

  target_link_libraries(yarp_ft232h_bme688
      PUBLIC
      YARP::YARP_os
      YARP::YARP_sig
      YARP::YARP_dev
  )

target_include_directories(yarp_ft232h_bme688 PUBLIC ${CMAKE_SOURCE_DIR}/extern/libftd2xx/include)
target_include_directories(yarp_ft232h_bme688 PUBLIC ${CMAKE_SOURCE_DIR}/extern/libmpsse/include)
target_link_libraries(yarp_ft232h_bme688 PUBLIC ${CMAKE_SOURCE_DIR}/extern/libftd2xx/libftd2xx.so)
target_link_libraries(yarp_ft232h_bme688 PUBLIC ${CMAKE_SOURCE_DIR}/extern/libmpsse/libmpsse.so)

  yarp_install(
    TARGETS yarp_ft232h_bme688
    EXPORT yarp-device-ft232h_bme688
    COMPONENT yarp-device-ft232h_bme688
    LIBRARY DESTINATION ${YARP_DYNAMIC_PLUGINS_INSTALL_DIR}
    ARCHIVE DESTINATION ${YARP_STATIC_PLUGINS_INSTALL_DIR}
    YARP_INI DESTINATION ${YARP_PLUGIN_MANIFESTS_INSTALL_DIR}
  )

  if(YARP_COMPILE_TESTS)
    add_subdirectory(tests)
  endif()

  set_property(TARGET yarp_ft232h_bme688 PROPERTY FOLDER "Plugins/Device")
endif()
