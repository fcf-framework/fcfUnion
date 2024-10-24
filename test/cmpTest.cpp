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

void cmpTest() {
  {
    fcf::Union u;
    u = "test";
    TEST(u == "test");
    TEST(u != "test1");
    u = 1;
    TEST(u == (unsigned int)1);
    TEST(u == (double)1);
    TEST(u != (double)1.1);
    u = fcf::UnionVector{};
    TEST(u != (double)1.1);
    TEST(u != "[]");

    TEST(!u.equal("[]", true));
    TEST(!u.equal("[]", false));
    u = 1;
    TEST(!u.equal(1.0, true));
    TEST(u.equal(1.0, false));
    TEST(u.equal("1", false));
    
    u = 999000999000;
    TEST(u == 999000999000);
    TEST(u == "999000999000");
  }
  {
    char s[2] = {0 ,0};
    const char* cs = "";
    TEST(fcf::Union((int)1) == fcf::Union((double)1.0));
    TEST(fcf::Union(1) == fcf::Union("1"));
    TEST(fcf::Union(fcf::undefined) == fcf::undefined);
    TEST(fcf::Union(fcf::null) == fcf::null);
    TEST(fcf::Union(fcf::undefined) != fcf::null);
    TEST(fcf::Union(fcf::null) != fcf::undefined);
    TEST(fcf::Union(fcf::undefined) != "");
    TEST(fcf::Union(fcf::undefined) != s);
    TEST(fcf::Union(fcf::undefined) != cs);
    TEST(fcf::Union(fcf::undefined) != 0);
    TEST(fcf::Union(fcf::undefined) != 1);
  }
  {
    fcf::Union u(false);
    TEST(u == false);
    TEST(u != true);
    TEST(u == "false");
    TEST(u != "true");
    TEST(u == "0");
    TEST(u != "1");
    TEST(u != "3");
  }
  {
    fcf::Union u(true);
    TEST(u != fcf::undefined);
    TEST(u != false);
    TEST(u == true);
    TEST(u != "false");
    TEST(u == "true");
    TEST(u != "0");
    TEST(u == "1");
    TEST(u == "3");
  }
  
  {
    fcf::Union u1("test1");
    fcf::Union u2("test2");
    TEST(u1.lessStr(u2));
    TEST(!u2.lessStr(u1));
  }
  
  {
    fcf::Union u1("key1");
    fcf::Union u2("key2");
    fcf::Union u3(31);
    TEST(u1.lessStr(u2));
    TEST(!u2.lessStr(u1));
    TEST(!u1.lessStr(u3));
    TEST(!u2.lessStr(u3));
    TEST(u3.lessStr(u1));
    TEST(u3.lessStr(u2));
  }

  {
    fcf::Union u1("key1");
    fcf::Union u2("key2");
    fcf::Union u3("key3");
    fcf::Union u4(3);
    TEST(!u1.lessStr(u1));
    TEST(u1.lessStr(u2));
    TEST(u1.lessStr(u3));
    TEST(!u1.lessStr(u4));

    TEST(!u2.lessStr(u2));
    TEST(!u2.lessStr(u1));
    TEST(u2.lessStr(u3));
    TEST(!u2.lessStr(u4));

    TEST(!u3.lessStr(u3));
    TEST(!u3.lessStr(u1));
    TEST(!u3.lessStr(u2));
    TEST(!u3.lessStr(u4));

    TEST(!u4.lessStr(u4));
    TEST(u4.lessStr(u1));
    TEST(u4.lessStr(u2));
    TEST(u4.lessStr(u3));
  }
  {
    char str[] = "key1";
    fcf::Union u1(&str[0]);
    TEST(u1 == "key1");
  }
  {
    fcf::Union u1((size_t)1);
    TEST(u1 == 1);
  }
  
  
  
   
}
