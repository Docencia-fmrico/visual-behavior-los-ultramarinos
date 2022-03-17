//#include "Coordinador.h"
#include "ros/ros.h"
#include "geometry_msgs/Pose2D.h"
//#include "visual_behavior_los_ultramarinos/RobotData.h"
//#include "visual_behavior_los_ultramarinos/PolarPoint.h"
#include "tf/tf.h"
#include <cmath>
#include <memory>

	int fr =  10;

	geometry_msgs::Pose2D person;
	geometry_msgs::Pose2D ball;
	geometry_msgs::Pose2D mSensorsData;

	double latest_person;
	double latest_ball;
	double now;
	const double MAX_WAIT = 1;
	bool found_ball = false;
	bool found_person = false;

	geometry_msgs::Pose2D lost_;

void personReceived(const geometry_msgs::Pose2DConstPtr& msg)
{
	mSensorsData = *msg;
	if ((std::abs(mSensorsData.y - person.y) < 0.3 && std::abs(mSensorsData.x - person.x) < 0.3) || !found_person){
	person = *msg;
	latest_person = ros::Time::now().toSec();
	}
}

void ballReceived(const geometry_msgs::Pose2DConstPtr& msg)
{
	mSensorsData = *msg;
	if (std::abs(mSensorsData.x - ball.x) < 0.5 || !found_ball){///////
	ball = *msg;
	latest_ball = ros::Time::now().toSec();
	}//////CAMBIOS**********
	
}

bool lost(double latest_msg, double now){
	
	return(now - latest_msg > MAX_WAIT);
}




int main(int argc, char** argv)
{

	ros::init(argc, argv, "Coordinador");
	ros::NodeHandle nh;
	ros::Rate loop_rate(fr);
    ros::Publisher cPub = nh.advertise<geometry_msgs::Pose2D> ("/controller_instructions", fr, true);
	ros::Subscriber personSub = nh.subscribe<geometry_msgs::Pose2D>("/person_data", fr, personReceived);
	ros::Subscriber ballSub = nh.subscribe<geometry_msgs::Pose2D>("/ball_data", fr, ballReceived);

    // ############# CAMBIO JORGE ###############
	geometry_msgs::Pose2D lost_;
	lost_.x = 1;
	//ball.x = 1;
	// ##########################################
	// ***###### IMPORTANTE ########**
    // PONER EN EL MENSAJE LOST DE COORDINADOR EN LA .x EL VALOR DE LA DISTANCIA DE SEGURIDAD
	
	while (ros::ok())
	{
	
	now = ros::Time::now().toSec();
	
    if (!lost(latest_ball, now))
    {
		std::cout << "found ball!" << std::endl;
		found_ball = true;
		found_person = false;
	    cPub.publish(ball);
    }
    else if (!lost(latest_person, now))
    {
		found_ball = false;
		found_person = true;
		std::cout << "found person!" << std::endl;
	    cPub.publish(person);
    }
	else
	{
		found_ball = false;
		found_person = false;
		std::cout << "lost all" << std::endl;
		cPub.publish(lost_);
	}

	

	ros::spinOnce();
	loop_rate.sleep();

	}

	return 0;

}