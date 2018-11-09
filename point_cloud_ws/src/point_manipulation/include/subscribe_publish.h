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

  PublisherSubscriber(std::string subscribeTopicName, std::string publishTopicName, int queueSize)
  {
    publisherObject = nH.advertise<PublishT>(publishTopicName, queueSize);
    subscriberObject = nH.subscribe<SubscribeT>(subscribeTopicName, queueSize, &PublisherSubscriber::subscriberCallback,this);


  }
  // Callback function
  void subscriberCallback(const typename SubscribeT::ConstPtr& input);





protected:
  ros::Subscriber subscriberObject;
  ros::Publisher publisherObject;
  ros::NodeHandle nH;


};

#endif

