## Datastructures, Algorithms and Utilities library in C++

A medium-sized header only library of selected datastructures, algorithms, machinelearning topics and utilities. This collection is built for educational purposes and for use in non-essential projects. All implementations are easy to read and well documented

While I am not an expert in datastructures nor C++, I am still aiming for reference type implementations in terms of efficiency and interface.

**To use these, simply include the *include directory*, which contains only finalized and tested headers.**

**Comparisons with standard library counterparts can be run using the Comparison.h header**

### Speed Comparison

*Relative to the fastest*

|                 |  vector  |  Stack   | HashMap  | LinkedList |
|:----------------|:--------:|:--------:|:--------:|:----------:|
| **std::**       |  *0.81*  |  *0.52*  |  *0.49*  |  **1.00**  |
| **cxstructs::** | **1.00** | **1.00** | **1.00** |   *0.92*   |

### Features

- **Performance**
  - every cxstruct is natively implemented (*no adapters*) at the *lowest* level
- **Custom Allocator**
  - with `#define CX_ALLOC` allows cxstructs to use the faster custom pool-allocator
- **Standard Interface**
    - *cxstructs* follow a similar interface to stl containers
    - but each *cxstruct* has added utility methods
- **Documentation**
    - all *cxstructs* and *cxalgos* come with full method and class docs
- **Testing**
    - every cxstruct has lots of tests (*removed in release*)
- **Debug Symbols**
    - custom assert macros (*cxassert.h*) only compiled in debug mode
- **Accessibility**
    - the source code is clean to read and nicely formatted

---

### Contents

#### Data Structures

- **Vector**(*vec*): *standard efficient vector*
- **Matrix**(*mat*): *flattened float array, lots of methods*
- **Row**(*row*): *compile-time sized, non-mutable container*
- **Pair**(*pair*): *static container for two types*
- **Trie**: *limited to ASCII (128)*
- **Stack**:
- **HashMap**: *linear Probing with LinkedLists but buffered with static arrays*
- **HashSet**: *linear Probing with LinkedLists but buffered with static arrays*
- **Linked List**: *allows for O(1) pop()*
- **Double Linked List**:
- **Queue**:
- **DeQueue**: *using centered array*
- **Binary Tree**:
- **Geometry**(*Rectangle, Circle*): *standard efficient 2D forms*

#### Machine Learning

###### *(NN = Neural Network)*

- **FNN**: *implemented using matrices (*default*) and without, switch:`#define CX_NO_MATRIX`*

#### Algorithms

- **Sorting**: *QuickSort, MergeSort, Bubblesort, Bogosort, Selectionsort*
- **Search**: *Binary Search (recursive and non-recursive),*
- **Graph Traversal**: *DepthFirstSearch (on 2d-vector as adjacency matrix),*
- **MathFunctions**: *Integrals,*

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

#### cxconfig.h
```cpp
typedef uint_fast32_t uint_32_cx;
typedef int_fast32_t int_32_cx;
typedef int_fast32_t int_32_cx;
```
#### macros

- `#define CX_ALLOC` to use the custom allocator (applies globally)
- `#define CX_NO_MATRIX`to use the FNN without matrix calculations
