#include <stdio.h>
#include "player.h"
#include "globals.h"
#include "projectiles.h"
#include "lives.h"
#define PLAYERSHOTDELAY 2;

bool player[PLAYERHEIGHT][PLAYERWIDTH] =
{
	{0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0},
	{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};
int nextShot;
void writePlayer(int x0, int y0)
{
	for (int y = 0; y < PLAYERHEIGHT; y++)
		for (int x = 0; x < PLAYERWIDTH; x++)
			if (player[y][x]) writeBit(x0+x*SIZE, y0+y*SIZE, PLAYERID);
}

void playerShoot()
{
	if(state.tick > nextShot)
	{
		newProjectile(1, state.playerX + (PLAYERWIDTH-PROJWIDTH)*SIZE/2, state.playerY);
		nextShot = state.tick + PLAYERSHOTDELAY;
	}
}

void initPlayer()
{
	state.playerY = SCREENHEIGHT - PLAYERHEIGHT*SIZE - HEARTYHEIGHT*SIZE - HEARTYGAP*SIZE*2;
	state.playerX = (SCREENWIDTH - PLAYERWIDTH*SIZE)/2;
	nextShot = state.tick;
}

void playerMoveLeft()
{
	if (state.playerX - PLAYERSPEED*SIZE > 0)state.playerX -= PLAYERSPEED*SIZE;
}

void playerMoveRight()
{
	if (state.playerX + PLAYERSPEED*SIZE + PLAYERWIDTH*SIZE < SCREENWIDTH) state.playerX += PLAYERSPEED*SIZE;
}
