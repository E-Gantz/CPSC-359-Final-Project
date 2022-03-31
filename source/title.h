#ifndef SCREEN
#define SCREEN
typedef struct Screen{
  unsigned int		width;
  unsigned int		height;
  unsigned char		image_pixels[224 * 256 * 4 + 1];
} screen;

#endif

extern screen playScreen;
extern screen exitScreen;
extern screen defaultScreen;

void writeTitle(int version);
