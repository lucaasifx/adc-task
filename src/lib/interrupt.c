#include "interrupt.h"
#include "button.h"
#include "led.h"
#include "pico/bootrom.h"
#include "pico/stdlib.h"
#include "ssd1306.h"
#include "hardware/pwm.h"
#include <stdio.h>


bool PWM = true;

// Função que lida com eventos de interrupção
void gpio_irq_handler(uint gpio, uint32_t events) {
    // eventos de acionamento de botões
    if(gpio == BUTTON_A || gpio == BUTTON_B || gpio == JOYSTICK_BUTTON) {
        // tratamento de debouncing dos botões
        if(!debouncing(300))
            return;
        if(gpio == JOYSTICK_BUTTON) {
        }

        if(gpio == BUTTON_A) {
            uint red_slice = pwm_gpio_to_slice_num(LED_RED);
            uint blue_slice = pwm_gpio_to_slice_num(LED_BLUE);
            if(PWM) {
                pwm_set_enabled(red_slice, false);
                pwm_set_enabled(blue_slice, false);
            }
            else {
                pwm_set_enabled(red_slice, true);
                pwm_set_enabled(blue_slice, true);
            }
            PWM = !PWM;
        }

        if(gpio == BUTTON_B) {
            //limpa o display
            ssd1306_fill(&ssd, false);
            ssd1306_send_data(&ssd);
            // Entra no modo BOOTSELL e encerra o programa
            printf("\nFinalizando o programa...\n");
            reset_usb_boot(0,0);
        }

    }
}