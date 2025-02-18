#include "interrupt.h"
#include "button.h"
#include "led.h"
#include "pico/bootrom.h"
#include "pico/stdlib.h"
#include "ssd1306.h"
#include <stdio.h>

// Função que lida com eventos de interrupção
void gpio_irq_handler(uint gpio, uint32_t events) {
    // eventos de acionamento de botões
    if(gpio == BUTTON_A || gpio == BUTTON_B || gpio == JOYSTICK_BUTTON) {
        // tratamento de debouncing dos botões
        if(!debouncing(300))
            return;
        sleep_ms(5000);
        // Habilita a entrada do modo BOOTSELL ao pressionar JOYSTICK_BUTTON
        if(gpio == JOYSTICK_BUTTON) {

        }

        if(gpio == BUTTON_A) {
            
        }

    }
}