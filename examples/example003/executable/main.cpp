#include <fcfUnion/union.hpp>
#include <iostream>
#include <fstream>

int main(int a_argc, char* a_argv[]){
  std::ifstream ifs("config.json");
  fcf::Union u;
  u.parse(ifs);

  std::cout << "  The file contains a JSON object: " << u.is<fcf::UnionMap>() << std::endl;
  std::cout << "    Fields: " << std::endl;
  std::cout << "      [param1]: " << u["param1"] << std::endl;
  return 0;
}

