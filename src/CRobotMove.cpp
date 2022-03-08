#include "CRobotMove.h"
#include <cmath>

CRobotMove::CRobotMove(ros::NodeHandle& nh) : 
	mNodeHandler(nh),
	mLinCtrler(std::make_unique<CPidController>("Linear Speed Controller", 100, 0.4)),
	mAngCtrler(std::make_unique<CPidController>("Angular Speed Controller", 100, 0.4)),
	mIsInPosition(true)
{
	mSpeed = nh.advertise<geometry_msgs::Twist> ("/cmd_vel", 1, true);
	mSensorsSubs = nh.subscribe<visual_behavior_los_ultramarinos::RobotData>("/sensors_data", 1, &CRobotMove::OnRobotDataReceived, this);
	mTimer = nh.createTimer(ros::Duration(0.1), &CRobotMove::spin, this);
	//mTimer.stop();
	mLinCtrler->SetTarget(1.f);

}

CRobotMove::~CRobotMove(){

}

void CRobotMove::computeError(double& distancia, double& angulo){
	visual_behavior_los_ultramarinos::PolarPoint pp_menor;
	if (mSensorsData.boundingboxes.size() > 0){
		pp_menor = mSensorsData.boundingboxes[0];
	}
	for(visual_behavior_los_ultramarinos::PolarPoint pp : mSensorsData.boundingboxes){
		if (pp_menor.distancia > pp.distancia){
			pp_menor = pp;
		}
	}
	distancia = pp_menor.distancia;
	angulo = pp_menor.angulo;
}

void CRobotMove::spin(const ros::TimerEvent& e){
	ROS_INFO("UNA ITERACION DEL ROBOT");
	double dist, ang;
	computeError(dist, ang);
	std::cout << "Error Angulo: " << ang << ", dist: " << dist << std::endl;
	if(std::abs(ang) > 0.05){
		float angSpeed;
		mAngCtrler->GetSystemInput(ang, &angSpeed);
		std::cout << "Velocidad angular: " << angSpeed << std::endl;
		mSpeedCmd.linear.x = 0.f;
		mSpeedCmd.linear.y = 0.f;
		mSpeedCmd.angular.z = -angSpeed;
	} else {
		float linSpeed;
		mLinCtrler->GetSystemInput(dist, &linSpeed);
		std::cout << "Velocidad lineal: " << linSpeed << std::endl;
		mSpeedCmd.linear.x = -linSpeed;
		mSpeedCmd.linear.y = 0.f;
		mSpeedCmd.angular.z = 0.f;
	} 
	mSpeed.publish(mSpeedCmd);
}

void CRobotMove::OnRobotDataReceived(const visual_behavior_los_ultramarinos::RobotDataConstPtr& msg)
{
	mSensorsData = *msg;
}