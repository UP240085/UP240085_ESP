#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED    GPIO_NUM_15
#define BUTTON GPIO_NUM_5

bool sos_activo = false;

// Funciones para SOS 
void punto()
{
    gpio_set_level(LED, 1);
    vTaskDelay(pdMS_TO_TICKS(200));
    gpio_set_level(LED, 0);
    vTaskDelay(pdMS_TO_TICKS(200));
}

void raya()
{
    gpio_set_level(LED, 1);
    vTaskDelay(pdMS_TO_TICKS(500));
    gpio_set_level(LED, 0);
    vTaskDelay(pdMS_TO_TICKS(500));
}

void sos()
{
    for (int i = 0; i < 3; i++) punto();
    for (int i = 0; i < 3; i++) raya();
    for (int i = 0; i < 3; i++) punto();
}

// Tarea que ejecuta el SOS 
void sos_task(void *pvParameter)
{
    while (true)
    {
        if (sos_activo)
        {
            sos(); // ejecuta sos
        }
        else
        {
            vTaskDelay(pdMS_TO_TICKS(100)); // descansa si está apagado
        }
    }
}

// Detección de clic simple o doble 
// Devuelve 1 = clic simple, 2 = doble clic
int detectClick(void)
{
    const TickType_t debounceDelay = pdMS_TO_TICKS(50);
    const TickType_t doubleClickTimeout = pdMS_TO_TICKS(600);
    TickType_t startTime;

    // Espera la primera pulsación
    while (gpio_get_level(BUTTON) == 1) {
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    vTaskDelay(debounceDelay);

    // Espera a que se suelte
    while (gpio_get_level(BUTTON) == 0) {
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    // Marca tiempo de la primera pulsación
    startTime = xTaskGetTickCount();

    // Espera segunda pulsación
    while ((xTaskGetTickCount() - startTime) < doubleClickTimeout) {
        if (gpio_get_level(BUTTON) == 0) {
            vTaskDelay(debounceDelay);
            while (gpio_get_level(BUTTON) == 0) {
                vTaskDelay(pdMS_TO_TICKS(10));
            }
            return 2; // doble clic
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    return 1; // clic simple
}

// --- Programa principal ---
void app_main(void)
{
    gpio_reset_pin(LED);
    gpio_reset_pin(BUTTON);

    gpio_set_direction(LED, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(BUTTON, GPIO_PULLUP_ONLY);
    gpio_set_direction(BUTTON, GPIO_MODE_INPUT);

    // Tarea que maneja el SOS para permitir que el doble click se detecte (tareas separadas)
    xTaskCreate(&sos_task, "sos_task", 2048, NULL, 5, NULL);

    while (true)
    {
        int click = detectClick();
        if (click == 1) {
            sos_activo = true;   // un clic enciende
        } else if (click == 2) {
            sos_activo = false;  // doble clic apaga
            gpio_set_level(LED, 0);
        }
    }
}