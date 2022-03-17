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
#include "geometry_msgs/Twist.h"
#include "visual_behavior/Controlador.hpp"
#include "visual_behavior/PIDController.hpp"
#include <stdio.h>
#include <stdlib.h>

double dg = 0;
double da = 0;
const int fr = 10;  // frecuencia
double v = 0;
double w = 0;

void messageCallback(const geometry_msgs::Pose2D::ConstPtr& msg)
{
  dg = msg->y;  // Bug en el pid, si el rango de entrada es de -1 a 1 no te da los valores correctos en la salida
  da = msg->x;
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "Control");

  ros::NodeHandle nh;
  Controlador controlador = Controlador();

  ros::Subscriber sub = nh.subscribe("/controller_instructions", fr, messageCallback);
  ros::Publisher pub_vel_ = nh.advertise<geometry_msgs::Twist>("mobile_base/commands/velocity", fr);

  geometry_msgs::Twist cmd;
  ros::Rate loop_rate(fr);

  while (ros::ok())
  {
    double errg = controlador.errorGiro(dg);
    double erra = controlador.errorAvance(da);

    w = controlador.velocidadAngular(errg);
    v = controlador.velocidadLineal(erra);

    cmd.angular.z = w;
    cmd.linear.x = v;

    pub_vel_.publish(cmd);
    ros::spinOnce();
  }
  return 0;
}
