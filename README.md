
## C++11 FCF::Union bidirectional conversion union type (With JSON Support)

### Content

* [Brief description](#short_description)
* [Including headers in an executable](#including_headers_executable)
* [Creating a shared library with the Union class](#creating_shared_library)
* [Description of types and methods](#description_types_and_methods)
	* [Enum fcf::UnionType](#description_union_type)
	* [Struct fcf::UnionStringifyOptions](#description_union_stringify_options)
	* [Class fcf::Union::iterator](#description_union__iterator)
	* [Class fcf::Union::const_iterator](#description_union__const_iterator)
	* [Class fcf::Union](#description_union)




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
- `fcf::UnionMap /*std::map<fcf::Union, fcf::Union, fcf::UnionMapLess>*/ (Numerical equivalent: fcf::UT_MAP),`

The library is distributed in the form of header files and to start using it, you need to declare the implementation in one cpp file. This is done by including the union.hpp file with the declared `FCF_UNION_IMPLEMENTATION` macro, in other files with the included union.hpp, the `FCF_UNION_IMPLEMENTATION` macro is not needed

```c++
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

  fcf::UnionStringifyOptions so;
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


<a name="including_headers_executable"></a>
### Including headers in an executable

As mentioned earlier, to include union.hpp it is necessary to declare the implementation (Include the union.hpp file with the declared `FCF_UNION_IMPLEMENTATION` macro). It is best to use a separate file for this, for example unionImpl.cpp. This will allow you to avoid recompiling union.hpp when editing the project. This approach is given as an example below:

**unionImpl.cpp file:**
```c++
#define FCF_UNION_IMPLEMENTATION
#include <fcfUnion/union.hpp>
```

**main.cpp file:**
```c++
#include <fcfUnion/union.hpp>
#include <iostream>
#include <fstream>

int main(int a_argc, char* a_argv[]){
  const char* confFilePath = "../test/config.json";
  std::ifstream ifs;
  ifs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    ifs.open(confFilePath);
    ifs.exceptions(std::ifstream::badbit);
  } catch(std::exception& e){
    std::cout << "ERROR: Failed open file '" << confFilePath << "'."  << std::endl;
    return 1;
  }
  fcf::Union u;
  u.parse(ifs);
  
  // Result
  // StdOut: The file contains a JSON object: 1
  // StdOut:   Fields:
  // StdOut:     [param1]: some value
  std::cout << "The file contains a JSON object: " << u.is<fcf::UnionMap>() << std::endl;
  std::cout << "  Fields: " << std::endl;
  std::cout << "    [param1]: " << u["param1"] << std::endl;
  return 0;
}
```

**CMakeLists.txt file**
```
cmake_minimum_required(VERSION 3.0)
project(example002)
include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/../../../)
add_executable(exemple002 unionImpl.cpp main.cpp)
```

<a name="creating_shared_library"></a>
### Creating a shared library with the Union class

You can also create a shared DLL/SO that exports the `fcf::Union` class.

1. To do this, the shared library project must declare the macro `FCF_UNION_EXPORT`
2. The library must declare an implementation of `fcf::Union` (The header union.hpp with the declared macro `FCF_UNION_IMPLEMENTATION` must be included).
3. In each project in which the library is connected, the macro FCF_UNION_IMPORT must be declared.

Below is a simple example of exporting and importing the fcf::Union class.

**executable/main.cpp file**
```c++
#include <fcfUnion/union.hpp>
#include <iostream>
#include <fstream>

int main(int a_argc, char* a_argv[]){
  std::ifstream ifs("config.json");
  fcf::Union u;
  u.parse(ifs);

  std::cout << "  The file contains a JSON object: " << u.is<fcf::UnionMap>() << std::endl;
  std::cout << "    Fields: " << std::endl;
  std::cout << "      [param1]: " << u["param1"] << std::endl;
  return 0;
}
```

**library/unionImpl.cpp file**
```c++
#define FCF_IMPLEMENTATION
#include <fcfUnion/union.hpp>
```

**CMakeLists.txt file**

In the project parameters, we declare the macros `FCF_UNION_IMPORT` and `FCF_UNION_EXPORT`

```
cmake_minimum_required(VERSION 3.0)
project(example003)
include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/../../../)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ../config.json DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})


add_library("example003l" SHARED ./library/unionImpl.cpp)
# Declare the FCF_UNION_EXPORT macro for the shared library
target_compile_definitions(example003l PRIVATE FCF_UNION_EXPORT)


add_executable(example003 ./executable/main.cpp)
# Declare the FCF_UNION_IMPORT macro for the executable
target_compile_definitions(example003 PRIVATE FCF_UNION_IMPORT)
target_link_libraries(example003 PRIVATE "example003l")
```

<a name="description_types_and_methods"></a>
### Description of types and methods

<a name="description_union_type"></a>
#### Enum fcf::UnionType

Enum of integer type identifiers stored by the `fcf::Union` class

* `UT_UNDEFINED` - `fcf::Undefined` type
* `UT_NULL` - `fcf::Null` type
* `UT_INT` - `int` type
* `UT_UINT` - `unsigned int` type
* `UT_LONGLONG` - `long long` type
* `UT_ULONGLONG` - `unsigned long long` type
* `UT_DOUBLE` - `double` type
* `UT_BOOL` - `bool` type
* `UT_STRING` - `std::string` type
* `UT_VECTOR` - `fcf::UnionVector` type ( `std::vector<fcf::Union>` )
* `UT_MAP` - - `fcf::UnionMap` type ( `std::map<fcf::Union, fcf::Union, fcf::UnionMapLess>` ),

<a name="description_union_stringify_options"></a>
#### Struct fcf::UnionStringifyOptions

Structure describing the parameters of translation into string format/JSON

##### Properties

* `bool friendly = false` - If `true`, then output is generated with line breaks and indentation.
* `const char* tab = "  "` - A line containing a tab.
* `fcf::UnionFormat mode = SF_JSON` - Output format. SF_JSON - Json format. SF_VALUE - the value format is similar to SF_JSON, if the root element is a string, then the value will not be enclosed in quotation marks when output.

<a name="description_union__iterator"></a>
#### Class fcf::Union::iterator

Class of iterator of children of the object [fcf::Union](#description_union)

##### Methods

* `Union key() const` - Child element name/key
* `Union* operator->()` - Gets a pointer to the child element
* `Union& operator()` - Gets a reference to a child element
*  `Union&  value()` - Gets a reference to a child element
* `base_iterator& operator++()` - increment
* `bool operator==(const base_iterator& a_it) const` - comparison
* `bool operator!=(const base_iterator& a_it) const` - comparison


<a name="description_union__const_iterator"></a>
#### Class fcf::Union::cons_iterator

Class of const iterator of children of the object [fcf::Union](#description_union)

##### Methods

* `Union key() const` - Child element name/key
* `const Union* operator->()` - Gets a pointer to the child element
* `const Union& operator()` - Gets a reference to a child element
*  `const Union&  value()` - Gets a reference to a child element
* `base_iterator& operator++()` - increment
* `bool operator==(const base_iterator& a_it) const` - comparison
* `bool operator!=(const base_iterator& a_it) const` - comparison


<a name="description_union"></a>
#### Class fcf::Union

The class object contains a value that has a type - one of the enum [fcf::UnionType](#description_union_type). The class object provides the functionality of bidirectional type conversion and the ability to JSON serialize.

##### Methods

* --- Constructors ---
* `Union()` - Initializes to `fcf::undefined`
* `Union(UnionType a_type)` - Initializes a new object with the given type.
* `template <typename Ty> Union(const Ty& a_value)` - Initializes a new object with the given value
*
* --- Base methods ---
* `template <typename Ty> bool is() const` - Returns `true` if `Ty` type is equal to the type of the stored object value
* `bool is(fcf::UnionType a_type) const` - Returns `true` if type index `a_type` is equal to the type of the stored object value
*  `template <typename Ty> bool isCompatible(bool a_stringifyMode = false) const` - Returns `true` if the stored value can be represented as type `Ty`. If the `a_stringifyMode` argument is `true`, the possibility of converting from string type to type `Ty` is also checked.
*  `bool isCompatible(fcf::UnionType a_type, bool a_stringifyMode = false) const` - Returns true if the stored value can be represented as type `a_type`. If the `a_stringifyMode` argument is true, the possibility of converting from string type to type `a_type` is also checked.
* `template <typename Ty> explicit operator Ty() const` - Returns the stored value in the Ty type representation. If the stored value cannot be converted, an `fcf::UnionException` exception is thrown.
*  `template <typename Ty> Ty get() const` - Returns the stored value in the Ty type representation. If the stored value cannot be converted, an `fcf::UnionException` exception is thrown.
*  `template <typename Ty> fcf::Details::NUnion::TypeHelper<Ty>::far_type& ref()` - Returns a reference to the stored value. If the stored value differs from the requested type, the stored data type is converted to the closest available type and a reference to the stored value is returned. If the conversion process fails, the object is initialized to an empty value.
*  `template <typename Ty> void set(const Ty& a_value)` - Sets a new value.
*  `void set(const fcf::Union& a_value)` - Sets a new value.
*  `template <typename Ty> void set()` - Sets a new empty value with the given type
*  `void set(fcf::UnionType a_type)` - Sets a new empty value with the given type
*  `template <typename Ty> fcf::Union& operator=(const Ty& a_value)` - Sets a new value.
*  `fcf::Union& operator=(const fcf::Union& a_union)` - Sets a new value.
*  `template <typename Ty> bool equal(const Ty& a_value, bool a_strict, bool a_deep) const` - Returns `true` if the stored value is equal to `a_value`. If `a_strict` is `true`, then a strict comparison is performed (types must match). If the `a_deep` parameter is true, then an element-wise comparison is performed for `fcf::UnionVector` and `fcf::UnionMap`.
* `template <typename Ty> bool operator==(const Ty& a_value) const` - Performs a non-strict and non-deep equality comparison. Returns `true` if the stored value is equal to `a_value`.
* `template <typename Ty> bool operator!=(const Ty& a_value) const` - Performs a non-strict and non-deep non-equality comparison. Returns `true` if the stored value is not equal to `a_value`.
*
* --- Serialization methods ---
*  `void stringify(std::string& a_dest, const fcf::UnionStringifyOptions& a_options = fcf::UnionStringifyOptions{}) const` - Saves the stored data to the variable `a_dest` in JSON format
*  `void stringify(std::basic_ostream<char>& a_dest, const UnionStringifyOptions& a_options = UnionStringifyOptions{}) const` - Saves the stored data to the `a_dest` stream in JSON format
*  `void parse(const std::string& a_source)` - Parses a JSON string. The original format may have deviations from JSON (like JS): may contain comments; may contain a comma after the last element; object keys do not have to be enclosed in quotes.
*  `void parse(std::basic_istream<char>& a_source)` - Parses a JSON stream. The original format may have deviations from JSON (like JS): may contain comments; may contain a comma after the last element; object keys do not have to be enclosed in quotes.
*  
* --- Accessing child elements ---
* `size_t size() const` - Returns the number of child elements
* `fcf::Union::iterator find(fcf::Union a_key)` - Searches for a child element by key. Returns an iterator to the found element. If the element is not found, returns an iterator to the end (`Union::end()`)
* `fcf::Union& at(fcf::Union a_key)` - Returns a reference to a child element by key. If the child element is not found, then for `fcf::UnionVector` and `fcf::UnionMap` an element is created, and for other types an `fcf::UnionException` is thrown.
* `const fcf::Union& cat(fcf::Union a_key) const` - Returns a const reference to a child element by key. If the child element is not found, an `fcf::UnionException` is thrown.
* `fcf::Union& operator[](fcf::Union a_key)` - Returns a reference to a child element by key. If the child element is not found, then for `fcf::UnionVector` and `fcf::UnionMap` an element is created, and for other types an `fcf::UnionException` is thrown. 
* `fcf::Union::iterator insert(fcf::Union a_value)` - Inserts a new child element into the object and returns a pointer to it. The key of the new element will be equal to its ordinal in the object - the `size()` value before incrementing. If the object is not a container (`fcf::UT_VECTOR` | `fcf::UT_MAP`), then `fcf::UnionException` is thrown.
* `fcf::Union::iterator insert(fcf::Union a_key, fcf::Union a_value)` - Inserts a new child element into the object and returns a pointer to it. If the object is not a container (`fcf::UT_VECTOR` | `fcf::UT_MAP`), then `fcf::UnionException` is thrown.
* `fcf::Union::iterator erase(const fcf::Union& a_key)` - Removes an element by key. Returns an iterator to the next element. If the element is not found, the function returns `end()`
* `fcf::Union::iterator erase(const fcf::Union::iterator& a_iterator)` - Removes an element. Returns an iterator to the next element. If the element is not found, the function returns `end()`.
*
* --- Iterators ---
* `fcf::Union::iterator begin()` - Returns an [iterator](#description_union__iterator) to the first child element
* `fcf::Union::iterator end()` - Returns an [iterator](#description_union__iterator) to end
* `fcf::Union::const_iterator cbegin() const` - Returns an [const iterator](#description_union__const_iterator) to the first child element
* `fcf::Union::const_iterator cend() const` - Returns an [const iterator](#description_union__const_iterator) to end
* `fcf::Union::iterator obegin()` - Returns an [iterator](#description_union__iterator) to the first child element, preserving sequence. If the `fcf::Union` object is an `fcf::UnionMap`, then the elements are iterated in the order they were added. For other types the method is similar to `begin()`.
* `fcf::Union::iterator oend()` - Returns an [iterator](#description_union__iterator) to end
* `fcf::Union::const_iterator cobegin() const` - Returns an [const iterator](#description_union__const_iterator) to the first child element, preserving sequence. If the `fcf::Union` object is an `fcf::UnionMap`, then the elements are iterated in the order they were added. For other types the method is similar to `cbegin()`.
* `fcf::Union::const_iterator coend() const` - Returns an [const iterator](#description_union__const_iterator) to end





