//
// Created by Lukas on 01.06.2023.
//

#ifndef CMAKE_TEST_RBTREE_H
#define CMAKE_TEST_RBTREE_H

template <typename T> class RBTree {

  enum class Color { RED, BLACK };

  Color color_ = Color::BLACK;
  RBTree *left_;
  RBTree *right_;
  T data;

public:
  RBTree(T data) : data(data) {};
  ~RBTree(){
      delete right_;
      delete left_;
  };

};

#endif // CMAKE_TEST_RBTREE_H
