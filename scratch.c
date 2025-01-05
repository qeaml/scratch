/* Scratch-Space, v1.0 */

#include "scratch.h"

qmlScratch qmlScratchInit(uint8_t *data, size_t size, qmlScratchArea *areas, size_t area_max) {
  qmlScratch scratch;
  scratch.data = data;
  scratch.size = size;
  scratch.areas = areas;
  scratch.areaCount = 0;
  scratch.areaMax = area_max;
  return scratch;
}

/**
 * @brief Add an area to the scratch-space.
 *
 * @param scratch Scratch-space
 * @param size Size of the area
 * @return qmlScratchArea* Pointer to the new area
 */
static qmlScratchArea *addArea(qmlScratch *scratch, size_t size) {
  if(scratch->areaCount == 0 || scratch->areaCount == scratch->areaMax) {
    scratch->areas[0].ptr = scratch->data;
    scratch->areas[0].size = size;
    scratch->areaCount = 1;
    return &scratch->areas[0];
  }

  qmlScratchArea *prevArea = &scratch->areas[scratch->areaCount - 1];
  uint8_t *prevEnd = prevArea->ptr + prevArea->size;
  uint8_t *scratchEnd = scratch->data + scratch->size;
  if(prevEnd + size > scratchEnd) {
    scratch->areas[0].ptr = scratch->data;
    scratch->areas[0].size = size;
    scratch->areaCount = 1;
    return &scratch->areas[0];
  }

  qmlScratchArea *area = &scratch->areas[scratch->areaCount];
  area->ptr = prevEnd;
  area->size = size;
  scratch->areaCount++;
  return area;
}

void *qmlScratchAlloc(qmlScratch *scratch, size_t size) {
  qmlScratchArea *area = addArea(scratch, size);
  return area->ptr;
}

/**
 * @brief Get the area by pointer.
 *
 * This find the `qmlScratchArea` that starts at the given pointer. If the
 * pointer is not the start of an area, then `NULL` is returned.
 *
 * @param scratch Scratch-space
 * @param ptr Pointer
 * @return qmlScratchArea* Area starting at the pointer or NULL
 */
static qmlScratchArea *findArea(qmlScratch *scratch, uint8_t *ptr) {
  size_t min = 0;
  size_t max = scratch->areaCount;
  while(min < max) {
    size_t mid = (min + max) / 2;
    qmlScratchArea *area = &scratch->areas[mid];
    if(area->ptr == ptr) {
      return area;
    }
    if(area->ptr < ptr) {
      min = mid + 1;
    } else {
      max = mid;
    }
  }
  return NULL;
}

/**
 * @brief Check if an area can be expanded in place.
 *
 * @param scratch Scratch-space
 * @param area Area we want to expand
 * @param size Size we want to expand to
 * @return int `1` if we can expand in-place, `0` otherwise
 */
static int canExpandInPlace(qmlScratch *scratch, qmlScratchArea *area, size_t size) {
  uint8_t *expandedAreaEnd = area->ptr + size;
  uint8_t *scratchEnd = scratch->data + scratch->size;
  qmlScratchArea *lastArea = &scratch->areas[scratch->areaCount - 1];
  return area == lastArea && expandedAreaEnd < scratchEnd;
}

void *qmlScratchRealloc(qmlScratch *scratch, void *ptr, size_t size) {
  qmlScratchArea *area = findArea(scratch, ptr);
  if(area == NULL) {
    return NULL;
  }
  if(area->size >= size) {
    return area->ptr;
  }
  if(canExpandInPlace(scratch, area, size)) {
    area->size = size;
    return area->ptr;
  }
  qmlScratchArea *newArea = addArea(scratch, size);
  for(size_t i = 0; i < area->size; ++i) {
    newArea->ptr[i] = area->ptr[i];
  }
  return newArea->ptr;
}

/* global scratch area used for shorthand functions */
static qmlScratch gScratch;

void qmlScratchInitGlobal(uint8_t *data, size_t size, qmlScratchArea *areas, size_t area_max) {
  gScratch = qmlScratchInit(data, size, areas, area_max);
}

void *qmlScratchAllocGlobal(size_t size) {
  return qmlScratchAlloc(&gScratch, size);
}

void *qmlScratchReallocGlobal(void *ptr, size_t size) {
  return qmlScratchRealloc(&gScratch, ptr, size);
}
