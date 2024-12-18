#define FCF_UNION_IMPLEMENTATION

#include <fcfUnion/union.hpp>
#include <iostream>
#include <sstream>

int main(int a_argc, const char* a_argv[]){

  //----------------------------
  // Example of a simple conversion
  //----------------------------
  fcf::Union  uDoubleValue(3.14);
  double      dDoubleValue = (double)uDoubleValue;
  int         iDoubleValue = (int)uDoubleValue;
  std::string sDoubleValue = (std::string)uDoubleValue;
  //
  // Result
  // StdOut: Union value: 3.140000; doubel value: 3.14; int value: 3; std::string: 3.140000
  // 
  std::cout << "Union value: "  << uDoubleValue << "; doubel value: " << dDoubleValue << "; int value: " << iDoubleValue << "; std::string: " << sDoubleValue << std::endl;

  //----------------------------
  // Example of JSON output
  //----------------------------
  fcf::Union  uMap(fcf::UT_MAP);
  uMap["key1"] = "value1";
  uMap["key2"] = fcf::Union(fcf::UT_VECTOR);
  uMap["key2"].insert(1);
  uMap["key2"].insert(2);
  uMap["key2"].insert(3);
  uMap["key3"] = "value3";

  fcf::UnionStringifyOptions so;
  so.friendly = true;
  so.tab = "    ";
  std::stringstream ss;
  uMap.stringify(ss, so);

  //
  // Result
  // StdOut: JSON for map value:
  // StdOut: {   
  // StdOut:   "key1": "value1",
  // StdOut:   "key2": [1, 2, 3],
  // StdOut:   "key3": "value3"
  // StdOut: }
  std::cout << std::endl;
  std::cout << "JSON for map value: \n" << ss.str() << std::endl;

  //----------------------------
  // JSON parsing example
  //----------------------------
  std::stringstream ssource(
      "{\n"
      "  // First value\n"
      "  1: \"value1\", \n"
      "\n"
      "  /* The second value in the sequence */ \n"
      "  key9: \"value9\", \n"
      "\n"
      "  \"key3\": [1,2,3], \n"
      "}"
  );
  fcf::Union  uJson;
  uJson.parse(ssource);
  //
  // Result
  // StdOut: The object obtained from JSON is a map: 1
  // StdOut: The object contains 3 values:
  // StdOut:   [1]: value1
  // StdOut:   [key9]: value9
  // StdOut:   [key3]: [1, 2, 3]
  // StdOut:
  // StdOut:   First value from object 'key3': 1
  // StdOut:   The 'key3' object contains 3 values:
  // StdOut:      [0]: 1
  // StdOut:      [1]: 2
  // StdOut:      [2]: 3
  std::cout << std::endl;
  std::cout << "The object obtained from JSON is a map: " << uJson.is<fcf::UnionMap>() << std::endl;
  std::cout << "The object contains " << uJson.size() << " values:" << std::endl;
  // We use iteration with preservation of the original order (obegin/oend)
  for(fcf::Union::iterator it = uJson.obegin(); it != uJson.oend(); ++it) {
    std::cout << "  [" << it.key() << "]: " << it.value() << std::endl;
  }
  // Access to vector elements is similar
  std::cout << std::endl;
  std::cout << "  First value from object 'key3': " << uJson["key3"][0] << std::endl;
  std::cout << "  The 'key3' object contains " << uJson["key3"].size() << " values:" << std::endl;
  for(fcf::Union::iterator it = uJson["key3"].obegin(); it != uJson["key3"].oend(); ++it) {
    std::cout << "        [" << it.key() << "]: " << it.value() << std::endl;
  }
  return 0;
}

