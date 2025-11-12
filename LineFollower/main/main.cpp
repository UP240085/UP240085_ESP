#include "cabeceras.h"
QTR8A sensor; 
uint16_t sensor_values[SENSOR_COUNT];
uint16_t position;

//void leerLinea(void *pvParam){}
//void control(void *pvParam){}
//void moverse(int speedLeft, int speedRight){}


esp_err_t configureGpio(void)
{
    // Configure GPIO pins for input and output modes
    gpio_reset_pin(LED1); // Reset LED pin
    gpio_reset_pin(LED2);  // Reset LED1 pin
    gpio_reset_pin(RDY);   // Reset button pin
    gpio_reset_pin(LEDESP);  // Reset button pin
    gpio_reset_pin(AIN1);  // Reset AIN1 pin
    gpio_reset_pin(AIN2);  // Reset AIN2 pin
    gpio_reset_pin(PWMA);  // Reset PWMA pin
    gpio_reset_pin(BIN1);  // Reset BIN1 pin
    gpio_reset_pin(BIN2);  // Reset BIN2 pin
    gpio_reset_pin(PWMB);  // Reset PWMB pin
    gpio_reset_pin(IR);    // Reset IR pin
    gpio_reset_pin(ISVM);  // Reset ISVM pin
    gpio_set_direction(LED1, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED2, GPIO_MODE_OUTPUT);
    gpio_set_direction(LEDESP, GPIO_MODE_OUTPUT);
    gpio_set_direction(RDY, GPIO_MODE_INPUT);  // Set button pin as input
    gpio_set_pull_mode(RDY, GPIO_PULLUP_ONLY); // Activa el pull-up interno
    gpio_set_direction(CAL, GPIO_MODE_INPUT);  // Set button pin as input
    gpio_set_pull_mode(CAL, GPIO_PULLUP_ONLY); // Activa el pull-up interno
    gpio_set_direction(ISVM, GPIO_MODE_INPUT); // Set button pin as input
    gpio_set_direction(IR, GPIO_MODE_OUTPUT);  // Set button pin as input
    gpio_set_direction(AIN1, GPIO_MODE_OUTPUT);
    gpio_set_direction(AIN2, GPIO_MODE_OUTPUT);
    gpio_set_direction(PWMA, GPIO_MODE_OUTPUT);
    gpio_set_direction(BIN1, GPIO_MODE_OUTPUT);
    gpio_set_direction(BIN2, GPIO_MODE_OUTPUT);
    gpio_set_direction(PWMB, GPIO_MODE_OUTPUT);
    return ESP_OK; // Return success
}

esp_err_t createSensor(void)
{
    // Initialize the sensor with the specified pins
    sensor.setSensorPins({D1, D2, D3, D4, D5, D6, D7, D8}, SENSOR_COUNT, THRESHOLD);
    sensor.setTypeAnalog(); // Set the sensor type to Analog
    sensor.setTimeout(2500); //Set the timeout for Analog sensors
    sensor.setSamplesPerSensor(15); //Set the number of samples per sensor
    sensor.setEmitterPin(IR); //Set the emitter pin for the sensor
    return ESP_OK;
}

esp_err_t calibrateSensor(void)
{
    sensor.calibrate(QTRReadMode::On);
    printf("Calibración iniciada...\n");
    gpio_set_level(LED1, 1);
    for (uint16_t i = 0; i < 150 ; ++i)
    {
        sensor.calibrate();
        printf("%d\n", i);
        vTaskDelay(pdMS_TO_TICKS(10));

    }
    printf("Calibración finalizada\n");
    gpio_set_level(LED1, 0);

    return ESP_OK;
}

void getMaxMinCal()
{
    for(int i = 0; i < SENSOR_COUNT; i ++)
        printf("%d\t", sensor.calibrationOn.minimum[i]);
    printf("\n");
    for (int i = 0; i < SENSOR_COUNT; i ++)
        printf("%d\t", sensor.calibrationOn.maximum[i]);
    printf("\n");
   
}

extern "C" void app_main(void)
{
    configureGpio();
    createSensor();

   // while(gpio_get_level(CAL) == 1) // Botón sin presionar
   // {
   //     vTaskDelay(pdMS_TO_TICKS(10));
   // }
    calibrateSensor();
    getMaxMinCal();
    while(1)
    {
        position = sensor.readLineBlack(sensor_values);
        for(int i = 0; i < SENSOR_COUNT; i++)
            printf("%d\t", sensor_values[i]);
        printf("P: %d\n", position);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
} 
