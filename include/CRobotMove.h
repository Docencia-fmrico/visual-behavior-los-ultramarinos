#ifndef CROBOTMOVE_H
#define CROBOTMOVE_H

#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "visual_behavior_los_ultramarinos/RobotData.h"
#include "tf/tf.h"
#include "CPidController.h"
#include <memory>

class CRobotMove
{
public:
	CRobotMove(ros::NodeHandle& nh);
	virtual ~CRobotMove();

	void OnRobotDataReceived(const visual_behavior_los_ultramarinos::RobotDataConstPtr& msg);

	void computeError(double& distancia, double& angulo);
private:

	void spin(const ros::TimerEvent& e);
	ros::NodeHandle& mNodeHandler;
	ros::Publisher mSpeed;
	ros::Subscriber mSensorsSubs;
	ros::Timer mTimer;
	visual_behavior_los_ultramarinos::RobotData mSensorsData;

	std::unique_ptr<CPidController> mLinCtrler;
	std::unique_ptr<CPidController> mAngCtrler;
	geometry_msgs::Pose2D mDestPose;
	geometry_msgs::Twist mSpeedCmd;
	bool mIsInPosition;
	bool mIsRobotMoving;
	
};

#endif