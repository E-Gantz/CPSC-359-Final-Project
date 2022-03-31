#include "globals.h"

void writeBit(int x, int y, int bit)
{
	for(int i = 0; i < SIZE; i++)
	{
		for(int j = 0; j < SIZE; j++)
		{
			fbArray[y+i][x+j] = bit;
		}
	}
}

int checkBit(int x, int y)
{
	for(int i = 0; i < SIZE; i++)
	{
		for(int j = 0; j < SIZE; j++)
		{
			if(fbArray[y+i][x+j] != 0) return fbArray[y+i][x+j];
		}
	}
	return 0;
}
