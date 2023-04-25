#include <core/lib.hpp>
#include <iostream>
#include <network/lib.hpp>

int main(int argc, char *argv[]) {
  //test_core("client");
  test_network("client");
  std::cout << "client is running!" << std::endl;
  return 0;
}
