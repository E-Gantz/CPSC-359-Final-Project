#include <stdlib.h>
#include <stdio.h>
#include <wiringPi.h>
#include <unistd.h>
#include <string.h>
#include "framebuffer.h"
#include "invaders.h"
#include "player.h"
#include "SNESdriver.h"
#include "globals.h"
#include "projectiles.h"
#include "lives.h"
#include "title.h"
#include "shield.h"
#include "win.h"
#include "lose.h"
#include "score.h"
#include "pause.h"



struct fbs framebufferstruct;

void updateHighScore();
void initState();
void writeFB();
void drawFB();
void clearScreen();
int play();

int main()
{
	framebufferstruct = initFbInfo();
	initGpio();
	
	int buttonArray[16];

	int option = 1;
	state.tick = 0;
	while(1)
	{
		getInput(buttonArray, 16);
		if ((buttonArray[5] == 0 )&&option == 1) {printf("you selected exit.\n"); option = 0;}
		else if ((buttonArray[4] == 0) && option == 0) {printf("you have selected Start.\n"); option = 1;}
		else if ((buttonArray[8] == 0) && option == 1) {printf("starting the game.\n"); delay(DELAY); break;}
		else if ((buttonArray[8] == 0) && option == 0) {printf("exiting the game.\n"); return 0;}
		
		if(state.tick < 5)
		{
			 writeTitle(option);
			 drawFB();
		}
		else
		{
			writeTitle(2);
			drawFB();
		}
		state.tick++;
		state.tick = state.tick % 10;
		delay(DELAY);	//100ms is about how fast someone can spam inputs, so we can wait that long in between inputs.
	}	//when start is selected a version of the main menu with start highlited should be displayed, when exit is selected a different version with exit highlited should be displayed.
	
	int newGame = 1;
	while(newGame)
	{
		newGame = play();
	}
	
	return 0;
}

int play()
{
	int buttonArray[16];
	int option = 2;
	initState();
	
	clearScreen();
	writeFB(); 
	drawFB();
	
	delay(1000);
	
	while(1)
	{
		getInput(buttonArray, 16);
		if (buttonArray[6] == 0) playerMoveLeft();
		if (buttonArray[7] == 0) playerMoveRight();
		if (buttonArray[8] == 0) playerShoot();
		if (buttonArray[3] == 0 && !(state.win || state.lose))
		{
			printf("you pressed Start, you are now in a pause menu with resume selected.\n");	//should go to a pause menu
			delay(500);	//NEW   wait for a bit so the player has time to stop pressing start to prevent going into the pause menu then exiting right back out
			while(1)
			{
				writePause(option);
				drawFB();
				getInput(buttonArray, 16);	//option 2 = resume, 1 = new game, 0 = quit
				if ((buttonArray[5] == 0) && option == 2) {printf("you selected New Game.\n"); option = 1;}
				else if ((buttonArray[5] == 0) && option == 1) {printf("you selected Quit.\n"); option = 0;}
				else if ((buttonArray[4] == 0) && option == 0) {printf("you have selected New Game.\n"); option = 1;}
				else if ((buttonArray[4] == 0) && option == 1) {printf("you have selected Resume.\n"); option = 2;}
				else if ((buttonArray[8] == 0) && option == 2) {printf("resuming.\n"); break;}
				else if ((buttonArray[8] == 0) && option == 1) {printf("starting the game.\n"); updateHighScore(); state.score = 0; return 1;} //return 1 to main to start new game
				else if ((buttonArray[8] == 0) && option == 0) {printf("exiting the game.\n"); clearScreen();drawFB();return 0;} //NEW, added clearScreen and draw
				else if (buttonArray[0] == 0) break;	//leave pause if they press B
				else if (buttonArray[3] == 0) break;	//leave pause if they press start again.
				delay(200);	//NEW (edited speed)
			}
		}

		moveInvaders();

		clearScreen();
		moveProjectiles();
		invadersShoot();
		writeFB();
		checkCollisions();
		drawFB();
		
		if (state.lose == 1) {
			clearScreen();
			updateHighScore();
			state.score = 0;
			writeLose();
			writeScore(25, 25, state.score);
			writeScore(89, 25, state.highScore);
			writeScore(169, 25, 0);
			drawFB();
			//drawLoseScreen here
			delay(1000);	//wait for a while so if the player is pressing anything when they lose it doesnt automatically restart.
			while (1) {
				getInput(buttonArray, 16);
				for (int i = 0; i<16;i++) {
					if (buttonArray[i] == 0) {
						return 1;
					}
				}
			}
			delay(DELAY);
			buttonArray[3] = 1;
		}
				
		if (state.win == 1)
		{
			clearScreen();
			updateHighScore();
			writeWin();
			writeScore(25, 25, state.score);
			writeScore(89, 25, state.highScore);
			writeScore(169, 25, 0);
			drawFB();
			delay(1000);	//feel free to use this second to tell the player to git gud.
			//drawWinScreen here
			while (1) {
				getInput(buttonArray, 16);
				for (int i = 0; i<16;i++) {
					if (buttonArray[i] == 0) {
						return 1;
					}
				}
			}
			delay(DELAY);
			buttonArray[3] = 1;
		}
		state.tick++;
		delay(DELAY);
	}
}

void initState()
{
	initPlayer();
	state.invaderDirection = 1;
	state.emptyProj = 0;
	initInvaders();
	initLives();
	initShield();
	removeAllProjectiles();
	state.win = 0;
	state.lose = 0;
}

void writeFB()
{
	clearScreen();
	writePlayer(state.playerX, state.playerY);
	writeInvaders(state.invaderX, state.invaderY);
	writeShield();
	writeProjectiles();
	writeLives();
	writeScore(25, 25, state.score);
	writeScore(89, 25, state.highScore);
	writeScore(169, 25, 0);
}

void drawFB()
{
	for(int i = 0; i < SCREENHEIGHT; i++)
	{
		for(int j = 0; j < SCREENWIDTH; j++)
		{
			int offset = ((i+YLOC)*framebufferstruct.x) +(j+XLOC);
			if(fbArray[i][j] >= 0x300) framebufferstruct.fptr[offset] = 0xFF00FF00;
			else if(fbArray[i][j] > 0) framebufferstruct.fptr[offset] = 0xFFFFFFFF;
			else if (fbArray[i][j] == -3) framebufferstruct.fptr[offset] = 0xFFFF0000;
			else if(fbArray[i][j] < 0) framebufferstruct.fptr[offset] = 0xFFFFFFFF;
			else framebufferstruct.fptr[offset] = 0;
		}
	}
}

void clearScreen()
{
	memset(fbArray, 0, sizeof(fbArray));
}

void updateHighScore()
{
	if(state.score > state.highScore)
	{
		state.highScore = state.score;
	}
}
