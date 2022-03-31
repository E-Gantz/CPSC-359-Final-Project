#include <stdio.h>
#include <string.h>
#include "projectiles.h"
#include "invaders.h"
#include "lives.h"
#include "shield.h"
#include "globals.h"

bool projectile[PROJHEIGHT][PROJWIDTH] = 
{
		{1},
		{1},
		{1},
		{1}
};

void writeProjectiles()
{
	for (int p = 0; p < MAXPROJECTILES; p++) if(state.projectilesDir[p] != 0){
		int collision = 0;
		int y0 = state.projectilesY[p];
		int x0 = state.projectilesX[p];
		for (int y = 0; y < PROJHEIGHT; y++)
			for (int x = 0; x < PROJWIDTH; x++){
				int tmp = checkBit(x0+x*SIZE, y0+y*SIZE);
				if(tmp > 0)												// collidable objects should have positive ID's in fbArray
					collision = tmp;
				else if (projectile[y][x]) writeBit(x0+x*SIZE, y0+y*SIZE, PROJID);
			}
		state.projectilesCol[p] = collision;
	}
}

void newProjectile(int dir, int x0, int y0)									// generate new projectile at coordinates. dir = 1 for up (player projectiles), 2 for down (enemy projectiles)
{
	if (state.emptyProj == MAXPROJECTILES) return;							// ignore newProjectile call if there are already too many projectiles onscreen
	state.projectilesDir[state.emptyProj] = dir;
	state.projectilesX[state.emptyProj] = x0;
	state.projectilesY[state.emptyProj] = y0;
	while((state.projectilesDir[state.emptyProj] != 0) &
		(state.emptyProj < MAXPROJECTILES))
			state.emptyProj++;												//find next empty index of projectiles array
}

void moveProjectiles()
{
	for (int p = 0; p < MAXPROJECTILES; p++)
	{
		switch(state.projectilesDir[p])
		{
			case(0): break;
			case(1):
				if (state.projectilesY[p] - PROJSPEED > 0)
					state.projectilesY[p] -= PROJSPEED;
				else removeProjectile(p);
				break;
			case(2):
				if (state.projectilesY[p] + PROJSPEED + PROJHEIGHT*SIZE < SCREENHEIGHT)
					state.projectilesY[p] += PROJSPEED;
				else removeProjectile(p);
				break;
		}
	}
}

void checkCollisions()														// process any projectile collisions
{
	for (int p = 0; p < MAXPROJECTILES; p++) if (state.projectilesCol[p] != 0)
	{
		int hitID = state.projectilesCol[p];
		switch(hitID>>8)
		{
			case 3:
			case 4:
			case 5:
				//enemy hit
				if (state.projectilesDir[p] == 1)
				{
					killInvader((hitID & 0xf), (hitID & 0xf0) >> 4);
					removeProjectile(p);
				}
				break;
			case 2:
				hitShield(0xf & hitID);
				removeProjectile(p);
				break;
			case 1:
				if (state.projectilesDir[p] == 2){
					reduceLives();
					removeProjectile(p);
				}
				
		}
		state.projectilesCol[p] = 0;
	}
}

void removeProjectile(int p)												// remove projectile from play by setting projectilesDir to 0
{
	state.projectilesDir[p] = 0;
	if (p < state.emptyProj) state.emptyProj = p; //replace current emptyProj if lower. note that all indices lower than emptyProj will always be filled so newProjectile() only needs to look higher
}

void removeAllProjectiles()
{
	memset(state.projectilesDir, 0, sizeof(state.projectilesDir));
}
