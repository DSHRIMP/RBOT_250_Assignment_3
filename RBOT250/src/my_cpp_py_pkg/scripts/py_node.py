#!/usr/bin/env python3


import math

from geometry_msgs.msg import Twist
from geometry_msgs.msg import TransformStamped


import rclpy
from rclpy.node import Node

from tf2_ros import TransformException
from tf2_ros.buffer import Buffer
from tf2_ros.transform_listener import TransformListener

from turtlesim.srv import Spawn


class FrameListener(Node):

    def __init__(self):
        super().__init__('turtle_tf2_frame_listener')
        # Declare and acquire `target_frame` parameter
        #super().__init__('minimal_subscriber')
        self.subscription = self.create_subscription(
            TransformStamped,
            'homeworks/hw1/tf',
            self.listener_callback,
            10)
        self.subscription  # prevent unused variable warning
        
    def listener_callback(self, msg):
        self.get_logger().info('I heard msg.transform.translation.x: "%s"' % msg.transform.translation.x)
        self.get_logger().info('I heard msg.transform.translation.y: "%s"' % msg.transform.translation.y)
        self.get_logger().info('I heard msg.transform.translation.z: "%s"' % msg.transform.translation.z)
        self.get_logger().info('I heard msg.transform.rotation.x: "%s"' % msg.transform.rotation.x)
        self.get_logger().info('I heard msg.transform.rotation.y: "%s"' % msg.transform.rotation.y)
        self.get_logger().info('I heard msg.transform.rotation.z: "%s"' % msg.transform.rotation.z)
        self.get_logger().info('I heard msg.transform.rotation.w: "%s"' % msg.transform.rotation.w)

    def on_timer(self):
        # Store frame names in variables that will be used to
        # compute transformations
        from_frame_rel = self.target_frame
        to_frame_rel = 'turtle2'

        if self.turtle_spawning_service_ready:
            if self.turtle_spawned:
                # Look up for the transformation between target_frame and turtle2 frames
                # and send velocity commands for turtle2 to reach target_frame
                try:
                    now = rclpy.time.Time()
                    trans = self.tf_buffer.lookup_transform(
                        to_frame_rel,
                        from_frame_rel,
                        now)
                except TransformException as ex:
                    self.get_logger().info(
                        f'Could not transform {to_frame_rel} to {from_frame_rel}: {ex}')
                    return

                msg = Twist()
                scale_rotation_rate = 1.0
                msg.angular.z = scale_rotation_rate * math.atan2(
                    trans.transform.translation.y,
                    trans.transform.translation.x)

                scale_forward_speed = 0.5
                msg.linear.x = scale_forward_speed * math.sqrt(
                    trans.transform.translation.x ** 2 +
                    trans.transform.translation.y ** 2)

                self.publisher.publish(msg)
            else:
                if self.result.done():
                    self.get_logger().info(
                        f'Successfully spawned {self.result.result().name}')
                    self.turtle_spawned = True
                else:
                    self.get_logger().info('Spawn is not finished')
        else:
            if self.spawner.service_is_ready():
                # Initialize request with turtle name and coordinates
                # Note that x, y and theta are defined as floats in turtlesim/srv/Spawn
                request = Spawn.Request()
                request.name = 'turtle2'
                request.x = float(4)
                request.y = float(2)
                request.theta = float(0)
                # Call request
                self.result = self.spawner.call_async(request)
                self.turtle_spawning_service_ready = True
            else:
                # Check if the service is ready
                self.get_logger().info('Service is not ready')


def main():
    	
    rclpy.init()
    node = FrameListener()
    try:
    	
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass

    rclpy.shutdown()
    
if __name__ == "__main__":
   print("starting subscriber")
   main()	

