#include <sstream>
#include <fstream>
#include <fcfUnion/union.hpp>

#ifndef TEST
  #define TEST_STRINGIZE(x) TEST_STRINGIZE2(x)
  #define TEST_STRINGIZE2(x) #x
  #define TEST_LINE_STRING TEST_STRINGIZE(__LINE__)
  #define TEST(exp) \
    if (!(exp)){ \
      throw std::runtime_error(std::string() + "Test error: " + #exp + "  [FILE: " + __FILE__ + ":" + TEST_LINE_STRING + "]");\
    }
#endif

void streamTest(){
  {
    std::ifstream ifs("files/error");
    fcf::Union u;
    TEST(ifs.fail());
    bool error = false;
    try {
      u.parse(ifs);
    } catch(std::exception& e){
      error = true;
    }
    TEST(error == true);
  }
  {
    std::ifstream ifs("files/empty");
    fcf::Union u;
    TEST(!ifs.fail());
    bool error = false;
    try {
      u.parse(ifs);
    } catch(std::exception& e){
      error = true;
    }
    TEST(error == true);
  }
  {
    std::ifstream ifs("files/1");
    fcf::Union u;
    TEST(!ifs.fail());
    bool error = false;
    try {
      u.parse(ifs);
    } catch(std::exception& e){
      error = true;
    }
    TEST(error == false);
    TEST(u == 1);
  }
}

