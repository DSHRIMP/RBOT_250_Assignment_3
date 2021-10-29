# RBOT_250_Assignment_3

How to Build and Run on Ubuntu Linux

Root of Ros2 Workspace where you will source your workspace and build.
~/RBOT250

1 Source your ROS2 installation. Location may vary.
. ~/ros2_galactic/install/local_setup.bash

2 Source the setup files.
. install/setup.bash

3 Run the build command.
colcon build

4 For HW1 use the launch command.
ros2 launch my_cpp_py_pkg hw3launch.py

For HW2 
In two separate terminals after doing steps 1 and 2 on each run this.
ros2 run cpp_srvcli server

ros2 run cpp_srvcli client 2 3

The result should be this 
[INFO] [rclcpp]: Sum: 5
