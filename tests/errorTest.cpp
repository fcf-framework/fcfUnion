#include "test.hpp"
#include <stdexcept>
#include <fcfUnion/union.hpp>

void errorTest() {

  {
    fcf::Union u;
    int l = -2;
    int c = -2;
    try {
      u.parse(
        "[\n"
       "  2iia,\n"
        "]"
      );
    } catch(const fcf::UnionException& e){
      l = (int)e.line();
      c = (int)e.column();
    }
    TEST(l == 2);
    TEST(c == 4);
  }

  {
    fcf::Union u;
    int l = -2;
    int c = -2;
    try {
      u.parse(
        "[\n"
       "  2,\n"
      "  {\n"
      "    'test': 1,\n"
      "    'test'1: 1,\n"
      "  },\n"
        "]"
      );
    } catch(const fcf::UnionException& e){
      TEST(std::string(e.what()) == "Incorrect map format [line: 5; column: 11]");
      TEST(std::string(e.swhat()) == "Incorrect map format");
      l = (int)e.line();
      c = (int)e.column();
    }
    TEST(l == 5);
    TEST(c == 11);
  }

  {
    fcf::Union u;
    int l = -2;
    int c = -2;
    try {
      u = 
        "[\n"
       "  2,\n"
      "  {\n"
      "    'test': 1,\n"
      "    'test'1: 1,\n"
      "  },\n"
        "]"
      ;
      u.get<fcf::UnionVector>();
    } catch(const fcf::UnionException& e){
      TEST(std::string(e.what()) == "Incorrect map format");
      TEST(std::string(e.swhat()) == "Incorrect map format");
      l = (int)e.line();
      c = (int)e.column();
    }
    TEST(l == SIZE_MAX);
    TEST(c == SIZE_MAX);
  }

  {
    fcf::Union u;
    int l = -2;
    int c = -2;
    try {
      u.parse("");
    } catch(const fcf::UnionException& e){
      TEST(std::string(e.what()) == "There is not enough data in the buffer to parse the value");
      TEST(std::string(e.swhat()) == "There is not enough data in the buffer to parse the value");
      l = (int)e.line();
      c = (int)e.column();
    }
    TEST(l == SIZE_MAX);
    TEST(c == SIZE_MAX);
  }

  {
    fcf::Union u;
    int l = -2;
    int c = -2;
    try {
      u.parse("asd");
    } catch(const fcf::UnionException& e){
      l = (int)e.line();
      c = (int)e.column();
    }
    TEST(l == 1);
    TEST(c == 1);
  }

  {
    fcf::Union u;
    int l = -2;
    int c = -2;
    try {
      u = 1;
      u.get<fcf::UnionMap>();
    } catch(const fcf::UnionException& e) {
      l = (int)e.line();
      c = (int)e.column();
    }
    TEST(l == SIZE_MAX);
    TEST(c == SIZE_MAX);
  }


  {
    fcf::Union u("");
    int l = -2;
    int c = -2;
    try {
      u.get<int>();
    } catch(const fcf::UnionException& e) {
      l = (int)e.line();
      c = (int)e.column();
    }
    TEST(l == SIZE_MAX);
    TEST(c == SIZE_MAX);
  }

  {
    fcf::Union u("1");
    TEST(u.get<int>() == 1);
  }



}

