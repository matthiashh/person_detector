#ifndef PERSON_DETECTOR_H
#define PERSON_DETECTOR_H
#include <ros/ros.h>                                    //general ROS-functionalities
#include <cob_people_detection_msgs/DetectionArray.h>   //Message type for the cob_people_detection_topic
#include <queue>                                       //used to store the detections
#include <vector>                                       //used for AMCL
#include <tf/transform_listener.h>                      //currently unused
#include <tf/transform_broadcaster.h>                   //used to broadcast detections
#include <visualization_msgs/Marker.h>                  //display markers on rviz
#include <visualization_msgs/MarkerArray.h>             //advanced display of marker on rviz
#include <person_detector/DetectionObjectArray.h>       //our detections
#include <person_detector/DetectionObject.h>            //used for a single detection
#include <person_detector/SpeechConfirmation.h>         //for speech confirmations we receive
#include <person_detector/ObstacleArray.h>              //to store found obstacles
#include <nav_msgs/OccupancyGrid.h>                     //the map format
#include <costmap_2d/layer.h>              //to use a costmap
#include <costmap_2d/costmap_2d_ros.h>     //to use a costmap
#include <sensor_msgs/Imu.h>                // to get information about the rotation
#include <geometry_msgs/PoseWithCovarianceStamped.h>    //for amcl

class person_detector_class
{
private:
  //ros-stuff
  ros::NodeHandle n_;
  ros::Subscriber sub_face_recognition_;
  ros::Publisher pub_all_recognitions_;
  ros::Subscriber sub_all_recognitions_;
  ros::Subscriber sub_map_;
  ros::Subscriber sub_local_costmap_;
  ros::Subscriber sub_obstacles_;
  ros::Subscriber sub_imu_;
  ros::Subscriber sub_confirmations_;
  ros::Subscriber sub_amcl_;
  //transformations
  tf::TransformListener tf_listener_;
  tf::StampedTransform transform_li_;
  tf::TransformBroadcaster tf_human_local_broadcaster_;
  tf::Transform transform_br_;
  tf::TransformBroadcaster tf_map_human_broadcaster_;
  tf::Transform transform_br_map_;
  ros::Duration tf_cache;

  //markers for rviz
  ros::Publisher human_marker_raw_pub_;
  visualization_msgs::Marker points;
  ros::Publisher human_marker_raw_text_pub_;
  visualization_msgs::Marker face_text;
  ros::Publisher pub_human_marker_;
  visualization_msgs::Marker heads_;
  ros::Publisher pub_human_marker_text_;
  visualization_msgs::Marker heads_text_;
  ros::Publisher pub_obstacle_text_;
  visualization_msgs::Marker obstacle_text_;
  ros::Publisher pub_obstacle_rviz_;
  visualization_msgs::Marker obstacle_marker_;


  //callbacks
  void faceRecognitionCallback_(const cob_people_detection_msgs::DetectionArray received_detections);
  void mapCallback_(const nav_msgs::OccupancyGrid received_map);
  void localCostmapCallback_ (const nav_msgs::OccupancyGrid received);
  void obstaclesCallback_ (const sensor_msgs::PointCloud pcl);
  void imuCallback_(const sensor_msgs::Imu imu);
  void confirmationCallback_(const person_detector::SpeechConfirmation conf);
  void amclCallback_(const geometry_msgs::PoseWithCovarianceStamped pose);

  //variables
  std::queue<cob_people_detection_msgs::DetectionArray> detection_temp_storage_;
  bool detection_temp_storage_in_use_;
  person_detector::DetectionObjectArray all_detections_array_;
  unsigned int detection_id;
  bool detection_array_in_use_;
  costmap_2d::Costmap2D static_map;
  costmap_2d::Costmap2DPublisher *pub_static_map;
  costmap_2d::Costmap2D updated_map;
  costmap_2d::Costmap2DPublisher *pub_updated_map;
  bool map_initialized_;
  costmap_2d::Costmap2D updated_cm_;
  costmap_2d::Costmap2D updated_counter_;
  costmap_2d::Costmap2D difference_map_;
  costmap_2d::Costmap2DPublisher *pub_difference_map;
  costmap_2d::Costmap2D dmap_new_;
  costmap_2d::Costmap2DPublisher *pub_dmap_new_;
  costmap_2d::Costmap2D dmap_pano_;
  costmap_2d::Costmap2DPublisher *pub_dmap_pano_;
  double imu_ang_vel_z;
  std::queue<person_detector::SpeechConfirmation> conf_queue_;
  std::vector<geometry_msgs::PoseWithCovarianceStamped> amcl_poses_;
  person_detector::ObstacleArray all_obstacles_;
  unsigned int obstacle_id;
  //global helperpoint
  geometry_msgs::Point p;
  double x_map;
  double y_map;

  //functions
  int preprocessDetections_();
  int classifyDetections_( cob_people_detection_msgs::DetectionArray detection_array );
  int addNewDetection(cob_people_detection_msgs::Detection new_detection);
  int updateDetection(cob_people_detection_msgs::Detection new_detection, unsigned int det_id);
  int findDistanceWinner_(std::vector< std::vector <double> > &distances, std::vector<unsigned int> &win_id, std::vector<double> &win_dist, unsigned int detection_array_size);
  int clearDoubleResults_(std::vector< std::vector <double> > &distances, std::vector<unsigned int> &win_id, std::vector<double> &win_dist, unsigned int detection_array_size);
  int substractHit(std::string label, unsigned int leave_id);
  int cleanDetectionArray_ (ros::Duration oldness);
  void showAllRecognitions();
  int generateDifferenceMap();
  int findObstacles();
  bool searchFurther(unsigned int orig_x, unsigned int orig_y, costmap_2d::Costmap2D* costmap, std::vector<geometry_msgs::Point> &points );
  void showAllObstacles();
  int inflateMap();
  int processConfirmations_();

  bool findAmclPose_ (geometry_msgs::PoseWithCovarianceStamped &pose, ros::Time stamp);



public:
  person_detector_class();
  int run();
};

#endif // PERSON_DETECTOR_H
