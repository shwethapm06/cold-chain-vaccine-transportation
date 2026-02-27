
#include"delay.h"

void delay(int num)
{
	int count1;
	int count2;
	for(count1=1;count1<=num;count1++)
		for(count2=1;count2<=num;count2++);
}
