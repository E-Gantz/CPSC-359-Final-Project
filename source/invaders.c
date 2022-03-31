#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "globals.h"
#include "invaders.h"
#include "projectiles.h"

#define PAWNX 12
#define PAWNY 8
#define PAWNGAP 3

#define KNIGHTX 11
#define KNIGHTY 8
#define KNIGHTPAD 1
#define KNIGHTGAP 4

#define QUEEN 8
#define QUEENPAD 2
#define QUEENGAP 7

#define HEIGHT 8

#define FBY INVADERHEIGHT*SIZE
#define FBX INVADERWIDTH*SIZE

#define VERTICALGAP 3
#define SHOOTCHANCE 42
#define ENEMYSHOTDELAY 6
#define ENEMYMOVEDELAY 25
#define DELAYINCREMENT 2
#define INVADERXMOVE 15
#define SPEEDUPEVERY 5
#define INVADERYMOVE 11

bool pawn1[PAWNY][PAWNX] =
{
	{0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0},
	{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0},
	{0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0},
	{0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0}
};

bool pawn2[PAWNY][PAWNX] =
{
	{0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0},
	{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0},
	{0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0},
	{1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1}
};

bool knight1[KNIGHTY][KNIGHTX] =
{
	{0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0},
	{0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
	{0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0},
	{0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1},
	{1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1},
	{0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0}
};

bool knight2[KNIGHTY][KNIGHTX] =
{
	{0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0},
	{1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1},
	{1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1},
	{1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
	{0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0},
	{0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0}
};

bool queen1[QUEEN][QUEEN] =
{
	{0, 0, 0, 1, 1, 0, 0, 0},
	{0, 0, 1, 1, 1, 1, 0, 0},
	{0, 1, 1, 1, 1, 1, 1, 0},
	{1, 1, 0, 1, 1, 0, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1},
	{0, 0, 1, 0, 0, 1, 0, 0},
	{0, 1, 0, 1, 1, 0, 1, 0},
	{1, 0, 1, 0, 0, 1, 0, 1}
};

bool queen2[QUEEN][QUEEN] =
{
	{0, 0, 0, 1, 1, 0, 0, 0},
	{0, 0, 1, 1, 1, 1, 0, 0},
	{0, 1, 1, 1, 1, 1, 1, 0},
	{1, 1, 0, 1, 1, 0, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1},
	{0, 0, 1, 0, 0, 1, 0, 0},
	{0, 1, 0, 0, 0, 0, 1, 0},
	{0, 0, 1, 0, 0, 1, 0, 0}
};

int nextShots[ARRAYX];
void writePawn(int x, int y, int id);
void writeKnight(int x, int y, int id);
void writeQueen(int x, int y, int id);

int parity = 0;
int nextSpeedUp;
int moveDelay;
int nextMove;
void writeInvaders(int x0, int y0)
{
	int x = QUEENPAD*SIZE;
	int y = 0;
	for(int i = 0; i < ARRAYY; i++)
	{
		for(int j = 0; j < ARRAYX; j++)
		{
			switch (i)
			{
				case 0:
				if(state.aliveInvaders[i][j]) writeQueen(x0+x, y0+y, (i<<4) + j);
				x += QUEEN*SIZE + QUEENGAP*SIZE;
				break;

				case 1:
				case 2:
				if(state.aliveInvaders[i][j]) writeKnight(x0+x, y0+y, (i<<4) + j);
				x += KNIGHTX*SIZE + KNIGHTGAP*SIZE;
				break;

				case 3:
				case 4:
				if(state.aliveInvaders[i][j]) writePawn(x0+x, y0+y, (i<<4) + j);
				x += PAWNX*SIZE + PAWNGAP*SIZE;
				break;
			}
		}
		switch (i)
		{
			case 0:
			case 1:
			x = KNIGHTPAD*SIZE/2;
			break;
			
			default:
			x = 0;
		}
		y = (HEIGHT*SIZE + VERTICALGAP*SIZE)*(i+1);
	}
}

void initInvaders()
{
	state.invaderX = 0;
	state.invaderY = 34*SIZE;
	memset(state.aliveInvaders, 1, sizeof(state.aliveInvaders));
	state.numAliveInvaders = ARRAYX * ARRAYY;
	moveDelay = ENEMYMOVEDELAY;
	nextSpeedUp = state.numAliveInvaders - SPEEDUPEVERY;
	for (int i = 0; i < ARRAYX; i++) 
	{
		nextShots[i] = state.tick;
		state.frontline[i] = ARRAYY-1;
	}
	nextMove = state.tick;
}

void writePawn(int x, int y, int id)
{
	bool bit = 0;
	for(int py = 0; py < PAWNY; py++)
	{
		for(int px = 0; px < PAWNX; px++)
		{
			if (parity) bit = pawn1[py][px];
			else bit = pawn2[py][px];
			if (bit) writeBit(x, y, ENEMYID + id);
			x += SIZE;
		}
		if (py != PAWNY - 1)
		{
			x -= PAWNX*SIZE;
			y += SIZE;
		}
	}
}

void writeKnight(int x, int y, int id)
{
	bool bit = 0;
	for(int ky = 0; ky < KNIGHTY; ky++)
	{
		for(int kx = 0; kx < KNIGHTX; kx++)
		{
			if (parity) bit = knight1[ky][kx];
			else bit = knight2[ky][kx];
			if (bit) writeBit(x, y, ENEMYID + id);
			x += SIZE;
		}
		if (ky != KNIGHTY - 1)
		{
			x -= KNIGHTX*SIZE;
			y += SIZE;
		}
	}
}

void writeQueen(int x, int y, int id)
{
	bool bit = 0;
	for(int qy = 0; qy < QUEEN; qy++)
	{
		for(int qx = 0; qx < QUEEN; qx++)
		{
			if(parity) bit = queen1[qy][qx];
			else bit = queen2[qy][qx];
			if (bit) writeBit(x, y, ENEMYID + id);
			x += SIZE;
		}
		if (qy != QUEEN - 1)
		{
			x -= QUEEN*SIZE;
			y += SIZE;
		}
	}
}
void killInvader(int x, int y)
{
	state.aliveInvaders[y][x] = 0;
	state.numAliveInvaders--;
	if (state.numAliveInvaders == 0) state.win = 1;
	if (state.numAliveInvaders <= nextSpeedUp)
	{
		if (moveDelay > 0) moveDelay -= DELAYINCREMENT;
		nextSpeedUp = state.numAliveInvaders - SPEEDUPEVERY;
	}
	switch(y)
	{
		case 0:
		state.score += 30;
		break;
		case 1:
		case 2:
		state.score += 20;
		break;
		case 3:
		case 4:
		state.score += 10;
		break;
	}
	while(state.aliveInvaders[state.frontline[x]][x] == 0) state.frontline[x]--; //check if current frontline is dead; set frontline to previous
}

void invadersShoot()
{
	for(int i = 0; i < ARRAYX; i++)
		if ((rand() % SHOOTCHANCE == 0) & (state.frontline[i] >= 0) &
			(nextShots[i] < state.tick))
		{
			int x = state.invaderX + i*(PAWNGAP+PAWNX)*SIZE + (PAWNX - PROJWIDTH)*SIZE/2;
			int y = state.invaderY + (state.frontline[i]+1)*(HEIGHT+VERTICALGAP)*SIZE;
			newProjectile(2, x, y);
			nextShots[i] = state.tick + ENEMYSHOTDELAY;
		}
}

void moveInvaders()
{
	if (nextMove > state.tick) return;
	int invaderMovement = state.invaderDirection;
	if (((invaderMovement == 0) & (state.invaderX - INVADERXMOVE*SIZE < 0)) |
		((invaderMovement == 1) & (state.invaderX + INVADERXMOVE*SIZE + INVADERWIDTH*SIZE > SCREENWIDTH)))
			invaderMovement = 2;
	if(invaderMovement == 1) state.invaderX += INVADERXMOVE*SIZE;
	else if(invaderMovement == 0) state.invaderX -= INVADERXMOVE*SIZE;
	else if(invaderMovement == 2) {
		state.invaderDirection = !state.invaderDirection;
		if (state.invaderY + INVADERYMOVE*SIZE < SCREENHEIGHT - INVADERBOT*SIZE) state.invaderY += INVADERYMOVE*SIZE;
		else state.lose = 1;
	}
	parity = (parity + 1) % 2;
	nextMove = state.tick + moveDelay;
}
