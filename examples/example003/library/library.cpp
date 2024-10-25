#include "library.hpp"

void someFunc() {
  fcf::Union u(fcf::undefined);
  std::string str (u.get<std::string>());
}