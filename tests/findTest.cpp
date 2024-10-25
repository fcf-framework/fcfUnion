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

void findTest(){
  {
    fcf::Union u(fcf::UT_VECTOR);
    u.insert("1");
    u.insert("2");
    u.insert("3");
    TEST(u.find("1") != u.end());
    TEST(u.find(1) != u.end());
    TEST(u.find(10) == u.end());
    TEST(u.find("10") == u.end());
    TEST(u.find(fcf::undefined) == u.end());
    TEST(u.find(2) != u.end());
    TEST(u.find(0) != u.end());
    TEST(u.find(3) == u.end());
  }
  {
    fcf::Union u(fcf::UT_MAP);
    u[100] = "value 100";
    TEST(u.find("100") != u.end());
    TEST(!(u.find("100") == u.end()));
    TEST(u.find("101") == u.end());
    TEST(!(u.find("101") != u.end()));
    u.insert("value 1");
    TEST(u.find(1) != u.end());
  }
}
