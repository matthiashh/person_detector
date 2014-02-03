#ifndef PERSON_DETECTOR_H
#define PERSON_DETECTOR_H
#include <ros/ros.h>                                    //general ROS-functionalities
#include <cob_people_detection_msgs/DetectionArray.h>   //Message type for the cob_people_detection_topic
#include <queue>                                       //used to store the detections
#include <tf/transform_listener.h>                      //currently unused
#include <tf/transform_broadcaster.h>                   //used to broadcast detections
#include <visualization_msgs/Marker.h>                  //display markers on rviz
#include <person_detector/NameLabel.h>


class person_detector_class
{
private:
  //ros-stuff
  ros::NodeHandle n_;
  ros::Subscriber sub_face_recognition_;
  //transformations
  tf::TransformListener tf_listener_;
  tf::StampedTransform transform_li_;
  tf::TransformBroadcaster tf_human_broadcaster_;
  tf::Transform transform_br_;

  //markers for rviz
  ros::Publisher human_marker_pub_;
  visualization_msgs::Marker points;
  ros::Publisher human_marker_text_pub_;
  visualization_msgs::Marker face_text;


  //callbacks
  void faceRecognitionCallback_(const cob_people_detection_msgs::DetectionArray received_detections);

  //variables
  std::queue<cob_people_detection_msgs::DetectionArray> detection_storage_;
 // DetectionObjectArray detection_array_;
  bool detection_array_in_use_;

  //functions
  int processDetections();

public:
  person_detector_class();
  int run();
};

#endif // PERSON_DETECTOR_H