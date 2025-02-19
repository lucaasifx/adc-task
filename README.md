# 📌 Controle de LEDs RGB e Display SSD1306 com Joystick no RP2040

# 📖 Descrição do Projeto
Este projeto tem como objetivo explorar o uso do conversor analógico-digital (ADC) no RP2040 para capturar os valores do joystick e controlar a intensidade luminosa de dois LEDs RGB via PWM, além de exibir a posição do joystick em um display SSD1306.

# 🎯 Objetivos do Projeto

- Compreender o funcionamento do conversor analógico-digital (ADC) no RP2040.

- Controlar a intensidade de dois LEDs RGB utilizando PWM baseado nos valores do joystick.

- Representar a posição do joystick em um display SSD1306 através de um quadrado móvel.

- Implementar comunicação via protocolo I2C para o display.

- Utilizar interrupções para o acionamento de botões e aplicar técnicas de debouncing.


# 🖥️ Requisitos

- Placa de desenvolvimento BitDogLab
- Joystick analógico
- LED RGB
- Push Bottons A e B
- Display OLED SSD1306 (128x64)
- VS Code com extensões para desenvolvimento no Raspberry Pi Pico
- ```**SDK do Raspberry Pi Pico versão 2.1.0**```
- CMake para compilação do projeto

# 🔧 Funcionalidades Implementadas

- 1️⃣ Controle de LEDs RGB via Joystick
  - ✅ O LED Azul tem sua intensidade ajustada com base no eixo Y do joystick.
  - ✅ O LED Vermelho tem sua intensidade ajustada com base no eixo X do joystick.
  - ✅ O LED Verde alterna seu estado a cada acionamento do botão do joystick.
  - ✅ Os LEDs são controlados via PWM para permitir variação suave da intensidade.
- 2️⃣ Exibição da Posição do Joystick no Display SSD1306
  - ✅ Um quadrado de 8x8 pixels representa a posição do joystick na tela.
  - ✅ A posição do quadrado é ajustada proporcionalmente aos valores capturados do ADC.
  - ✅ O botão do joystick altera o estilo da borda do display ao ser pressionado.

- 3️⃣ Controle Global dos LEDs
  - ✅ O botão "A" ativa ou desativa os LEDs RGB via PWM a cada acionamento.
- 4️⃣ Implementação de Interrupções e Debouncing
  - ✅ Todas as funcionalidades relacionadas aos botões utilizam rotinas de interrupção (IRQ).
  - ✅ O tratamento de bouncing dos botões foi implementado via software.
 
# 🚀 Como Executar o Projeto
1. Clone este repositório:
```bash
   https://github.com/lucaasifx/adc-task.git
```
2. Abra o projeto no VS Code.

3. Certifique-se de que as extensões Raspberry Pi Pico estão instaladas.

4. Importe o projeto utilizando a extensão da Raspberry Pi pico
    - **Certifique-se de possuir o sdk 2.1.0 instalado**
5. Clique em Compilar.
6. Carregue o software na placa BitDogLab.

# 🎥 Demonstração

- 📌 Um vídeo de demonstração foi gravado, apresentando:
  - ✅ Explicação das funcionalidades.
  - ✅ Execução do código na placa BitDogLab.
  - 🔗 [Acesse o vídeo de demonstração](https://www.youtube.com/watch?v=vWzc-5qkFN0)
# #
