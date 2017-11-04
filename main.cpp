//
// Created by Shihao Jing on 11/4/17.
//

#include "rwlist.h"
#include <iostream>

int main() {
  rwlist c(100);
  c.insert(1);
  c.insert(2);
  c.insert(5);
  for (int i = 0; i < c.getSize(); ++i) {
    std::cout << c.getElement(i) << std::endl;
  }
  c.remove(5);
  for (int i = 0; i < c.getSize(); ++i) {
    std::cout << c.getElement(i) << std::endl;
  }

}