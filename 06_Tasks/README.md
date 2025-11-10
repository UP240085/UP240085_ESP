## Tareas

![Circuito 6](https://github.com/UP240085/UP240085_ESP/blob/76a3aa22936d70f4f4dced83a9b0662eca24d902/Circuito%206.jpg)

Con este programa aprendimos el uso de las tareas dentro del ESP32 y como podemos ejecutar varias funciones en paralelo sin que sean afectadas por los retardos. Para ello pusimos en práctica un programa con dos tareas:

La primer tarea es leer el valor de un canal del ADC y almacenar la lectura en una variable global.

La segunda tarea es controlar el sentido de giro y velocidad de un motor de CD siguiendo la siguiente regla:

Si el valor del ADC está entre 0 y 2048, el motor gira en sentido horario a una velocidad proporcional.
Si el valor del ADC está entre 2049 y 4095, el motor gira en sentido antihorario a una velocidad proporcional.
