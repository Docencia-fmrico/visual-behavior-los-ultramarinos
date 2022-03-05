#include "CRobotMove.h"
#include <cmath>

CRobotMove::CRobotMove(ros::NodeHandle& nh) : 
	mNodeHandler(nh),
	mLinCtrler(std::make_unique<CPidController>("Linear Speed Controller")),
	mAngCtrler(std::make_unique<CPidController>("Angular Speed Controller")),
	mIsInPosition(true)
{
	mSpeed = nh.advertise<geometry_msgs::Twist> ("/cmd_vel", 1, true);
	mSensorsSubs = nh.subscribe<visual_behavior_los_ultramarinos::RobotData>("/sensors_data", 1, &CRobotMove::OnRobotDataReceived, this);
	mTimer = nh.createTimer(ros::Duration(0.1), &CRobotMove::spin, this);


}

CRobotMove::~CRobotMove(){

}

void CRobotMove::moveTo(double x, double y){
	mDestPose.x = x;
	mDestPose.y = y;
	float theta = std::atan2(y - mSensorsData.position.y, x - mSensorsData.position.x) - mSensorsData.position.theta;
	float x2 = x - mSensorsData.position.x;
	float y2 = y - mSensorsData.position.y;
	float dist = std::sqrt(x2*x2 + y2*y2);
	std::cout << "Error Angulo: " << theta << ", dist: " << dist << std::endl;
	//mIsInPosition = false;
}

void CRobotMove::OnRobotDataReceived(const visual_behavior_los_ultramarinos::RobotDataConstPtr& msg)
{
	mSensorsData = *msg;
}

void CRobotMove::spin(const ros::TimerEvent& e){
	if(not mIsInPosition){

	}
}