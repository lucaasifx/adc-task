# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================



cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD pico_w CACHE STRING "Board type")
include(pico_sdk_import.cmake)
project(adc-task C CXX ASM)
pico_sdk_init()


# Add executable. Default name is the project name, version 0.1
# Inclui todos os arquivos .c da pasta src
file(GLOB SOURCES "src/*.c")
# Inclui todos os arquivos .c da pasta src/lib
file(GLOB LIB_SOURCES "src/lib/*.c")
# Combina todos os arquivos em um único executável
add_executable(adc-task ${SOURCES} ${LIB_SOURCES})

# Para permitir depuração
pico_enable_stdio_uart(adc-task 1)  # Habilita UART
pico_enable_stdio_usb(adc-task 1)   # Habilita USB

target_link_libraries(adc-task PRIVATE 
    pico_stdlib #lib padrao
    pico_time
    hardware_clocks # para eventos de temporização
    hardware_uart   # para eventual debug
    hardware_i2c    # habilita o uso do display
    hardware_adc    # habilita o uso da biblioteca adc
    hardware_pwm
)
pico_add_extra_outputs(adc-task) 



