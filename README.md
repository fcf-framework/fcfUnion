
## C++11 FCF::Union bidirectional conversion union type (With JSON Support)

### Content

* [Brief description](#short_description)

<a href="https://nowpayments.io/donation?api_key=9E422CC-88M4J1V-H0S6M1Y-TZ7FPYX" target="_blank">
 <img src="https://nowpayments.io/images/embeds/donation-button-white.svg" alt="Cryptocurrency & Bitcoin donation button by NOWPayments" style="    max-width: 100%;">
</a>


<a name="short_description"></a>
### Brief description

The class provides a simple interface for combining several basic types into a single union.

An fcf::Union object can store one of the following types:
- `fcf::Undefined (Numerical equivalent: fcf::UT_UNDEFINED),`
- `fcf::Null (Numerical equivalent: fcf::UT_NULL),`
- `int (Numerical equivalent: fcf::UT_INT),`
- `unsigned int (Numerical equivalent: fcf::UT_UINT),`
- `long long (Numerical equivalent: fcf::UT_LONGLONG),`
- `unsigned long long (Numerical equivalent: fcf::UT_ULONGLONG),`
- `double (Numerical equivalent: fcf::UT_DOUBLE),`
- `bool (Numerical equivalent: fcf::UT_BOOL),`
- `std::string (Numerical equivalent: fcf::UT_STRING),`
- `fcf::UnionVector /*std::vector<fcf::Union>*/ (Numerical equivalent: fcf::UT_VECTOR),`
- `fcf::UnionMap /*std::map<fcf::Union, fcf::Union, fcf::MapLess>*/ (Numerical equivalent: fcf::UT_MAP),`

The library is distributed in the form of header files and to start using it, you need to declare the implementation in one cpp file. This is done by including the union.hpp file with the declared `FCF_UNION_IMPLEMENTATION` macro, in other files with the included union.hpp, the `FCF_UNION_IMPLEMENTATION` macro is not needed

```c++
#define FCF_UNION_IMPLEMENTATION

#include <union/union.hpp>
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
  std::cout << "Union value: "  << uDoubleValue
            << "; doubel value: " << dDoubleValue
            << "; int value: " << iDoubleValue
            << "; std::string: " << sDoubleValue << std::endl;

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

  fcf::StringifyOptions so;
  so.friendly = true;
  so.tab = "    ";
  std::stringstream ss;
  uMap.stringify(ss, so);

  //
  // Result
  // StdOut: JSON for map value:
  // StdOut: {   
  // StdOut:     "key1": "value1",
  // StdOut:     "key2": [1, 2, 3],
  // StdOut:     "key3": "value3"
  // StdOut: }
  std::cout << std::endl;
  std::cout << "JSON for map value: \n" << ss.str() << std::endl;

  //----------------------------
  // JSON/JSObject parsing example
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
  for(fcf::Union::iterator it = uJson["key3"].begin(); it != uJson["key3"].end(); ++it) {
    std::cout << "        [" << it.key() << "]: " << it.value() << std::endl;
  }
  return 0;
}

```







