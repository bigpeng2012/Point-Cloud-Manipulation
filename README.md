# Point-Cloud-Manipulation
The goal of this project is building an ROS node(point_manipulation_node) which is subscribing to a point topic, doing manipulaitons accordingly and republishing them with a new topic.
The manipulation includes angular rotations and linear translations along threee axises.
This node works as a "subscriber" and a "publisher" at the same time. The PCL library has been implemneted for point cloud manipulation. 

## How To Run The Node:

1. Make sure that a roscore is up and running:
```
$ roscore
```
2. Sourced the setup.sh file for local catkin workspace. 
```
  # In your catkin workspace
  $ cd ~/point_cloud_ws
  $ source ./devel/setup.bash
```
3. ```catkin_make``` the workspace.  

4. Rosrun the C++ executable file, point_manipulaiton_exe, under the package of point_manipulation.
```
  $ rosrun point_manipulaiton point_manipulation_exe
```
5. Then we are ready to subscribe to the topic"/velodyne_points". We can play a bagfile which is containg this topic to test the node.
```
  $ rosbag play test.bag
```
6. The point_manipulation_node begin to subscribe to topic"/velodyne_points", transform the cloud point
and publish the new cloud point with the new topic "output_topic". 


## How Does the Node work
1. The node subscribs to topic "/velodyne_points", the type of ROS messagae is PointCloud2. 

2. The node uses the pcl::fromROSMsg function to convert ROS message sensor_msgs/PointCloud2 to cloud point pcl::PointCloud<pcl::PointXYZ>.  

3. The angular rotations and linear translation can be achieved by defining a rotation matrix (see https://en.wikipedia.org/wiki/Rotation_matrix) 
```
  Eigen::Matrix4f transform_1
```
  or use the Affine3f library
```
  Eigen::Affine3f transform_2
```
  Here I use six different paramters to represent the rotation around x ,y ,z axis and linear translation on x, y , z direction: 
  rotate_x, rotate_y, rotate_z, move_x, move_y, move_z. 
  Notice: rotate around x axis is  pi * rotate_x
  
4. The node implements the pcl::transformPointCloud function to execute the linear translating and augular rotating transformation.

5. The node uses the pcl::toROSMsg function to convet cloud point pcl::PointCloud<pcl::PointXYZ> to ROS message sensor_msgs/PointCloud2.

6. The node publishes ROS message with a new topic. 


