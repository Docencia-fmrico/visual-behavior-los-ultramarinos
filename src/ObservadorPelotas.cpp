// Copyright 2022 los ultramarinos
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <iostream>
#include "ros/ros.h"
#include "sensor_msgs/Image.h"
#include "cv_bridge/cv_bridge.h"
#include "darknet_ros_msgs/BoundingBoxes.h"
#include "tf/tf.h"
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include "geometry_msgs/Pose2D.h"
#include <memory>
#include <string>

ros::Publisher mSensorsPublisher;

void callback_bbx(const sensor_msgs::ImageConstPtr& img, const darknet_ros_msgs::BoundingBoxesConstPtr& boxes)
{
  cv_bridge::CvImagePtr mImageData = cv_bridge::toCvCopy(*img, sensor_msgs::image_encodings::TYPE_32FC1);
  std::string tag = "sports ball";
  geometry_msgs::Pose2D pp;
  float px_center = mImageData->image.cols/2;
  float py_center = mImageData->image.rows/2;
  float min_dist = 10;
  bool found = false;

  for (int i = 0; i < boxes->bounding_boxes.size(); i++)
  {
    if (boxes->bounding_boxes[i].Class == tag)
    {
      float px = (boxes->bounding_boxes[i].xmax + boxes->bounding_boxes[i].xmin) / 2;
      float py = (boxes->bounding_boxes[i].ymax + boxes->bounding_boxes[i].ymin) / 2;

      float dist = mImageData->image.at<float>(cv::Point(px, py));  // * 0.001f;

      if (dist < min_dist)
      {
        // now = ros::Time::now().toSec();
        // mMsgSensorData.header.stamp.sec = now;
        min_dist = dist;
        pp.x = dist;
        float angle = (px - px_center) / (px_center);
        pp.y = angle;
        found = true;
      }
    }
  }

  if (found)
  {
    mSensorsPublisher.publish(pp);
    std::cout << "ball found" << std::endl;
  }
}

int main(int argc, char** argv)
{
  int fr = 10;
  ros::init(argc, argv, "ObservadorPelotas");

  ros::NodeHandle nh;

  message_filters::Subscriber<sensor_msgs::Image> image_depth_sub(nh, "/camera/depth/image_raw", fr);
  message_filters::Subscriber<darknet_ros_msgs::BoundingBoxes> bbx_sub(nh, "/darknet_ros/bounding_boxes", fr);

  typedef message_filters::sync_policies::ApproximateTime<sensor_msgs::Image, darknet_ros_msgs::BoundingBoxes> bbx;
  message_filters::Synchronizer<bbx> sync_bbx(bbx(10), image_depth_sub, bbx_sub);

  sync_bbx.registerCallback(boost::bind(&callback_bbx, _1, _2));

  mSensorsPublisher = nh.advertise<geometry_msgs::Pose2D>("/ball_data", fr);

  ros::spin();
  return 0;
}
