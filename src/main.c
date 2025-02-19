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
#define DEAD_ZONE 200
#define CENTER_JS 2048

// Configurações da I2C
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

#define SPEED_SCALE 5
// Posição inicial do quadrado
int pos_x = 60;
int pos_y = 28;

// Função para inicializar o PWM
uint pwm_init_gpio(uint gpio, uint wrap) {
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    pwm_set_wrap(slice_num, wrap);
    pwm_set_enabled(slice_num, false);
    return slice_num;
}


void led_set_intensity (uint8_t ledGPIO, uint16_t axis) {
    int16_t diff = axis - CENTER_JS;
    diff = abs(diff);
    // Verifica se o joystick está no centro
    diff < DEAD_ZONE    ? pwm_set_gpio_level(ledGPIO, 0)
                        : pwm_set_gpio_level(ledGPIO, diff * 2);
}


uint16_t joystick_read_value(uint8_t jsGPIO) {
    adc_select_input(jsGPIO - 26);
    uint16_t axis_value = adc_read();
    return axis_value;
}



void update_position() {
    // Ler valores do ADC
    adc_select_input(0);
    int vrx = adc_read(); // Eixo X

    adc_select_input(1);
    int vry = adc_read(); // Eixo Y

    // Calcular deslocamento relativo ao centro do joystick (2048)
    int dx = (vrx - 2048) * -1;
    int dy = (vry - 2048) * -1;
    // Aplica deadzone
    if (abs(dx) < DEAD_ZONE) dx = 0;
    if (abs(dy) < DEAD_ZONE) dy = 0;

    // Normaliza deslocamento (reduzir a velocidade)
    dx /= SPEED_SCALE;
    dy /= SPEED_SCALE;

    // Atualizar posição do quadrado
    pos_x += dx;
    pos_y += dy;

    // Limitar dentro da tela
    if (pos_x < 0) pos_x = 0;
    if (pos_x > 120) pos_x = 120;
    if (pos_y < 0) pos_y = 0;
    if (pos_y > 56) pos_y = 56;
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

    ssd1306_rect(&ssd, 28, 60, 8, 8, true, true);
    ssd1306_send_data(&ssd);

    // Inicializa botões com interrupção
    init_button_with_interrupt(JOYSTICK_BUTTON, GPIO_IRQ_EDGE_FALL, true);
    init_button_with_interrupt(BUTTON_A, GPIO_IRQ_EDGE_FALL, true);
    init_button_with_interrupt(BUTTON_B, GPIO_IRQ_EDGE_FALL, true);

    // Inicia o LED verde
    init_led(LED_GREEN);

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
            led_set_intensity(LED_RED, vrx_value);
            // Controle de intensidade do led azul
            led_set_intensity(LED_BLUE, vry_value);
        }
        
        int pos_x = map_value(vrx_value, 0, 4095, 0, 120); // X varia de 0 a 120 pixels
        int pos_y = map_value(vry_value, 4095, 0, 0, 56);  // Y varia de 0 a 56 pixels
        
        update_position();
        ssd1306_fill(&ssd, false); // Limpa a tela
        ssd1306_rect(&ssd, pos_y, pos_x, 8, 8, true, true); // Desenha o quadrado
        ssd1306_send_data(&ssd);


        sleep_ms(10);
    }
    return 0;
}
