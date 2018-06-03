# SortVisualization
Visualization of different sorting algorithms with C++ and SDL

Features
--------
- Visualization of different sorting algorithms
- Configurable
  - Time to copy and to compare elements is configurable (simulate different data structures)
  - Number of elements
  - Screen resolution
- Optional multithreading
- Time measurement

Setup
-----
1) The configuration will be loaded during startup from "config.txt"
2) If config.txt is not available, the user will be asked to specify the configuration
Optional) The user can specify a different file through parameters for example: SortVisualization.exe customConfig.txt

- Config.txt
- ----------
- - Vertical screen resolution
- - Horizontal screen resolution
- - Number of elements
- - Copy/Assignment delay in ns
- - Compare delay in ns

- - Example:
-   - 1200
-   - 800
-   - 1200
-   - 500
-   - 250

Usage

HOTKEY|FUNCTION
------|-------------------------
  0   |random init data
  1   |std::sort
  2   |bubblesort
  3   |bubblesort recursivly
  4   |insertionsort
  5   |insertionsort with binary search
  6   |selectionsort
  7   |gnomesort
  8   |gnomesort with jump (insertionsort variation)
  9   |cyclesort
  Q   |shellsort
  W   |combsort
  E   |odd-even-sort (2 threads)
  R   |shakersort
  T   |radixsort
  Z   |radixsort slow (copy instead of move operations used)
  U   |radixsort in-place & insertionsort (optional parallel)
  I   |bogosort
  O   |bozosort
  P   |quicksort (optional parallel)
  A   |mergesort (optional parallel) 
  S   |heapsort 
  D   |introsort (optional parallel)
  F   |std::stablesort 
  X   |reverse order
  V   |verify order
  B   |increase max. threads (default 4)
  N   |decrease max. threads
  
  -> If a key is pressed, sorting will stop. (std::stablesort, std::sort and heapsort can not be stopped). 




Sorting Algorithms are used from my other Repo CPP_SortingAlgorithm_Templates
