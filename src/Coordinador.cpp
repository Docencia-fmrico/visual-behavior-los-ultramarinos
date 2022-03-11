//#include "Coordinador.h"
#include "ros/ros.h"
#include "geometry_msgs/Pose2D.h"
//#include "visual_behavior_los_ultramarinos/RobotData.h"
//#include "visual_behavior_los_ultramarinos/PolarPoint.h"
#include "tf/tf.h"
#include <memory>

	int fr =  10;
	//visual_behavior_los_ultramarinos::RobotData mSensorsData;
	geometry_msgs::Pose2D person;
	geometry_msgs::Pose2D ball;
	//visual_behavior_los_ultramarinos::PolarPoint pp_final;

	double latest_person;
	double latest_ball;
	double now;
	const double MAX_WAIT = 1;

	geometry_msgs::Pose2D lost_;
	//lost_.x = 0;
	//lost_.y = 0;
	//lost_.z = 0.0;

void personReceived(const geometry_msgs::Pose2DConstPtr& msg)
{
	//mSensorsData = *msg;
	person = *msg;
	latest_person = ros::Time::now().toSec();
}

void ballReceived(const geometry_msgs::Pose2DConstPtr& msg)
{
	//mSensorsData = *msg;
	ball = *msg;
	latest_ball = ros::Time::now().toSec();

	//std::cout << ball.x << std::endl;
	//std::cout << ball.y << std::endl;
	//std::cout << ball.x << std::endl;
	//std::cout << latest_ball << std::endl;
}

bool lost(double latest_msg, double now){
	
	//std::cout << now << std::endl;
	//std::cout << latest_msg << std::endl;

	return(now - latest_msg > MAX_WAIT);
}
/*
visual_behavior_los_ultramarinos::PolarPoint selectObject(visual_behavior_los_ultramarinos::PolarPoint[] objects)
{	
	visual_behavior_los_ultramarinos::PolarPoint pp_final = objects[0];

	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i].distance < pp_final.distance)
		{
			pp_final = objects[i];
		}
	}

    return pp_final;
}
*/



int main(int argc, char** argv)
{

	ros::init(argc, argv, "Coordinador");
	ros::NodeHandle nh;
	ros::Rate loop_rate(fr);
    ros::Publisher cPub = nh.advertise<geometry_msgs::Pose2D> ("/controller_instructions", fr, true);
	ros::Subscriber personSub = nh.subscribe<geometry_msgs::Pose2D>("/person_data", fr, personReceived);
	ros::Subscriber ballSub = nh.subscribe<geometry_msgs::Pose2D>("/ball_data", fr, ballReceived);
	
	while (ros::ok())
	{
	
	now = ros::Time::now().toSec();
	
    if (!lost(latest_ball, now))
    {
		std::cout << "found ball!" << std::endl;
		//std::cout << ball.x << std::endl;
		//std::cout << ball.y << std::endl;
	    cPub.publish(ball);
    }
    else if (!lost(latest_person, now))
    {
		std::cout << "found person!" << std::endl;
	    cPub.publish(person);
    }
	else
	{
		std::cout << "lost all" << std::endl;
		cPub.publish(lost_);
	}

	

	ros::spinOnce();
	loop_rate.sleep();

	}
	

	//std::cout << "se esta finalizando el programa" << std::endl;
	
	return 0;

}