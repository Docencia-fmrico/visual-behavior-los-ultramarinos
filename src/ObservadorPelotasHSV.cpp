#include <iostream>
#include "ros/ros.h"
#include "sensor_msgs/Image.h"
#include "cv_bridge/cv_bridge.h"
#include "darknet_ros_msgs/BoundingBoxes.h"
#include "tf/tf.h"
#include "visual_behavior_los_ultramarinos/RobotData.h"
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <memory>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/types.hpp>
#include <math.h>
#include <opencv2/photo.hpp>

cv_bridge::CvImagePtr profundidad;
cv_bridge::CvImagePtr filtrada;
ros::Publisher Publicador;

void callback_bbx(const sensor_msgs::ImageConstPtr& prof , const sensor_msgs::ImageConstPtr& filt ){

    //paso la imagen filtrada a cv2 Imagen 

    cv_bridge::CvImagePtr profundidad = cv_bridge::toCvCopy(*prof , sensor_msgs::image_encodings::TYPE_32FC1);
    cv_bridge::CvImagePtr filtrada = cv_bridge::toCvCopy(*filt , sensor_msgs::image_encodings::BGR8);

    float distanciaAlCentro = profundidad->image.cols/2;

    float width =  profundidad->image.cols ;
    float height =  profundidad->image.rows ;

    //paso la imagen a binaria 

    cv::Mat img_ ;
    cv::cvtColor(filtrada->image, img_, cv::COLOR_BGR2GRAY);
    //cv::fastNlMeansDenoising(img_,img_,30,7,21); 
    cv::threshold(img_, img_, 0, 255, CV_THRESH_BINARY);
    
    // **************POSIBLE COSA***************
    cv::erode(profundidad->image, profundidad->image, cv::Mat());
    ///
    // saco el contorno de los objetos en escena 


    //cv::Mat profundidadG_ ;
    //cv::GaussianBlur(profundidad->image,profundidadG_,cv::Size(33,33),0,0,0);
    
    /// **************POSIBLE COSA***************
    cv::erode(img_, img_, cv::Mat());
    cv::erode(img_, img_, cv::Mat());
    ///
    // saco el contorno de los objetos en escena 
    
    std::vector<std::vector<cv::Point> > contours;
	  cv::findContours(img_, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    cv::Scalar colorCircle1(255,255 , 255);
    
    // si hay un mas de un contorno me quedo con el mas cercano
    std::cout << contours.size() << std::endl;
    if (contours.size() > 0) { 

      geometry_msgs::Pose2D pp;
      float distMenor = 100000000000000.0 ;
      std::vector<cv::Point> pelota ;
    
      for (size_t i = 0; i < contours.size(); i++)
      {  
       
       cv::Moments M = cv::moments(contours[i]);
       cv::Point center(M.m10/M.m00, M.m01/M.m00);

       int px = (int)center.x;
       int py = (int)center.y;
        
      if (px > 0 && py > 0 && px < width && py < height ) {
       float distAc = profundidad->image.at<float>(cv::Point(px, py));
      
       if (distAc < distMenor)
            pelota = contours[i] ;        
            distMenor = distAc ;
      }

      }

       // saco la cordenada de la pelota  

       cv::Moments M = cv::moments(pelota);
       cv::Point center(M.m10/M.m00, M.m01/M.m00);
       cv::circle(img_, center, 30, colorCircle1,2);
       int px = (int)center.x;
       int py = (int)center.y;
       
       if (px > 0 && py > 0 && px < width && py < height ) {
       // publico 

       pp.x = distMenor  *0.001f;
			 float angle = (px - distanciaAlCentro)/(distanciaAlCentro);
			 pp.y = angle;
			 Publicador.publish(pp);
		   std::cout << "ball found" << std::endl;
       ROS_INFO("dsitt %f", distMenor* 0.001f);
       }
    }
    

    cv::imshow("Otra Imagen Filtrada", img_);
    //cv::imshow("TIto GAus xd", profundidadG_);
    cv::imshow("Prof", profundidad->image);
    cv::waitKey(3);
}

int fr = 10; 
int main(int argc, char** argv)
{

	ros::init(argc, argv, "ObservadorPelotasHSV");

	ros::NodeHandle nh;
    
    message_filters::Subscriber<sensor_msgs::Image> Sub_prof(nh, "/camera/depth/image_raw", fr);
  	message_filters::Subscriber<sensor_msgs::Image> Sub_fil(nh, "/hsv/image_filtered", fr);
    Publicador = nh.advertise<geometry_msgs::Pose2D>("/ball_data", fr) ;  
    
    typedef message_filters::sync_policies::ApproximateTime<sensor_msgs::Image,sensor_msgs::Image> MySyncPolicy_bbx;
  	message_filters::Synchronizer<MySyncPolicy_bbx> sync_bbx(MySyncPolicy_bbx(10), Sub_prof, Sub_fil);
  	sync_bbx.registerCallback(boost::bind(&callback_bbx, _1, _2));

    
    
	

	ros::spin();
	
  return 0;
}
