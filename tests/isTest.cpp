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

void isTest() {
  TEST(fcf::Union((int)0).is<int>() == true);
  TEST(fcf::Union((int)0).isCompatible<long>(false) == true);
  TEST(fcf::Union((int)0).isCompatible<bool>(false) == true);
  TEST(fcf::Union(false).isCompatible<int>(false) == true);
  TEST(fcf::Union(1).isCompatible<std::string>(false) == false);
  TEST(fcf::Union(false).isCompatible<std::string>(false) == false);
  {
    fcf::Union u("1");
    TEST(u.is<std::string>());
    TEST(u.is(fcf::UT_STRING));
    TEST(u.isCompatible<int>(true));
    TEST(u.isCompatible<bool>(true) == true);
    TEST(u.isCompatible<fcf::Undefined>(true) == false);
    TEST(u.isCompatible<fcf::Null>(true) == false);
  }
  {
    fcf::Union u("-10");
    TEST((bool)u == true);
    TEST(u.isCompatible<int>(true));
    TEST(u.isCompatible<bool>(true) == true);
    TEST(u.isCompatible<fcf::Undefined>(true) == false);
    TEST(u.isCompatible<fcf::Null>(true) == false);
  }
  {
    fcf::Union u("true");
    TEST(u.isCompatible<bool>(true) == true);
    TEST(u.isCompatible<int>(true) == false);
    TEST(u.isCompatible<fcf::Undefined>(true) == false);
    TEST(u.isCompatible<fcf::Null>(true) == false);
  }
  {
    fcf::Union u("undefined");
    TEST(u.isCompatible<int>(true) == false);
    TEST(u.isCompatible<fcf::Undefined>(true) == true);
    TEST(u.isCompatible<fcf::Null>(true) == false);
  }
  {
    fcf::Union u("null");
    TEST(u.isCompatible<int>(true) == false);
    TEST(u.isCompatible<fcf::Undefined>(true) == false);
    TEST(u.isCompatible<fcf::Null>(true) == true);
    TEST(u.isCompatible<fcf::UnionVector>(true) == false);
  }
  {
    fcf::Union u("[1, 2, 3,]");
    TEST(u.isCompatible<int>(true) == false);
    TEST(u.isCompatible<fcf::Undefined>(true) == false);
    TEST(u.isCompatible<fcf::Null>(true) == false);
    TEST(u.isCompatible<fcf::UnionVector>(true) == true);
  }  
}
