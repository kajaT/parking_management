#include <ros/ros.h>
#include <geometry_msgs/Pose.h>

#include <tf/transform_broadcaster.h>

#include <std_msgs/Int16.h>
#include <std_msgs/String.h>
#include <fstream>
#include <iostream>


#include <sstream>
#include <iostream>

#include <Eigen/Eigen>

class master 
{
private: 
  ros::NodeHandle nh_; 

  float minDistance;
  
  int x1, x2, x3;
  int y1, y2, y3;
  float pow1, pow2, pow3, pow4, pow5, pow6;
  float d1, d2, d3, d4, d5, d6;
  
  tf::Quaternion q;

  void chatter_section1(const geometry_msgs::Pose& msg);
  void chatter_section2(const geometry_msgs::Pose& msg);
  void chatter_section3(const geometry_msgs::Pose& msg);
  // void chatter_section4(const geometry_msgs::Pose& msg);  
  // void chatter_section5(const geometry_msgs::Pose& msg);
  // void chatter_section6(const geometry_msgs::Pose& msg);



  void calculate_optimal_place();


  ros:: Subscriber section_1;
  ros:: Subscriber section_2;
  ros:: Subscriber section_3;
  // ros:: Subscriber section_4;
  // ros:: Subscriber section_5;
  // ros:: Subscriber section_6;



public: 
  ros:: Publisher publish_free_space;

  geometry_msgs::Pose optimal_position;
  
  geometry_msgs::Pose free_in_section_1;
  geometry_msgs::Pose free_in_section_2;
  geometry_msgs::Pose free_in_section_3;
  // geometry_msgs::Pose free_in_section_4;
  // geometry_msgs::Pose free_in_section_5;
  // geometry_msgs::Pose free_in_section_6;


  master(ros::NodeHandle nh) : nh_(nh)
  {
    
    section_1 = nh.subscribe("/section_1/best_pose", 10, &master::chatter_section1, this);
    section_2 = nh.subscribe("/section_2/best_pose", 10, &master::chatter_section2, this);
    section_3 = nh.subscribe("/section_3/best_pose", 10, &master::chatter_section3, this);
    // section_4 = nh.subscribe("/section_4/best_pose", 10, &master::chatter_section4, this);
    // section_5 = nh.subscribe("/section_5/best_pose", 10, &master::chatter_section5, this);
    // section_6 = nh.subscribe("/section_6/best_pose", 10, &master::chatter_section6, this);

    

    publish_free_space = nh_.advertise<geometry_msgs::Pose>("optimal_position", 10);

  }
};


void master::chatter_section1(const geometry_msgs::Pose& msg)
{
  free_in_section_1 = msg;
 
  pow1 = pow((free_in_section_1.position.x-31),2)+pow((free_in_section_1.position.y-8),2);     
  d1 = sqrt(pow1);
  
  calculate_optimal_place();
}

void master::chatter_section2(const geometry_msgs::Pose& msg)
{
  free_in_section_2 = msg;
 
  pow2 = pow((free_in_section_2.position.x-31),2) + pow((free_in_section_2.position.y-8),2); 
  d2 = sqrt(pow2);

  calculate_optimal_place();
}

void master::chatter_section3(const geometry_msgs::Pose& msg)
{
  free_in_section_3 = msg; 
  pow3 = pow((free_in_section_3.position.x-31),2) + pow((free_in_section_3.position.y-8),2); 
  d3 = sqrt(pow3);

  calculate_optimal_place();
}

// void master::chatter_section4(const geometry_msgs::Pose& msg)
// {
//   free_in_section_4 = msg; 
//   pow4 = pow(abs((free_in_section_4.position.x-31)),2) + pow(abs((free_in_section_4.position.y-8)),2); 
//   d4 = sqrt(pow4);

//   calculate_optimal_place();
// }
// void master::chatter_section5(const geometry_msgs::Pose& msg)
// {
//   free_in_section_5 = msg; 
//   pow5 = pow(abs((free_in_section_5.position.x-31)),2) + pow(abs((free_in_section_5.position.y-8)),2); 
//   d5 = sqrt(pow4);

//   calculate_optimal_place();
// }
// void master::chatter_section6(const geometry_msgs::Pose& msg)
// {
//   free_in_section_6 = msg; 
//   pow6 = pow(abs((free_in_section_6.position.x-31)),2) + pow(abs((free_in_section_6.position.y-8)),2); 
//   d6 = sqrt(pow4);

//   calculate_optimal_place();
// }


void master::calculate_optimal_place(){
      geometry_msgs::Pose bestOfAll;
      float minOfAll = 1000;

      if (d1 < minOfAll)
      {
        minOfAll = d1;
        bestOfAll = free_in_section_1;
      }
      if (d2 < minOfAll)
      {
        minOfAll = d2;
        bestOfAll = free_in_section_2;
      }
      if (d3 < minOfAll)
      {
        minOfAll = d3;
        bestOfAll = free_in_section_3;
      }
      // if (d4 < minOfAll)
      // {
      //   minOfAll = d4;
      //   bestOfAll = free_in_section_4;
      // }
      // if (d5 < minOfAll)
      // {
      //   minOfAll = d5;
      //   bestOfAll = free_in_section_5;
      // }
      // if (d6 < minOfAll)
      // {
      //   minOfAll = d6;
      //   bestOfAll = free_in_section_6;
      // }
      optimal_position = bestOfAll;
      ROS_INFO ("minOfAll: %f", minOfAll);
      
      if (minOfAll <1000)
      {
        publish_free_space.publish(optimal_position);
      }
      else
      {
        ROS_INFO ("No Vacant Parking Spaces on Parking Lot!!");
      }
     
}

int main(int argc, char **argv)
{
// initializing the node
  ros::init(argc, argv, "master");

  ros::NodeHandle n;

  master master_pL(n);

  ros::Rate loop_rate(1);

  while (ros::ok())
  {
    master_pL.publish_free_space.publish(master_pL.optimal_position);

    ros::spinOnce();

    loop_rate.sleep();
  }

  return 0;
}