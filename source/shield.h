#define SHIELDPAD 12
#define SHIELDHEIGHT 16
#define SHIELDWIDTH 20
#define SHIELDID 0x200
#define SHIELDYOFFSET 200

#ifndef SHIELD
#define SHIELD
typedef struct Shield{
  unsigned int		width;
  unsigned int		height;
  unsigned char		image_pixels[20 * 16 * 4 + 1];
} shield;

#endif

void initShield();
void writeShield();
void hitShield(int i);

extern shield shield1;
extern shield shield2;
extern shield shield3;
extern shield shield4;
extern shield shield5;
