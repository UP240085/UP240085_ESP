#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "QTR8A.h"              //Infrarrojo
#include "driver/ledc.h"       // PWM CONTROL

//Definición de infrarrojos, Sensor de Linea
#define D1 GPIO_NUM_36
#define D2 GPIO_NUM_39
#define D3 GPIO_NUM_34
#define D4 GPIO_NUM_35
#define D5 GPIO_NUM_32
#define D6 GPIO_NUM_33
#define D7 GPIO_NUM_25
#define D8 GPIO_NUM_26
#define IR GPIO_NUM_23

//Motor Derecho
#define PWMB GPIO_NUM_19
#define BIN2 GPIO_NUM_18
#define BIN1 GPIO_NUM_5

//Motor Izquierdo
#define PWMA GPIO_NUM_4
#define AIN2 GPIO_NUM_16
#define AIN1 GPIO_NUM_17

//Pines para motores
#define CAL GPIO_NUM_15
#define RDY GPIO_NUM_13

//Pines de Leds
#define YELLOW GPIO_NUM_27
#define GREEN GPIO_NUM_14
#define LEDESP GPIO_NUM_2

#define ISVM GPIO_NUM_12

//Configuración de constantes y variables
#define SENSOR_COUNT 8  //Cantidad de sensores en el QTR8A
#define CAL_MIN 1023  //Valor minimo de calibración
#define CAL_MAX 4095  //Valor máximo de calibración
#define SETPOINT 3500 // depende de la posición ideal
#define THRESHOLD 300 // Umbral para sensores QTR (si detecta mas de este valor, es línea)
#define MAX_SPEED 255 // Velocidad máxima del robot (0-255)
#define UMBRAL_LINEA 980 // Umbral para detectar la línea