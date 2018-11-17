#include <ros/ros.h>
#include <pcl_ros/point_cloud.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/sample_consensus/method_types.h>
#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/io/ply_io.h>
#include <pcl/point_cloud.h>
#include <pcl/console/parse.h>
#include <pcl/common/transforms.h>

#include <subscribe_publish.h>

#include <ros/console.h>




template<>
void PublisherSubscriber<sensor_msgs::PointCloud2,sensor_msgs::PointCloud2> ::subscriberCallback(const sensor_msgs::PointCloud2::ConstPtr& input)
{

  ROS_DEBUG("Point Cloud Received");

  // Conditional logger
  //ROS_DEBUG_COND(x < 0, "Uh oh, x = %d, this is bad", x);
  //ROS_DEBUG_STREAM_COND(x < 0, "Uh oh, x = " << x << ", this is bad");

  ROS_DEBUG_COND(input->data.size() <= 0, "Cloud is empty! ");



  // Convert the sensor_msgs/PointCloud2 data to pcl/PointCloud
  pcl::PointCloud<pcl::PointXYZ> cloud_source;
  pcl::fromROSMsg (*input, cloud_source);



  // Print point cloud
  //ROS_INFO("Received Points: " , cloud_source);

  for (size_t i = 0; i < cloud_source.points.size (); ++i)
      std::cout << "    " << cloud_source.points[i].x
                << " "    << cloud_source.points[i].y
                << " "    << cloud_source.points[i].z << std::endl;

  float theta_x = M_PI*rotate_over_x;
  float theta_y = M_PI*rotate_over_y;
  float theta_z = M_PI*rotate_over_z;

 /* Reminder: how transformation matrices work :

           |-------> This column is the translation
    | 1 0 0 x |  \
    | 0 1 0 y |   }-> The identity 3x3 matrix (no rotation) on the left
    | 0 0 1 z |  /
    | 0 0 0 1 |    -> We do not use this line (and it has to stay 0,0,0,1)


    METHOD #1: Using a Matrix4f
    This is the "manual" method, perfect to understand but error prone !
  */


  Eigen::Matrix4f transform_1 = Eigen::Matrix4f::Identity();

  // Define a rotation matrix (see https://en.wikipedia.org/wiki/Rotation_matrix)
  // rotation around z axis
  transform_1 (0,0) = cos (theta_z);
  transform_1 (0,1) = -sin(theta_z);
  transform_1 (1,0) = sin (theta_z);
  transform_1 (1,1) = cos (theta_z);
  //    (row, column)

  // Define a translation of 2.5 meters on the x axis.
  transform_1 (0,3) = trans_x;


  /*  METHOD #2: Using a Affine3f
     This method is easier and less error prone
   */
   Eigen::Affine3f transform_2 = Eigen::Affine3f::Identity();

   // Define a translation of 2.5 meters on the x axis.
   transform_2.translation() << trans_x, trans_y, trans_z;

   // The same rotation matrix as before; theta radians around x y z axis
   transform_2.rotate (Eigen::AngleAxisf (theta_x, Eigen::Vector3f::UnitX()));
   transform_2.rotate (Eigen::AngleAxisf (theta_y, Eigen::Vector3f::UnitY()));
   transform_2.rotate (Eigen::AngleAxisf (theta_z, Eigen::Vector3f::UnitZ()));


   // Print the transformation
   //printf ("\nMethod #2: using an Affine3f\n");
   //std::cout << transform_2.matrix() << std::endl;

   // Executing the transformation
  pcl::PointCloud<pcl::PointXYZ> cloud_transformed;
  pcl::transformPointCloud (cloud_source, cloud_transformed, transform_2);


  // Convert to ROS message data type
  sensor_msgs::PointCloud2 output;
  pcl::toROSMsg(cloud_transformed, output);


  // Publish the new message
  publisherObject.publish (output);
}



int main (int argc, char** argv)
{

  // Initialize ROS
  ros::init (argc, argv, "point_manipulaiton_node");

  // rotation over x axis, pi*rotate_x
  double rotate_x;
  rotate_x = 0.25;

  // rotation over y axis, pi*rotate_y
  double rotate_y;
  rotate_y = 0;

  // rotation over z axis, pi*rotate_z
  double rotate_z;
  rotate_z = 0;


  int input_queue_size;
  input_queue_size = 1;

  // translation along x axis
  double move_x;
  move_x = 2.5;

  // translation along y axis
  double move_y;
  move_y = 0;

  // translation along z axis
  double move_z;
  move_z = 0;


  PublisherSubscriber<sensor_msgs::PointCloud2, sensor_msgs::PointCloud2>
      PubSub ("/velodyne_points", "output_topic",input_queue_size, rotate_x, rotate_y, rotate_z, move_x, move_y, move_z);



  // Spin
  ros::spin ();
}
