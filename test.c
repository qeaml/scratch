#include "scratch.h"
#include <stdio.h>

#define SCRATCH_SIZE 1024
#define AREA_MAX 10

static uint8_t gData[SCRATCH_SIZE];
static qmlScratchArea gAreas[AREA_MAX];

int main(int argc, char **argv) {
  qmlScratch scratch = qmlScratchInit(gData, SCRATCH_SIZE, gAreas, AREA_MAX);

  char *str = qmlScratchAlloc(&scratch, 3);
  str[0] = 'a';
  str[1] = 'b';
  str[2] = '\0';
  printf("Allocated: %s\n", str);

  char *str2 = qmlScratchAlloc(&scratch, 3);
  str2[0] = 'c';
  str2[1] = 'd';
  str2[2] = '\0';
  printf("Allocated: %s\n", str2);

  str = qmlScratchRealloc(&scratch, str, 5);
  str[2] = 'e';
  str[3] = 'f';
  str[4] = '\0';
  printf("Reallocated: %s\n", str);

  return 0;
}
