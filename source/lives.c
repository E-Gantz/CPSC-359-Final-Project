#include <stdio.h>
#include <unistd.h>
#include "lives.h"
#include "globals.h"

bool heart[HEARTYHEIGHT][HEARTXWIDTH] =
{
	{0, 1, 1, 0, 1, 1, 0},
	{1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1},
	{0, 1, 1, 1, 1, 1, 0},
	{0, 0, 1, 1, 1, 0, 0},
	{0, 0, 0, 1, 0, 0, 0},
};

void initLives()
{
	state.lives = STARTINGLIVES;
}

void writeLives()
{
	int xOffset = HEARTXGAP;
	int yOffset = SCREENHEIGHT-HEARTYHEIGHT*SIZE-HEARTYGAP;
	for (int l = 0; l < state.lives; l++)
	{
		for (int y = 0; y < HEARTYHEIGHT; y++)
			for (int x = 0; x < HEARTXWIDTH; x++)
				if (heart[y][x]) writeBit(xOffset + x*SIZE, yOffset+y*SIZE, HEARTID);
		xOffset += HEARTXGAP + HEARTXWIDTH*SIZE;
	}
}

void reduceLives()
{
	if (state.lives > 0) state.lives--;
	else state.lose = 1;
}
