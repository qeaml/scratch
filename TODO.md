# TODO

* Use binary search in [`findArea()`](scratch.c#L60).
* Add `qmlScratchFree`:
  * Areas marked as "free" can be used to expand allocations in-place more
    often, making more efficient use of memory.
  * Also allow of reuse of "free" areas by `qmlScratchAlloc`?