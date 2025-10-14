#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED    GPIO_NUM_2
#define BUTTON GPIO_NUM_5

bool sos_activo = false;

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
    for(int i = 0; i < 3; i++) punto();
    for(int i = 0; i < 3; i++) raya();
    for(int i = 0; i < 3; i++) punto();
}

bool detectClick(bool* doubleClickDetected)
{
    const TickType_t debounceDelay = pdMS_TO_TICKS(50);
    const TickType_t doubleClickTimeout = pdMS_TO_TICKS(500);
    TickType_t startTime;

    // Espera la primera pulsación
    while (gpio_get_level(BUTTON) == 1)
    {
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    vTaskDelay(debounceDelay);

    while (gpio_get_level(BUTTON) == 0)
    {
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    startTime = xTaskGetTickCount();

    // Espera segunda pulsación para detectar doble clic
    while ((xTaskGetTickCount() - startTime) < doubleClickTimeout)
    {
        if (gpio_get_level(BUTTON) == 0)
        {
            vTaskDelay(debounceDelay);
            while (gpio_get_level(BUTTON) == 0)
            {
                vTaskDelay(pdMS_TO_TICKS(10));
            }

            *doubleClickDetected = true;
            return true;
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }

    *doubleClickDetected = false;
    return true; // Se detectó al menos un clic
}

void app_main(void)
{
    gpio_reset_pin(LED);
    gpio_reset_pin(BUTTON);

    gpio_set_direction(LED, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(BUTTON, GPIO_PULLUP_ONLY);
    gpio_set_direction(BUTTON, GPIO_MODE_INPUT);

    while (true)
    {
        bool doubleClick = false;

        if (detectClick(&doubleClick))
        {
            if (doubleClick)
            {
                sos_activo = false; // Apagar con doble clic
            }
            else
            {
                sos_activo = true; // Encender con clic simple
            }
        }

        if (sos_activo)
        {
            sos();
        }

        vTaskDelay(pdMS_TO_TICKS(20));
    }
}



