#include "robot_hardware/hardware_interface.hpp"
#include "pluginlib/class_list_macros.hpp"
#include <cmath>

namespace robot_hardware
{

hardware_interface::CallbackReturn RobotHardware::on_init(
  const hardware_interface::HardwareInfo & info)
{
  if (hardware_interface::SystemInterface::on_init(info) !=
      hardware_interface::CallbackReturn::SUCCESS)
  {
    return hardware_interface::CallbackReturn::ERROR;
  }

  size_t num_joints = info_.joints.size();

  hw_positions_.resize(num_joints, 0.0);
  hw_velocities_.resize(num_joints, 0.0);
  hw_commands_.resize(num_joints, 0.0);
  prev_pos_.resize(num_joints, 0.0);


  return hardware_interface::CallbackReturn::SUCCESS;
}

std::vector<hardware_interface::StateInterface>
RobotHardware::export_state_interfaces()
{
  std::vector<hardware_interface::StateInterface> state_interfaces;

  for (size_t i = 0; i < info_.joints.size(); i++)
  {
    state_interfaces.emplace_back(
      info_.joints[i].name, "position", &hw_positions_[i]);

    state_interfaces.emplace_back(
      info_.joints[i].name, "velocity", &hw_velocities_[i]);
  }

  return state_interfaces;
}

std::vector<hardware_interface::CommandInterface>
RobotHardware::export_command_interfaces()
{
  std::vector<hardware_interface::CommandInterface> command_interfaces;

  for (size_t i = 0; i < info_.joints.size(); i++)
  {
    command_interfaces.emplace_back(
      info_.joints[i].name, "velocity", &hw_commands_[i]);
  }

  return command_interfaces;
}

hardware_interface::CallbackReturn RobotHardware::on_activate(
  const rclcpp_lifecycle::State &)
{
  serial_port.Open("/dev/ttyACM0");
  serial_port.SetBaudRate(LibSerial::BaudRate::BAUD_115200);
  sleep(2);

  return hardware_interface::CallbackReturn::SUCCESS;
}

hardware_interface::CallbackReturn RobotHardware::on_deactivate(
  const rclcpp_lifecycle::State &)
{
  return hardware_interface::CallbackReturn::SUCCESS;
}

hardware_interface::return_type RobotHardware::read(
  const rclcpp::Time &, const rclcpp::Duration & period)
{
  std::string line;

  if(serial_port.IsDataAvailable())
  {
    serial_port.ReadLine(line);

    if(line.size() > 0)
    {
      int lf, lr, rf, rr;

      if(sscanf(line.c_str(),"E %d %d %d %d",&lf,&lr,&rf,&rr) == 4)
      {
        double ticks_per_rev = 2500.0;

        double lf_rad = (lf / ticks_per_rev) * 2 * M_PI;
        double lr_rad = (lr / ticks_per_rev) * 2 * M_PI;
        double rf_rad = (rf / ticks_per_rev) * 2 * M_PI;
        double rr_rad = (rr / ticks_per_rev) * 2 * M_PI;

        hw_positions_[0] = lf_rad;
        hw_positions_[1] = lr_rad;
        hw_positions_[2] = rf_rad;
        hw_positions_[3] = rr_rad;

        double dt = period.seconds();

        hw_velocities_[0] = (lf_rad - prev_pos_[0]) / dt;
        hw_velocities_[1] = (lr_rad - prev_pos_[1]) / dt;
        hw_velocities_[2] = (rf_rad - prev_pos_[2]) / dt;
        hw_velocities_[3] = (rr_rad - prev_pos_[3]) / dt;

        prev_pos_[0] = lf_rad;
        prev_pos_[1] = lr_rad;
        prev_pos_[2] = rf_rad;
        prev_pos_[3] = rr_rad;
      }
    }
  }

  return hardware_interface::return_type::OK;
}

hardware_interface::return_type RobotHardware::write(
  const rclcpp::Time &, const rclcpp::Duration &)
{
  double left = (hw_commands_[0] + hw_commands_[1]) / 2.0;
  double right = (hw_commands_[2] + hw_commands_[3]) / 2.0;

  int pwm_left = left * 100;
  int pwm_right = right * 100;

  std::string cmd =
      "M " + std::to_string(pwm_left) +
      " " + std::to_string(pwm_right) + "\n";

  serial_port.Write(cmd);
  return hardware_interface::return_type::OK;
}

}

PLUGINLIB_EXPORT_CLASS(
  robot_hardware::RobotHardware,
  hardware_interface::SystemInterface
)