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

  PublisherSubscriber(std::string subscribeTopicName, std::string publishTopicName, int queueSize, int div, int movex)
  {
    publisherObject = nH.advertise<PublishT>(publishTopicName, queueSize);
    subscriberObject = nH.subscribe<SubscribeT>(subscribeTopicName, queueSize, &PublisherSubscriber::subscriberCallback,this);
    in_div = div;
    in_move_x = movex;

  }
  // Callback function
  void subscriberCallback(const typename SubscribeT::ConstPtr& input);





protected:
  ros::Subscriber subscriberObject;
  ros::Publisher publisherObject;
  ros::NodeHandle nH;

  int in_div;
  int in_move_x;


};

#endif

