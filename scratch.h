/*
scratch.h
---------
The Scratch-Space
*/

#ifndef QML_SCRATCH_H_
#define QML_SCRATCH_H_

#ifndef QML_SCRATCH_API
#define QML_SCRATCH_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

typedef struct qmlScratchAreaS {
  uint8_t *ptr;
  size_t size;
} qmlScratchArea;

typedef struct qmlScratchS {
  uint8_t *data;
  size_t size;
  qmlScratchArea *areas;
  size_t areaCount;
  size_t areaMax;
} qmlScratch;

/**
 * @brief Initialize a scratch-space.
 *
 * @param data Data buffer
 * @param size Size of data buffer
 * @param areas Allocation area buffer
 * @param area_max Maximum number of areas at once
 * @return qmlScratch A scratch-space
 */
QML_SCRATCH_API
qmlScratch qmlScratchInit(uint8_t *data, size_t size, qmlScratchArea *areas, size_t area_max);

/**
 * @brief Allocate memory from a scratch-space.
 *
 * @param scratch Scratch-space
 * @param size Size of allocation
 * @return void* Pointer to allocated memory
 */
QML_SCRATCH_API
void *qmlScratchAlloc(qmlScratch *scratch, size_t size);

/**
 * @brief Reallocate memory in a scratch-space.
 *
 * This will try to expand in-place if the pointer is at the end of the
 * scratch-space. Otherwise, it will allocate a new area and copy the data.
 *
 * @param scratch Scratch-space
 * @param ptr Pointer to previous scratch-space allocation
 * @param size Size to reallocate to
 * @return void* Pointer to the modified memory
 */
QML_SCRATCH_API
void *qmlScratchRealloc(qmlScratch *scratch, void *ptr, size_t size);

/**
 * @brief Initialize a global scratch-space.
 *
 * This is a shorthand for `qmlScratchInit` that uses a global scratch-space.
 * Most useful if you have only one scratch-space in your program and you don't
 * want to pass it around everywhere.
 *
 * @param data Data buffer
 * @param size Size of data buffer
 * @param areas Allocation area buffer
 * @param area_max Maximum number of areas at once
 * @sa qmlScratchInit
 */
QML_SCRATCH_API
void qmlScratchInitGlobal(uint8_t *data, size_t size, qmlScratchArea *areas, size_t area_max);

/**
 * @brief Allocate memory from the global scratch-space.
 *
 * @param size Size of memory
 * @return void* Pointer to the allocated memory
 * @sa qmlScratchAlloc
 */
QML_SCRATCH_API
void *qmlScratchAllocGlobal(size_t size);

/**
 * @brief Reallocate memory in the global scratch-space.
 *
 * @param ptr Pointer to previous global scratch-space allocation
 * @param size Size to reallocate to
 * @return void* Pointer to the modified memory
 */
QML_SCRATCH_API
void *qmlScratchReallocGlobal(void *ptr, size_t size);

#ifdef __cplusplus
}
#endif

#endif // QML_SCRATCH_H_
