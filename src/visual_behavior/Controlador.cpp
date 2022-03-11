#include "stdio.h"
#include <cmath>
#include "visual_behavior/PIDController.hpp"
#include "visual_behavior/Controlador.hpp"
#include <gtest/gtest.h>

// Preguntar a Paco -//

Controlador::Controlador() 
: PIDAngular(0, 10.0, -0.75 ,0.75), 
  PIDLineal(0, 2.0, -0.75 ,0.75)
/*
: PIDAngular(rangoAngular[0],rangoAngular[1],rangoAngularSalida[0],rangoAngularSalida[1]), 
  PIDLineal(rangoLineal[0],rangoLineal[1],rangoLinealSalida[0],rangoLinealSalida[1])
  */
{
    

    distanciaMaxima = 2.0; // *** Cambiar *****
    distanciaSeguridad = 0.0 ; 
     
    // intervalos de las referencias de entrada 
    rangoAngular = {0,10.0};
    rangoLineal = {0,distanciaMaxima};

    // intervalor de las salida 
    rangoAngularSalida = {-0.75,0.75};
    rangoLinealSalida = {-0.75,0.75};
     
    //
    alfa = 1.0 ; //parametro que relaiona la el cambio lineal del angulo que el cambio lineal
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