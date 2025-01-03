#include "scratch.h"
#include <stdio.h>

#define SCRATCH_SIZE 1024
#define AREA_MAX 10

/* Example of using a qmlScratch object with stack-allocated memory. Useful if
   you need a bunch of throwaway memory for the duration of one function. */
void localScratchSpace() {
  uint8_t data[SCRATCH_SIZE];
  qmlScratchArea areas[AREA_MAX];
  qmlScratch scratch = qmlScratchInit(data, SCRATCH_SIZE, areas, AREA_MAX);

  char *str = qmlScratchAlloc(&scratch, 3);
  str[0] = 'a';
  str[1] = 'b';
  str[2] = '\0';
  printf("Allocated 1: %s\n", str);

  char *str2 = qmlScratchAlloc(&scratch, 3);
  str2[0] = 'c';
  str2[1] = 'd';
  str2[2] = '\0';
  printf("Allocated 2: %s\n", str2);

  str = qmlScratchRealloc(&scratch, str, 5);
  str[2] = 'e';
  str[3] = 'f';
  str[4] = '\0';
  printf("Reallocated: %s\n", str);

  char *str3 = qmlScratchAlloc(&scratch, 3);
  str3[0] = 'g';
  str3[1] = 'h';
  str3[2] = '\0';
  printf("Allocated 3: %s\n", str3);

  str2 = qmlScratchRealloc(&scratch, str2, 5);
  str2[2] = 'i';
  str2[3] = 'j';
  str2[4] = '\0';
  printf("Reallocated 2: %s\n", str2);

  str = qmlScratchRealloc(&scratch, str, 3);
  str[2] = '\0';
  printf("Reallocated 1: %s\n", str);
}

static uint8_t gData[SCRATCH_SIZE];
static qmlScratchArea gAreas[AREA_MAX];

/* Example of using the global scratch-space with static memory. Useful if you
   only ever plan to use one scratch-space. */
void globalScratchSpace() {
  qmlScratchInitGlobal(gData, SCRATCH_SIZE, gAreas, AREA_MAX);

  char *str = qmlScratchAllocGlobal(3);
  str[0] = 'a';
  str[1] = 'b';
  str[2] = '\0';
  printf("Allocated 1: %s\n", str);

  char *str2 = qmlScratchAllocGlobal(3);
  str2[0] = 'c';
  str2[1] = 'd';
  str2[2] = '\0';
  printf("Allocated 2: %s\n", str2);

  str = qmlScratchReallocGlobal(str, 5);
  str[2] = 'e';
  str[3] = 'f';
  str[4] = '\0';
  printf("Reallocated: %s\n", str);

  char *str3 = qmlScratchAllocGlobal(3);
  str3[0] = 'g';
  str3[1] = 'h';
  str3[2] = '\0';
  printf("Allocated 3: %s\n", str3);

  str2 = qmlScratchReallocGlobal(str2, 5);
  str2[2] = 'i';
  str2[3] = 'j';
  str2[4] = '\0';
  printf("Reallocated 2: %s\n", str2);

  str = qmlScratchReallocGlobal(str, 3);
  str[2] = '\0';
  printf("Reallocated 1: %s\n", str);
}

int main(int argc, char **argv) {
  printf("Local scratch space:\n");
  localScratchSpace();
  printf("\nGlobal scratch space:\n");
  globalScratchSpace();
}
