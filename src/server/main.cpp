#include <core/lib.hpp>
#include <iostream>
#include <network/lib.hpp>

int main(int argc, char *argv[]) {
  test_core("server");
  test_network("server");
  std::cout << "server is running!" << std::endl;
  return 0;
}
