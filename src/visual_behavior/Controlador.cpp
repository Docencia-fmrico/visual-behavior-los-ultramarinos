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


#include "visual_behavior/PIDController.hpp"
#include "visual_behavior/Controlador.hpp"
#include <stdio.h>
#include <cmath>
#include <gtest/gtest.h>

// Preguntar a Paco -//

Controlador::Controlador()
: PIDAngular(0, 10.0, -0.8, 0.8), PIDLineal(0, 3.0, -0.5, 0.5)
{
  distanciaMaxima = 1.2;  // *** Cambiar *****
  distanciaSeguridad = 0.35;

  // intervalos de las referencias de entrada
  rangoAngular = {0, 10.0};
  rangoLineal = {0, distanciaMaxima};

  // intervalor de las salida
  rangoAngularSalida = {-0.75, 0.75};
  rangoLinealSalida = {-0.75, 0.75};

  alfa = 1;  // parametro que relaciona la el cambio lineal del angulo que el cambio lineal
}

double Controlador::errorGiro(double dg)
{
  return -dg;
}

double Controlador::errorAvance(double da)
{
  return da - distanciaSeguridad;
}

double Controlador::velocidadLineal(double errorLineal)
{
  return PIDLineal.get_output(errorLineal);
}

double Controlador::velocidadAngular(double errorAngular)
{
  double relacionLinealAngular = alfa;
  return PIDAngular.get_output(errorAngular) * relacionLinealAngular;
}
