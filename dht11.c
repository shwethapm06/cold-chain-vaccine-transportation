#include "stm32f401xe.h"
#include "dht11.h"
#include <stdint.h>

#define DHT_PORT GPIOB
#define DHT_PIN 5

static void delay_us(uint32_t us) {
    SysTick->LOAD = (us * 16) - 1; // 16MHz = 1us per 16 cycles
    SysTick->VAL = 0;
    SysTick->CTRL = 5;
    while ((SysTick->CTRL & 0x10000) == 0);
    SysTick->CTRL = 0;
}

static void DHT_SetPinOutput(void) {
    DHT_PORT->MODER &= ~(3 << (DHT_PIN * 2));
    DHT_PORT->MODER |= (1 << (DHT_PIN * 2));
}

static void DHT_SetPinInput(void) {
    DHT_PORT->MODER &= ~(3 << (DHT_PIN * 2));
}

static void DHT_Write(uint8_t value) {
    if (value)
        DHT_PORT->ODR |= (1 << DHT_PIN);
    else
        DHT_PORT->ODR &= ~(1 << DHT_PIN);
}

static uint8_t DHT_ReadPin(void) {
    return (DHT_PORT->IDR & (1 << DHT_PIN)) != 0;
}

void DHT11_Init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    DHT_SetPinOutput();
    DHT_Write(1);
}

int DHT11_Read(uint8_t *temperature, uint8_t *humidity) {
    uint8_t data[5] = {0};

    DHT_SetPinOutput();
    DHT_Write(0);
    delay_us(18000);
    DHT_Write(1);
    delay_us(30);
    DHT_SetPinInput();

    if (DHT_ReadPin()) return 1;
    while (!DHT_ReadPin());
    while (DHT_ReadPin());

    for (int i = 0; i < 40; i++) {
        while (!DHT_ReadPin());
        delay_us(40);
        if (DHT_ReadPin())
            data[i / 8] |= (1 << (7 - (i % 8)));
        while (DHT_ReadPin());
    }

    if ((data[0] + data[1] + data[2] + data[3]) != data[4]) return 2;

    *humidity = data[0];
    *temperature = data[2];
    return 0;
}
