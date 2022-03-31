#define PROJWIDTH 1
#define PROJHEIGHT 4
#define PROJSPEED 15
#define PROJID -1

void writeProjectiles();
void moveProjectiles();
void checkCollisions();
void newProjectile(int dir, int x0, int y0);
void removeProjectile(int p);
void removeAllProjectiles();
