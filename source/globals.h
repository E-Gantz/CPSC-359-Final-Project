#include <stdbool.h>
#define SIZE 2
#define XLOC 100
#define YLOC 100

#define ARRAYX 11
#define ARRAYY 5

#define SCREENWIDTH 224*SIZE
#define SCREENHEIGHT 256*SIZE

#define MAXPROJECTILES 50

#define DELAY 100

int fbArray[SCREENHEIGHT][SCREENWIDTH];
//int colArray[SCREENHEIGHT][SCREENWIDTH];

void writeBit(int x, int y, int id);
int checkBit(int x, int y);
//void writeCol(int x, int y, int id);

struct GameState{
	int tick;
	int aliveInvaders[ARRAYY][ARRAYX];
	int numAliveInvaders;
	int frontline[ARRAYX];					//only frontline enemies are able to shoot back
	int playerX;
	int playerY;
	int invaderX;
	int invaderY;
	int invaderSpeed;
	bool invaderDirection;					//1 if going right, 2 if going left
	int shieldX;
	int shieldY;
	int lives;
	int score;
	int highScore;
	int projectilesDir[MAXPROJECTILES];		//direction of each projectile on screen. also indicates if projectile exists (set to 0 to remove projectile from consideration)
	int projectilesX[MAXPROJECTILES];
	int projectilesY[MAXPROJECTILES];
	int projectilesCol[MAXPROJECTILES];		//stores ID of object that projectile collided with after last check (0 if no collision)
	int emptyProj;							//current lowest empty index of projectile array
	int lose;
	int win;
} state;
