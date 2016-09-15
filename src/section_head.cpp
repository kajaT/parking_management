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


class section 
{
private:
  ros::NodeHandle nh_;

  int max_id;
  float minDistance;
 
  int x1, x2, x3;
  int y1, y2, y3;
  float d1, d2, d3;
  float pow1, pow2, pow3;

  bool free_place1;
  bool free_place2;
  bool free_place3;
  
  std::string map_file_path;
  Eigen::MatrixXd map_points;
  
  tf::Quaternion q;

  void chatter_place(const  geometry_msgs::Pose& msg);	
  void read_map_coordinates(std::string map_file_path,int max_id);
  void calculate_free_place();

  ros:: Subscriber place_1;
  ros:: Subscriber place_2;      
  ros:: Subscriber place_3;
 

public:
  ros::Publisher publish_free;

  geometry_msgs::Pose free_pose1;
  geometry_msgs::Pose free_pose2;
  geometry_msgs::Pose free_pose3;

  geometry_msgs::Pose best_pose;



// constructor
  section(ros::NodeHandle nh, int argc, char** argv) : nh_(nh)
  {
  	free_place1 = 0;
  	free_place2 = 0;
  	free_place3 = 0;
    nh_.param<std::string>("map_file", map_file_path, "/home/kaja/catkin_ws/src/parking_management/maps/map.txt");
    nh_.param<int>("max_id", max_id, 17);
    read_map_coordinates(map_file_path,max_id);

// listening to parking spaces in this section
    place_1 = nh.subscribe("/id_1/current_position", 10, &section::chatter_place, this);
    place_2 = nh.subscribe("/id_2/current_position", 10, &section::chatter_place, this);
    place_3 = nh.subscribe("/id_3/current_position", 10, &section::chatter_place, this);
   	
   	publish_free = nh_.advertise<geometry_msgs::Pose>("best_pose", 10);

// reading position of parking spaces from mapfile    
    free_pose1.position.x = map_points(atoi(argv[1]), 0);
    free_pose2.position.x = map_points(atoi(argv[2]), 0);
    free_pose3.position.x = map_points(atoi(argv[3]), 0);

    free_pose1.position.y =  map_points(atoi(argv[1]), 1);
    free_pose2.position.y =  map_points(atoi(argv[2]), 1);
    free_pose3.position.y =  map_points(atoi(argv[3]), 1);

//reading orientation of parking space from mapfile
    free_pose1.orientation.x = map_points(atoi(argv[1]),3);
    free_pose1.orientation.y = map_points(atoi(argv[1]),4);
    free_pose1.orientation.z = map_points(atoi(argv[1]),5);

    free_pose2.orientation.x = map_points(atoi(argv[2]),3);
    free_pose2.orientation.y = map_points(atoi(argv[2]),4);
    free_pose2.orientation.z = map_points(atoi(argv[2]),5);

    free_pose3.orientation.x = map_points(atoi(argv[3]),3);
    free_pose3.orientation.y = map_points(atoi(argv[3]),4);
    free_pose3.orientation.z = map_points(atoi(argv[3]),5);


// calculation distance to the entrance (x:21, y:8) for each parking spce of this section
	pow1 = pow((free_pose1.position.x-31),2) + pow((free_pose1.position.y-14),2); 
    d1 = sqrt(pow1);
    pow2 = pow((free_pose2.position.x-31),2) + pow((free_pose2.position.y-14),2);
    d2 = sqrt(pow2);
    pow3 = pow((free_pose3.position.x-31),2) + pow((free_pose3.position.y-14),2);
    d3 = sqrt(pow3);
    
    ROS_INFO("d1 in section_head: %f", d1);
    ROS_INFO("d2 in section_head: %f", d2);
    ROS_INFO("d3 in section_head: %f", d3);

  	calculate_free_place();
  }
};

// void section::chatter_place(const std_msgs::String& msg)
void section::chatter_place(const geometry_msgs::Pose& msg)
{
	if ((msg.position.x == free_pose1.position.x) &&(msg.position.y == free_pose1.position.y))
  		free_place1 = 1;
  	if ((msg.position.x == free_pose2.position.x) &&(msg.position.y == free_pose2.position.y))
  		free_place2 = 1;
  	if ((msg.position.x == free_pose3.position.x) &&(msg.position.y == free_pose3.position.y))
  		free_place3 = 1;
  	
  ROS_INFO("free_place1 = %i", free_place1);
  ROS_INFO("free_place2 = %i", free_place2);
  ROS_INFO("free_place3 = %i", free_place3);
  calculate_free_place();
}


void section::calculate_free_place(){

	   geometry_msgs::Pose best;
	   // std_msgs::Int16 minD;
	   int minD = 1000;

    if (free_place1 == 0){
    	minD = d1;
    	best = free_pose1;
    }
    if (free_place2 == 0){
    	if (d2 < minD) {
    	minD = d2;
    	best = free_pose2;
    	}	
    }
    if (free_place3 == 0){
    	if (d3 < minD) {
    	minD = d3;
    	best = free_pose3;
    	}		
    }
    best_pose = best;
    minDistance = minD;
    if (minD < 1000){	
    	publish_free.publish(best_pose);
    }
    else 
    	{
    		ROS_INFO("I have NO free space");
    	}
}


void section::read_map_coordinates(std::string map_file_path,int max_id)
{
    int id;
    std::ifstream f_in(map_file_path.c_str());
    map_points = Eigen::MatrixXd::Zero(max_id+1,6);

    if (!f_in)
        ROS_ERROR("Error in opening file %s",map_file_path.c_str());
    while(f_in>>id)
    {   

        if (!(f_in>>map_points(id,0)>>map_points(id,1)>>map_points(id,2)>>map_points(id,3)>>map_points(id,4)>>map_points(id,5)))
            ROS_ERROR("Error in reading file %s",map_file_path.c_str());
			std::cout<<id<<" "<<map_points(id,0)<<" "<<map_points(id,1)<<" "<<std::endl;
    }
}



int main(int argc, char** argv)
{  
// initializing the node
  ros::init(argc, argv, "section1");


//starting the node 
  ros::NodeHandle n;

  section sec(n, argc, argv);
  ros::Rate loop_rate(.5);

// returns false if node has finished shutting down
  while (ros::ok())
  {
    sec.publish_free.publish(sec.best_pose);

    ros::spinOnce();

    loop_rate.sleep();
  }


  return 0;
}