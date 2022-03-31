#include <unistd.h>
#include <stdio.h>
#include "GPIO_INIT.h"
#include <wiringPi.h>
#include "SNESdriver.h"
unsigned int *gpio;
void init_GPIO(int lineNumber, int functionCode);
void write_Latch(int writeBit);
void write_Clock(int writeBit);
int read_Data();
void initGpio();

void initGpio (){
	gpio = gpioPtr();
	
	init_GPIO(9, 1);	//set latch to output
	init_GPIO(11, 1);	//set clock to output
	init_GPIO(10, 0);	//set data to input
}

void init_GPIO(int lineNumber, int functionCode) {
		int x = lineNumber/10;	//x = function select register offset
		int y = (lineNumber%10)*3;	//y = line number 0-9
		gpio[x] = (gpio[x] & ~(7<<(y))) | ((functionCode)<<(y));
	}

void write_Latch(int writeBit){
	if (writeBit == 1){
		gpio[7] = 1<<9;	//set latch
		}
	else {
		gpio[10] = 1<<9;	//clear latch
		}
	}

void write_Clock(int writeBit){
	if (writeBit == 1){
		gpio[7] = 1<<11;	//set clock
		}
	else {
		gpio[10] = 1<<11;	//clear clock
		}
	}

int read_Data(){
	int x;
	x = (gpio[13] >> 10) & 1;	//x has 0 or 1 now
	
	return x;
	}

void getInput(int *buttonArray, int size){
	write_Clock(1);
	write_Latch(1);	//set latch line high for 12 microseconds
	delayMicroseconds(12);
	write_Latch(0);
	for (int i = 0; i<size; i++) {
		delayMicroseconds(6);
		write_Clock(0);	//set clock low for 6 microseconds
		delayMicroseconds(6);
		buttonArray[i] = read_Data();
		write_Clock(1);
		}
}
