#include <ros/ros.h>
#include <geometry_msgs/Pose.h>

#include <tf/transform_broadcaster.h>
#include <tf/transform_datatypes.h>
//#include <tf/transform_listener.h>
//#include <std_msgs/String.h>

#include <std_msgs/Int16.h>
#include <std_msgs/String.h>
#include <fstream>
#include <iostream>


#include <sstream>
#include <iostream>

#include <Eigen/Eigen>



class car_position
{
private: 
      ros::NodeHandle nh_;
      float x;
      float y;
      tf::Quaternion q;
      void chatterCallback(const geometry_msgs::Pose& msg);

      ros:: Subscriber master_free;

public: 
  ros:: Publisher publish_position;
  geometry_msgs::Pose current_position;
  
  car_position(ros::NodeHandle nh) : nh_(nh)
  {
    nh_.param<float>("x", x, 0);
    nh_.param<float>("y", y, 0);
    current_position.position.x=x;
    current_position.position.y=y;

    // new car subscribes master to get its parking place position
    master_free = nh.subscribe("/optimal_position", 10, &car_position::chatterCallback, this);

    // new car publishes its parking place position ( to section_head)
    publish_position = nh_.advertise<geometry_msgs::Pose>("current_position", 10);
 
  }
};

void car_position::chatterCallback(const geometry_msgs::Pose& msg){

  static tf::TransformBroadcaster car_broadcast;
  tf::Transform transform;
  tf::Quaternion q;
 // q.x = msg.orientation.x;
  // position for new (blue) car
  transform.setOrigin( tf:: Vector3(msg.position.x, msg.position.y, msg.position.z));
//  transform.setOrigin( tf::Vector3(7.0, -8.0 , 0.0) );// moves blue car to the first parking place
//  transform.setOrigin( tf::Vector3(22.0, -8.0 , 0.0) ); // moves blue car to the parking place in the middle
//  transform.setOrigin( tf::Vector3(37.0, -8.0 , 0.0) ); // moves blue car to the last parking place
  //quaternionTFtoMsg(msg.orientation,q);
    q.setRPY(msg.orientation.x, msg.orientation.y, msg.orientation.z);

  transform.setRotation(q);
  car_broadcast.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "world", "drived_car/body_link"));
  current_position = msg;
  publish_position.publish(current_position);
}


//  void car_position::chatterCallback(const std_msgs::String::ConstPtr& msg)
// {
//      std_msgs::String msg_send;
  
//     ROS_INFO("Free position at %s ", msg->data.c_str());
// }


int main(int argc, char **argv)
{
  ros::init(argc, argv, "car_position");

  ros:: NodeHandle n;
  
  car_position id_0(n);
  // car_position id_3(n);
  // car_position id_4(n);
  // car_position id_5(n);
  // car_position id_6(n);
  // car_position id_7(n);
  // car_position id_8(n);
  ros::Rate loop_rate(1);

 
   while (ros::ok())
   {
    // publish topic /id_0/current_position ..
    //id_0.publish_position.publish(id_0.current_position);
    // id_3.publish_position.publish(id_3.current_position);
    // id_4.publish_position.publish(id_4.current_position);
    // id_5.publish_position.publish(id_5.current_position);
    // id_6.publish_position.publish(id_6.current_position);
    // id_7.publish_position.publish(id_7.current_position);
    // id_8.publish_position.publish(id_8.current_position);

    ros::spinOnce();

    loop_rate.sleep();
   }


  return 0;
}