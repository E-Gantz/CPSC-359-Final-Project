#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define DIGITHEIGHT 7
#define DIGITWIDTH 5
#define DIGITGAP 3
#define SCOREID -1
#define DIGITOFFSET 8
#define SCORE 25

typedef struct Score{
  unsigned int		width;
  unsigned int		height;
  unsigned char		image_pixels[224 * 7 * 4 + 1];
} score;

extern score scoreScreen;

void writeScore(int a, int b, int s);


