#include "ros/ros.h"
#include "std_msgs/String.h"
#include <std_msgs/Int16.h>

#include <sstream>

/*class section
{
  private:
    // the node handle
    ros::NodeHandle n;
    int section_id;
    int section_place;
   
    // void poseCallback(const std_msgs::Int16& msg);
    void chatterCallback(const std_msgs::String::ConstPtr& msg);
    void read_informations(int section_id, int section_place);
    
    // subscribers
    ros::Subscriber sub;

    // ros::Subscriber blocked_place;


  public:
    // constructor
    section(ros::NodeHandle nh) : n(nh)
    {
        // nh_.param<int>("section_id", section_id, 3);
          n.param<int>("section_id", section_id, 0);
          n.param<int>("section_place", section_place, 5);


          // subscribe to topics
          read_informations(section_id, section_place);
          sub = n.subscribe("/car/parking_position", 10, &section::chatterCallback, this);
        // blocked_place = nh.subscribe(nh_.resolveName("blocked_parking_place"), 10, &parking::chatterCallback, this);
        // read_map_positions = nh.subscribe(nh_.resolveName("free_parking_place"), 10, &parking::poseCallback,this);
     // void chatterCallback(const std_msgs::String::ConstPtr& msg);
    }
};

void section::read_informations(int section_id, int section_place)
{
  ros::Rate loop_rate(1);

  /**
   * A count of how many messages we have sent. This is used to create
   * a unique string for each message.
   */
  // int place = n.getParam("place", place);
/*
  while (ros::ok())
  {
    
    std_msgs::String msg;

    std::stringstream ss;
    ss << "free position in this section: " << section_place;
    msg.data = ss.str();

    ROS_INFO("%s", msg.data.c_str());
    printf("free position in this section: %d\n", section_place);
    // ROS_INFO("Section Id: %d", section_id);

    std_msgs::String msg2;
    
    std::stringstream ss2;
    ss2 << "Section Id: " << section_id;
    msg2.data = ss2.str();

    ROS_INFO("%s", msg2.data.c_str());

    /**
     * The publish() function is how you send messages. The parameter
     * is the message object. The type of this object must agree with the type
     * given as a template parameter to the advertise<>() call, as was done
     * in the constructor above.
     */
/*
    ros::spinOnce();

    loop_rate.sleep();
  }
}*/


 void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
  // std_msgs::String msg;
  ROS_INFO("I heard: [%s]", msg->data.c_str());
}


int main(int argc, char **argv)
{

  ros::init(argc, argv, "sections");

  /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
  ros::NodeHandle n;
  //std_msgs::String msg;


  ros::Publisher section_free_space = n.advertise<std_msgs::String>("/section_head_1/free_space", 10);
  //section_free_space.publish(msg);



  ros::Subscriber sub = n.subscribe("/car/parking_position", 10, chatterCallback);


  ros::Rate loop_rate(1);

  // int place = n.getParam("place", place);

  int section_id;
  n.getParam("section_id", section_id);
  // n.param<int>("section_id", section_id); // wrong numbers
  // ros::param::get("section_id", section_id);

  int section_place;
  n.getParam("section_place", section_place);
  // ros::param::get("section_place", section_place);



  while (ros::ok())
  {
    
    std_msgs::String msg;

    std::stringstream ss;
    ss << " free position in this section: " << section_place;
    msg.data = ss.str();

    std_msgs::String pub_msg;
    
    std::stringstream ss2;
    ss2 << "Section Id: " << section_id;
    pub_msg.data = ss2.str() + ss.str();
    printf("%s\n", pub_msg.data.c_str());

    ROS_INFO("%s", pub_msg.data.c_str());

    section_free_space.publish(pub_msg);

    ros::spinOnce();

    loop_rate.sleep();
  }


  return 0;
}