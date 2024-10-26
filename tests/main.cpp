#define FCF_UNION_IMPLEMENTATION
#include <fcfUnion/union.hpp>
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
void streamTest();
void errorTest();

int main(int a_argc, char* a_argv[]){

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
  streamTest();
  errorTest();

  std::cout << "Complete!" << std::endl;
  return 0;
}
