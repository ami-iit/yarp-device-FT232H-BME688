# SPDX-FileCopyrightText: 2023 Istituto Italiano di Tecnologia (IIT)
# SPDX-License-Identifier: BSD-3-Clause

yarp_prepare_plugin(ft232h_bme688
  CATEGORY device
  TYPE FT232H_BME688_Driver
  INCLUDE FT232H_BME688_Driver.h
  INTERNAL ON
)

if(NOT SKIP_ft232h_bme688)
  yarp_add_plugin(yarp_ft232h_bme688)

  target_sources(yarp_ft232h_bme688
    PRIVATE
    FT232H_i2c.cpp
    FT232H_i2c.h
    BME688.cpp
    BME688.h
    FT232H_BME688_Driver.cpp
    FT232H_BME688_Driver.h
  )

  target_link_libraries(yarp_ft232h_bme688
    PRIVATE
      YARP::YARP_os
      YARP::YARP_sig
      YARP::YARP_dev
  )

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
