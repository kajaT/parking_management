#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
//#include <tf/transform_listener.h>
//#include <std_msgs/String.h>

#include <std_msgs/Int16.h>
#include <std_msgs/String.h>
#include <fstream>
#include <iostream>

#include <Eigen/Eigen>

std::string car;

class parking
{
  private:
    // the node handle
    ros::NodeHandle nh_;
    int max_id;
    std::string map_file_path;
    Eigen::MatrixXd map_points;
   
    void read_map_coordinates(std::string map_file_path,int max_id);
    void poseCallback(const std_msgs::Int16& msg);
    // void chatterCallback(const std_msgs::Int16& msg);
    
    // subscribers
    ros::Subscriber read_map_positions;

    ros::Subscriber blocked_place;

  public:
    // constructor
    parking(ros::NodeHandle nh) : nh_(nh)
    {
        nh_.param<std::string>("map_file", map_file_path, "");
        nh_.param<int>("max_id", max_id, 3);
    //    nh_.param<int>("max_id", max_id, "2");
        read_map_coordinates(map_file_path,max_id);
                // subscribe to topics
        // blocked_place = nh.subscribe(nh_.resolveName("blocked_parking_place"), 10, &parking::chatterCallback, this);
        read_map_positions = nh.subscribe(nh_.resolveName("free_parking_place"), 10, &parking::poseCallback,this);
    }
};


void parking::read_map_coordinates(std::string map_file_path,int max_id)
{
    std::ifstream f_in(map_file_path.c_str());
    int id;
    map_points=Eigen::MatrixXd::Zero(max_id+1,6);

    if (!f_in)
        ROS_ERROR("Error in opening file %s",map_file_path.c_str());
        //ROS_ERROR("Error in reading file %s",f_in>>);
    while(f_in>>id)
    {   

//        if (!(f_in>>map_points(id,0)>>map_points(id,1)>>map_points(id,2)))
        if (!(f_in>>map_points(id,0)>>map_points(id,1)>>map_points(id,2)>>map_points(id,3)>>map_points(id,4)>>map_points(id,5)))
            ROS_ERROR("Error in reading file %s",map_file_path.c_str());


//            std::cout<<id<<" "<<map_points(id,0)<<" "<<map_points(id,1)<<" "<<map_points(id,2)<<std::endl;
          std::cout<<id<<" "<<map_points(id,0)<<" "<<map_points(id,1)<<" "<<std::endl;
    }
}
        
/*void parking::chatterCallback(const std_msgs::Int16& msg)
{
  ROS_INFO("I heard: [%s]", msg->data.c_str();
}   */     

void parking::poseCallback(const std_msgs::Int16& msg){

  static tf::TransformBroadcaster car_broadcast;
  tf::Transform transform;
  tf::Quaternion q;
  // position for new (blue) car
  transform.setOrigin( tf:: Vector3(map_points(msg.data, 0), map_points(msg.data, 1), map_points(msg.data, 2)));
//  transform.setOrigin( tf::Vector3(7.0, -8.0 , 0.0) );// moves blue car to the first parking place
//  transform.setOrigin( tf::Vector3(22.0, -8.0 , 0.0) ); // moves blue car to the parking place in the middle
//  transform.setOrigin( tf::Vector3(37.0, -8.0 , 0.0) ); // moves blue car to the last parking place
  q.setRPY(map_points(msg.data,3), map_points(msg.data,4), map_points(msg.data,5));
  transform.setRotation(q);
  car_broadcast.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "world", "drived_car/body_link"));

// parked_car: transform verschiebt Auto ebenfalls 
//  transform.setOrigin( tf::Vector3(0.0, 0.0, 0.0));  // moves red car
//  tf::Quaternion q;
//  q.setRPY(0, 0, 0);
//  q.setRPY(map_points(msg.data,3), map_points(msg.data,4), map_points(msg.data,5));
//  transform.setRotation(q);
//  car_broadcast.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "world", "parked_car/body_link"));
}

int main(int argc, char** argv){
  car="body_link";
  ros::init(argc, argv, "parking");


  ros::NodeHandle node;
//  printf("Your free parking place is there!\n");
  parking parking_node(node);

  ros::spin();
  
  return 0;
};
