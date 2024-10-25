#include <iostream>
#include <fcfUnion/union.hpp>

#ifdef WIN32
__declspec(dllexport) void testInnerSharedFunc() {
#else
void testInnerSharedFunc() {
#endif
  fcf::Union u(1);
  u = "Call from  shred library...";
  std::cout  << u << std::endl;
}


