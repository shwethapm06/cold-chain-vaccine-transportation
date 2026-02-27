#include "stm32f401xe.h"   // Device-specific header file for STM32F401xE
#include "lcd.h"           // LCD header for I2C-based LCD functions
#include <stdint.h>
#include "delay.h"
#include "keypad.h"// Delay functions
#include "dht11.h"
#include <stdio.h>
#include "button.h"

int corr=0;
int column=0;
int button=0;
int clear=0;





int main(void)
{

	RCC->AHB1ENR |=(1<<0);
	RCC->AHB1ENR |=(1<<1);

	GPIOA-> MODER |=(1<<26);
	GPIOA-> MODER &= ~(1<<27);

	GPIOA-> MODER |=(1<<28);
	GPIOA-> MODER &= ~(1<<29);

	GPIOA-> MODER |=(1<<30);
	GPIOA-> MODER &= ~(1<<31);

	GPIOB-> MODER |=(1<<8);
	GPIOB-> MODER &= ~(1<<9);

	GPIOA-> MODER |=(1<<24);
	GPIOA-> MODER &= ~(1<<25);

	 Keypad_GPIO_Init();   // Init keypad
	 I2C1_Init();
	 DHT11_Init();// Init I2C
	 LCD_Init();           // Init LCD
	 Init_button();
 	 LCD_Clear();
 	 LCD_GotoXY(0, 0);
	 LCD_Print("COLD CHAIN TRANSPORT");
	 LCD_GotoXY(1, 0);
	 LCD_Print("Enter Password:");
	 LCD_GotoXY(2, 0);
	 uint8_t temperature = 0, humidity = 0;
	 char buffer[16];

	 while(1)
	 {

			 corr = Keypad_Scan(0);
		 	 delay(200);
		 	 if(corr==1)
		 	 {
		 		 for(clear;clear<2;clear++)
		 		 {
		 		 LCD_Clear();
		 		 }
		 	 	 LCD_GotoXY(0, 0);
		 		 LCD_Print("COLD CHAIN TRANSPORT");
		 		if (DHT11_Read(&temperature, &humidity) == 0 &&  button == 0)
		 		        {
		 		            LCD_GotoXY(1, 0);
		 		            sprintf(buffer, "Temp: %d%cC", temperature, 0xDF);
		 		            LCD_Print(buffer);
		 		            LCD_GotoXY(2, 0);
		 		            sprintf(buffer, "Humidity: %d%%", humidity);
		 		            LCD_Print(buffer);
		 		            GPIOA -> ODR |=(1<<14);
		 		            if(humidity>=80)
		 		            {
		 		               LCD_GotoXY(3, 0);
			 		            LCD_Print("EMERGENCY");
		 						GPIOA -> ODR |=(1<<13);
		 	      				GPIOA -> ODR |=(1<<12);
		 	      				GPIOA -> ODR &=~(1<<14);
		 						for(int i=0;i<20;i++)
		 						{
		 							int k=i*5;
		 							GPIOB -> ODR |=(1<<4);
		 							delay(k);
		 							GPIOB -> ODR &=~(1<<4);
		 							delay(k);
		 						}
		 						GPIOA -> ODR &=~(1<<13);
		 	      				GPIOA -> ODR &=~(1<<12);
		 		            }
	 	      				LCD_GotoXY(3, 0);
	 	      				LCD_Print("           ");
		 		            button = Read_button();
		 		        }

		 		else if(button==1)
		 		{
		 			while(button==1)
		 			{
						 LCD_Clear();
						 LCD_GotoXY(0, 0);
						 LCD_Print("OPTIONS");
						 LCD_GotoXY(1, 0);
						 LCD_Print("OPTION 1-POLIO");
						 LCD_GotoXY(2, 0);
						 LCD_Print("OPTION 2-RABIES");
						 LCD_GotoXY(4, 0);
						 LCD_Print("OPTION 3-COVAX");
						 delay(2000);
						 button=Read_button();
						while(button==0)
						{
							corr=Keypad_Scan(1);
							button=Read_button();
						}
						if (corr==4)
						{
							while(button==1)
							{
								button=0;
							}
						}
		 			}
		 			corr=0;
		 			LCD_Clear();
		 			LCD_GotoXY(0, 0);
		 			LCD_Print("COLD CHAIN TRANSPORT");
				    LCD_GotoXY(1,0);
				    LCD_Print("Enter Password:");
				    LCD_GotoXY(2, 0);
				    clear=0;
		 		}


//		 		for (volatile int i = 0; i < 1000000; i++); // simple delay
		 	 }
		 	 else if(corr==2)
		 	 {
		 		corr=0;

		 		LCD_GotoXY(2, 0);
		 		LCD_Print("     ");
		 		LCD_GotoXY(3, 0);
		 		LCD_Print("                ");
		 		LCD_GotoXY(2, 0);
		 	 }
	 }









}
