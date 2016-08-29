#include <ros/ros.h>
#include <tf/transform_broadcaster.h>

#include <std_msgs/Int16.h>
#include <std_msgs/String.h>
#include <fstream>
#include <iostream>

#include <Eigen/Eigen>

std::string car;

class blocked_space
{
  private:
    // the node handle
    ros::NodeHandle nh_;
    int max_id;
    std::string map_file_path;
    Eigen::MatrixXd map_points;
   
    void read_map_coordinates(std::string map_file_path,int max_id);
    void poseCallback(const std_msgs::Int16& msg);
    
    // subscribers
    ros::Subscriber read_map_positions;

  public:
    // constructor
    blocked_space(ros::NodeHandle nh) : nh_(nh)
    {
        nh_.param<std::string>("map_file", map_file_path, "");
        nh_.param<int>("max_id", max_id, 3);
        read_map_coordinates(map_file_path,max_id);
      	read_map_positions = nh.subscribe(nh_.resolveName("blocked_parking_place"), 10, &blocked_space::poseCallback,this);
    }
};

void blocked_space::read_map_coordinates(std::string map_file_path,int max_id)
{
    std::ifstream f_in(map_file_path.c_str());
    int id;
    map_points=Eigen::MatrixXd::Zero(max_id+1,6);

    if (!f_in)
        ROS_ERROR("Error in opening file %s",map_file_path.c_str());
    while(f_in>>id)
    {   
        if (!(f_in>>map_points(id,0)>>map_points(id,1)>>map_points(id,2)>>map_points(id,3)>>map_points(id,4)>>map_points(id,5)))
            ROS_ERROR("Error in reading file %s",map_file_path.c_str());
          std::cout<<id<<" "<<map_points(id,0)<<" "<<map_points(id,1)<<" "<<std::endl;
    }
}
        

void blocked_space::poseCallback(const std_msgs::Int16& msg){

  static tf::TransformBroadcaster car_broadcast;
  tf::Transform transform;
  tf::Quaternion q;

  transform.setOrigin( tf::Vector3(0.0, 0.0, 0.0));  // moves red car
  // transform.setOrigin( tf:: Vector3(map_points(msg.data, 0), map_points(msg.data, 1), map_points(msg.data, 2)));

  q.setRPY(0, 0, 0);
  transform.setRotation(q);
  car_broadcast.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "world", "parked_car/body_link"));
}


int main(int argc, char** argv)
{
	car="body_link";
	ros::init(argc, argv, "blocked_space");

	ros::NodeHandle node;
ros:: Publisher pub_space= node.advertise<std_msgs::Int16>("blocked_parking_place", 10);	//???

srand(time(0));
ros::Rate rate(10);
  // ros::Rate loop_rate(10);
 int count = 0;
  /*while (ros::ok())
  {*/

std_msgs::Int16 msg;
	printf("blocked parking spaces:\n");
	blocked_space blocked_node(node);

//Publish the message
pub_space.publish(msg);

//Delays untill it is time to send another message
rate.sleep();
/*ros::spinOnce();

    loop_rate.sleep();
    ++count;
    }
*/


  ros::spin();
  
  return 0;
}