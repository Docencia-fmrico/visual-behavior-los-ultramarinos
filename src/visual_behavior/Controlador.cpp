#include "stdio.h"
#include <cmath>
#include "visual_behavior/PIDController.hpp"
#include "visual_behavior/Controlador.hpp"
#include <gtest/gtest.h>

// Preguntar a Paco -//

Controlador::Controlador() 
: PIDAngular(0, 10.0, -0.8 ,0.8), PIDLineal(0, 3.0, -0.5 ,0.5)
{
    
    distanciaMaxima = 1.2; // *** Cambiar *****
    distanciaSeguridad = 0.35 ; 
     
    // intervalos de las referencias de entrada 
    rangoAngular = {0,10.0};
    rangoLineal = {0,distanciaMaxima};

    // intervalor de las salida 
    rangoAngularSalida = {-0.75,0.75};
    rangoLinealSalida = {-0.75,0.75};
     
    //
    alfa = 1 ; //parametro que relaiona la el cambio lineal del angulo que el cambio lineal
}



 double Controlador::errorGiro( double dg){
     return -dg; 
}

 double Controlador::errorAvance( double da){
     return da - distanciaSeguridad;
}

 double Controlador::velocidadLineal( double errorLineal){
    return PIDLineal.get_output(errorLineal);
}

 double Controlador::velocidadAngular( double errorAngular){
    double relacionLinealAngular = alfa ;
    return PIDAngular.get_output(errorAngular) * relacionLinealAngular ;
}

/*
float Controlador::velocidadAngular(float errorAngular){
    float relacionLinealAngular = 0 ;
    return PIDAngular.get_output(errorAngular) * relacionLinealAngular ;
}


float Controlador::reglaDeTres(float widthconocido){
    float absWidthc = std::abs(widthconocido);
    return absWidthc/(width/2);
}
*/