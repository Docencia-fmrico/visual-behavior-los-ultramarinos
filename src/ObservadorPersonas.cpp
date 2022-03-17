#include <iostream>
#include "ros/ros.h"
//#include "nav_msgs/Odometry.h"
#include "sensor_msgs/Image.h"
#include "cv_bridge/cv_bridge.h"
#include "darknet_ros_msgs/BoundingBoxes.h"
#include "tf/tf.h"
#include "visual_behavior_los_ultramarinos/RobotData.h"
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>

#include <memory>

cv_bridge::CvImagePtr mImageData;
ros::Publisher mSensorsPublisher;
float min_dist;
bool found;
double now;
int fr = 10;
geometry_msgs::Pose2D pp;



void callback_bbx(const sensor_msgs::ImageConstPtr& img, const darknet_ros_msgs::BoundingBoxesConstPtr& boxes){

	mImageData = cv_bridge::toCvCopy(*img, sensor_msgs::image_encodings::TYPE_32FC1);
	cv::Mat profundidadG_ ;
	/////////////////////
    //cv::GaussianBlur(mImageData->image,profundidadG_,cv::Size(33,33),0,0,0);
	
	cv::erode(mImageData->image, profundidadG_, cv::Mat());
	//////////////////////******************CAMBIOS*****************
	std::string tag = "person";
	
	float px_center=mImageData->image.cols/2;
	float py_center=mImageData->image.rows/2;
	// #############################
	//min_dist=3;
	min_dist=5;
	// #####################################
	found = false;
	
	for(int i=0;i<boxes->bounding_boxes.size();i++)
	{
		if (boxes->bounding_boxes[i].Class == tag)
		{
			float px = (boxes->bounding_boxes[i].xmax + boxes->bounding_boxes[i].xmin) / 2;
			float py = (boxes->bounding_boxes[i].ymax + boxes->bounding_boxes[i].ymin) / 2;

			float dist = profundidadG_.at<float>(cv::Point(px, py)) * 0.001f;

			if (dist < min_dist)
			{
				min_dist = dist;
				pp.x = dist;
				float angle = (px - px_center)/(px_center);
				pp.y = angle;
				found = true;
				}
		}
	}

	if (found){
		ROS_INFO("PERSON DETECTED AT [%f],[%f]", pp.x, pp.y);
		mSensorsPublisher.publish(pp);
		}
}

int main(int argc, char** argv)
{

	ros::init(argc, argv, "ObservadorPersonas");

	ros::NodeHandle nh;

	message_filters::Subscriber<sensor_msgs::Image> image_depth_sub(nh, "/camera/depth/image_raw", fr);
  	message_filters::Subscriber<darknet_ros_msgs::BoundingBoxes> bbx_sub(nh, "/darknet_ros/bounding_boxes", fr);

 	typedef message_filters::sync_policies::ApproximateTime<sensor_msgs::Image, darknet_ros_msgs::BoundingBoxes> Sync;
  	message_filters::Synchronizer<Sync> sync_bbx(Sync(10), image_depth_sub, bbx_sub);

  	sync_bbx.registerCallback(boost::bind(&callback_bbx, _1, _2));

	mSensorsPublisher = nh.advertise<geometry_msgs::Pose2D>("/person_data", fr);

	ros::spin();
	

}