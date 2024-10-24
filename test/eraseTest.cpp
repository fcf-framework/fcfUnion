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

void eraseTest(){
  {
    fcf::Union u(fcf::UT_VECTOR);
    u.insert("value1");
    u.insert("value2");
    u.insert("value3");
    u.erase(1);
    TEST(u.get<std::string>() == "[\"value1\", \"value3\"]");
  }
  {
    fcf::Union u(fcf::UT_MAP);
    u.insert("value1");
    u.insert("value2");
    u.insert("value3");
    u.erase(1);
    TEST(u.get<std::string>() == "{\"0\": \"value1\", \"2\": \"value3\"}");
  }
  {
    fcf::Union u(fcf::UT_MAP);
    u.insert("key_g1", "value1");
    u.insert("key_r2", "value2");
    u.insert("key_p3", "value3");
    u.insert("key_a4", "value4");

    fcf::Union::iterator it = u.begin();
    while(it != u.end()){
      if (it.key() == "key_r2"){
        u.erase((it++).key());
      } else {
        ++it;
      }
    }
    TEST(u.get<std::string>() == "{\"key_g1\": \"value1\", \"key_p3\": \"value3\", \"key_a4\": \"value4\"}");
  }
  {
    fcf::Union u(fcf::UT_MAP);
    u.insert("key_g1", "value1");
    u.insert("key_r2", "value2");
    u.insert("key_p3", "value3");
    u.insert("key_a4", "value4");

    fcf::Union::iterator it = u.obegin();
    while(it != u.oend()){
      fcf::Union::iterator eit(it);
      if (it.key() == "key_r2"){
        u.erase((it++).key());
      } else {
        ++it;
      }
    }
    TEST(u.get<std::string>() == "{\"key_g1\": \"value1\", \"key_p3\": \"value3\", \"key_a4\": \"value4\"}");
  }
  {
    fcf::Union u(fcf::UT_MAP);
    u.insert("key_g1", "value1");
    u.insert("key_r2", "value2");
    u.insert("key_p3", "value3");
    u.insert("key_a4", "value4");

    fcf::Union::const_iterator it = u.cobegin();
    while(it != u.coend()){
      if (it.key() == "key_r2"){
        u.erase((it++).key());
      } else {
        ++it;
      }
    }
    TEST(u.get<std::string>() == "{\"key_g1\": \"value1\", \"key_p3\": \"value3\", \"key_a4\": \"value4\"}");
  }

  {
    fcf::Union u(fcf::UT_MAP);
    u.insert("key_g1", "value1");
    u.insert("key_r2", "value2");
    u.insert("key_p3", "value3");
    u.insert("key_a4", "value4");

    bool rm = false;
    for(fcf::Union::iterator it = u.begin(); it != u.end();) {
      if (it.key() == "key_g1"){
        rm = true;
      }
      if (rm) {
        it = u.erase(it);
      } else {
        ++it;
      }
    }
    TEST(u.get<std::string>() == "{\"key_a4\": \"value4\"}");
  }
  {
    fcf::Union u(fcf::UT_MAP);
    u.insert("key_g1", "value1");
    u.insert("key_r2", "value2");
    u.insert("key_p3", "value3");
    u.insert("key_a4", "value4");

    bool rm = false;
    for(fcf::Union::iterator it = u.obegin(); it != u.oend();) {
      if (it.key() == "key_r2"){
        rm = true;
      }
      if (rm) {
        it = u.erase(it);
      } else {
        ++it;
      }
    }
    TEST(u.get<std::string>() == "{\"key_g1\": \"value1\"}");
  }
  {
    fcf::Union u(fcf::UT_VECTOR);
    u.insert("value1");
    u.insert("value2");
    u.insert("value3");
    u.insert("value4");

    bool rm = false;
    for(fcf::Union::iterator it = u.begin(); it != u.end();) {
      if (it.key() == 1){
        rm = true;
      }
      if (rm) {
        it = u.erase(it);
      } else {
        ++it;
      }
    }
    TEST(u.get<std::string>() == "[\"value1\"]");
  }  
  
}
