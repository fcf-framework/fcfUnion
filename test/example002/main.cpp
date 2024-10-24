#include <union/union.hpp>
#include <iostream>
#include <fstream>

int main(int a_argc, char* a_argv[]){
  const char* confFilePath = "../test/config.json";
  std::ifstream ifs;
  ifs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    ifs.open(confFilePath);
    ifs.exceptions(std::ifstream::badbit);
  } catch(std::exception& e){
    std::cout << "ERROR: Failed open file '" << confFilePath << "'."  << std::endl;
    return 1;
  }
  fcf::Union u;
  u.parse(ifs);
  
  // Result
  // StdOut: The file contains a JSON object: 1
  // StdOut: Fields:
  // StdOut:   [param1]: some value
  std::cout << "The file contains a JSON object: " << u.is<fcf::UnionMap>() << std::endl;
  std::cout << "  Fields: " << std::endl;
  std::cout << "    [param1]: " << u["param1"] << std::endl;
  return 0;
}

