#include "stm32f401xe.h"
#include "keypad.h"
#include "delay.h"
#include "lcd.h"
#include <stdint.h>
#include <stdio.h>

#define PASSWORD 1234

int entered = 0;   // stores number being entered
int count = 0;     // number of digits entered
int correct = 0;//for the return of correctness of password


void Keypad_GPIO_Init(void)
{
    // Enable clock for GPIOA and GPIOB
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN;

    // === Configure PA4–PA7 as Output ===
    GPIOA->MODER &= ~(0xFF << (2 * 4));         // Clear MODER8–15
    GPIOA->MODER |=  (0x55 << (2 * 4));         // Set PA4–PA7 to output
    GPIOA->OTYPER &= ~(0xF << 4);               // Push-pull
    GPIOA->OSPEEDR &= ~(0xFF << (2 * 4));
    GPIOA->OSPEEDR |=  (0xFF << (2 * 4));       // High speed
    GPIOA->PUPDR &= ~(0xFF << (2 * 4));         // No pull-up/down

    // === Configure PA0–PA1 as read with Pull-Up ===
    GPIOA->MODER &= ~(0xF << (2 * 0));
    GPIOA->PUPDR &= ~(0xF << (2 * 0));
    GPIOA->PUPDR |=  (0x5 << (2 * 0));

    // === Configure PB0–PB1 as read with Pull-Up ===
    GPIOB->MODER &= ~(0xF << (2 * 0));
    GPIOB->PUPDR &= ~(0xF << (2 * 0));
    GPIOB->PUPDR |=  (0x5 << (2 * 0));
}

void Handle_Key(int digit)
{
	correct=0;
    if (digit >= 0)
    {
        entered = entered * 10 + digit;
        count++;
    }

    //Check password when 4 digits entered
    if (count == 4) {
        LCD_GotoXY(3, 0);
        if (entered == PASSWORD) {
            LCD_Print("Access Granted ");
            correct=1;
        } else {
            LCD_Print("Access Denied  ");
            correct=2;
            delay(1000);

        }


        // Reset for next attempt
        entered = 0;
        count = 0;
//        delay(1000);
//        LCD_Clear();   // Clear LCD
//        LCD_GotoXY(0,0);
//        LCD_Print("Enter Password:");
//        LCD_GotoXY(1,0);
    }
}


int Keypad_Scan(int read)
{
    for (int row = 0; row < 4; row++)
    {
        GPIOA->ODR |= (0xF << 4);             // Set all rows HIGH
        GPIOA->ODR &= ~(1 << (row + 4));      // Pull current row LOW

        for (volatile int d = 0; d < 10000; d++); // Small delay

        // ---- Check PA0, PA1 ----
        for (int col = 0; col < 2; col++)
        {
            if (!(GPIOA->IDR & (1 << col)))
            {
                while (!(GPIOA->IDR & (1 << col)));  // Wait release

                int digit = -1;
                if (row == 0 && col == 0) {
                	digit = 1;
                	if(read==0)
                	LCD_Print("1");
                	else if(read==1)
                	{
         		 		 LCD_Clear();
						 LCD_GotoXY(0, 0);
						 LCD_Print("STOCK DETAILS");
          		 	 	 LCD_GotoXY(1, 0);
          		 		 LCD_Print("NAME:POLIO Vaccine");
          		 	 	 LCD_GotoXY(2, 0);
          		 		 LCD_Print("EXP DATE:2-09-30");
          		 		 LCD_GotoXY(3, 0);
          		 		 LCD_Print("QUANTITY:100K");
                	}
                }

                else if (row == 0 && col == 1)
                	{
                		digit = 2;
                		if(read==0)
                		LCD_Print("2");
                		else if(read==1)
                    	{
           		 		 LCD_Clear();
						 LCD_GotoXY(0, 0);
						 LCD_Print("STOCK DETAILS");
           		 	 	 LCD_GotoXY(1, 0);
           		 		 LCD_Print("NAME:Rabies Vaccine");
           		 	 	 LCD_GotoXY(2, 0);
           		 		 LCD_Print("EXP DATE:12-02-28");
           		 		 LCD_GotoXY(3, 0);
           		 		 LCD_Print("QUANTITY:10K");
                    	}

                	}
                else if (row == 1 && col == 0)
                {
                       digit = 4;
                       if(read==0)
                       LCD_Print("4");
                }
                else if (row == 1 && col == 1)
                	{
                		digit = 5;
                		if(read==0)
                		LCD_Print("5");
                	}
                else if (row == 2 && col == 0)
                	{
                		digit = 7;
                		if(read==0)
                		LCD_Print("7");
                	}
                else if (row == 2 && col == 1)
                	{
                		digit = 8;
                		if(read==0)
                		LCD_Print("8");
                	}
                else if (row == 3 && col == 1)
                	{
                		digit = 0;
                		if(read==0)
                		LCD_Print("0");
                	}
                else if (row == 3 && col == 0)
                	{
                		digit = 0;
                		LCD_Print(" ");
                		if(read==1)
                		{
                			correct=4;
                		}

                	}
                if(read==0)
                {
                	Handle_Key(digit);
                }
            }
        }

        for (int col = 2; col < 4; col++)
        {
            int pin = col - 2;
            if (!(GPIOB->IDR & (1 << pin)))
            {
                while (!(GPIOB->IDR & (1 << pin)));  // Wait release

                int digit = -1;
                if (row == 0 && col == 2)
                	{
                		digit = 3;
                		if(read==0)
                		LCD_Print("3");
                		else if(read==1)
                    	{
             		 		 LCD_Clear();
             		 		 LCD_GotoXY(0, 0);
             		 		 LCD_Print("STOCK DETAILS");
              		 	 	 LCD_GotoXY(1, 0);
              		 		 LCD_Print("NAME:co-Vaccine");
              		 	 	 LCD_GotoXY(2, 0);
              		 		 LCD_Print("EXP DATE:22-12-28");
              		 		 LCD_GotoXY(3, 0);
              		 		 LCD_Print("QUANTITY:50K");
                    	}

                	}
                else if (row == 1 && col == 2)
                	{
                		digit = 6;
                		if(read==0)
                		LCD_Print("6");
                	}
                else if (row == 2 && col == 2)
                	{
                		digit = 9;
                		if(read==0)
                			LCD_Print("9");
                	}
                else if (row == 0 && col == 3)
                	{
                		digit = 0;
                		LCD_Print("A");
                	}
				   else if (row == 1 && col == 3)
					   {
					   	   digit = 0;
					   	   LCD_Print("B");
					   }
				   else if (row == 2 && col == 3)
					   {
					   	   LCD_Print("C");
					   	   	digit = 0;
					   }
				   else if (row == 3 && col == 2)
					   {
					   	   LCD_Print("#");
					   	   digit = 0;
					   }
				   else if (row == 3 && col == 3)
					   {
					   	   LCD_Print("D");
				 			LCD_Clear();
				 			LCD_GotoXY(0, 0);
				 			LCD_Print("COLD CHAIN TRANSPORT");
						    LCD_GotoXY(1,0);
						    LCD_Print("Enter Password:");
						    LCD_GotoXY(2, 0);

					   }
                if(read==0)
                {
                	Handle_Key(digit);
                }
            }
        }
    }
    return correct;
}


