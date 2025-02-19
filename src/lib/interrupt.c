#include "interrupt.h"
#include "button.h"
#include "led.h"
#include "pico/bootrom.h"
#include "pico/stdlib.h"
#include "ssd1306.h"
#include "hardware/pwm.h"
#include <stdio.h>

// inicia ativado
volatile bool PWM = true;

// Função que lida com eventos de interrupção
void gpio_irq_handler(uint gpio, uint32_t events) {
    // eventos de acionamento de botões
    if(gpio == BUTTON_A || gpio == BUTTON_B || gpio == JOYSTICK_BUTTON) {
        // tratamento de debouncing dos botões
        if(!debouncing(300))
            return;
        // alterna o estado do led verde
        if(gpio == JOYSTICK_BUTTON) {
            gpio_put(LED_GREEN, !gpio_get(LED_GREEN));
            border_len++;
            if(border_len >= 4)
                border_len = 0;
            
        }

        if(gpio == BUTTON_A) {
            if(PWM) {
                pwm_set_gpio_level(LED_RED, 0);
                pwm_set_gpio_level(LED_BLUE, 0);
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