#include <config/lib.hpp>
#include <fstream>

json get_config() {
  std::fstream f("config.json");
  return json::parse(f);
}
