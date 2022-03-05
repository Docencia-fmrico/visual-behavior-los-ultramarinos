#include "CSensors.h"
#include "CRobotMove.h"


int main(int argc, char** argv){
	ros::init(argc, argv, "follow_person");

	ros::NodeHandle nh;
	CSensors pSensors(nh);
	CRobotMove pRobotMove(nh);
	
	//pSensors = new CSensors(&nh);
	//pRobotMove = new CRobotMove(&nh);
	
	ros::spin();
	std::cout << "se esta finalizando el programa" << std::endl;
	//delete pRobotMove;
	//delete pSensors;
	
	return 0;
}