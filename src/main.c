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
#define VRY_PIN 26
#define WRAP 4095
#define DEAD_ZONE_RED 150 // Define uma faixa de tolerância
#define DEAD_ZONE_BLUE 200
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


void led_set_intensity (uint8_t ledGPIO, uint16_t axis, uint8_t DEADZONE) {
    int16_t diff = axis - CENTER_JS;
    diff = abs(diff);
    // Verifica se o joystick está no centro
    diff < DEADZONE     ? pwm_set_gpio_level(ledGPIO, 0)
                        : pwm_set_gpio_level(ledGPIO, diff * 2);
}


uint16_t joystick_read_value(uint8_t jsGPIO) {
    adc_select_input(jsGPIO - 26);
    uint16_t axis_value = adc_read();
    return axis_value;
}

int main() {
    stdio_init_all();

    // Inicializa o módulo ADC do Raspberry Pi Pico
    adc_init();
    adc_gpio_init(VRX_PIN); // Configura GP27 (ADC0) para o eixo X do joystick
    adc_gpio_init(VRY_PIN);

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

    uint blue_slice = pwm_init_gpio(LED_BLUE, WRAP);
    pwm_set_enabled(blue_slice, true); // Ativa o PWM

    while (true) {
        uint16_t vrx_value = joystick_read_value(VRX_PIN);
        uint16_t vry_value = joystick_read_value(VRY_PIN);
        // se o pwm estiver ativo
        if(PWM) {
            // Controle de intensidade do led vermelho]
            led_set_intensity(LED_RED, vrx_value, DEAD_ZONE_RED);
            // Controle de intensidade do led azul
            led_set_intensity(LED_BLUE, vry_value, DEAD_ZONE_BLUE);
        }



        sleep_ms(10);
    }
    return 0;
}
