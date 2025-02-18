#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"

// ************************************** LIBS ********************************************
#include "lib/button.h"
#include "lib/led.h"
#include "lib/interrupt.h"
#include "lib/ssd1306.h"

// ************************************* MACROS ******************************************

#define VRX_PIN 26
#define VRY_PIN 27
#define WRAP 4095



// Função para inicializar o PWM
uint pwm_init_gpio(uint gpio, uint wrap) {
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    pwm_set_wrap(slice_num, wrap);
    pwm_set_enabled(slice_num, false);
    return slice_num;
}




int main() {
    stdio_init_all();

    // Inicializa o módulo ADC do Raspberry Pi Pico
    adc_init();
    adc_gpio_init(VRX_PIN); // Configura GP26 (ADC0) para o eixo X do joystick
    adc_gpio_init(VRY_PIN); // Configura GP27 (ADC1) para o eixo Y do joystick

    // Inicializa os botões com uso de interrupção
    init_button_with_interrupt(JOYSTICK_BUTTON, GPIO_IRQ_EDGE_FALL, true);
    init_button_with_interrupt(BUTTON_A, GPIO_IRQ_EDGE_FALL, true);

    // Inicializa o slice de PWM dos leds
    uint blue_slice = pwm_init_gpio(LED_BLUE, WRAP);
    uint green_slice = pwm_init_gpio(LED_GREEN, WRAP);
    
    while(true) {


        // para evitar que o loop seja executado muito rapidamente
        sleep_ms(50);
    }
    return 0;
}