[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-f059dc9a6f8d3a56e377f745f24479a46679e63a5d9fe6f495e02850cd0d8118.svg)](https://classroom.github.com/online_ide?assignment_repo_id=7213482&assignment_repo_type=AssignmentRepo)
# visual-behaviors

La práctica se compone de tres partes:

1. Seguimiento visual de la pelota: El robot debe seguir una pelota. El robot debe avanzar hasta estar a un metro, aproximadamente, de la pelota. A partir de ahí, seguira orientandose hacia ella, y tratando de mantener la distancia de un metro incluso si la pelota se mueve hacia el robot. Se usará:
   1.  Un filtro de color en HSV
   2.  Una estimación de la distancia al objeto filtrado, ya sea con PointCloud o con la imagen de profundidad.
   3.  Behavior Trees para programar la lógica de control.
   4.  PIDs para la orientación y la distancia.
2. Seguimiento visual de una persona: Similar al punto anterior, pero detectando a la persona con darket_ros.
3. Comportamiento mixto: El robot debe seguir tanto a las personas como a las pelotas que perciba con la cámara, teniendo prioridad la pelota.

Entrega: Viernes 18 de Marzo.



## Índice
- [Introducción](#Introducción)
  - [Control](#Control)
  - [Visión](#Visión)
  - [Coordinación](#Coordinación)
- [Behavior Tree](#Behavior-Tree)
- [Primer Programa](#Primer-Programa)
- [Follow Person](#Follow-Person)
- [Tercer Programa](#Tercer-Programa)

## Introducción <img src="https://camo.githubusercontent.com/a6af43479d42a1a2fb5c9b40ee7c8cb4166fe525162357d400ee99afe3eac2fa/68747470733a2f2f63756c746f667468657061727479706172726f742e636f6d2f706172726f74732f68642f676974687562706172726f742e676966" width="30" height="30"/>
Práctica 2 de la asignatura software para robots:
Consiste en conseguir que un robot kobuki pueda seguir personas, pelotas (u objetos similasres) o ambos.

<img src="https://user-images.githubusercontent.com/69701088/158229459-9540713f-98b9-4e3a-9b9b-2cc24609c321.gif" width="480" height="270"/> <img src="https://user-images.githubusercontent.com/69701088/158232410-e77b989b-465b-4e73-a331-04579db9fa5a.gif" width="320" height="270"/>

La funcionalidad de los programas está dividida en tres partes: control, visión y coordinación.

Grafo de computación:

<img src="https://user-images.githubusercontent.com/98589920/158873516-9a189ae3-e1b1-4671-b1b0-cffa7876735e.png" width="300" height="300"/>




## Visión <img src="https://camo.githubusercontent.com/9ed64b042a76b8a97016e877cbaee0d6df224a148034afef658d841cf0cd1791/68747470733a2f2f63756c746f667468657061727479706172726f742e636f6d2f706172726f74732f68642f6c6170746f705f706172726f742e676966" width="30" height="30"/>


## Coordinación <img src="https://camo.githubusercontent.com/9ed64b042a76b8a97016e877cbaee0d6df224a148034afef658d841cf0cd1791/68747470733a2f2f63756c746f667468657061727479706172726f742e636f6d2f706172726f74732f68642f6c6170746f705f706172726f742e676966" width="30" height="30"/>

El nodo de coordinación contiene la inteligencia del programa. Recibe los datos de los nodos de visión y los selecciona para después enviarlos al nodo de control.

Esta selección consta de tres partes.

### Parte 1

Datos consistentes: 

Los nodos de visión envían 10 mensajes por segundo. La mayoría son del objeto que estamos siguiendo, pero es posible que de un frame a otro se pierda el objeto que estamos siguiendo momentáneamente. Si no hay otros objetos a los que prestar atención, esto no sería un problema, porque el nodo de visión no mandaría ningún mensaje, pero a menudo nos encontramos con que el entorno contiene otro objeto que pasa el filtro, y recibimos sus datos. Estas distracciones momentáneas introducen ruido en nuestros datos e impiden al robot moverse consistentemente. El coordinador se asegura de mantener la atención del robot en el objeto que está siguiendo en ese momento.

### Parte 2

Datos recientes: 

Siempre que se recibe un dato válido de cualquier observador, se actualiza su recencia. Esta recencia se usa para mantener la atención del robot sobre el objeto incluso si se pierde temporalmente entre frames. Los objetos se consideran perdidos cuando su recencia supera 1 segundo.

### Parte 3

PELOTAS!!!

Finalmente, el nodo de coordinación se asegura de actualizar el estado (encontrado o no) de las pelotas y las personas de forma independiente, y ordena al nodo de control seguir pelotas antes que personas.

Deteccion de Pelota : (HSV + DETECCION CONTORNOS)

<img src="https://user-images.githubusercontent.com/69701088/158876552-6242faa4-2ccc-42e2-b2fe-b714b85d998e.png" width="740" height="200"/>

Deteccion de Humanos : ( Darknet )

<img src="https://user-images.githubusercontent.com/69701088/158878029-ee796c93-177c-4f43-89ae-59969c43ecd7.png" width="300" height="200"/>

Siempre coger el objeto mas cercano :

<img src="https://user-images.githubusercontent.com/69701088/158886196-0b647c28-72f4-4dc4-9cfe-d98d68dbc708.gif" width="400" height="225"/> <img src="https://user-images.githubusercontent.com/69701088/158886098-818bb482-23ad-42d2-b8ce-0a4e02ec7001.gif" width="225" height="225"/>

## Control <img src="https://camo.githubusercontent.com/9ed64b042a76b8a97016e877cbaee0d6df224a148034afef658d841cf0cd1791/68747470733a2f2f63756c746f667468657061727479706172726f742e636f6d2f706172726f74732f68642f6c6170746f705f706172726f742e676966" width="30" height="30"/>
**La parte de control se encarga de calcular las velocidades del robot, en función de lo que obtiene del coordinador.**

Así es como hemos definido los parámetros y los errores:

<img src="https://user-images.githubusercontent.com/98589920/158071760-f407a529-e18c-4e84-a5fc-3bbd6799aefe.png" width="250" height="230"/><img src="https://user-images.githubusercontent.com/98589920/158072158-0a0dbcdf-c13a-4b1c-940b-8a5d6683d8ff.png" width="250" height="230"/>	


Diagrama de bloques del controlador:

<img src="https://user-images.githubusercontent.com/98589920/158069828-c22068a9-308f-4203-98a3-f074b5ffc7de.png" width="400" height="100"/> <img src="https://user-images.githubusercontent.com/98589920/158070001-26d49b98-c20d-4eb4-8361-679309338973.png" width="100" height="100"/>	




Aplicamos el PID:


<img src="https://user-images.githubusercontent.com/69701088/156851248-dc2d6777-d927-4fbb-9507-f9312af57cbf.gif" width="400" height="400"/> 

<img src="https://user-images.githubusercontent.com/69701088/158868571-c13e575f-00d7-4798-9704-a3262c3ffb87.gif" width="400" height="225"/>		

Funcionalidades del nodo
| Funciones | Descripción |
| --- | --- |
| errorGiro | Función que calcula el error de giro as partir de dg |
| errorAvance | Función que obtiene el error de avance a partir de da |
| velocidadLineal | Función que calcula la velocidad lineal a partir del error de avance |
| velocidadAngular | Función que calcula la velocidad angular a partir del error de giro |


## Siguiendo la pelota

### - Funcionamiento

Este programa usa un nodo de visión, uno de coordinación y uno de control. 

El nodo de visión usa un filtro HSV para filtrar el color rosa y determinar la posición de la pelota usando la imagen de profundidad.
	
Después el nodo coordinador se asegura de que los datos recibidos son válidos y los envía al nodo de control.
	
Finalmente el nodo de control usa PIDs para determinar las velocidades angular y lineal del robot y las publica.
	
### - Vídeo

## Siguiendo a una persona

### - Funcionamiento

Este programa usa un nodo de visión, uno de coordinación y uno de control. 

En este caso el nodo de visión usa darknet_ros para detectar a la persona y determina su posición en la imagen de profundidad igual que en el seguimiento de la pelota.

Los nodos coordinador y de control tienen el mismo comportamiento.
	
### - Vídeo

## Pelotas y personas

### - Funcionamiento

Este programa usa dos nodos de visión, uno de coordinación y uno de control.

Los nodos de visión son los empleados en los programas anteriores, con HSV para pelotas y darknet_ros para personas.

El nodo de coordinación ahora además se encarga de priorizar el objeto de seguimiento, enviando solo la posición de la pelota si hay una pelota y una persona detectadas, y actualizando el estado de ambos objetos.

El nodo de control recibe los datos filtrados por el nodo coordinador, siguiendo siempre al objeto determinado por el mismo.

### - Vídeo
