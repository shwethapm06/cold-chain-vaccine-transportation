#define STM32F401xE
#include "stm32f401xe.h"
#include "lcd.h"
#include <stdint.h>
#include <stdio.h>

#define LCD_ADDR (0x27 << 1)
#define RS 0x01
#define EN 0x04
#define BL 0x08

void delay_ms(uint32_t ms) {
    SysTick->LOAD = 16000 - 1;  // Assuming 16 MHz clock
    SysTick->VAL = 0;
    SysTick->CTRL = 5;
    for (uint32_t i = 0; i < ms; i++)
        while (!(SysTick->CTRL & (1 << 16)));
    SysTick->CTRL = 0;
}

void I2C1_Init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    // PB8 (D15) - SCL, PB9 (D14) - SDA, Alternate Function 4
    GPIOB->MODER &= ~((3 << (8 * 2)) | (3 << (9 * 2))); // Clear mode
    GPIOB->MODER |=  (2 << (8 * 2)) | (2 << (9 * 2));   // Set to AF

    GPIOB->AFR[1] &= ~((0xF << (0 * 4)) | (0xF << (1 * 4)));
    GPIOB->AFR[1] |=  (4 << (0 * 4)) | (4 << (1 * 4));   // AF4

    GPIOB->OTYPER |= (1 << 8) | (1 << 9);               // Open-drain
    GPIOB->PUPDR &= ~((3 << (8 * 2)) | (3 << (9 * 2)));  // Clear PUPD
    GPIOB->PUPDR |=  (1 << (8 * 2)) | (1 << (9 * 2));    // Pull-up

    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
    I2C1->CR1 &= ~I2C_CR1_PE;
    I2C1->CR2 = 16;       // 16 MHz peripheral clock
    I2C1->CCR = 80;       // ~100 kHz
    I2C1->TRISE = 17;     // Max rise time
    I2C1->CR1 |= I2C_CR1_PE;
}

void I2C1_Start(void) {
    I2C1->CR1 |= I2C_CR1_START;
    while (!(I2C1->SR1 & I2C_SR1_SB));
}

void I2C1_Stop(void) {
    I2C1->CR1 |= I2C_CR1_STOP;
}

void I2C1_WriteAddr(uint8_t addr) {
    I2C1->DR = addr;
    while (!(I2C1->SR1 & I2C_SR1_ADDR));
    (void)I2C1->SR2;
}

void I2C1_Write(uint8_t data) {
    while (!(I2C1->SR1 & I2C_SR1_TXE));
    I2C1->DR = data;
    while (!(I2C1->SR1 & I2C_SR1_BTF));
}

void LCD_WriteI2C(uint8_t data) {
    I2C1_Start();
    I2C1_WriteAddr(LCD_ADDR);
    I2C1_Write(data | BL);
    I2C1_Write((data | EN) | BL);
    delay_ms(1);
    I2C1_Write((data & ~EN) | BL);
    I2C1_Stop();
}

void LCD_SendCmd(uint8_t cmd) {
    LCD_WriteI2C(cmd & 0xF0);//UPPER NIBBLE IS SENT
    LCD_WriteI2C((cmd << 4) & 0xF0);//LOWER IBBLE IS SENT
    delay_ms(2);
}

void LCD_SendData(uint8_t data) {
    LCD_WriteI2C((data & 0xF0) | RS);
    LCD_WriteI2C(((data << 4) & 0xF0) | RS);
    delay_ms(2);
}

void LCD_Init(void) {
    delay_ms(50);
    LCD_WriteI2C(0x30); delay_ms(5);
    LCD_WriteI2C(0x30); delay_ms(1);
    LCD_WriteI2C(0x30); delay_ms(10);
    LCD_WriteI2C(0x20); delay_ms(10); // 4-bit mode

    LCD_SendCmd(0x28); // Function set: 4-bit, 2 line, 5x8 dots
    LCD_SendCmd(0x0C); // Display ON
    LCD_SendCmd(0x01); // Clear display
    delay_ms(2);
    LCD_SendCmd(0x06); // Entry mode set
}

void LCD_GotoXY(uint8_t row, uint8_t col) {
    uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
    if (row > 3) row = 3;
    LCD_SendCmd(0x80 | (row_offsets[row] + col));
}

void LCD_Print(char *str) {
    while (*str) {
        LCD_SendData(*str++);
    }
}

void LCD_SendInt(int num)
{
	char buffer[20];
	sprintf(buffer,"%d",num);
	LCD_Print(buffer);
}

void LCD_Clear()
{
	LCD_SendCmd(0x01);
}





