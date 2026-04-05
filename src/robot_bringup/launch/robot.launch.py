from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, TimerAction
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os


def generate_launch_description():

    bringup_pkg = get_package_share_directory('robot_bringup')
    nav_pkg = get_package_share_directory('robot_navigation')
    nav2_pkg = get_package_share_directory('nav2_bringup')
    rplidar_pkg = get_package_share_directory('rplidar_ros')

    map_file = os.path.join(nav_pkg, 'maps', 'my_room.yaml')
    nav_params = os.path.join(nav_pkg, 'config', 'nav2_params.yaml')
    rviz_config = os.path.join(nav_pkg, 'rviz', 'nav2.rviz')

    bringup_launch = os.path.join(bringup_pkg, 'launch', 'bringup.launch.py')
    rplidar_launch = os.path.join(rplidar_pkg, 'launch', 'rplidar_a1_launch.py')

    localization_launch = os.path.join(nav2_pkg, 'launch', 'localization_launch.py')
    navigation_launch = os.path.join(nav2_pkg, 'launch', 'navigation_launch.py')

    return LaunchDescription([

        # Robot hardware + controllers
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource(bringup_launch)
        ),

        # Lidar
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource(rplidar_launch)
        ),

        # Localization (map_server + AMCL)
        TimerAction(
            period=3.0,
            actions=[
                IncludeLaunchDescription(
                    PythonLaunchDescriptionSource(localization_launch),
                    launch_arguments={
                        'map': map_file,
                        'use_sim_time': 'false'
                    }.items()
                )
            ]
        ),

        # Navigation (planner + controller)
        TimerAction(
            period=6.0,
            actions=[
                IncludeLaunchDescription(
                    PythonLaunchDescriptionSource(navigation_launch),
                    launch_arguments={
                        'use_sim_time': 'false',
                        'params_file': nav_params
                    }.items()
                )
            ]
        ),

        # RViz
        Node(
            package='rviz2',
            executable='rviz2',
            arguments=['-d', rviz_config],
            output='screen'
        )
    ])