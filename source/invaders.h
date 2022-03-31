#define INVADERWIDTH 162
#define INVADERHEIGHT 52
#define INVADERYOFFSET 10

#define INVADERBOT 100
#define ENEMYID 0x300

void writeInvaders(int x0, int y0);
void initInvaders();
void killInvader(int x, int y);
void moveInvaders();
void invadersShoot();
