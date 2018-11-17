#ifndef SR_NODE_EXAMPLE_CORE_H
#define SR_NODE_EXAMPLE_CORE_H

#include <ros/ros.h>
#include <string>

//using std::string;

template<typename PublishT, typename SubscribeT>

class PublisherSubscriber
{
public:
  //! Constructor.
  PublisherSubscriber();

  //! Destructor.
  //~PublisherSubscriber();

  PublisherSubscriber(std::string subscribeTopicName, std::string publishTopicName,
                      int queueSize, int rotate_x, int rotate_y, int rotate_z, int move_x , int move_y, int move_z)
  {
    publisherObject = nH.advertise<PublishT>(publishTopicName, queueSize);
    subscriberObject = nH.subscribe<SubscribeT>(subscribeTopicName, queueSize, &PublisherSubscriber::subscriberCallback,this);

    rotate_over_x = rotate_x;
    rotate_over_y = rotate_y;
    rotate_over_z = rotate_z;

    trans_x = move_x;
    trans_y = move_y;
    trans_z = move_z;

  }
  // Callback function
  void subscriberCallback(const typename SubscribeT::ConstPtr& input);





protected:
  ros::Subscriber subscriberObject;
  ros::Publisher publisherObject;
  ros::NodeHandle nH;

  double rotate_over_x;
  double rotate_over_y;
  double rotate_over_z;

  double trans_x;
  double trans_y;
  double trans_z;

};

#endif

