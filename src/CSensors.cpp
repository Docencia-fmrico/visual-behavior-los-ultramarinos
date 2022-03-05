#include "CSensors.h"
#include <iostream>
CSensors::CSensors(ros::NodeHandle& nh) : 
	mNodeHandler(nh)
{
	mOdometria =  mNodeHandler.subscribe<nav_msgs::Odometry> ("/odom", 1, &CSensors::OnOdometryUpdate, this);
	mDepthImage = mNodeHandler.subscribe<sensor_msgs::Image> ("/camera/depth/image_raw", 1, &CSensors::OnDepthImageReceived, this);
	mDarknet = mNodeHandler.subscribe<darknet_ros_msgs::BoundingBoxes> ("/darknet_ros/bounding_boxes", 1, &CSensors::OnBoundingBoxReceived, this);
	mSensorsPublisher = mNodeHandler.advertise<visual_behavior_los_ultramarinos::RobotData>("/sensors_data", 1000);
}

CSensors::~CSensors(){

}

void CSensors::OnOdometryUpdate(const nav_msgs::OdometryConstPtr& msg){
	mMsgSensorData.position.x = msg->pose.pose.position.x;
	mMsgSensorData.position.y = msg->pose.pose.position.y;
	mMsgSensorData.position.theta = tf::getYaw(msg->pose.pose.orientation);


	mSensorsPublisher.publish(mMsgSensorData);
	//std::cout << "x: " << msg->pose.pose.position.x; << "\n"
	//		<< "y: " << msg->pose.pose.position.y << "\n"
	//		<< "th: " << tf::getYaw(msg->pose.pose.orientation) << std::endl;
}

void CSensors::OnDepthImageReceived(const sensor_msgs::ImageConstPtr& msg)
{
	mImageData = cv_bridge::toCvCopy(*msg, sensor_msgs::image_encodings::TYPE_32FC1);
}

void CSensors::OnBoundingBoxReceived(const darknet_ros_msgs::BoundingBoxesConstPtr& msg){
	mMsgSensorData.boundingboxes.clear();
	float px_center=mImageData->image.cols/2;
	float py_center=mImageData->image.rows/2;
	for(int i=0;i<msg->bounding_boxes.size();i++)
	{
		if (msg->bounding_boxes[i].Class=="sports ball")
		{
			float px = (msg->bounding_boxes[i].xmax + msg->bounding_boxes[i].xmin) / 2;
			float py = (msg->bounding_boxes[i].ymax + msg->bounding_boxes[i].ymin) / 2;

			float dist = mImageData->image.at<float>(cv::Point(px, py));// * 0.01f;

			float len1 = std::sqrt((px - px_center) * (px - px_center) + (py - py_center) * (py - py_center));
    		float len2 = std::abs(py - py_center);

    		float dot = px * px_center + py * py;

    		float angle = std::acos(dot / (len1 * len2));
			std::cout<<"HAY UNA PELOTA A " << dist << ", ÁNGULO: " << angle << "(" << angle*180.0/M_PI << ")" << std::endl;
		}
	}
}
