#include <gtest/gtest.h>
#include "ros/ros.h"
#include "visual_behavior/Controlador.hpp"



class TestControl: public Controlador
{
    public:
        const double & gira()
        {
            Controlador controlador;
            return controlador.errorGiro(10.11);
        }
        
};


TEST(Controlador_Test, test_init) 
{
  TestControl visual_behavior;

  ASSERT_EQ(visual_behavior.gira(), 7777.9);


}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "test");
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}