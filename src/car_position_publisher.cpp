#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <std_msgs/String.h>

std::string free_space;



void poseCallback(const std_msgs::String& msg){
  static tf::TransformBroadcaster car_broadcast;
  tf::Transform transform;
//  transform.setOrigin( tf::Vector3(7.0, -8.0 , 0.0) );	// moves blue car to the first parking place
  transform.setOrigin( tf::Vector3(22.0, -8.0 , 0.0) );	// moves blue car to the parking place in the middle
//  transform.setOrigin( tf::Vector3(37.0, -8.0 , 0.0) );	// moves blue car to the last parking place
  tf::Quaternion q;
  q.setRPY(0, 0, 0);
  transform.setRotation(q);
  car_broadcast.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "world", free_space));
}

int main(int argc, char** argv){
  ros::init(argc, argv, "my_tf_broadcaster");

  free_space="body_link";
  //free_space="parking_car"

  ros::NodeHandle node;
  printf("Your free parking place is there!\n");
  ros::Subscriber sub = node.subscribe("free_parking_place", 10, &poseCallback);

  ros::spin();
  return 0;
};
