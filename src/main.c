#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"
#include <stdio.h>
// ************************************** LIBS ********************************************
#include "lib/button.h"
#include "lib/led.h"
#include "lib/interrupt.h"
#include "lib/ssd1306.h"

// ************************************* MACROS ******************************************

#define VRX_PIN 27
#define WRAP 4095
#define DEAD_ZONE 140  // Define uma faixa de tolerância
#define CENTER_JS 2048

// Configurações da I2C
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

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
    adc_gpio_init(VRX_PIN); // Configura GP27 (ADC0) para o eixo X do joystick
    // Inicializa I2C
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_send_data(&ssd);
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    // Inicializa botões com interrupção
    init_button_with_interrupt(JOYSTICK_BUTTON, GPIO_IRQ_EDGE_FALL, true);
    init_button_with_interrupt(BUTTON_A, GPIO_IRQ_EDGE_FALL, true);
    init_button_with_interrupt(BUTTON_B, GPIO_IRQ_EDGE_FALL, true);



    uint red_slice = pwm_init_gpio(LED_RED, WRAP);
    pwm_set_enabled(red_slice, true); // Ativa o PWM

    while (true) {
        // Controle de intensidade do led vermelho
        adc_select_input(1);
        uint16_t vrx_value = adc_read();
        int16_t diff_x = vrx_value - CENTER_JS;
        diff_x = abs(diff_x);
        printf("diff: %d\n", diff_x);
        // Verifica
        diff_x < DEAD_ZONE  ? pwm_set_gpio_level(LED_RED, 0) 
                            : pwm_set_gpio_level(LED_RED, diff_x * 2);



        printf("VRX: %d\n", vrx_value);

        sleep_ms(10);
    }
    return 0;
}
