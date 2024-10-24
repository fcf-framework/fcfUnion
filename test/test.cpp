#include <union/union.hpp>
#include <iostream>
#include <sstream>

#ifndef TEST
  #define TEST_STRINGIZE(x) TEST_STRINGIZE2(x)
  #define TEST_STRINGIZE2(x) #x
  #define TEST_LINE_STRING TEST_STRINGIZE(__LINE__)
  #define TEST(exp) \
    if (!(exp)){ \
      throw std::runtime_error(std::string() + "Test error: " + #exp + "  [FILE: " + __FILE__ + ":" + TEST_LINE_STRING + "]");\
    }
#endif


void readStringVectorTest(){
  
  {
    fcf::Union v;
    v.set(std::string("[]"));
    fcf::UnionVector vector = v.get<fcf::UnionVector>();
    if (vector.size() != 0) {
      throw std::runtime_error("Test error: vector.size() != 0");
    }
  }
  {
    fcf::Union v;
    v.set(std::string("[-1.1]"));
    fcf::UnionVector vector = v.get<fcf::UnionVector>();
    if (vector.size() != 1) {
      throw std::runtime_error("Test error: vector.size() != 1; source: [-1.1]");
    }
    if (vector[0].get<double>() != -1.1){
      throw std::runtime_error("Test error: vector[0].get<double>() != -1; source: [-1.1]");
    }
  }
  {
    fcf::Union v;
    std::string source("[5, false, true, 'Test']");
    v.set(source);
    fcf::UnionVector vector = v.get<fcf::UnionVector>();
    if (vector.size() != 4) {
      throw std::runtime_error(std::string() + "Test error: vector.size() != 4; source: " + source);
    }
    if (vector[0].get<int>() != 5){
      throw std::runtime_error(std::string() + "Test error: vector[0].get<int>() != 5; source: " + source);    
    }
    if (vector[1].get<bool>() != false){
      throw std::runtime_error(std::string() + "Test error: vector[1].get<bool>() != false; source: " + source);    
    }
    if (vector[2].get<bool>() != true){
      throw std::runtime_error(std::string() + "Test error: vector[2].get<bool>() != true; source: " + source);    
    }
  }
  {
    fcf::Union v;
    std::string source("[\"Test\\\"\", []]");
    v.set(source);
    fcf::UnionVector vector = v.get<fcf::UnionVector>();
    if (vector.size() != 2) {
      throw std::runtime_error(std::string() + "Test error: vector.size() != 2; source: " + source);
    }
    if (vector[0].get<std::string>() != "Test\""){
      throw std::runtime_error(std::string() + "Test error: vector[0].get<std::string>() != \"Test\\\"\"; source: " + source);    
    }
    if (!vector[1].is<fcf::UnionVector>()){
      throw std::runtime_error(std::string() + "Test error: !vector[1].is<fcf::UnionVector>(); source: " + source);    
    }
    if (vector[1].ref<fcf::UnionVector>().size() != 0){
      throw std::runtime_error(std::string() + "Test error: vector[1].ref<fcf::UnionVector>().size() != 0; source: " + source);    
    }
  }
  {
    fcf::Union v;
    std::string source("[\"Test\\\"\\\\\", [3], false]");
    v.set(source);
    fcf::UnionVector vector = v.get<fcf::UnionVector>();
    TEST(vector.size() == 3);
    TEST(vector[0].get<std::string>() == "Test\"\\");
    TEST(vector[1].is<fcf::UnionVector>());
    TEST(vector[1].ref<fcf::UnionVector>().size() == 1);
    TEST(vector[1].ref<fcf::UnionVector>()[0].isCompatible<unsigned int>());
    TEST(vector[1].ref<fcf::UnionVector>()[0].get<int>() == 3);
    fcf::Union rv(vector);
    TEST(rv.get<std::string>() == source);
  }
}

void unionCopyTest() {
  {
    fcf::UnionVector uv;
    fcf::Union u1(uv);
    fcf::Union u2;
    TEST(u1.is<fcf::UnionVector>());
    u1.ref<fcf::UnionVector>().push_back(1);
    u2 = u1;
    TEST(u2.is<fcf::UnionVector>());
    TEST(u2.ref<fcf::UnionVector>().size() == 1);
    TEST(u2.ref<fcf::UnionVector>()[0].get<int>() == 1);
  }
  {
    fcf::Union u1(fcf::UnionVector{});
    u1.ref<fcf::UnionVector>().push_back(1);
    u1.ref<fcf::UnionVector>().push_back(2);
    TEST(u1.ref<fcf::UnionVector>().size() == 2);
    u1.ref<int>() = 7;
    TEST(u1.get<int>() == 7);
    TEST(u1.ref<int>() == 7);
    TEST(u1.ref<fcf::UnionVector>().size() == 0);

    TEST(u1.ref<int>() == 0);
    u1.ref<int>() = 7;
    TEST(u1.ref<unsigned int>() == 7);
    TEST(u1.ref<std::string>() == "7");
    TEST(u1.is<std::string>());
    TEST(u1.ref<unsigned int>() == 7);
  }

}


void unionMapTest() {
  {
    std::string source("['String', 2]");
    fcf::Union v;
    v.set(std::string(source));
    v.set(v.get<fcf::UnionVector>());

    fcf::UnionMap um = v.get<fcf::UnionMap>();
    fcf::Union mv(um);
    TEST(mv.get<std::string>() == "{\"0\": \"String\", \"1\": 2}");
    fcf::Union uv(mv.get<fcf::UnionVector>());
    if (uv.get<std::string>() != "[\"String\", 2]"){
      throw std::runtime_error(std::string() + "Test error: uv.get<std::string>() != \"[\\\"String\\\", 2]\"; source: " + source);
    }
  }

  {
    std::string source("{\"key1\": 1, 3: 'test', \"vector\": [1,2,{'test': 'info'}] }");
    fcf::Union v;
    v.set(source);
    v.set(v.get<fcf::UnionMap>());
    std::string json(v.get<std::string>());
    TEST(json == "{\"key1\": 1, \"3\": \"test\", \"vector\": [1, 2, {\"test\": \"info\"}]}");
  }

  {
    std::string source("{test: 1}");
    fcf::Union v;
    v.set(source);
    v.set(v.get<fcf::UnionMap>());
    std::string json(v.get<std::string>());
    TEST(json == "{\"test\": 1}");
  }

  {
    std::string source("{1: 1, 2: [1,2,3]}");
    fcf::Union v;
    v.set(source);
    v.set(v.get<fcf::UnionMap>());
    std::string json(v.get<std::string>());
    TEST(json == "{\"1\": 1, \"2\": [1, 2, 3]}");
  }
  
  {
    fcf::Union u(fcf::UnionMap{});
    u["value"] = fcf::Union(fcf::UnionMap{});
    u["value"]["diff"] = fcf::Union(fcf::UnionMap{});
    u["value"]["diff"][1] = "test1";
    u["value"]["diff"][2] = "test2";
    fcf::StringifyOptions so;
    so.friendly = true;
    std::stringstream ss;
    u.stringify(ss, so);
    std::string expeted = "{\n"
    "  \"value\": {\n"
    "    \"diff\": {\n"
    "      \"1\": \"test1\",\n"
    "      \"2\": \"test2\"\n"
    "    }\n"
    "  }\n"
    "}"
    ;
    TEST(ss.str() == expeted);

    fcf::Union u2;
    u2.parse(ss);
    std::stringstream ss2;
    u2.stringify(ss2, so);

    fcf::Union u3;
    u3.parse(ss);

    std::stringstream ss3;
    u3.stringify(ss3, so);

    TEST(ss2.str() == expeted);
    TEST(ss3.str() == expeted);

  }

}

void unionIteratorTest() {
  {
    fcf::Union u(fcf::UnionVector{});
    u.ref<fcf::UnionVector>().push_back(1);
    u.ref<fcf::UnionVector>().push_back(2);
    size_t count1 = 0;
    size_t count2 = 0;
    for(fcf::Union::iterator it = u.begin(); it != u.end(); ++it){
      ++count1;
      count2 += (int)it.value();
    }
    TEST(count1 == 2);
    TEST(count2 == 3);
    TEST(u.at(0) == 1);
    TEST(u.at(1) == 2);
    TEST(u[0] == 1);
    TEST(u[1] == 2);
    u.insert(31);
    TEST(u[2] == 31);
    TEST(u.size() == 3);
    }

  {
    fcf::Union u(fcf::UnionVector{});
    u.ref<fcf::UnionVector>().push_back(1);
    u.ref<fcf::UnionVector>().push_back(2);
    size_t count1 = 0;
    size_t count2 = 0;
    size_t count3 = 0;
    for(fcf::Union::const_iterator it = u.cbegin(); it != u.cend(); ++it){
      ++count1;
      count2 += (int)it.value();
      count3 += (int)it.key();
    }
    TEST(count1 == 2);
    TEST(count2 == 3);
    TEST(count3 == 1);
  }

  {
    fcf::Union u(fcf::UnionMap{});
    u.ref<fcf::UnionMap>()["key1"] = 1;
    u.ref<fcf::UnionMap>()["key2"] = 2;
    size_t count1 = 0;
    size_t count2 = 0;
    for(fcf::Union::iterator it = u.begin(); it != u.end(); ++it){
      ++count1;
      count2 += (int)it.value();
    }
    TEST(count1 == 2);
    TEST(count2 == 3);
    TEST(u.at("key1") == 1);
    TEST(u.at("key2") == 2);
    u["key3"] = 3;
    TEST(u.at("key3") == 3);
    TEST(u.size() == 3);
    u.insert(31);
    TEST(u.at("3") == 31);
  }

}

void unionTypeTest() {
  {
    fcf::Union u;
    u.set<int>();
    u = 1;
    int ivalue = (int)u + 1;
    TEST((std::string)u == "1");
    TEST((int)u == 1);
    TEST(((std::string)u.get<std::string>()) == std::string("1"));

    u = (const char*)"test";
    TEST((std::string)u.get<std::string>() == "test");
    TEST(u == "test");

    u = (char*)"test2";
    TEST((std::string)u.get<std::string>() == "test2");
    TEST(u == "test2");

    u = "213";
    TEST((std::string)u.get<std::string>() == std::string("213"));
    TEST(u == 213);
    TEST(u != 214);

    TEST(u.lessStr(fcf::Union(530)));

  }
  {
    fcf::Union u1(fcf::undefined);
    TEST((fcf::Undefined)u1 == fcf::undefined);
    TEST(!((fcf::Undefined)u1 != fcf::undefined));
    TEST((std::string)u1 == "undefined");
    fcf::Union u2(3);
    TEST((fcf::Undefined)u2 == fcf::undefined);
  }
  {
    fcf::Union uv("{ 1: undefined }");
    uv = uv.get<fcf::UnionMap>();
    TEST((std::string)uv == "{\"1\": undefined}")
  }
  {
    fcf::Union uv("[1,'123', undefined]");
    uv = uv.get<fcf::UnionVector>();
    TEST((std::string)uv == "[1, \"123\", undefined]")
  }
  {
    fcf::Union uv(0);
    fcf::Union e = uv.get<fcf::Undefined>();
  }
  {
    fcf::Union uv(fcf::undefined);
    bool error = false;
    try {
      int e = uv.get<int>();
    } catch(std::exception&){
      error=  true;
    }
    TEST(error == true);
  }
  {
    fcf::Union uv((unsigned int)0);
    fcf::Union e = uv.get<fcf::Undefined>();
  }
  {
    fcf::Union uv(fcf::undefined);
    bool error = false;
    try {
      unsigned int e = uv.get<unsigned int>();
    } catch(std::exception&){
      error=  true;
    }
    TEST(error == true);
  }
  {
    fcf::Union uv(fcf::UnionVector{});
    fcf::StringifyOptions so;
    so.friendly = true;
    std::string str;

    uv.stringify(str, so);
    TEST(str == "[]")

    uv.ref<fcf::UnionVector>().push_back(1);
    uv.stringify(str, so);
    TEST(str == "[1]")

    uv.ref<fcf::UnionVector>().push_back(2);
    uv.stringify(str, so);
    TEST(str == "[1, 2]")
  }
  {
    fcf::Union uv(fcf::UnionVector{});
    fcf::StringifyOptions so;
    so.friendly = true;
    std::string str;

    uv.stringify(str, so);
    TEST(str == "[]")

    uv.ref<fcf::UnionVector>().push_back(1);
    uv.stringify(str, so);
    TEST(str == "[1]")

    uv.ref<fcf::UnionVector>().push_back(fcf::undefined);
    uv.stringify(str, so);
    TEST(str == "[1, undefined]")
  }
  {
    std::string expected;
    fcf::Union uv(fcf::UnionVector{});
    fcf::StringifyOptions so;
    so.friendly = true;
    std::string str;

    uv.stringify(str, so);
    expected =  "[]";
    TEST(expected == str);


    uv.ref<fcf::UnionVector>().push_back(1);
    uv.stringify(str, so);
    expected =  "[1]";
    TEST(expected == str);

    uv.ref<fcf::UnionVector>().push_back(fcf::UnionMap{});
    uv.stringify(str, so);
    expected =  "[\n"
                "  1,\n"
                "  {}\n"
                "]";
    TEST(expected == str);


    uv.ref<fcf::UnionVector>().back().ref<fcf::UnionMap>()["Test1"] = 1;
    uv.stringify(str, so);
    expected =  "[\n"
                "  1,\n"
                "  {\n"
                "    \"Test1\": 1\n"
                "  }\n"
                "]";
    TEST(expected == str);

    uv.ref<fcf::UnionVector>().back().ref<fcf::UnionMap>()["Test2"] = "Second";
    uv.stringify(str, so);
    expected =  "[\n"
                "  1,\n"
                "  {\n"
                "    \"Test1\": 1,\n"
                "    \"Test2\": \"Second\"\n"
                "  }\n"
                "]";
    TEST(expected == str);
  }

  {
    std::string expected;
    std::stringstream ss;
    fcf::Union uv(fcf::UnionVector{});
    fcf::StringifyOptions so;
    so.friendly = true;

    uv.stringify(ss, so);
    expected = "[]";
    TEST(ss.str() == expected);

    uv.ref<fcf::UnionVector>().push_back(1);
    ss.str("");
    uv.stringify(ss, so);
    expected = "[1]";
    TEST(ss.str() == expected);

    uv.ref<fcf::UnionVector>().push_back(fcf::UnionMap{});
    ss.str("");
    uv.stringify(ss, so);
    expected = "[\n"
               "  1,\n"
               "  {}\n"
               "]";
    TEST(ss.str() == expected);

    uv.ref<fcf::UnionVector>().back().ref<fcf::UnionMap>()["Test1"] = 1;
    ss.str("");
    uv.stringify(ss, so);
    expected = "[\n"
               "  1,\n"
               "  {\n"
               "    \"Test1\": 1\n"
               "  }\n"
               "]";
    TEST(ss.str() == expected);
  }

  {
    fcf::StringifyOptions so;
    fcf::Union uv("String");
    std::string output;
    uv.stringify(output, so);
    TEST(output == "\"String\"");

    std::stringstream ss;
    ss << uv;
    TEST(ss.str() == "String");
  }

  {
    fcf::Union u;
    u.parse("[1]");
    TEST(u.is<fcf::UnionVector>());
    TEST(u.ref<fcf::UnionVector>().size() == 1);
    TEST(u.ref<fcf::UnionVector>()[0].get<int>() == 1);
  }
  {
    fcf::Union u;
    u.parse("\"String\"");
    TEST(u.is<std::string>());
    TEST(u.ref<std::string>() == "String");
  }
  
 
}

void unionCommentTest(){
 
  { 
    std::string source =
      "[\n"
      "  // Test line\n"
      "  1,//Test line 2\n"
      "]";
    fcf::Union u;
    u.parse(source);
    TEST(u[0] == 1);
    TEST(u.size() == 1);
  }
  { 
    std::string source =
      "[\n"
      "  /*\n"
      "  Multiline Comment\n"
      "  */"
      "  1,//Test line 2\n"
      "  2,\n"
      "  '// /*info*/'"
      "]";
    fcf::Union u;
    u.parse(source);
    TEST(u.size() == 3);
    TEST(u[0] == 1);
    TEST(u[1] == 2);
    TEST(u[2] == "// /*info*/");
  }
  { 
    std::string source =
      "[\n"
      "  /*\n"
      "  Multiline Comment\n"
      "  */"
      "  1,//Test line 2\n"
      "  2,\n"
      "  '// /*i\\'nfo*/'"
      "]";
    fcf::Union u;
    u.parse(source);
    TEST(u.size() == 3);
    TEST(u[0] == 1);
    TEST(u[1] == 2);
    TEST(u[2] == "// /*i\'nfo*/");
  }

  {
    fcf::Union u(fcf::UT_MAP);
    u.insert("first");
    u.insert("1", "1");
    u.insert("3", "1");
    u.insert("002", "second");
    u.at("003") = "3";
    TEST(u["003"] == 3);
    TEST(u.size() == 5);
  }
  
}
