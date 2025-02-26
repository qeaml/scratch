#include "scratch.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define SCRATCH_SIZE 1024
#define AREA_MAX 10

void gauntlet(qmlScratch *scratch) {
  char *str = qmlScratchAlloc(scratch, 3);
  str[0] = 'a';
  str[1] = 'b';
  str[2] = '\0';
  printf("Allocated 1: %s\n", str);

  char *str2 = qmlScratchAlloc(scratch, 3);
  str2[0] = 'c';
  str2[1] = 'd';
  str2[2] = '\0';
  printf("Allocated 2: %s\n", str2);

  str = qmlScratchRealloc(scratch, str, 5);
  str[2] = 'e';
  str[3] = 'f';
  str[4] = '\0';
  printf("Reallocated: %s\n", str);

  char *str3 = qmlScratchAlloc(scratch, 3);
  str3[0] = 'g';
  str3[1] = 'h';
  str3[2] = '\0';
  printf("Allocated 3: %s\n", str3);

  str2 = qmlScratchRealloc(scratch, str2, 5);
  str2[2] = 'i';
  str2[3] = 'j';
  str2[4] = '\0';
  printf("Reallocated 2: %s\n", str2);

  str = qmlScratchRealloc(scratch, str, 3);
  str[2] = '\0';
  printf("Reallocated 1: %s\n", str);

  printf("Areas: %zu\n", scratch->areaCount);
}

/* Example of using a qmlScratch object with stack-allocated memory. Useful if
   you need a bunch of throwaway memory for the duration of one function. */
void localScratchSpace() {
  uint8_t data[SCRATCH_SIZE];
  qmlScratchArea areas[AREA_MAX];
  qmlScratch scratch = qmlScratchInit(data, SCRATCH_SIZE, areas, AREA_MAX);
  gauntlet(&scratch);
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

void overflow() {
  uint8_t data[SCRATCH_SIZE];
  qmlScratchArea areas[AREA_MAX];
  qmlScratch scratch = qmlScratchInit(data, SCRATCH_SIZE, areas, AREA_MAX);

  for(int i = 1; i < 15; ++i) {
    void *ptr = qmlScratchAlloc(&scratch, i * 10);
  }

  printf("Areas: %zu\n", scratch.areaCount);
}

#define BIG_SIZE 10240
#define MANY_AREAS 256
#define POINTERS 256

void benchmark() {
  uint8_t data[BIG_SIZE];
  qmlScratchArea areas[MANY_AREAS];
  void *pointers[POINTERS];
  size_t sizes[POINTERS];
  qmlScratch scratch = qmlScratchInit(data, BIG_SIZE, areas, MANY_AREAS);

  for(int i = 0; i < POINTERS; ++i) {
    sizes[i] = 10 + i;
    pointers[i] = qmlScratchAlloc(&scratch, sizes[i]);
  }
  for(int j = 0; j < POINTERS*20; ++j) {
    for(int i = 0; i < POINTERS; ++i) {
      sizes[i] += 2;
      pointers[i] = qmlScratchRealloc(&scratch, pointers[i], sizes[i]);
    }
  }
}

void benchmarkMalloc() {
  void *pointers[POINTERS];
  size_t sizes[POINTERS];

  for(int i = 0; i < POINTERS; ++i) {
    sizes[i] = 10 + i;
    pointers[i] = malloc(sizes[i]);
  }
  for(int j = 0; j < POINTERS*20; ++j) {
    for(int i = 0; i < POINTERS; ++i) {
      sizes[i] += 2;
      pointers[i] = realloc(pointers[i], sizes[i]);
    }
  }
  for(int i = 0; i < POINTERS; ++i) {
    free(pointers[i]);
  }
}

#ifdef __linux__

#include <sys/mman.h>
#include <unistd.h>

void memoryPage() {
  int pageSize = getpagesize();
  /* 2 pages: one for the data and one for the areas */
  void *pages = mmap(NULL, 2*pageSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  assert(pages != NULL);

  /* store areas in first page in case of buffer overruns in scratch-space */
  qmlScratchArea *areas = pages;
  size_t area_max = pageSize / sizeof(qmlScratchArea);

  uint8_t *data = pages + pageSize;
  size_t size = pageSize;

  printf(
    "Page Size: %d\n"
    "Pages: %p\n"
    "area_max: %zu\n",
    pageSize, pages, area_max);

  qmlScratch scratch = qmlScratchInit(data, size, areas, area_max);

  gauntlet(&scratch);

  assert(munmap(pages, 2*pageSize) == 0);
}

#endif

int main(int argc, char **argv) {
  if(argc >= 2 && argv[1][0] == 'b') {
    if(argc >= 3 && argv[2][0] == 'm') {
      benchmarkMalloc();
    } else {
      benchmark();
    }
    return 0;
  }

  printf("Local scratch space:\n");
  localScratchSpace();
  printf("\nGlobal scratch space:\n");
  globalScratchSpace();
  printf("\nData overflow:\n");
  overflow();
  #ifdef __linux__
  printf("\nBacked by pages:\n");
  memoryPage();
  #endif
}
