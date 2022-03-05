#ifndef CSENSORS_H
#define CSENSORS_H

#include "ros/ros.h"
#include "nav_msgs/Odometry.h"
#include "sensor_msgs/Image.h"
#include "cv_bridge/cv_bridge.h"
#include "darknet_ros_msgs/BoundingBoxes.h"
#include "tf/tf.h"

#include "visual_behavior_los_ultramarinos/RobotData.h"

#include <memory>



class CSensors
{
public:
	CSensors(ros::NodeHandle& nh);
	virtual ~CSensors();

	void OnOdometryUpdate(const nav_msgs::OdometryConstPtr& msg);
	void OnDepthImageReceived(const sensor_msgs::ImageConstPtr& msg);
	void OnBoundingBoxReceived(const darknet_ros_msgs::BoundingBoxesConstPtr& msg);
	
private:
	visual_behavior_los_ultramarinos::RobotData mMsgSensorData;
	cv_bridge::CvImagePtr mImageData;
	ros::NodeHandle& mNodeHandler;
	ros::Subscriber mOdometria;
	ros::Subscriber mDarknet;
	ros::Subscriber mDepthImage;
	ros::Publisher mSensorsPublisher;
	ros::Timer mTimer;
};


#endif

