#include <iostream>
#include <string>

#include "core/Player.h"

void test_core(const std::string &source) {
  std::cout << "hello " + source + ", from core!" << std::endl;
  Player* p = new Player();
  delete p;
}
