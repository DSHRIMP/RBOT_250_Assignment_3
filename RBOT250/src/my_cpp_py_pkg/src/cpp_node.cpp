#include <geometry_msgs/msg/transform_stamped.hpp>
#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include "std_msgs/msg/string.hpp"
#include <rclcpp/rclcpp.hpp>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_ros/transform_broadcaster.h>
#include <turtlesim/msg/pose.hpp>
#include <math.h>
#include <memory>
#include <string>

using std::placeholders::_1;
using namespace std::literals::chrono_literals;
class FramePublisher : public rclcpp::Node
{
public:
  FramePublisher()
  : Node("turtle_tf2_frame_publisher")
  {
    // Declare and acquire `turtlename` parameter
    this->declare_parameter<std::string>("turtlename", "turtle");
    this->get_parameter("turtlename", turtlename_);

    // Initialize the transform broadcaster
    tf_broadcaster_ =
      std::make_unique<tf2_ros::TransformBroadcaster>(*this);

    // Subscribe to a turtle{1}{2}/pose topic and call handle_turtle_pose
    // callback function on each message
    std::ostringstream stream;
    stream << "/" << turtlename_.c_str() << "/pose";
    std::string topic_name = stream.str();
    publisher_ = this->create_publisher<geometry_msgs::msg::TransformStamped>("homeworks/hw1/tf", 10);

    //subscription_ = this->create_subscription<turtlesim::msg::Pose>(
    //  topic_name, 10,
    //  std::bind(&FramePublisher::handle_turtle_pose, this, _1));
      
    timer_ = this->create_wall_timer(500ms, std::bind(&FramePublisher::timer_callback, this));

  }
  /*
  MinimalPublisher()
    : Node("minimal_publisher"), count_(0)
    {
      publisher_ = this->create_publisher<std_msgs::msg::String>("topic", 10);
      timer_ = this->create_wall_timer(
      500ms, std::bind(&MinimalPublisher::timer_callback, this));
    }
  */
private:
  //input some object with xyz coordinates.
  void handle_turtle_pose(const std::shared_ptr<turtlesim::msg::Pose> msg)
  {
    rclcpp::Time now = this->get_clock()->now();
    geometry_msgs::msg::TransformStamped t;

    // Read message content and assign it to
    // corresponding tf variables
    t.header.stamp = now;
    t.header.frame_id = "base";
    t.child_frame_id = "elbow";

    // Turtle only exists in 2D, thus we get x and y translation
    // coordinates from the message and set the z coordinate to 0
    t.transform.translation.x = msg->x;
    t.transform.translation.y = msg->y;
    t.transform.translation.z = 0.0;

    // For the same reason, turtle can only rotate around one axis
    // and this why we set rotation in x and y to 0 and obtain
    // rotation in z axis from the message
    tf2::Quaternion q;
    q.setRPY(0, 0, msg->theta);
    t.transform.rotation.x = q.x();
    t.transform.rotation.y = q.y();
    t.transform.rotation.z = q.z();
    t.transform.rotation.w = q.w();

    // Send the transformation
    tf_broadcaster_->sendTransform(t);
  }
  rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr subscription_;
  std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
  std::string turtlename_;
  
  geometry_msgs::msg::TransformStamped get_tf2_value()
  {
     rclcpp::Time now = this->get_clock()->now();
     geometry_msgs::msg::TransformStamped t;

     // Read message content and assign it to
     // corresponding tf variables
     t.header.stamp = now;
     t.header.frame_id = "base";
     t.child_frame_id = "elbow";

     // Turtle only exists in 2D, thus we get x and y translation
     // coordinates from the message and set the z coordinate to 0
     t.transform.translation.x = 1.57;
     t.transform.translation.y = 3.142;
     t.transform.translation.z = -2 * M_PI/3;

     // For the same reason, turtle can only rotate around one axis
     // and this why we set rotation in x and y to 0 and obtain
     // rotation in z axis from the message
     tf2::Quaternion q;
     q.setRPY(-0.977, 0.374, 2.612);
     t.transform.rotation.x = 0.123;
     t.transform.rotation.y = 1.57;
     t.transform.rotation.z = 5*M_PI/6;
     t.transform.rotation.w = 1;

     // Send the transformation
     tf_broadcaster_->sendTransform(t);     
     return t;
  }
  
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<geometry_msgs::msg::TransformStamped>::SharedPtr publisher_;
  size_t count_;
  void timer_callback()
  {
    auto message = geometry_msgs::msg::TransformStamped();
    //handle_turtle_pose();
    //message.data = "Hello, world! " + std::to_string(count_++);
    message = get_tf2_value();
    RCLCPP_INFO(this->get_logger(), "Publishing tfx: '%f'", message.transform.translation.x);
    RCLCPP_INFO(this->get_logger(), "Publishing tfy: '%f'", message.transform.translation.y);
    RCLCPP_INFO(this->get_logger(), "Publishing tfz: '%f'", message.transform.translation.z);
    RCLCPP_INFO(this->get_logger(), "Publishing rtx: '%f'", message.transform.rotation.x);
    RCLCPP_INFO(this->get_logger(), "Publishing rty: '%f'", message.transform.rotation.y);
    RCLCPP_INFO(this->get_logger(), "Publishing rtz: '%f'", message.transform.rotation.z);
    publisher_->publish(message);
  }
  


};

int main(int argc, char * argv[])
{
  std::cout << "Starting Broadcaster";
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<FramePublisher>());
  rclcpp::shutdown();
  return 0;
}
