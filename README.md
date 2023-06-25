## Datastructures, Algorithms and Utilities Collection in C++

A small (but growing) header only collection of selected datastructures and algorithms. 
This collection is primarily built for educational purposes as the implementations are easy to read and well documented.  

That said, getting more experience iam aiming for reference type implementations in both speed and interface.

To use these, simply include the **include directory**, which contains only finalized and tested headers.

#### Features

- Each algorithm comes with a runtime description and a documentation of the underlying principle
- The data structures have fully documented methods, adhering to a consistent calling syntax for ease of use
- Each header is **self-contained** without any cross dependencies.
- Comparisons with STL counterparts can be run using the Comparison.h header

#### Namespaces
- **`cxstructs` for datastructures**
- **`cxalgos` for algorithms**
- **`cxml` for machinelearning** 
- *`cxhelper`is used to hide helper methods or structs*
- *`cxtests` (still) used to hide tests*

#### Global Macros
- `#define CX_ALLOC` to use the custom allocator (unfinished)

### Data Structures

- **HashMap** *(Linear Probing with LinkedLists)*
- **Linked List**
- **Double Linked List**
- **Vector**(*vec*) *(very fast!)*
- **Matrix**(*mat*) *(lots of methods)*
- **Queue**
- **DeQueue**
- **Stack**
- **Binary Tree**
- **Trie** *(limited to ASCII -> very fast!)*
- 
### Machine Learning

- **FNN** *first implementation using matrices, second using traditional loops*
- 

### Algorithms

- **Sorting**: QuickSort, MergeSort, Bubblesort, Bogosort, Selectionsort
- **Search**: Binary Search *(recursive and non-recursive)*
- **Graph Traversal**: DepthFirstSearch *(on 2d-vector as adjacency matrix)*
- **MathFunctions**: Integral,


### Utilities

- **Geometry**: Rectangle, Circle
- 