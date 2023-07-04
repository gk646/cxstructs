## Datastructures, Algorithms and Utilities library in C++

A medium-sized header only library of selected datastructures, algorithms, machinelearning topics and utilities. This
collection is built for educational purposes and for use in non-essential projects. All implementations are easy to read
and well documented.

While I am not an expert in datastructures nor C++, I am still aiming for reference type implementations in terms of
efficiency and interface.

**To use these, simply include the *include directory*, and `#include "CXStructs.h"` to include all headers.**

**Comparisons with standard library counterparts can be run using the Comparison.h header**

### Speed Comparison

*Relative to the fastest / with CX_ALLOC*

|                 |  vector  |  Stack   | HashMap  | HashSet | LinkedList |  Queue   |
|:----------------|:--------:|:--------:|:--------:|:-------:|:----------:|:--------:|
| **std::**       |  *0.81*  |  *0.52*  |  *0.51*  | *0.52*  |   *0.71*   | **1.00** |
| **cxstructs::** | **1.00** | **1.00** | **1.00** | **1.0** |  **1.00**  |  *0.90*  |

### Features

- **Performance**
    - every cxstruct is natively implemented (*no adapters*) at the *lowest* level
- **Custom Allocator**
    - per default uses the faster CXPoolAllocator
    - can be disabled individually via the last template option `cstruct<int,false>`
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

- **Vector**(*vec*):
- **Matrix**(*mat*): *flattened float array, lots of methods*
- **Row**(*row*): *compile-time sized, non-mutable container*
- **Pair**: *static container for two types*
- **Trie**: *limited to ASCII (128)*
- **Stack**:
- **HashMap**: *linear Probing with LinkedLists but buffered with static arrays*
- **HashSet**: *linear Probing with LinkedLists but buffered with static arrays*
- **Linked List**:
- **Double Linked List**:
- **Queue**:
- **DeQueue**: *using centered array*
- **Binary Tree**:
- **QuadTree**: *(soon)*
- **Geometry**(*Rect,Circle,Point*): *standard efficient 2D shapes*

#### Machine Learning

###### *(NN = Neural Network)*

- **FNN**: *implemented using matrices (*default*) and without, switch:`#define CX_NO_MATRIX`*

#### Algorithms

- **Sorting**: *QuickSort, MergeSort, Bubblesort, Bogosort, Selectionsort*
- **Search**: *Binary Search (recursive and non-recursive),*
- **Graph Traversal**: *DepthFirstSearch (on 2d-vector as adjacency matrix),*
- **MathFunctions**: *Integrals,next_power_of_2,*
- **PatterMatching**: *Brute-Force, KMP, Boyer-Moore*

#### Utilities

- **cxtime** *easily measure the time from `now()` to `printTime()`*
- **cxio** *load files*
- **cxassert** *custom assertions with optional text*

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
- `#define CX_NO_MATRIX`to use the FNN without matrix calculations
- `CX_ASSERT(expr,msg(optinal))` enhanced assertion with optional text
- `CX_WARNING(expr,msg(optinal))` similar to *CX_ASSERT* but doesn't abort