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

#include "ros/ros.h"
#include "geometry_msgs/Pose2D.h"
#include "tf/tf.h"
#include <cmath>
#include <memory>

int fr = 10;

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
  if ((std::abs(mSensorsData.y - person.y) < 0.3 && std::abs(mSensorsData.x - person.x) < 0.3) || !found_person)
  {
    person = *msg;
    latest_person = ros::Time::now().toSec();
  }
}

void ballReceived(const geometry_msgs::Pose2DConstPtr& msg)
{
  mSensorsData = *msg;
  if (std::abs(mSensorsData.x - ball.x) < 0.5 || !found_ball)
  {
    ball = *msg;
    latest_ball = ros::Time::now().toSec();
  }
}

bool lost(double latest_msg, double now)
{
  return (now - latest_msg > MAX_WAIT);
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "Coordinador");
  ros::NodeHandle nh;
  ros::Rate loop_rate(fr);
  ros::Publisher cPub = nh.advertise<geometry_msgs::Pose2D> ("/controller_instructions", fr, true);
  ros::Subscriber personSub = nh.subscribe<geometry_msgs::Pose2D>("/person_data", fr, personReceived);
  ros::Subscriber ballSub = nh.subscribe<geometry_msgs::Pose2D>("/ball_data", fr, ballReceived);

  geometry_msgs::Pose2D lost_;
  lost_.x = 1;

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
