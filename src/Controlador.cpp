#include "Controlador.hpp"
#include "stdio.h"
#include <cmath>

Controlador::Controlador(){

     width = 10 ; // ver en los parametros de la camara xd

     distanciaMaxima = 2.0;
     distaciaSeguridad = 0.2 ; // **** Posibles Cambios ***** 
     
     // intervalos de las referencias de entrada 
     rangoAngular = {-width/2,width/2};
     rangoLineal = {0,distanciaMaxima};

     // intervalor de las salida 
     rangoAngularSalida = {-0.75,0.75};
     rangoLinealSalida = {-0.75,0.75};
     
     // PIDs angular y lineal 
     PIDAngular = PIDController(rangoAngular[0],rangoAngular[1],rangoAngularSalida[0],rangoAngularSalida[1]);
     PIDLineal = PIDController(rangoLineal[0],rangoLineal[1],rangoLinealSalida[0],rangoLinealSalida[1]);
      
}

float Controlador::reglaDeTres(float widthconocido){
    float absWidthc = std::abs(widthconocido);
    return absWidthc/(width/2)
}

float Controlador::errorGiro(float dg){
     return -dg; //xd 
}

float Controlador::errorAvance(float da){
     return da - distaciaSeguridad;
}

float Controlador::velocidadLineal(float errorLineal){
    return PIDLineal.get_output(errorLineal);
}

float Controlador::velocidadAngular(float errorAngular){
    float relacionLinealAngular = 0 ;
    return PIDAngular.get_output(errorAngular) * relacionLinealAngular ;
}
