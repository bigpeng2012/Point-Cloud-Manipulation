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
 // PublisherSubscriber<sensor_msgs::PointCloud2, sensor_msgs::PointCloud2> PubSub ("/velodyne_points", "output_topic",1, 4);
void PublisherSubscriber<sensor_msgs::PointCloud2,
sensor_msgs::PointCloud2> ::subscriberCallback(const sensor_msgs::PointCloud2::ConstPtr& input)
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

  //float theta = M_PI/4; // The angle of rotation in radians
  float theta = M_PI/in_div;

  transform_1 (0,0) = cos (theta);
  transform_1 (0,1) = -sin(theta);
  transform_1 (1,0) = sin (theta);
  transform_1 (1,1) = cos (theta);
  //    (row, column)

  // Define a translation of 2.5 meters on the x axis.
  transform_1 (0,3) = in_move_x;

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
  int input_pi_div;
  input_pi_div = 4;

  int input_queue_size;
  input_queue_size = 1;

  int input_trans_x;
  input_trans_x = 2.5;

  ros::init (argc, argv, "point_manipulaiton_node");
  PublisherSubscriber<sensor_msgs::PointCloud2, sensor_msgs::PointCloud2>
      PubSub ("/velodyne_points", "output_topic",input_queue_size, input_pi_div, input_trans_x);



  // Spin
  ros::spin ();
}
