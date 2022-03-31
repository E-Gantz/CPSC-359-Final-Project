#ifndef SCREEN
#define SCREEN
typedef struct Screen{
  unsigned int		width;
  unsigned int		height;
  unsigned char		image_pixels[170 * 59 * 4 + 1];
} screen;

#endif

extern screen resumeScreen;
extern screen newGameScreen;
extern screen quitScreen;

void writePause(int version);
