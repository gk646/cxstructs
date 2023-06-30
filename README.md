## Datastructures, Algorithms and Utilities Collection in C++

A small (but growing) header only collection of selected datastructures and algorithms.
This collection is primarily built for educational purposes as the implementations are easy to read and well documented.

That said, getting more experience iam aiming for reference type implementations in both speed and interface.

To use these, simply include the **include directory**, which contains only finalized and tested headers.

Comparisons with standard library counterparts can be run using the Comparison.h header

### Speed Comparison

*Relative to the fastest*

|                 |  vector  |  Stack   | HashMap  | LinkedList |
|:----------------|:--------:|:--------:|:--------:|:----------:|
| **std::**       |  *0.81*  |  *0.52*  |  *0.49*  |  **1.00**  |
| **cxstructs::** | **1.00** | **1.00** | **1.00** |   *0.92*   |

### Features

- **Custom Allocator** `#define CX_ALLOC`  (recommended)
- **Extensive featureset**
    - Each cxstruct contains lots of utility methods
    - Each cxstruct has an iterator if applicable
- **Rich documentation**
    - Algorithms come with full runtime complexity and core principle
    - data structures each have a description header and fully documented methods
- **Tests**
    - Every algorithm and data structure has lots of tests
- **Debug Symbols**
    - assert *(from <cassert>) are used to show out of line access when compiling in Debug
- **Clarity**
    - the source code is clean to read and nicely formatted

---

### Contents

#### Data Structures

- **Vector**(*vec*): *very fast!*
- **Matrix**(*mat*): *flattened float array, lots of methods*
- **Row**(*row*): *compile-time non-mutable container*
- **Pair**(*pair*): static container for two types
- **Trie**: *limited to ASCII -> very fast!*
- **HashMap**: *Linear Probing with LinkedLists but buffered with stack arrays ->fast!*
- **Linked List**:
- **Double Linked List**:
- **Queue**:
- **DeQueue**: *using centered array*
- **Stack**:
- **Binary Tree**:
- **Geometry**: Rectangle, Circle

#### Machine Learning

###### *(NN = Neural Network)*

- **FNN**(*Feed-forward*) *first implementation using matrices, second using traditional loops*

#### Algorithms

- **Sorting**: QuickSort, MergeSort, Bubblesort, Bogosort, Selectionsort
- **Search**: Binary Search *(recursive and non-recursive)*
- **Graph Traversal**: DepthFirstSearch *(on 2d-vector as adjacency matrix)*
- **MathFunctions**: Integral,

#### Utilities

- **cxtime** *easily measure the time from `now()` to `printTime()`*
- **cxio** *load files*

---

### Library Notes

#### namespaces

- **`cxstructs` for datastructures**
- **`cxalgos` for algorithms**
- **`cxml` for machinelearning**
- **`cxutil` for utilities**
- *internal:*
    - *`cxhelper`is used to hide helper methods or structs*
    - *`cxtests` used to hide tests*

#### config.h
```cpp
typedef uint_fast32_t uint_32_cx;
typedef int_fast32_t int_32_cx;
typedef int_fast32_t int_32_cx;
```
#### macros

- `#define CX_ALLOC` to use the custom allocator (applies globally)

