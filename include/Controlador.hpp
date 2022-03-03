
#ifndef CONTROLADOR_HPP_
#define CONTROLADOR_HPP_
#include "stdio.h"

class Controlador {

 public:

     float errorGiro(float dg) ;
     float errorAvance(float da ) ;
     float velocidadAngular(float errorAngular );
     float velocidadLineal( float errorAvance ) ;

private:  
     
     int width ;
     float distanciaMaxima ;

     std:vector<float> rangoAngular ;
     std:vector<float> rangoLineal ;

     PIDController PIDAngular ;
     PIDController PIDLineal ;



} ;

#endif