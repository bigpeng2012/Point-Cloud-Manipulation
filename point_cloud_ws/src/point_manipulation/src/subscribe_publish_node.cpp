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


template<>
void PublisherSubscriber<sensor_msgs::PointCloud2, sensor_msgs::PointCloud2> ::subscriberCallback(const sensor_msgs::PointCloud2::ConstPtr& input)
{
  // Convert the sensor_msgs/PointCloud2 data to pcl/PointCloud
  pcl::PointCloud<pcl::PointXYZ> cloud_source;
  pcl::fromROSMsg (*input, cloud_source);

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
  float theta = M_PI/4; // The angle of rotation in radians
  transform_1 (0,0) = cos (theta);
  transform_1 (0,1) = -sin(theta);
  transform_1 (1,0) = sin (theta);
  transform_1 (1,1) = cos (theta);
  //    (row, column)

  // Define a translation of 2.5 meters on the x axis.
  transform_1 (0,3) = 2.5;

  // Print the transformation
 // printf ("Method #1: using a Matrix4f\n");
 // std::cout << transform_1 << std::endl;

   // Executing the transformation
  pcl::PointCloud<pcl::PointXYZ> cloud_transformed;
  pcl::transformPointCloud (cloud_source, cloud_transformed, transform_1);


  // Convert to ROS message data type
  sensor_msgs::PointCloud2 output;
  pcl::toROSMsg(cloud_transformed, output);

  // print out
  /*
  printf ("Cloud: width = %d, height = %d\n", output.width, output.height);
    BOOST_FOREACH (unsigned char pt2, output.data)
    {
      printf ("\t(%d)", pt2);
    }
    printf("\n");
    */

  // Publish the model coefficients
  publisherObject.publish (output);
}

//ros::Publisher pub;


int main (int argc, char** argv)
{

  // Initialize ROS
  double input_div;
  input_div = 4.0;

  double intput_mX;
  intput_mX = 2.5;

  ros::init (argc, argv, "point_manipulaiton_node");
  PublisherSubscriber<sensor_msgs::PointCloud2, sensor_msgs::PointCloud2> PubSub ("/velodyne_points", "output_topic",1);

  // Spin
  ros::spin ();
}
