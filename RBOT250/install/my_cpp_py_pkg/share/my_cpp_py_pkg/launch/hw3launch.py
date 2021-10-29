from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        Node(
            package='turtlesim',
            executable='turtlesim_node',
            name='sim'
        ),
        Node(
            package='my_cpp_py_pkg',
            executable='cpp_executable',
            name='broadcaster1',
            parameters=[
                {'turtlename': 'turtle1'}
            ]
        ),
        Node(
            package='my_cpp_py_pkg',
            executable='py_node.py',
            name='subscriber1',
            parameters=[
                {'turtlename': 'turtle2'}
            ]
        ),
    ])
