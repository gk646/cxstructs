## Datastructures, algorithms, machine-learning and utilities library in C++

A medium-sized modern header only library of selected datastructures, algorithms, machinelearning topics and utilities.
This
collection is built for educational purposes and for use in non-essential projects. All implementations are easy to read
and well documented.

While I am not an expert in datastructures nor C++, I am still aiming for reference type implementations in terms of
efficiency and interface.

The general namespace is `cxstructs`.

**1.** [Contents](#contents)   
**2.** [Usage Guide](#usage-guide)   
**3.** [Installation](#installation)  
**4.** [Library Notes](#library-notes)  
**5.** [Contributing](#contributing)  
**6.** [Resources](#resources)

### Speed Comparison

*Relative to the fastest / with CXPoolAllocator*

|                 |  vector  |  Stack   | HashMap  | HashSet | LinkedList |  Queue   | DeQueue  |
|:----------------|:--------:|:--------:|:--------:|:-------:|:----------:|:--------:|:--------:|
| **std::**       |  *0.81*  |  *0.52*  |  *0.51*  | *0.52*  |   *0.71*   |  *0.46*  |  *0.57*  |
| **cxstructs::** | **1.00** | **1.00** | **1.00** | **1.0** |  **1.00**  | **1.00** | **1.00** |

### Features

- **Performance**
    - every cxstruct is natively implemented (*no adapters*) at the *lowest* level
- **Custom Allocator**
    - per default uses the faster CXPoolAllocator
    - can be disabled individually via the last template option `cxstruct<int,false>`
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
- **Cross-Compiler**
    - C++20 >= required
    - the library is tested on Linux(*gcc*), Windows(*MSCV 17.0 (2022), mingw-gcc(13.1), zig*)

---

### Contents

#### Data Structures

- **Vector**(*vec*):
- **Matrix**(*mat*): *flattened float array, lots of methods*
- **Row**(*row*): *compile-time sized, non-mutable container*
- **Pair**: *static container for two types*
- **Trie**: *limited to ASCII (128)*
- **Stack**:
- **HashMap**: *using separate chaining with LinkedLists with static buffer*
- **HashSet**: *using separate chaining with LinkedLists with static buffer*
- **Linked List**:
- **Double Linked List**:
- **Queue**: *using circular array*
- **DeQueue**: *using circular array*
- **PriorityQueue**: *using binary heap*
- **Binary Tree**:
- **QuadTree**: *allows custom Types with x() and y() getters*
- **Geometry**(*Rect,Circle,Point*): *standard efficient 2D shapes*

#### Machine Learning

- **FeedForwardNeuralNetwork**(*FNN*): *implemented using matrices(*default*) and without, switch:`#define CX_LOOP_FNN`*
- **k-Nearest Neighbour**(*k-NN2D,k-NNXD*): *2D works with a QuadTree,*
- **Word2Vec**: *simple word to vector network (WIP)*

#### Algorithms

- **Sorting**: *QuickSort, MergeSort, Bubblesort, Bogosort, Selectionsort*
- **Search**: *Binary Search (recursive and non-recursive),*
- **Graph Traversal**: *DepthFirstSearch (on 2d-vector as adjacency matrix),*
- **MathFunctions**: *Integrals,*
- **PatterMatching**: *Brute-Force, KMP, Boyer-Moore*
- **Misc**: *Maze generator(simple)*

#### Utilities

- **cxtime**: *easily measure the time from `now()` to `printTime()`*
- **cxio**: *load_text,*
- **cxassert**: *custom assertions with optional text*
- **cxmath**: *activation functions,distance function, next_power_of_2*
- **cxgraphics**: *simple native windowing and graphics output header*

---

### Usage Guide

Short explanation of advanced use cases.

#### CXAllocator

Use the last template option to specify wether to use the allocator or not `cxstruct<Type,false>`.
Generally **use** the CXAllocator if you use the cxstruct for longer and as a standalone.
In turn, generally **do not use** it if it's a temporary or fixed size structure
In case of slower performance just switch to the other.

#### FNN

Takes either matrices or vectors as input. In both ways every row is interpreted as a separate input.

#### k-NN

- The k-NN will per default not duplicate any data (or make lookup tables) and only works on references
    - this allows for large datasets but might be slower

In order to allow for a multitude of data, k-NN has a generic interface.
There is an abstract base class which you can use but really any type works for as long as it has those basic getter
functions. You can define your categories as you like.  
2D Example:

```cpp
 enum Category { A, B, C };
    struct DataPoint : public DataPoint_<Category> {
      Category category;
      float x_;
      float y_;
      DataPoint(float x, float y, Category category) : x_(x), y_(y), category(category) {}
      float x() const final { return x_; }
      float y() const final { return y_; }
      Category getCategory() final { return category; }
      float getWeight() const override { return 0; }
    };
```

Multidimensional Example:

### Installation

#### CMake

Simply add these lines around your build target in your CMakeLists.txt. This will automatically download the newest
version and add them to your *includes*.

```cmake
include(FetchContent)
FetchContent_Declare(cxstructs GIT_REPOSITORY https://github.com/gk646/CXStructures.git)
FetchContent_MakeAvailable(cxstructs)

#add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE cxstructs)
```

Or manually download the source and include the *include* directory.

```cmake
include_directories("cxstructs/download/folder/include")
```

#### Manual

Download the source and add the *include* directory to your build system include path.

### Library Notes

#### namespaces

- **`cxstructs` for all headers**
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

- `#define CX_LOOP_FNN` to use the FNN without matrix calculations (slower)
- `CX_ASSERT(expr,msg(optinal))` enhanced assertion with optional text
- `CX_WARNING(expr,msg(optinal))` similar to *CX_ASSERT* but doesn't abort

### Contributing

Add `DBUILD_TESTS=ON` to your CMake options to build an executable target from the src/ directory.
You can then add a main.cpp and test any changes.

Run tests by `#include "CXTests.h"` and calling `test_all`.

Feel free to contribute!

### Resources

[Inside Code Youtube Channel](https://www.youtube.com/@insidecode) provides excellent videos on datastructures. (used
for PriorityQueue)

### Misc

- Initially I made all the cxstructs with automatic shrinking when reaching a min_size
  as I thought the STL containers are in fact doing that
    - turns out they don't, and it was the reason I didn't beat them yet in performance

- The CX_ASSERT macro is inspired by the
  video ["How I use C++;a line-by-line code review"](https://www.youtube.com/watch?v=W8-G_PL6p-0&pp=ygUYbXkgYysrIGlzIGluc2FuZSBzdHJhZ2Vy)
  by Strager

