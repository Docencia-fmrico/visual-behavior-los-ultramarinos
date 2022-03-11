
#ifndef CONTROLADOR_HPP_
#define CONTROLADOR_HPP_

#include "stdio.h"
#include <iostream>
#include <vector>
#include "visual_behavior/PIDController.hpp"
#include "visual_behavior/Controlador.hpp"


class Controlador {
     

 public:

     Controlador();

      double errorGiro( double dg) ;
      double errorAvance( double da ) ;
      double velocidadAngular( double errorAngular );
      double velocidadLineal(  double errorAvance ) ;

     
protected:  
     
     
     double distanciaMaxima ;
     double distanciaSeguridad;


     std::vector<double> rangoAngular ;
     std::vector<double> rangoLineal ;

     
     std::vector<double> rangoAngularSalida;
     std::vector<double> rangoLinealSalida;

     PIDController PIDAngular;
     PIDController PIDLineal;

     double alfa;
} ;


#endif 
