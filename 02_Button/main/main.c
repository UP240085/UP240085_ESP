#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED    GPIO_NUM_15
#define BUTTON GPIO_NUM_5

bool sos_activo = false;
TickType_t last_click_time = 0;
int click_count = 0;

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

void app_main(void)
{
    gpio_reset_pin(LED);
    gpio_reset_pin(BUTTON);

    gpio_set_direction(LED, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(BUTTON, GPIO_PULLUP_ONLY);
    gpio_set_direction(BUTTON, GPIO_MODE_INPUT);

    int last_button_state = 1;

    while (true)
    {
        int current_button_state = gpio_get_level(BUTTON);

        if (current_button_state == 0 && last_button_state == 1)
        {
            TickType_t now = xTaskGetTickCount();

            if ((now - last_click_time) < pdMS_TO_TICKS(1000))
            {
                click_count++;
            }
            else
            {
                click_count = 1;
            }

            last_click_time = now;

            if (click_count == 2)
            {
                sos_activo = false; // doble clic  apagar
                click_count = 0;
            }
            else if (click_count == 1)
            {
                sos_activo = true; // clic simple  encender
            }
        }

        last_button_state = current_button_state;

        if (sos_activo)
        {
            sos();
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
