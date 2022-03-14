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



## Control <img src="https://camo.githubusercontent.com/9ed64b042a76b8a97016e877cbaee0d6df224a148034afef658d841cf0cd1791/68747470733a2f2f63756c746f667468657061727479706172726f742e636f6d2f706172726f74732f68642f6c6170746f705f706172726f742e676966" width="30" height="30"/>
**La parte de control se encarga de calcular las velocidades del robot, en función de lo que obtiene del coordinador.**

Así es como hemos definido los parámetros y los errores:

<img src="https://user-images.githubusercontent.com/98589920/158070001-26d49b98-c20d-4eb4-8361-679309338973.png" width="200" height="200"/><img src="https://user-images.githubusercontent.com/98589920/158071760-f407a529-e18c-4e84-a5fc-3bbd6799aefe.png" width="250" height="230"/><img src="https://user-images.githubusercontent.com/98589920/158072158-0a0dbcdf-c13a-4b1c-940b-8a5d6683d8ff.png" width="250" height="230"/>	


Diagrama de bloques del controlador:

<img src="https://user-images.githubusercontent.com/98589920/158069828-c22068a9-308f-4203-98a3-f074b5ffc7de.png" width="369" height="75"/>	

Aplicamos el PID:

<img src="https://user-images.githubusercontent.com/69701088/156851248-dc2d6777-d927-4fbb-9507-f9312af57cbf.gif" width="369" height="369"/>	<img src="https://user-images.githubusercontent.com/98589920/158074027-2c4906e3-9d57-4742-b7c3-fba1c1d4dea1.gif" width="369" height="369"/>

Funcionalidades del nodo
| Funciones | Descripción |
| --- | --- |
| errorGiro | Función que calcula el error de giro as partir de dg |
| errorAvance | Función que obtiene el error de avance a partir de da |
| velocidadLineal | Función que calcula la velocidad lineal a partir del error de avance |
| velocidadAngular | Función que calcula la velocidad angular a partir del error de giro |


## Coordinación <img src="https://camo.githubusercontent.com/9ed64b042a76b8a97016e877cbaee0d6df224a148034afef658d841cf0cd1791/68747470733a2f2f63756c746f667468657061727479706172726f742e636f6d2f706172726f74732f68642f6c6170746f705f706172726f742e676966" width="30" height="30"/>


## Visión <img src="https://camo.githubusercontent.com/9ed64b042a76b8a97016e877cbaee0d6df224a148034afef658d841cf0cd1791/68747470733a2f2f63756c746f667468657061727479706172726f742e636f6d2f706172726f74732f68642f6c6170746f705f706172726f742e676966" width="30" height="30"/>

## Behavior Tree
Esta es el behaviortree:
```mermaid
graph TD;
  BehaviorTree-->Reactive_Sequence;
	Reactive_Sequence-->FallBack;
    FallBack-->ballseen?;
    FallBack-->Turn;
	Reactive_Sequence-->Reac_Seq;
```

## Primer Programa
### 1. Objetivo
El objetivo del primer programa es ...
### 2. Funciones
| Funciones | Descripción |
| --- | --- |
|  |  |
|  |  |

```c++

```
### 3. Vídeo

## Follow Person
### 1. Objetivo
El objetivo del segundo programa es seguir a una persona detectandola con darket_ros.
### 2. Lanzar el programa
Al ejecutar roscore ponemos en marcha el Master, el servidor de parámetros y rosout que es el log donde podemos mostrar datos para depuración entre otros.
```
roscore
```
Lanzamos el simulador.
```
roslaunch robots sim.launch
```
Lanzamos el darknet_ros para detectar la persona.
```
roslaunch darknet_ros darknet_ros.launch image:=/camera/rgb/image_raw
```
Lanzamos el programa.
```
roslaunch visual_behavior_los_ultramarinos follow_person.launch
```
### 3. Funciones
| Funciones | Descripción |
| --- | --- |
|  |  |
|  |  |

```c++

```
### 4. Vídeo

## Tercer Programa
### 1. Objetivo
El objetivo del tercer programa es ...
### 2. Funciones
| Funciones | Descripción |
| --- | --- |
|  |  |
|  |  |

```c++

```
### 3. Vídeo

## Tests
