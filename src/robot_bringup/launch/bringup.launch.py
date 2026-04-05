from launch import LaunchDescription
from launch_ros.actions import Node
import os

from ament_index_python.packages import get_package_share_directory
from launch.substitutions import Command


def generate_launch_description():

    pkg_desc = get_package_share_directory('robot_description')
    pkg_bringup = get_package_share_directory('robot_bringup')

    xacro_file = os.path.join(pkg_desc, 'urdf', 'robot.urdf.xacro')

    robot_description = {
        "robot_description": Command(["xacro ", xacro_file])
    }

    controller_config = os.path.join(
        pkg_bringup,
        "config",
        "diff_drive_controller.yaml"
    )

    return LaunchDescription([

        Node(
            package="robot_state_publisher",
            executable="robot_state_publisher",
            parameters=[robot_description],
            output="screen"
        ),

        Node(
            package="controller_manager",
            executable="ros2_control_node",
            parameters=[robot_description, controller_config],
            output="screen"
        ),

       
        Node(
            package="controller_manager",
            executable="spawner",
            arguments=["joint_state_broadcaster"]
        ),

        Node(
            package="controller_manager",
            executable="spawner",
            arguments=["diff_drive_controller"]
        )

    ])