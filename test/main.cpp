#ifndef FCF_UNION_SHARED
  #define FCF_UNION_IMPLEMENTATION
#endif
#include <union/union.hpp>
#include <iostream>

void unionCopyTest();
void readStringVectorTest();
void unionMapTest();
void unionTypeTest();
void unionIteratorTest();
void unionCommentTest();
void cmpTest();
void orderTest();
void isTest();
void findTest();
void eraseTest();

int main(int a_argc, char* a_argv[]){

  #ifdef FCF_UNION_SHARED
    #ifdef WIN32
    __declspec(dllimport) void testInnerSharedFunc();
    #else
      void testInnerSharedFunc();
    #endif
    testInnerSharedFunc();
  #endif

  cmpTest();
  unionIteratorTest();
  unionCopyTest();
  readStringVectorTest();
  unionMapTest();
  unionTypeTest();
  unionCommentTest();
  orderTest();
  isTest();
  findTest();
  eraseTest();

  std::cout << "Complete!" << std::endl;
  return 0;
}
