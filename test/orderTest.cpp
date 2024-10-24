#include <iostream>
#include <union/union.hpp>

#ifndef TEST
  #define TEST_STRINGIZE(x) TEST_STRINGIZE2(x)
  #define TEST_STRINGIZE2(x) #x
  #define TEST_LINE_STRING TEST_STRINGIZE(__LINE__)
  #define TEST(exp) \
    if (!(exp)){ \
      throw std::runtime_error(std::string() + "Test error: " + #exp + "  [FILE: " + __FILE__ + ":" + TEST_LINE_STRING + "]");\
    }
#endif


void orderTest() {
  fcf::Union u(fcf::UnionMap{});
  u["7-first"] = 1;
  u["5-second"] = 2;
  u["3-third"] = 3;

  TEST(u["7-first"].order == 1 && (int)u["7-first"] == 1);
  TEST(u["5-second"].order == 2 && (int)u["5-second"] == 2);
  TEST(u["3-third"].order == 3 && (int)u["3-third"] == 3);

  {
    const char* expected[] = {"3-third", "5-second", "7-first"};
    size_t i = 0;
    for(auto it = u.begin(); it != u.end(); ++it){
      TEST(it.key() == expected[i]);
      ++i;
    }
  }

  {
    const char* expected[] = {"7-first", "5-second", "3-third"};
    size_t i = 0;
    for(auto it = u.obegin(); it != u.oend(); ++it){
      TEST(it.key() == expected[i]);
      ++i;
    }
  }

  {
    const char* expected[] = {"7-first", "5-second", "3-third"};
    size_t i = 0;
    for(auto it = u.cobegin(); it != u.coend(); ++it){
      TEST(it.key() == expected[i]);
      ++i;
    }
  }

  {
    fcf::Union u1(fcf::UT_MAP);
    u1["3"] = 1;
    u1["1"] = 2;
    u1["2"] = 3;
    TEST(u1.get<std::string>() == "{\"3\": 1, \"1\": 2, \"2\": 3}");
    TEST(fcf::Union(u1.get<fcf::UnionVector>()).get<std::string>() == "[1, 2, 3]");
    u1["3"].order = 0;
    u1["1"].order = 0;
    u1["2"].order = 0;
    TEST(u1.get<std::string>() == "{\"1\": 2, \"2\": 3, \"3\": 1}");
    TEST(fcf::Union(u1.get<fcf::UnionVector>()).get<std::string>() == "[2, 3, 1]");
  }
}
