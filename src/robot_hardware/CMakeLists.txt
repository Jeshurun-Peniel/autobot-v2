cmake_minimum_required(VERSION 3.8)
project(robot_hardware)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(hardware_interface REQUIRED)
find_package(pluginlib REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SERIAL libserial)

add_library(robot_hardware SHARED
  src/hardware_interface.cpp
)

target_include_directories(robot_hardware PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

ament_target_dependencies(
  robot_hardware
  rclcpp
  hardware_interface
  pluginlib
  rclcpp_lifecycle
)

target_link_libraries(robot_hardware ${SERIAL_LIBRARIES})

pluginlib_export_plugin_description_file(
  hardware_interface
  plugin.xml
)

install(
  TARGETS robot_hardware
  DESTINATION lib
)

install(
  DIRECTORY include/
  DESTINATION include
)

install(
  FILES plugin.xml
  DESTINATION share/${PROJECT_NAME}
)

ament_package()