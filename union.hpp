#ifndef ___FCF__UNION__UNION_HPP___
#define ___FCF__UNION__UNION_HPP___
#include <stdexcept>
#include <algorithm>
#include <memory>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <sstream>

#include <iostream>


#ifdef FCF_IMPLEMENTATION
  #define FCF_UNION_IMPLEMENTATION
#endif

#ifndef FCF_UNION_DELC_EXTERN
  #ifdef FCF_UNION_IMPLEMENTATION
    #define FCF_UNION_DELC_EXTERN
  #else
    #define FCF_UNION_DELC_EXTERN extern
  #endif
#endif

#ifndef FCF_UNION_DECL_TEMPLATE_EXTERN
  #ifdef _MSC_VER
    #define FCF_UNION_DECL_TEMPLATE_EXTERN
  #else
    #ifdef FCF_UNION_IMPLEMENTATION
      #define FCF_UNION_DECL_TEMPLATE_EXTERN
    #else
      #define FCF_UNION_DECL_TEMPLATE_EXTERN extern
    #endif
  #endif
#endif

#ifndef FCF_UNION_DECL_EXPORT
  #ifdef WIN32
    #if defined(FCF_UNION_EXPORT)
      #define FCF_UNION_DECL_EXPORT __declspec(dllexport)
    #elif defined(FCF_UNION_IMPORT)
      #define FCF_UNION_DECL_EXPORT __declspec(dllimport)
    #else
      #define FCF_UNION_DECL_EXPORT
    #endif
  #else
    #define FCF_UNION_DECL_EXPORT
  #endif
#endif

#ifndef WIN32
  #define FCF_UNION_DECL_VISIBILITY_HIDDEN  __attribute__((visibility("hidden")))
#else
  #define FCF_UNION_DECL_VISIBILITY_HIDDEN
#endif

namespace fcf {

  #ifndef FCF_THROW_OR_RESULT_ERROR
    #define FCF_THROW_OR_RESULT_ERROR(a_dstError, a_errorMessage, a_resolver, a_ignoreFirst) \
      if (a_dstError) { \
        *a_dstError = a_errorMessage; \
      } else { \
        throw UnionException(a_errorMessage, a_resolver.line(), a_resolver.column(), a_ignoreFirst); \
      }
  #endif

  #ifndef FCF_CONST_CHAR_PTR_DEFINED
  #define FCF_CONST_CHAR_PTR_DEFINED
    typedef const char* ConstCharPtr;
  #endif

  #ifndef FCF_CHAR_PTR_DEFINED
  #define FCF_CHAR_PTR_DEFINED
    typedef char* CharPtr;
  #endif

  #ifndef FCF_JSON_FORMAT_STRUCT_DEFINED
  #define FCF_JSON_FORMAT_STRUCT_DEFINED
    struct JSONFormat {};
  #endif

  #ifndef FCF_TNOP_STRUCT_DEFINED
  #define FCF_TNOP_STRUCT_DEFINED
    struct TNOP {};
  #endif

  #ifndef FCF_UNDEFINED_STRUCT_DEFINED
    #define FCF_UNDEFINED_STRUCT_DEFINED
    struct Undefined {
      bool operator <  (const Undefined& a_value) const { return false; }
      bool operator == (const Undefined& a_value) const { return true; }
      bool operator != (const Undefined& a_value) const { return false; }
    };
    FCF_UNION_DELC_EXTERN FCF_UNION_DECL_EXPORT Undefined undefined;
  #endif

  #ifndef FCF_NULL_STRUCT_DEFINED
  #define FCF_NULL_STRUCT_DEFINED
    struct Null {
      bool operator <  (const Null& a_value) const { return false; }
      bool operator == (const Null& a_value) const { return true; }
      bool operator != (const Null& a_value) const { return false; }
    };
    FCF_UNION_DELC_EXTERN FCF_UNION_DECL_EXPORT Null null;
  #endif

  enum UnionFormat {
    SF_VALUE = 0,
    SF_JSON = 1
  };

  struct UnionStringifyOptions {
    bool             friendly;
    const char*      tab;
    UnionFormat mode;
    UnionStringifyOptions()
      : friendly(false)
      , tab("  ")
      , mode(SF_JSON) {
    }
  };

  namespace Details {
    namespace NUnion {
      template <typename Ty>
      struct TypeHelper;
    }
  }
  struct Union;

  struct UnionMapLess{
    template <typename Ty>
    inline bool operator()(const Ty& a_left, const Ty& a_right) const{
      return a_left.lessStr(a_right);
    }
  };

  typedef std::vector<Union> UnionVector;
  typedef std::map<Union, Union, UnionMapLess> UnionMap;

  enum UnionType{
    UT_UNDEFINED,
    UT_NULL,
    UT_INT,
    UT_UINT,
    UT_LONGLONG,
    UT_ULONGLONG,
    UT_DOUBLE,
    UT_BOOL,
    UT_STRING,
    UT_VECTOR,
    UT_MAP,
  };

  enum { FIRST_NUMBER_TYPE = UT_INT };
  enum { LAST_NUMBER_TYPE = UT_DOUBLE };

  union UnionValue{
    Undefined vundefined;
    Null      vnull;
    int       vint;
    long long vlonglong;
    double    vdouble;
    bool      vbool;
    char      vstring[sizeof(std::string)];
    char      vvector[sizeof(UnionVector)];
    char      vmap[sizeof(UnionMap)];
  };

  class UnionException : public std::runtime_error {
    public:
      inline UnionException(const char* a_message)
        : runtime_error(a_message)
        , _line(SIZE_MAX)
        , _column(SIZE_MAX) {
      }

      inline UnionException(const char* a_message, size_t a_line, size_t a_col)
        : runtime_error(_tostr(a_message, a_line, a_col))
        , _line(a_line)
        , _column(a_col) {
      }
      inline UnionException(const char* a_message, size_t a_line, size_t a_col, bool a_ignoreFirst)
        : runtime_error(_tostr(a_message,  a_ignoreFirst && a_line == 1 && a_col == 1 ? SIZE_MAX : a_line, a_ignoreFirst && a_line == 1 && a_col == 1 ? SIZE_MAX : a_col))
        , _line(a_ignoreFirst && a_line == 1 && a_col == 1 ?  SIZE_MAX: a_line)
        , _column(a_ignoreFirst && a_line == 1 && a_col == 1 ?  SIZE_MAX: a_col) {
      }
      inline size_t line() const{
        return _line;
      }
      inline size_t column() const {
        return _column;
      }
      inline std::string swhat() const {
        std::string w = what();
        const char* p = strstr(w.c_str(), " [");
        return std::string(p ? w.substr(0, p - w.c_str()) : w);
      }

    protected:
      FCF_UNION_DECL_EXPORT static std::string _tostr(const char* a_message, size_t a_line, size_t a_col);
      size_t      _line;
      size_t      _column;
  };

  struct Union {
    UnionValue value;
    UnionType  type;
    size_t     orderc;
    size_t     order;

    struct FCF_UNION_DECL_EXPORT base_iterator {
        friend struct Union;
        typedef std::shared_ptr< std::vector< UnionMap::iterator > > oiterators_type;
        struct oiterator_type {
          oiterators_type                             iterators;
          std::vector< UnionMap::iterator >::iterator iterator;
        };
        enum IteratorType{
          IT_UNDEFINED,
          IT_VECTOR,
          IT_MAP,
          IT_OMAP
        };
        union value_iterator{
          char vivector[sizeof(UnionVector::iterator)];
          char vcivector[sizeof(UnionVector::const_iterator)];
          char vimap[sizeof(UnionMap::iterator)];
          char vcimap[sizeof(UnionMap::const_iterator)];
          char voiterators[sizeof(oiterator_type)];
        };
        base_iterator();
        base_iterator(oiterators_type a_iterators, std::vector< UnionMap::iterator >::iterator a_iterator, const UnionMap* a_owner);
        base_iterator(const base_iterator& a_iterator);
        ~base_iterator();
        base_iterator& operator=(const base_iterator& a_iterator);
        bool operator ==(const base_iterator& a_it) const;
        bool operator !=(const base_iterator& a_it) const;
        base_iterator& operator++();
        base_iterator operator++(int);
        Union key() const;

      protected:
        bool _isend() const;
        value_iterator               _viiterator;
        IteratorType                 _vitype;
        void*                        _owner;
    };

    template <typename TPointer, typename TRef, typename TMapIterator, typename TVectorIterator>
    struct FCF_UNION_DECL_EXPORT basic_iterator : public base_iterator {
      using base_iterator::base_iterator;
      inline basic_iterator() : base_iterator(){};
      basic_iterator(TVectorIterator a_iterator, const UnionVector* a_owner);
      basic_iterator(TMapIterator a_iterator, const UnionMap* a_owner);
      TPointer operator->();
      TRef& operator*();
      TRef& value();
    };

    typedef Union& TRef;
    typedef const Union& TConstRef;
    typedef Union* TPointer;
    typedef const Union* TConstPointer;
    typedef basic_iterator<TPointer, TRef, UnionMap::iterator, UnionVector::iterator> iterator;
    typedef basic_iterator<TConstPointer, TConstRef, UnionMap::const_iterator, UnionVector::const_iterator> const_iterator;

    FCF_UNION_DECL_EXPORT Union();
    FCF_UNION_DECL_EXPORT Union(const Union& a_union);
    FCF_UNION_DECL_EXPORT Union(UnionType a_type);
    template <typename Ty>
    FCF_UNION_DECL_EXPORT Union(const Ty& a_value);

    FCF_UNION_DECL_EXPORT Union(const char* a_value);

    FCF_UNION_DECL_EXPORT ~Union();

    FCF_UNION_DECL_EXPORT iterator obegin();

    FCF_UNION_DECL_EXPORT iterator oend();

    FCF_UNION_DECL_EXPORT const_iterator cobegin() const;

    FCF_UNION_DECL_EXPORT const_iterator coend() const;

    FCF_UNION_DECL_EXPORT iterator begin();

    FCF_UNION_DECL_EXPORT iterator end();

    FCF_UNION_DECL_EXPORT const_iterator cbegin() const;

    FCF_UNION_DECL_EXPORT const_iterator cend() const;

    FCF_UNION_DECL_EXPORT size_t size() const;

    FCF_UNION_DECL_EXPORT iterator find(Union a_key);

    FCF_UNION_DECL_EXPORT const Union& cat(Union a_key) const;

    FCF_UNION_DECL_EXPORT Union& at(Union a_key);

    FCF_UNION_DECL_EXPORT Union& operator[](Union a_key);

    FCF_UNION_DECL_EXPORT iterator insert(Union a_value);

    FCF_UNION_DECL_EXPORT iterator insert(Union a_key, Union a_value);

    FCF_UNION_DECL_EXPORT iterator erase(const Union& a_key);

    FCF_UNION_DECL_EXPORT iterator erase(const iterator& a_iterator);

    FCF_UNION_DECL_EXPORT void swap(Union& a_union);

    template <typename Ty>
    FCF_UNION_DECL_EXPORT bool is() const;

    inline bool is(UnionType a_type) const
    { return type == a_type; }

    template <typename Ty>
    inline bool isCompatible(bool a_stringifyMode = false) const
    { return isCompatible((UnionType)fcf::Details::NUnion::TypeHelper<Ty>::type_index, a_stringifyMode); }

    FCF_UNION_DECL_EXPORT bool isCompatible(UnionType a_type, bool a_stringifyMode = false) const;

    FCF_UNION_DECL_EXPORT void parse(const std::string& a_source);

    FCF_UNION_DECL_EXPORT void parse(std::basic_istream<char>& a_source);

    FCF_UNION_DECL_EXPORT void stringify(std::string& a_dest, const UnionStringifyOptions& a_options = UnionStringifyOptions{}) const;

    FCF_UNION_DECL_EXPORT void stringify(std::basic_ostream<char>& a_dest, const UnionStringifyOptions& a_options = UnionStringifyOptions{}) const;

    template <typename Ty>
    FCF_UNION_DECL_EXPORT Ty get() const;

    template <typename Ty>
    FCF_UNION_DECL_EXPORT typename Details::NUnion::TypeHelper<Ty>::far_type& ref();

    template <typename Ty>
    FCF_UNION_DECL_EXPORT void set(const Ty& a_value);

    inline void set(const char* a_value)
    { set<const char*>(a_value); }

    FCF_UNION_DECL_EXPORT void set(const Union& a_value);

    template <typename Ty>
    inline void set()
    { set((UnionType)fcf::Details::NUnion::TypeHelper<Ty>::type_index); }

    FCF_UNION_DECL_EXPORT void set(UnionType a_type);

    template <typename Ty>
    inline Union& operator=(const Ty& a_value)
    { set(a_value); return *this; }

    inline Union& operator=(const Union& a_union)
    { set(a_union); return *this; }

    template <typename Ty>
    FCF_UNION_DECL_EXPORT bool equal(const Ty& a_value, bool a_strict) const;

    inline bool equal(const char* a_value, bool a_strict) const
    { return equal<const char*>(a_value, a_strict); }

    FCF_UNION_DECL_EXPORT bool equal(const Union& a_value, bool a_strict) const;

    template <typename Ty>
    FCF_UNION_DECL_EXPORT bool lessStr(const Ty& a_value) const;

    inline bool lessStr(const char* a_value) const
    { return lessStr<const char*>(a_value); }

    FCF_UNION_DECL_EXPORT bool lessStr(Union& a_value) const;

    FCF_UNION_DECL_EXPORT bool lessStr(const Union& a_value) const;

    template <typename Ty>
    inline bool operator==(const Ty& a_value) const
    { return equal(a_value, false); }

    template <typename Ty>
    inline bool operator!=(const Ty& a_value)
    { return !equal(a_value, false); }

    template <typename Ty>
    inline explicit operator Ty() const
    { return get<Ty>(); }
  };

  #ifdef FCF_UNION_IMPLEMENTATION
    namespace Details {
      namespace NConvert {

        FCF_UNION_DECL_VISIBILITY_HIDDEN bool isDouble(const std::string& a_str, bool a_unsigned);

        template <typename TFrom, typename TTo, typename TFormat = TNOP>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            a_receiver((const TTo&)a_resolver());
          }
        };

        template <typename Ty>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN ConstResolver {
          typedef Ty item_type;
          typedef Ty value_type;
          ConstResolver(const value_type& a_ref, bool a_enableLineCounter = false)
            : _ref(a_ref) {
          }
          inline const value_type& operator()(){
            return _ref;
          }

          size_t line() {
            return SIZE_MAX;
          }

          size_t column() {
            return SIZE_MAX;
          }

          const Ty& _ref;
        };

        template <typename TString>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN SimpleConstResolver {
          typedef char item_type;
          typedef std::string value_type;

          template <typename Ty>
          SimpleConstResolver(Ty& a_ref, bool a_enableLineCounter = false)
            : _ref((TString&)a_ref)
            , _index(0){
          }

          inline const std::string& operator()(){
            return _ref;
          }

          inline item_type read() {
            return _ref[_index];
          }

          inline void next() {
             ++_index;
          }

          inline bool end() {
            return !_ref[_index];
          }

          size_t line() {
            return SIZE_MAX;
          }

          size_t column() {
            return SIZE_MAX;
          }

          TString&  _ref;
          size_t    _index;
        };


        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN SimpleConstResolver< std::basic_istream<char> > {
          typedef char item_type;
          typedef std::basic_istream<char> value_type;

          SimpleConstResolver(std::basic_istream<char>& a_ref, bool a_enableLineCounter = false)
            : _ref(a_ref) {
            _ch = _ref.get();
          }

          inline std::basic_istream<char>& operator()(){
            return _ref;
          }

          inline item_type read() {
            return _ch;
          }

          inline void next() {
            _ch = _ref.get();
          }

          inline bool end() {
            return _ref.eof() || _ref.fail();
          }

          size_t line() {
            return SIZE_MAX;
          }

          size_t column() {
            return SIZE_MAX;
          }

          std::basic_istream<char>&  _ref;
          item_type                  _ch;
        };

        template <typename Ty>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN ConstUncommentResolver {
          public:
            typedef Ty resolver_type;
            typedef typename resolver_type::item_type item_type;
            typedef typename resolver_type::value_type value_type;

            template <typename TRef>
            ConstUncommentResolver(TRef& a_ref, bool a_enableLineCounter = false)
               : _resolver((TRef&)a_ref), _q(0), _n1(-1), _n2(-1), _nb1(-1), _s(0), _l(a_enableLineCounter ? 1 : SIZE_MAX), _c(a_enableLineCounter ? 1: SIZE_MAX) {
            }

            inline const value_type& operator()(){
              return _resolver.operator()();
            }

            inline item_type read() {
              if (_n1 != -1) {
                return _n1;
              }
              return _resolver.read();
            }

            void next() {
             if (_n1 != -1){
                _n1 = -1;
                return;
              }
             char c;
              while(!_resolver.end()) {
                _next();
                if (_resolver.end()){
                  break;
                }
                c = _resolver.read();
                if (!_q) {
                  if (_n1 == '/' && _n2 == '/') {
                    if (c == '\n') {
                      _n1 = -1;
                      _n2 = -1;
                      _nb1 = -1;
                      continue;
                    } else {
                      continue;
                    }
                  }
                  if (_n1 == '/' && _n2 == '*') {
                    if (_nb1 == '*' && c == '/' ) {
                      _n1 = -1;
                      _n2 = -1;
                      _nb1 = -1;
                      continue;
                    } else if (c == '*') {
                      _nb1 = c;
                      continue;
                    } else {
                      _nb1 = -1;
                      continue;
                    }
                  }
                  if (c == '/' && _n1 == '/'){
                    _n2 = c;
                    continue;
                  }
                  if (c == '*' && _n1 == '/'){
                    _n2 = c;
                    continue;
                  }
                  if (c == '/' && _n1 == -1){
                    _n1 = c;
                    continue;
                  }
                  break;
                } else {
                  break;
                }
              }
              if (_resolver.end() && _n1 != -1){
                _n1 = -1;
                _n2 = -1;
                _nb1 = -1;
              }
              if (!_resolver.end()) {
                if (_q) {
                  if (c == '\\') {
                    ++_s;
                  }
                  if (c == _q && (_s % 2) == 0) {
                    _q = 0;
                  }
                  if (c != '\\') {
                    _s = 0;
                  }
                } else {
                  if (c == '\'' || c == '\"') {
                    _q = c;
                  }
                }
              }
            }

            inline bool end() {
              if (_n1 != -1){
                return false;
              }
              return _resolver.end();
            }

            size_t line() {
              return _l;
            }

            size_t column() {
              return _c;
            }

          protected:

            void _next() {
              if (_l != SIZE_MAX) {
                const char c = _resolver.read();
                if (c == '\n'){
                  ++_l;
                  _c = 1;
                } else {
                  ++_c;
                }
              }
              _resolver.next();
            }

            resolver_type _resolver;
            char          _q;
            int           _n1;
            int           _n2;
            int           _nb1;
            size_t        _s;
            size_t        _l;
            size_t        _c;
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN ConstResolver< std::string > : public ConstUncommentResolver< SimpleConstResolver<std::string> >{
          typedef ConstUncommentResolver< SimpleConstResolver<std::string> > base_type;
          using ConstUncommentResolver< SimpleConstResolver<std::string> >::ConstUncommentResolver;
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN ConstResolver< const char* > : public ConstUncommentResolver< SimpleConstResolver<const char*> >{
          typedef ConstUncommentResolver< SimpleConstResolver<const char*> > base_type;
          using ConstUncommentResolver< SimpleConstResolver<const char*> >::ConstUncommentResolver;
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN ConstResolver< std::basic_istream<char> > : public ConstUncommentResolver< SimpleConstResolver< std::basic_istream<char> > >{
        typedef ConstUncommentResolver< SimpleConstResolver< std::basic_istream<char> > > base_type;
          using ConstUncommentResolver< SimpleConstResolver< std::basic_istream<char> > >::ConstUncommentResolver;
        };

        template <typename Ty, typename TOptions>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Receiver {
          typedef Ty value_type;

          inline void operator()(const Ty& a_value){
            _ref = a_value;
          }
          inline Ty& get(){
            return _ref;
          }
          Ty& _ref;
          const TOptions& options;
        };

        template <typename TOptions>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Receiver<std::string, TOptions> {
          typedef std::string value_type;

          Receiver(std::string& a_ref, const TOptions& a_options)
            : ref(a_ref)
            , options(a_options)
            , level (0) {
          }

          template <typename TOperand>
          void operator()(const TOperand& a_value){
            ref += a_value;
          }

          std::string& get(){
            return ref;
          }

          std::string&      ref;
          const TOptions&   options;
          int               level;
        };

        template <typename TOptions>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Receiver<std::basic_ostream<char>, TOptions> {
          typedef std::basic_ostream<char> destination_type;
          typedef std::string value_type;

          Receiver(destination_type& a_ref, const TOptions& a_options)
            : ref(a_ref)
            , options(a_options)
            , level (0) {
          }

          template <typename TOperand>
          void operator()(const TOperand& a_value){
            ref << std::string(a_value);
          }

          void operator()(const destination_type& a_value){
          }

          destination_type& get(){
            return ref;
          }

          destination_type& ref;
          const TOptions&    options;
          int                level;
        };

        template <typename TOptions>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Receiver<UnionVector, TOptions> {
          typedef UnionVector value_type;

          template <typename TOperand>
          inline void operator()(const TOperand& a_value){
            _ref.push_back(a_value);
          };

          inline value_type& get(){
            return _ref;
          }

          value_type& _ref;
          const TOptions& options;
        };

        template <typename TOptions>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Receiver<UnionMap, TOptions> {
          typedef UnionMap value_type;

          inline void operator()(const UnionMap& a_value){
            _ref = a_value;
          };

          template <typename TOperand>
          inline void operator()(const TOperand& a_value){
            UnionMap::iterator it = _ref.insert(a_value).first;
            it->second.order = _ref.size();
          };

          inline value_type& get(){
            return _ref;
          }

          value_type& _ref;
          const TOptions& options;
        };



        ///////////////////////////////////////
        // Convertion from int
        ///////////////////////////////////////
        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<int, std::string, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            return  a_receiver(std::to_string(a_resolver()));
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<int, UnionVector, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect vector value", a_resolver, false);
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<int, UnionMap, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect map value", a_resolver, false);
          }
        };

        ///////////////////////////////////////
        // Convertion from unsigned int
        ///////////////////////////////////////
        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<unsigned int, std::string, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            return  a_receiver(std::to_string(a_resolver()));
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<unsigned int, UnionVector, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect vector value", a_resolver, false);
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<unsigned int, UnionMap, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect map value", a_resolver, false);
          }
        };


        ///////////////////////////////////////
        // Convertion from long long
        ///////////////////////////////////////
        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<long long, std::string, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            return  a_receiver(std::to_string(a_resolver()));
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<long long, UnionVector, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect vector value", a_resolver, false);
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<long long, UnionMap, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect map value", a_resolver, false);
          }
        };

        ///////////////////////////////////////
        // Convertion from unsigned long long
        ///////////////////////////////////////
        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<unsigned long long, std::string, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            return  a_receiver(std::to_string(a_resolver()));
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<unsigned long long, UnionVector, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect vector value", a_resolver, false);
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<unsigned long long, UnionMap, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect map value", a_resolver, false);
          }
        };

        ///////////////////////////////////////
        // Convertion from double
        ///////////////////////////////////////
        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<double, std::string, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            return  a_receiver(std::to_string(a_resolver()));
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<double, UnionVector, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect vector value", a_resolver, false);
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<double, UnionMap, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect map value", a_resolver, false);
          }
        };

        ///////////////////////////////////////
        // Convertion from bool
        ///////////////////////////////////////
        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<bool, int, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            return  a_receiver(a_resolver() ? 1 : 0 );
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<bool, unsigned int, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            return  a_receiver(a_resolver() ? 1 : 0 );
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<bool, long long, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            return  a_receiver(a_resolver() ? 1 : 0 );
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<bool, unsigned long long, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            return  a_receiver(a_resolver() ? 1 : 0 );
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<bool, double, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            return  a_receiver(a_resolver() ? 1.0 : 0.0 );
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<bool, std::string, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            return  a_receiver(a_resolver() ? "true" : "false" );
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<bool, UnionVector, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect vector value", a_resolver, false);
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<bool, UnionMap, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect map value", a_resolver, false);
          }
        };


        ///////////////////////////////////////
        // Convertion from undefined
        ///////////////////////////////////////
        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<Undefined, Null, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            a_receiver(fcf::null);
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<Undefined, int, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect int value", a_resolver, false);
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<Undefined, unsigned int, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect unsigned int value", a_resolver, false);
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<Undefined, long long, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect long long value", a_resolver, false);
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<Undefined, unsigned long long, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect unsigned long long value", a_resolver, false);
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<Undefined, double, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect double value", a_resolver, false);
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<Undefined, std::string, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            a_receiver("undefined");
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<Undefined, UnionVector, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect vector value", a_resolver, false);
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<Undefined, UnionMap, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect map value", a_resolver, false);
          }
        };

        ///////////////////////////////////////
        // Convertion from null
        ///////////////////////////////////////
        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<Null, Undefined, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            a_receiver(fcf::undefined);
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<Null, int, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect int value", a_resolver, false);
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<Null, unsigned int, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect undefined int value", a_resolver, false);
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<Null, long long, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect long long value", a_resolver, false);
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<Null, unsigned long long, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect unsigned long long value", a_resolver, false);
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<Null, double, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect double value", a_resolver, false);
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<Null, std::string, TNOP>{
        template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            a_receiver("null");
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<Null, UnionVector, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect vector value", a_resolver, false);
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<Null, UnionMap, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect map value", a_resolver, false);
          }
        };

        ///////////////////////////////////////
        // Convertion from string
        ///////////////////////////////////////

        template <typename TResolver, typename Ty1, typename Ty2, typename Ty3 >
        FCF_UNION_DECL_VISIBILITY_HIDDEN void parseNumber(TResolver& a_resolver, bool a_unsignedMode, bool* a_dstMinus, bool* a_dstPoint, Ty1* a_value1, Ty2* a_value2, Ty3* a_value3, const char** a_dstErrorMessage) {
          if (a_value1) {
            *a_value1 = 0;
          }
          if (a_value2) {
            *a_value2 = 0;
          }
          if (a_value3) {
            *a_value3 = 0;
          }
          size_t d = 0;
          double dd = 1;
          size_t m = 0;
          size_t i = 0;
          unsigned char lc;
          while(!a_resolver.end()){
            unsigned char c = (unsigned char)a_resolver.read();
            if (c == '-'){
              if (m == 0 && i == 0 && !a_unsignedMode){
                ++m;
                ++i;
              } else {
                FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect number format", a_resolver, false);
                return;
              }
            } else if (c == '.') {
              if (d == 0 && ( (m && i > 1) || (!m && i > 0))) {
                ++d;
                ++i;
              } else {
                FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect number format", a_resolver, false);
                return;
              }
            } else if (c <= (unsigned char)' ') {
              if (i == 0){
                ++i;
              } else {
                break;
              }
            } else  if (c >= (unsigned char)'0' && c <= (unsigned char)'9') {
              int v = c - (unsigned char)'0';
              if (v && !d && i && lc == '0' && (!!a_value1 && !*a_value1)) {
                FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect number format", a_resolver, false);
                return;
              }
              if (!d) {
                if (a_value1) {
                  *a_value1 *= 10;
                  *a_value1 += v;
                }
                if (a_value2) {
                  *a_value2 *= 10;
                  *a_value2 += v;
                }
                if (a_value3) {
                  *a_value3 *= 10;
                  *a_value3 += v;
                }
              } else {
                dd /= 10.0;
                if (a_value1) {
                  *a_value1 += v*dd;
                }
                if (a_value2) {
                  *a_value2 += v*dd;
                }
                if (a_value3) {
                  *a_value3 += v*dd;
                }
              }
              ++i;
            } else {
              break;
            }
            lc = c;
            a_resolver.next();
          }
          size_t mins = 1;
          if (d) {
            ++mins;
          }
          if (m) {
            if (a_value1) {
              *a_value1 = -*a_value1;
            }
            if (a_value2) {
              *a_value2 = -*a_value2;
            }
            if (a_value3) {
              *a_value3 = -*a_value3;
            }
            ++mins;
          }
          if (i < mins) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect number format", a_resolver, false);
            return;
          }
          if (a_dstPoint){
            *a_dstPoint = d;
          }
          if (a_dstMinus){
            *a_dstMinus = m;
          }
        }

        template <typename TResolver>
        FCF_UNION_DECL_VISIBILITY_HIDDEN std::string parseInnerString(TResolver& a_resolver, const char** a_dstErrorMessage) {
          std::string str;
          skipSpaces(a_resolver);
          if (a_resolver.end()) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect string format", a_resolver, false);
            return std::string();
          }
          unsigned char ce = a_resolver.read();
          a_resolver.next();
          if (a_resolver.end() || (ce != '\"' && ce != '\'')) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect string format", a_resolver, false);
            return std::string();
          }
          size_t sc = 0;
          unsigned char c;
          while(!a_resolver.end()) {
            c = a_resolver.read();
            if (sc && sc % 2) {
              str += c;
              if (c == '\\') {
                ++sc;
              }
            } else if (c == '\\') {
              ++sc;
            } else if (c == ce) {
              a_resolver.next();
              break;
            } else {
              str += c;
            }
            if (c != '\\') {
              sc = 0;
            }
            a_resolver.next();
          }
          if (c != ce) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect string format", a_resolver, false);
            return std::string();
          }
          return str;
        }

        template <typename TResolver>
        FCF_UNION_DECL_VISIBILITY_HIDDEN bool parseKey(TResolver& a_resolver, Union& a_key, const char** a_dstErrorMessage);

        template <typename TResolver>
        FCF_UNION_DECL_VISIBILITY_HIDDEN Undefined parseUndefined(TResolver& a_resolver, const char** a_dstErrorMessage) {
          skipSpaces(a_resolver);
          if (a_resolver.end()) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect undefined value format", a_resolver, false);
            return fcf::undefined;
          }
          const char* expected = "undefined";
          while(!a_resolver.end()) {
            unsigned char c = (unsigned char)a_resolver.read();
            if (c == *expected){
             ++expected;
              a_resolver.next();
              if (!*expected) {
                return fcf::undefined;
              }
            } else {
              FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect undefined value format", a_resolver, false);
              return fcf::undefined;
            }
          }
          FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect undefined value format", a_resolver, false);
          return fcf::undefined;
        }

        template <typename TResolver>
        FCF_UNION_DECL_VISIBILITY_HIDDEN Null parseNull(TResolver& a_resolver, const char** a_dstErrorMessage) {
          skipSpaces(a_resolver);
          if (a_resolver.end()) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect null value format", a_resolver, false);
            return fcf::null;
          }
          const char* expected = "null";
          while(!a_resolver.end()) {
            unsigned char c = (unsigned char)a_resolver.read();
            if (c == *expected){
             ++expected;
              a_resolver.next();
              if (!*expected) {
                return fcf::null;
              }
            } else {
              FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect null value format", a_resolver, false);
              return fcf::null;
            }
          }
          FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect null value format", a_resolver, false);
          return fcf::null;
        }

        template <typename TResolver>
        FCF_UNION_DECL_VISIBILITY_HIDDEN bool parseBool(TResolver& a_resolver, const char** a_dstErrorMessage) {
          size_t i = 0;
          unsigned char c;
          const char* pattern;
          bool result;
          while(!a_resolver.end()) {
            c = (unsigned char)a_resolver.read();
            if (c > (unsigned char)' ') {
              break;
            }
            a_resolver.next();
          };
          if (a_resolver.end()){
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect bool value format", a_resolver, false);
            return false;
          }
          if (c == 'f') {
            result = false;
            pattern = "alse";
            a_resolver.next();
          } else if (c == 't') {
            result = true;
            pattern = "rue";
            a_resolver.next();
          } else {
            const char*  errorMessageValue = 0;
            const char** errorMessage = a_dstErrorMessage ? a_dstErrorMessage : &errorMessageValue;
            double dvalue;
            parseNumber(a_resolver, false, 0, 0, &dvalue, (double*)0, (double*)0, errorMessage);
            if (*errorMessage) {
              FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect bool value format", a_resolver, false);
              return false;
            }
            return dvalue > 0.000001 || dvalue < -0.000001;
          }
          while(!a_resolver.end()) {
            c = (unsigned char)a_resolver.read();
            if (c == *pattern){
             ++pattern;
              a_resolver.next();
              if (!*pattern) {
                return result;
              }
            } else {
              FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect bool value format", a_resolver, false);
              return false;
            }
          }
          FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect bool value format", a_resolver, false);
          return false;
        }

        template <typename TResolver>
        FCF_UNION_DECL_VISIBILITY_HIDDEN void skipSpaces(TResolver& a_resolver) {
          while(!a_resolver.end()) {
            unsigned char c = (unsigned char)a_resolver.read();
            if (c > (unsigned char)' ') {
              break;
            }
            a_resolver.next();
          };
        }

        template <typename TResolver, typename TReceiver>
        FCF_UNION_DECL_VISIBILITY_HIDDEN void parseVector(TResolver& a_resolver, TReceiver& a_receiver, const char** a_dstErrorMessage);

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<std::string, std::string, TNOP>{
          template <typename TResolver, typename TReceiver>
          void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            if (!a_inner) {
              a_receiver(a_resolver());
            } else {
              a_receiver("\"");
              while(!a_resolver.end()) {
                unsigned char c = a_resolver.read();
                if (c == '\\' || c == '\"'){
                  a_receiver("\\");
                }
                std::string sc;
                sc += c;
                a_receiver(sc);
                a_resolver.next();
              }
              a_receiver("\"");
            }
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<std::string, fcf::Undefined, TNOP>{
          template <typename TResolver, typename TReceiver>
          void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            const char* expected = "undefined";
            while(*expected && !a_resolver.end()){
              if (a_resolver.read() != *expected) {
                break;
              }
              a_resolver.next();
              ++expected;
            }
            if (*expected){
              FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect undefined format", a_resolver, false)
              return;
            }
            a_receiver(fcf::undefined);
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<std::string, fcf::Null, TNOP>{
          template <typename TResolver, typename TReceiver>
          void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            const char* expected = "null";
            while(*expected && !a_resolver.end()){
              if (a_resolver.read() != *expected) {
                break;
              }
              a_resolver.next();
              ++expected;
            }
            if (*expected){
              FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect null format", a_resolver, false)
              return;
            }
            a_receiver(fcf::null);
          }
        };

        template <typename Ty, typename TResolver, typename TReceiver>
        inline void convertStringToNumber(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
          skipSpaces(a_resolver);
          if (a_resolver.read() == 't' || a_resolver.read() == 'f') {
            const char* error = 0;
            bool b = parseBool(a_resolver, &error);
            if (error) {
              FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect number format", a_resolver, false);
              return;
            }
            a_receiver(b ? (Ty)1 : (Ty)0);
            return;
          } else {
            Ty value = 0;
            parseNumber(a_resolver, false, 0, 0, &value, (Ty*)0, (Ty*)0, a_dstErrorMessage);
            a_receiver(value);
          }
        }

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<std::string, int, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            convertStringToNumber<int>(a_resolver, a_receiver, a_inner, a_dstErrorMessage);
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<std::string, long long, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            convertStringToNumber<long long>(a_resolver, a_receiver, a_inner, a_dstErrorMessage);
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<std::string, unsigned long long, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            convertStringToNumber<unsigned long long>(a_resolver, a_receiver, a_inner, a_dstErrorMessage);
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<std::string, unsigned int, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            convertStringToNumber<unsigned int>(a_resolver, a_receiver, a_inner, a_dstErrorMessage);
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<std::string, double, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            convertStringToNumber<double>(a_resolver, a_receiver, a_inner, a_dstErrorMessage);
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<std::string, bool, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            a_receiver(parseBool(a_resolver, a_dstErrorMessage));
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<std::string, UnionVector, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            parseVector(a_resolver, a_receiver, a_dstErrorMessage);
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<std::string, UnionMap, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            parseMap(a_resolver, a_receiver, a_dstErrorMessage);
          }
        };

        ///////////////////////////////////////
        // Convertion from vector
        ///////////////////////////////////////
        template <typename Ty>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<UnionVector, Ty, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect conversion from  vector value", a_resolver, false);
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<UnionVector, unsigned int, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect unsigned integer value", a_resolver, false);
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<UnionVector, long long, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect long long value", a_resolver, false);
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<UnionVector, unsigned long long, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect unsigned long long value", a_resolver, false);
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<UnionVector, double, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect double value", a_resolver, false);
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<UnionVector, bool, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect boolean value", a_resolver, false);
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<UnionVector, std::string, TNOP>{
          template <typename TResolver, typename TReceiver>
          FCF_UNION_DECL_VISIBILITY_HIDDEN void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage);
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<UnionVector, UnionMap, TNOP>{
          template <typename TResolver, typename TReceiver>
          FCF_UNION_DECL_VISIBILITY_HIDDEN void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage){
            a_receiver.get().clear();
            const UnionVector& uv = a_resolver();
            for(size_t i = 0; i < uv.size(); ++i){
              std::pair<Union, Union> p(Union((int)i), uv[i]);
              a_receiver(p);
            }
          }
        };

        ///////////////////////////////////////
        // Convertion from map
        ///////////////////////////////////////
        template <typename Ty>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<UnionMap, Ty, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect integer value", a_resolver, false);
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<UnionMap, unsigned int, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect unsigned integer value", a_resolver, false);
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<UnionMap, long long, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect long long value", a_resolver, false);
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<UnionMap, unsigned long long, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect unsigned long long value", a_resolver, false);
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<UnionMap, double, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect double value", a_resolver, false);
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<UnionMap, bool, TNOP>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect boolean value", a_resolver, false);
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<UnionMap, std::string, TNOP>{
          template <typename TResolver, typename TReceiver>
          void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage);
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<UnionMap, UnionVector, TNOP>{
          template <typename TResolver, typename TReceiver>
          void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage);
        };

        template <typename TFrom, typename TTo>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<TFrom, TTo, JSONFormat>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            Converter<TFrom, TTo, TNOP>()(a_resolver, a_receiver, a_inner, a_dstErrorMessage);
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Converter<std::string, std::string, JSONFormat>{
          template <typename TResolver, typename TReceiver>
          inline void operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
            Converter<std::string, std::string, TNOP>()(a_resolver, a_receiver, true, a_dstErrorMessage);
          }
        };

        template <typename TResolver>
        bool FCF_UNION_DECL_VISIBILITY_HIDDEN parseValue(TResolver& a_resolver, Union& a_union, const char** a_dstErrorMessage);

      } // NConvert namespace
    } // Details namespace
  #endif

  namespace Details {
    namespace NUnion {
      template <typename Ty>
      inline void callDestructor(Ty* a_ptr) {
        a_ptr->~Ty();
      }

      template <typename Type, typename FarType, int TypeIndex>
      struct FCF_UNION_DECL_VISIBILITY_HIDDEN BaseTypeHelper{
        typedef FarType far_type;
        enum { type_index = TypeIndex };

        inline static FarType init()    { return FarType{}; };
        inline static bool isnumber()   { return false; };
        inline static bool isunsigned() { return false; };
        inline static bool isdouble()   { return false; };
        template <typename TValue> inline static long long tonumber(TValue& a_value){ return 0; }
        template <typename TValue> inline static double todouble(TValue& a_value){ return 0; }
        template <typename TValue> inline static bool isfalse(const TValue&) { return false; }
        template <typename TValue> inline static const char* cstr(TValue& a_str){ return (const char*)0; }
        template <typename TValue> inline static std::string str(TValue& a_value){ return std::string(); }
      };

      template <typename Type, typename FarType, int TypeIndex>
      struct FCF_UNION_DECL_VISIBILITY_HIDDEN BaseBoolTypeHelper : public BaseTypeHelper<Type, FarType, TypeIndex> {
        inline static bool init()                                                         { return false; };
        template <typename TValue> inline static FarType farValue(const TValue& a_value)  { return (FarType)a_value; }
        template <typename TValue> inline static bool isfalse(const TValue& a_value)      { return a_value; }
      };

      template <typename Type, typename FarType, int TypeIndex>
      struct FCF_UNION_DECL_VISIBILITY_HIDDEN BaseNumberTypeHelper : public BaseTypeHelper<Type, FarType, TypeIndex> {
        inline static Type init()                                                         { return 0; };
        template <typename TValue> inline static FarType farValue(const TValue& a_value) { return (FarType)a_value; }
        inline static bool isnumber()   { return true; };
        inline static bool isunsigned() { return false; };
        inline static bool isdouble()   { return false; };
        template <typename TValue> inline static long long tonumber(TValue& a_value){ return (long long)a_value; }
        template <typename TValue> inline static double todouble(TValue& a_value){ return (double)a_value; }
        template <typename Ty> inline static bool isfalse(Ty& a_value)   { return !a_value;  }
        template <typename Ty> inline static const char* cstr(Ty& a_str) { return 0; }
        template <typename Ty> inline static std::string str(Ty& a_value){ return std::to_string(a_value); }
      };

      template <typename Type, typename FarType, int TypeIndex>
      struct FCF_UNION_DECL_VISIBILITY_HIDDEN BaseUnsignedNumberTypeHelper : public BaseNumberTypeHelper<Type, FarType, TypeIndex> {
        inline static bool isnumber()   { return true; };
        inline static bool isunsigned() { return true; };
        inline static bool isdouble()   { return false; };
        template <typename Ty> inline static bool isfalse(Ty& a_value)   { return !a_value;  }
        template <typename Ty> inline static const char* cstr(Ty& a_str) { return 0; }
        template <typename Ty> inline static std::string str(Ty& a_value){ return std::to_string(a_value); }
      };

      template <typename Type, typename FarType, int TypeIndex>
      struct FCF_UNION_DECL_VISIBILITY_HIDDEN BaseDoubleTypeHelper : public BaseNumberTypeHelper<Type, FarType, TypeIndex> {
        inline static bool isdouble() { return true; };
      };

      template <typename Type, typename FarType, int TypeIndex>
      struct FCF_UNION_DECL_VISIBILITY_HIDDEN BaseCStrTypeHelper : public BaseTypeHelper<Type, FarType, TypeIndex>{
        template <typename TValue> inline static FarType farValue(const TValue& a_value) { return FarType(a_value); }
        template <typename Ty> inline static bool isfalse(Ty& a_str){ return !a_str[0]; }
        template <typename Ty> inline static const char* cstr(Ty& a_str){ return &a_str[0]; }
        template <typename Ty> inline static std::string str(Ty& a_str){ return std::string(&a_str[0]); }
      };

      template <typename Type, typename FarType, int TypeIndex>
      struct FCF_UNION_DECL_VISIBILITY_HIDDEN BaseStrTypeHelper : public BaseCStrTypeHelper<Type, FarType, TypeIndex>{
        template <typename Ty> inline static std::string& str(Ty& a_str){ return a_str; }
        template <typename TValue> inline static FarType& farValue(const TValue& a_value) { return (FarType&)a_value; }
      };

      template <typename Type, typename FarType, int TypeIndex>
      struct FCF_UNION_DECL_VISIBILITY_HIDDEN BaseOStreamTypeHelper : public BaseCStrTypeHelper<Type, FarType, TypeIndex>{
        template <typename TValue> inline static std::string str(TValue& a_str){ return std::string(); }
        template <typename TValue> inline static FarType farValue(const TValue& a_value) { return FarType{}; }
      };


      template <typename Ty>
      struct FCF_UNION_DECL_VISIBILITY_HIDDEN TypeHelper : public BaseTypeHelper<Ty, Ty, UT_UNDEFINED>                                                                 { };
      template <>
      struct FCF_UNION_DECL_VISIBILITY_HIDDEN TypeHelper<fcf::Undefined>: public BaseTypeHelper<fcf::Undefined, fcf::Undefined, UT_UNDEFINED>                          {
        template <typename TValue> inline static far_type farValue(const TValue& a_value) { return far_type{}; }
      } ;
      template <>
      struct FCF_UNION_DECL_VISIBILITY_HIDDEN TypeHelper<fcf::Null>: public BaseTypeHelper<fcf::Null, fcf::Null, UT_NULL>                                              {
        template <typename TValue> inline static far_type farValue(const TValue& a_value) { return far_type{}; }
      };
      template <>
      struct FCF_UNION_DECL_VISIBILITY_HIDDEN TypeHelper<bool>: public BaseBoolTypeHelper<bool, bool, UT_BOOL>                                                         { };
      template <>
      struct FCF_UNION_DECL_VISIBILITY_HIDDEN TypeHelper<double>: public BaseDoubleTypeHelper<double, double, UT_DOUBLE>                                               { };
      template <>
      struct FCF_UNION_DECL_VISIBILITY_HIDDEN TypeHelper<float>: public BaseDoubleTypeHelper<float, double, UT_DOUBLE>                                                 { };
      template <>
      struct FCF_UNION_DECL_VISIBILITY_HIDDEN TypeHelper<int>: public BaseNumberTypeHelper<int, int, UT_INT>                                                           { };
      template <>
      struct FCF_UNION_DECL_VISIBILITY_HIDDEN TypeHelper<unsigned int>: public BaseUnsignedNumberTypeHelper<unsigned int, unsigned int, UT_UINT>                       { };
      template <>
      struct FCF_UNION_DECL_VISIBILITY_HIDDEN TypeHelper<short int>: public BaseNumberTypeHelper<short int, int, UT_INT>                                               { };
      template <>
      struct FCF_UNION_DECL_VISIBILITY_HIDDEN TypeHelper<unsigned short int>: public BaseUnsignedNumberTypeHelper<unsigned short int, unsigned int, UT_UINT>           { };
      template <>
      struct FCF_UNION_DECL_VISIBILITY_HIDDEN TypeHelper<long>: public BaseNumberTypeHelper<long, long long, UT_LONGLONG>                                             { };
      template <>
      struct FCF_UNION_DECL_VISIBILITY_HIDDEN TypeHelper<unsigned long>: public BaseUnsignedNumberTypeHelper<unsigned long long, unsigned long long, UT_ULONGLONG>    { };
      template <>
      struct FCF_UNION_DECL_VISIBILITY_HIDDEN TypeHelper<long long>: public BaseNumberTypeHelper<long long, long long, UT_LONGLONG>                                   { };
      template <>
      struct FCF_UNION_DECL_VISIBILITY_HIDDEN TypeHelper<unsigned long long>: public BaseUnsignedNumberTypeHelper<unsigned long long, unsigned long long, UT_ULONGLONG>           { };
      template <>
      struct FCF_UNION_DECL_VISIBILITY_HIDDEN TypeHelper<std::string>: public BaseStrTypeHelper<std::string, std::string, UT_STRING>                                   { };
      template <>
      struct FCF_UNION_DECL_VISIBILITY_HIDDEN TypeHelper< std::basic_ostream<char>  >: public BaseOStreamTypeHelper<std::basic_ostream<char>, std::string, UT_STRING>  { };
      template <>
      struct FCF_UNION_DECL_VISIBILITY_HIDDEN TypeHelper<char*>: public BaseCStrTypeHelper<char*, std::string, UT_STRING>                                              { };
      template <>
      struct FCF_UNION_DECL_VISIBILITY_HIDDEN TypeHelper<const char*>: public BaseCStrTypeHelper<const char*, std::string, UT_STRING>                                  { };
      template <size_t Size>
      struct FCF_UNION_DECL_VISIBILITY_HIDDEN TypeHelper< char[Size] >: public BaseCStrTypeHelper< char[Size], std::string, UT_STRING>                                 { };
      template <size_t Size>
      struct FCF_UNION_DECL_VISIBILITY_HIDDEN TypeHelper< const char[Size] >: public BaseCStrTypeHelper< const char[Size], std::string, UT_STRING>                     { };
      template <>
      struct FCF_UNION_DECL_VISIBILITY_HIDDEN TypeHelper<UnionVector>: public BaseTypeHelper<UnionVector, UnionVector, UT_VECTOR>                                      {
        template <typename TValue> inline static far_type& farValue(const TValue& a_value) { return (far_type&)a_value; }
      };
      template <>
      struct FCF_UNION_DECL_VISIBILITY_HIDDEN TypeHelper<UnionMap>: public BaseTypeHelper<UnionMap, UnionMap, UT_MAP>                                                  {
        template <typename TValue> inline static far_type& farValue(const TValue& a_value) { return (far_type&)a_value; }
      };

    } // NUnion namespace
  } // Details namespace



  #ifdef FCF_UNION_IMPLEMENTATION
    namespace Details {
      namespace NUnion {
        template <typename Ty>
        struct ExecutorSetArgs;


        enum ExecutorIndexs{
          EI_CONST_GET,
          EI_TO_JSON,
          EI_GET,
          EI_SET,
          EI_INITIALIZE,
          EI_COPY,
          EI_CONVERT,
          EI_DESTROY,
          EI_LESS,
          EI_LESS2,
          EI_EQUAL,
          EI_EQUAL2,
        };

        template <typename TIterator>
        FCF_UNION_DECL_VISIBILITY_HIDDEN bool orderLess(const TIterator& a_left, const TIterator& a_right){
          if (a_left->second.order && a_right->second.order) {
            return a_left->second.order != a_right->second.order
                ? a_left->second.order < a_right->second.order
                : a_left->first.lessStr(a_right->first);
          } else if (a_left->second.order != a_right->second.order) {
            return a_left->second.order > a_right->second.order;
          } else {
            return a_left->first.lessStr(a_right->first);
          }
          return true;
        }

        template <typename Ty>
        struct StringCleaner{
          inline void operator()(Ty& a_value){
          }
        };
        template <>
        struct StringCleaner<std::string>{ inline void operator()(std::string& a_value){ a_value.clear(); } };

        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Cmp{
          template <typename TLeft, typename  TRight>
          static bool equal(TLeft& a_left, TRight a_right, bool a_strict);
          template <typename TLeft, typename  TRight>
          static bool lessStr(TLeft& a_left, TRight a_right);
          static int cmpStr(const char* a_left, const char* a_right);
        };


        template <int ExecutorIndex, typename TSelf, typename Ty>
        struct Executor{};

        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Selector {
          template <typename TResult, int ExecutorIndex, typename Ty, typename TOperand>
          static TResult select(int a_type, TOperand& a_operand) {
            switch(a_type){
              case UT_UNDEFINED:  return Executor<ExecutorIndex, Undefined, Ty>()(a_operand);
              case UT_NULL:       return Executor<ExecutorIndex, Null, Ty>()(a_operand);
              case UT_INT:        return Executor<ExecutorIndex, int, Ty>()(a_operand);
              case UT_LONGLONG:   return Executor<ExecutorIndex, long long, Ty>()(a_operand);
              case UT_ULONGLONG:  return Executor<ExecutorIndex, unsigned long long, Ty>()(a_operand);
              case UT_UINT:       return Executor<ExecutorIndex, unsigned int, Ty>()(a_operand);
              case UT_DOUBLE:     return Executor<ExecutorIndex, double, Ty>()(a_operand);
              case UT_BOOL:       return Executor<ExecutorIndex, bool, Ty>()(a_operand);
              case UT_STRING:     return Executor<ExecutorIndex, std::string, Ty>()(a_operand);
              case UT_VECTOR:     return Executor<ExecutorIndex, UnionVector, Ty>()(a_operand);
              case UT_MAP:        return Executor<ExecutorIndex, UnionMap, Ty>()(a_operand);
            }
            throw UnionException("Invalid union type");
          }
        };

        template <typename TSelf, typename Ty>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Executor<EI_CONST_GET, TSelf, Ty>{
          template <typename TCallData>
          void operator()(TCallData& a_callData){
            typedef typename TCallData::options_type options_type;
            typedef typename Details::NUnion::TypeHelper<Ty>::far_type far_type;
            fcf::Details::NConvert::Receiver<Ty, options_type> rc{a_callData.destination, a_callData.options};
            fcf::Details::NConvert::ConstResolver<TSelf> r{*(TSelf*)(void*)&a_callData.value} ;
            fcf::Details::NConvert::Converter<TSelf, far_type>()(r, rc, false, a_callData.error);
          }
        };

        template <typename Ty>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Executor<EI_CONST_GET, Ty, Ty>{
          template <typename TCallData>
          inline void operator()(TCallData& a_callData){
            a_callData.destination = *(Ty*)(void*)&a_callData.value.vint;
          }
        };

        template <typename TSelf, typename Ty>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Executor<EI_TO_JSON, TSelf, Ty>{
          template <typename TCallData>
          void operator()(TCallData& a_callData){
            typedef typename TCallData::options_type options_type;
            typedef typename Details::NUnion::TypeHelper<Ty>::far_type far_type;
            fcf::Details::NConvert::Receiver<Ty, options_type> rc{a_callData.destination, a_callData.options};
            fcf::Details::NConvert::ConstResolver<TSelf> r{*(TSelf*)(void*)&a_callData.value} ;
            fcf::Details::NConvert::Converter<TSelf, far_type, JSONFormat>()(r, rc, false, a_callData.error);
          }
        };

        template <typename TSelf, typename Ty>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Executor<EI_GET, TSelf, Ty>{
          template <typename TCallData>
          Ty& operator()(TCallData& a_callData){
            UnionType type = (UnionType)TypeHelper<Ty>::type_index;
            if (type != (UnionType)a_callData.type) {
              Ty newValue;

              const char* errorBuffer = 0;
              struct CallData{
                typedef UnionStringifyOptions options_type;
                const UnionValue&        value;
                Ty&                      destination;
                const char**             error;
                const UnionStringifyOptions   options;
              };
              CallData cd{a_callData.value, newValue, (a_callData.error ? a_callData.error : &errorBuffer)};
              Details::NUnion::Selector::select<void, Details::NUnion::EI_CONST_GET, Ty>(a_callData.type, cd);
              if (*cd.error){
                *cd.error = 0;
                newValue = Details::NUnion::TypeHelper<Ty>::init();
              }
              Details::NUnion::Selector::select<void, Details::NUnion::EI_DESTROY, TNOP>(a_callData.type, a_callData.value);
              a_callData.type = UT_UNDEFINED;
              Details::NUnion::ExecutorSetArgs<Ty> esa{type, a_callData.value, newValue};
              Details::NUnion::Executor<fcf::Details::NUnion::EI_SET, Ty, TNOP>()(esa);
              a_callData.type = type;
            }
            return *(Ty*)(void*)&a_callData.value.vint;
          }
        };

        template <typename TSelf, typename Ty>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Executor<EI_CONVERT, TSelf, Ty>{
          template <typename TCallData>
          inline void operator()(TCallData a_callData){
            typedef typename TCallData::receiver_type::value_type value_type;
            fcf::Details::NConvert::ConstResolver<TSelf> r{*(TSelf*)(void*)&a_callData.value} ;
            fcf::Details::NConvert::Converter<TSelf, value_type>()(r, a_callData.receiver, true, a_callData.error);
          }
        };

        ///////////////////////////////////////
        // Set
        ///////////////////////////////////////
        template <typename Ty>
        struct ExecutorSetArgs {
          UnionType&  dstType;
          UnionValue& dstValue;
          const Ty&   value;
        };

        template <typename Ty>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Executor<EI_SET, Ty, TNOP>{
          template <typename TData>
          inline void operator()(TData& a_data){
            a_data.dstType = (UnionType)Details::NUnion::TypeHelper<Ty>::type_index;
            *((Ty*)(void*)&a_data.dstValue.vint) = a_data.value;
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Executor<fcf::Details::NUnion::EI_SET, std::string, TNOP>{
          template <typename TData>
          inline void operator()(TData& a_data){
            a_data.dstType = UT_STRING;
            new ((void*)&a_data.dstValue.vstring[0])std::string(a_data.value);
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Executor<fcf::Details::NUnion::EI_SET, UnionVector, TNOP>{
          template <typename TData>
          inline void operator()(TData& a_data){
            a_data.dstType = UT_VECTOR;
            new ((void*)&a_data.dstValue.vstring[0])UnionVector(a_data.value);
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Executor<fcf::Details::NUnion::EI_SET, UnionMap, TNOP>{
          template <typename TData>
          inline void operator()(TData& a_data){
            a_data.dstType = UT_MAP;
            new ((void*)&a_data.dstValue.vstring[0])UnionMap(a_data.value);
          }
        };

        ///////////////////////////////////////
        // Initiliaze
        ///////////////////////////////////////
        template <typename Ty>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Executor<EI_INITIALIZE, Ty, TNOP>{
          template <typename TData>
          inline void operator()(TData& a_data){
            typedef typename Details::NUnion::TypeHelper<Ty>::far_type far_type;
            Details::NUnion::TypeHelper<Ty>::init();
            new ((void*)&a_data.destination.vint)far_type( Details::NUnion::TypeHelper<Ty>::init() );
          }
        };


        ///////////////////////////////////////
        // Less
        ///////////////////////////////////////
        template <typename Ty>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Executor<EI_LESS, Ty, TNOP>{
          template <typename TData>
          inline bool operator()(TData& a_data){
            Ty& left = *(Ty*)(void*)&a_data.left;
            return Cmp::lessStr(left, a_data.right);
          }
        };

        template <typename Ty>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Executor<EI_LESS2, Ty, TNOP>{
          template <typename TData>
          bool operator()(TData& a_data){
            Ty& right = *(Ty*)(void*)&a_data.right;
            struct CallData{
              const UnionValue&  left;
              Ty&                right;
            };
            CallData cd{a_data.left, right};
            return Details::NUnion::Selector::select<bool, Details::NUnion::EI_LESS, TNOP>(a_data.leftType, cd);
          }
        };

        template <typename Ty>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Executor<EI_EQUAL2, Ty, TNOP>{
          template <typename TData>
          bool operator()(TData& a_data){
            Ty& right = *(Ty*)(void*)&a_data.right;
            struct CallData{
              const UnionValue&  left;
              Ty&                right;
              bool               strict;
            };
            CallData cd{a_data.left, right, a_data.strict};
            return Details::NUnion::Selector::select<bool, Details::NUnion::EI_EQUAL, TNOP>(a_data.leftType, cd);
          }
        };

        template <typename Ty>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Executor<EI_EQUAL, Ty, TNOP>{
          template <typename TData>
          inline bool operator()(TData& a_data){
            Ty& left = *(Ty*)(void*)&a_data.left;
            return Cmp::equal(left, a_data.right, a_data.strict);
          }
        };


        ///////////////////////////////////////
        // Copy
        ///////////////////////////////////////
        template <typename Ty>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Executor<EI_COPY, Ty, TNOP>{
          template <typename TCallData>
          inline void operator()(TCallData a_callData){
            Ty& value = *((Ty*)(void*)&a_callData.source.vint);
            new ((void*)&a_callData.destination.vint)Ty(value);
          }
        };
        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Executor<EI_COPY, UnionVector, TNOP>{
          template <typename TCallData>
          inline void operator()(TCallData a_callData){
            UnionVector& value = *((UnionVector*)(void*)&a_callData.source.vvector[0]);
            new ((UnionVector*)&a_callData.destination.vvector[0])UnionVector(value);
          }
        };

        ///////////////////////////////////////
        // Destroy
        ///////////////////////////////////////
        template <typename Ty>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Executor<EI_DESTROY, Ty, TNOP>{
          inline void operator()(UnionValue& a_uvalue){
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Executor<EI_DESTROY, std::string, TNOP>{
          inline void operator()(UnionValue& a_uvalue){
            std::string* value = (std::string*)(void*)&a_uvalue.vstring[0];
            callDestructor(value);
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Executor<EI_DESTROY, UnionVector, TNOP>{
          inline void operator()(UnionValue& a_uvalue){
            UnionVector* value = (UnionVector*)(void*)&a_uvalue.vstring[0];
            callDestructor(value);
          }
        };

        template <>
        struct FCF_UNION_DECL_VISIBILITY_HIDDEN Executor<EI_DESTROY, UnionMap, TNOP>{
          inline void operator()(UnionValue& a_uvalue){
            UnionMap* value = (UnionMap*)(void*)&a_uvalue.vstring[0];
            callDestructor(value);
          }
        };

      } // NUnion namespace
    } // Details namespace
  #endif // #ifdef FCF_UNION_IMPLEMENTATION

  inline std::basic_ostream<char>& operator<<(std::basic_ostream<char>& a_stream, const fcf::Union& a_unioun) {
    fcf::UnionStringifyOptions options;
    options.mode = SF_VALUE;
    a_unioun.stringify(a_stream, options);
    return a_stream;
  }


  #ifdef FCF_UNION_IMPLEMENTATION
    namespace Details {
      namespace NConvert {

        ///////////////////////////////////////
        // Convertion from vector
        ///////////////////////////////////////
        template <typename TResolver, typename TReceiver>
        void Converter<fcf::UnionVector, std::string, fcf::TNOP>::operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
          bool friendly = a_receiver.options.friendly;

          const UnionVector& uv = a_resolver();
          if (friendly){
            bool objmode = false;
            for(size_t i = 0; i < uv.size(); ++i){
              if (uv[i].type == UT_MAP || uv[i].type == UT_VECTOR){
                objmode = true;
                break;
              }
            }
            friendly = objmode;
          }

          a_receiver(friendly ? "[\n" : "[" );
          for(size_t i = 0; i < uv.size(); ++i){
            UnionType type = uv[i].type;
            struct TCallData {
              typedef TReceiver receiver_type;
              TReceiver&        receiver;
              const UnionValue& value;
              const char**      error;
            };
            TCallData cd{a_receiver, uv[i].value, a_dstErrorMessage};
            ++a_receiver.level;
            if (friendly) {
              for(int ti = 0; ti < a_receiver.level; ++ti) {
                a_receiver(a_receiver.options.tab);
              }
            }
            Details::NUnion::Selector::select<void, Details::NUnion::EI_CONVERT, TNOP>(type, cd);
            --a_receiver.level;
            if (i != uv.size()-1 ) {
              a_receiver(friendly ? "," : ", ");
            }
            if (friendly){
              a_receiver("\n");
            }
          }
          a_receiver("]");
        };

        template <typename TResolver>
        FCF_UNION_DECL_VISIBILITY_HIDDEN bool parseValue(TResolver& a_resolver, Union& a_union, const char** a_dstErrorMessage){
          skipSpaces(a_resolver);
          if (a_resolver.end()) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "There is not enough data in the buffer to parse the value", a_resolver, true);
            return false;
          }
          unsigned char c = (unsigned char)a_resolver.read();
          if (c == '-' || (c >= (unsigned char)'0' && c <= (unsigned char)'9')){
            bool               minus;
            bool               point;
            double             dvalue;
            long long          ivalue;
            unsigned long long uvalue;
            parseNumber(a_resolver, false, &minus, &point, &dvalue, &ivalue, &uvalue, a_dstErrorMessage);
            if (point) {
              a_union.set(dvalue);
            } else if (minus) {
              a_union.set(ivalue);
            } else {
              a_union.set(uvalue);
            }
            return true;
          } else if (c == 'n') {
            parseNull(a_resolver, a_dstErrorMessage);
            a_union.set(fcf::null);
            return true;
          } else if (c == 'u') {
            parseUndefined(a_resolver, a_dstErrorMessage);
            a_union.set(fcf::undefined);
            return true;
          } else if (c == 'f' || c == 't') {
            bool value = parseBool(a_resolver, a_dstErrorMessage);
            a_union.set(value);
            return true;
          } else if (c == '\"' || c == '\'') {
            std::string value = parseInnerString(a_resolver, a_dstErrorMessage);
            a_union.set(value);
            return true;
          } else if (c == '[') {
            a_union.set(UnionVector());
            Receiver<UnionVector, UnionStringifyOptions> r{a_union.ref<UnionVector>(), UnionStringifyOptions()};
            parseVector(a_resolver, r, a_dstErrorMessage);
            return true;
          } else if (c == '{') {
            a_union.set(UnionMap());
            Receiver<UnionMap, UnionStringifyOptions> r{a_union.ref<UnionMap>(), UnionStringifyOptions()};
            parseMap(a_resolver, r, a_dstErrorMessage);
            return true;
          }
          return false;
        }

        template <typename TResolver, typename TReceiver>
        FCF_UNION_DECL_VISIBILITY_HIDDEN void parseVector(TResolver& a_resolver, TReceiver& a_receiver, const char** a_dstErrorMessage) {
          a_receiver.get().clear();
          skipSpaces(a_resolver);
          if (a_resolver.end()) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect vector format", a_resolver, false);
            return;
          }
          unsigned char c = (unsigned char)a_resolver.read();
          if (c != '[') {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect vector format", a_resolver, false);
            return;
          }
          a_resolver.next();
          if (a_resolver.end()) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect vector format", a_resolver, false);
            return;
          }

          bool lastValid = true;

          while(!a_resolver.end()) {
            skipSpaces(a_resolver);
            if (a_resolver.end()) {
              FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect vector format", a_resolver, false);
              return;
            }
            Union u;
            if (parseValue(a_resolver, u, a_dstErrorMessage)){
              a_receiver(u);

              skipSpaces(a_resolver);
              if (a_resolver.end()) {
                FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect vector format", a_resolver, false);
                return;
              }

              c = (unsigned char)a_resolver.read();
              if (c == ',') {
                a_resolver.next();
                if (a_resolver.end()) {
                  FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect vector format", a_resolver, false);
                  return;
                }
              }
              lastValid = true;
            } else {
              if (!lastValid){
                FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect vector format", a_resolver, false);
                return;
              }
              lastValid = false;
            }
            c = (unsigned char)a_resolver.read();
            if (c == ']') {
              a_resolver.next();
              return;
            }
          }
          FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect vector format", a_resolver, false);
        }

        template <typename TResolver, typename TReceiver>
        FCF_UNION_DECL_VISIBILITY_HIDDEN void parseMap(TResolver& a_resolver, TReceiver& a_receiver, const char** a_dstErrorMessage) {
          a_receiver.get().clear();
          skipSpaces(a_resolver);
          if (a_resolver.end()) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect map format", a_resolver, false);
            return;
          }
          unsigned char c = (unsigned char)a_resolver.read();
          if (c != '{') {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect map format", a_resolver, false);
            return;
          }
          a_resolver.next();
          if (a_resolver.end()) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect map format", a_resolver, false);
            return;
          }

          bool lastValid = true;

          while(!a_resolver.end()) {
            skipSpaces(a_resolver);
            if (a_resolver.end()) {
              FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect map format", a_resolver, false);
              return;
            }

            Union key;
            bool validKey = parseKey(a_resolver, key, a_dstErrorMessage);

            skipSpaces(a_resolver);
            if (a_resolver.end()) {
              FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect map format", a_resolver, false);
              return;
            }

            c = (unsigned char)a_resolver.read();

            if (!validKey) {
              if (c == '}') {
                a_resolver.next();
                return;
              } else {
                FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect map format", a_resolver, false);
                return;
              }
            }

            if (c != ':') {
              FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect map format", a_resolver, false);
              return;
            }

            a_resolver.next();
            if (a_resolver.end()) {
              FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect map format", a_resolver, false);
              return;
            }

            Union value;
            bool validValue = parseValue(a_resolver, value, a_dstErrorMessage);

            if (!validValue){
              FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect map format", a_resolver, false);
              return;
            }

            a_receiver(std::pair<Union, Union>(key, value));

            skipSpaces(a_resolver);
            if (a_resolver.end()) {
              FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect map format", a_resolver, false);
              return;
            }

            c = (unsigned char)a_resolver.read();
            if (c == ',') {
              a_resolver.next();
              if (a_resolver.end()) {
                FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect map format", a_resolver, false);
                return;
              }
            }
          }
          FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect map format", a_resolver, false);
        }



        ///////////////////////////////////////
        // Convertion from vector
        ///////////////////////////////////////
        template <typename TResolver, typename TReceiver>
        void Converter<fcf::UnionMap, std::string, fcf::TNOP>::operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage) {
          const UnionMap& um = a_resolver();
          bool friendly = !um.empty() && a_receiver.options.friendly;

          a_receiver(friendly ? "{\n" : "{");

          std::vector< UnionMap::const_iterator > iterators(um.size());
          size_t i = 0;
          for(UnionMap::const_iterator it = um.cbegin(); it != um.cend(); ++it){
            iterators[i] = it;
            ++i;
          }
          std::sort(iterators.begin(), iterators.end(),  Details::NUnion::orderLess<UnionMap::const_iterator>);

          std::vector< UnionMap::const_iterator >::iterator itIt = iterators.begin();
          ++a_receiver.level;
          bool lastTab = false;
          while(itIt != iterators.end()){
            if (friendly) {
              for(int it = 0; it < a_receiver.level; ++it) {
                a_receiver(a_receiver.options.tab);
              }
            }
            std::string key = (*itIt)->first.get<std::string>();
            fcf::Details::NConvert::ConstResolver<std::string> r{key} ;
            Converter<std::string, std::string, TNOP>()(r, a_receiver, true, a_dstErrorMessage);
            a_receiver(": ");
            struct TCallData{
              typedef TReceiver receiver_type;
              TReceiver&   receiver;
              const UnionValue& value;
              const char** error;
            };
            TCallData cd{a_receiver, (*itIt)->second.value, a_dstErrorMessage};
            Details::NUnion::Selector::select<void, Details::NUnion::EI_CONVERT, TNOP>((*itIt)->second.type, cd);
            ++itIt;
            if (itIt != iterators.end()) {
              a_receiver(friendly ? "," : ", ");
            }
            if (friendly) {
              a_receiver("\n");
              lastTab = true;
            }
          }
          --a_receiver.level;
          if (lastTab){
            for(int it = 0; it < a_receiver.level; ++it) {
              a_receiver(a_receiver.options.tab);
            }
          }
          a_receiver("}");
        };

        ///////////////////////////////////////
        // Convertion from map
        ///////////////////////////////////////
        template <typename TResolver, typename TReceiver>
        void Converter<UnionMap, UnionVector, TNOP>::operator()(TResolver& a_resolver, TReceiver& a_receiver, bool a_inner, const char** a_dstErrorMessage){
          a_receiver.get().clear();
          const UnionMap& um = a_resolver();

          std::vector< UnionMap::const_iterator > iterators(um.size());
          size_t i = 0;
          for(UnionMap::const_iterator it = um.cbegin(); it != um.cend(); ++it){
            iterators[i] = it;
            ++i;
          }
          std::sort(iterators.begin(), iterators.end(),  Details::NUnion::orderLess<UnionMap::const_iterator>);
          std::vector< UnionMap::const_iterator >::iterator itIt = iterators.begin();

          for(; itIt != iterators.cend(); ++itIt) {
            a_receiver((*itIt)->second);
          }
        };

        template <typename TResolver>
        bool parseKey(TResolver& a_resolver, Union& a_key, const char** a_dstErrorMessage) {
          skipSpaces(a_resolver);
          if (a_resolver.end()) {
            FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect key format", a_resolver, false);
            return false;
          }
          unsigned char c = (unsigned char)a_resolver.read();
          if (c == '\"' || c == '\''){
            a_key.set(parseInnerString(a_resolver, a_dstErrorMessage));
            return true;
          } else if (c == '-' || (c >= (unsigned char)'0' && c < (unsigned char)'9')){
            bool         minus;
            bool         point;
            double       dvalue;
            int          ivalue;
            unsigned int uvalue;
            parseNumber(a_resolver, false, &minus, &point, &dvalue, &ivalue, &uvalue, a_dstErrorMessage);
            if (point) {
              a_key.set(dvalue);
            } else if (minus) {
              a_key.set(ivalue);
            } else {
              a_key.set(uvalue);
            }
            return true;
          } else if (c != '}'){
            std::string str;
            c = (unsigned char)a_resolver.read();
            while(!a_resolver.end() && c != '{' && c != '}' && c != '[' && c != ']' && c != ':' && c != ',' && c > (unsigned char)' '){
              str += c;
              a_resolver.next();
              c = (unsigned char)a_resolver.read();
            }
            if (str.empty()){
              FCF_THROW_OR_RESULT_ERROR(a_dstErrorMessage, "Incorrect key format", a_resolver, false);
              return false;
            }
            a_key.set(str);
            return true;
          }
          return false;
        }
      } // NConvert namespace
    } // Details namespace
  #endif // #ifdef FCF_UNION_IMPLEMENTATION

  //////////////////////////////////////////////
  // Compare impementation
  ////////////////////////!//////////////////////

  #ifdef FCF_UNION_IMPLEMENTATION
    namespace Details {
      namespace NUnion {

        int Cmp::cmpStr(const char* a_left, const char* a_right) {
          if (strcmp(a_left, "undefined") == 0) {
            if (strcmp(a_right, "undefined") == 0) {
              return 0;
            } else {
              return -1;
            }
          } else if (strcmp(a_right, "undefined") == 0) {
            return 1;
          }

          if (strcmp(a_left, "null") == 0) {
            if (strcmp(a_right, "null") == 0) {
              return 0;
            } else {
              return -1;
            }
          } else if (strcmp(a_right, "null") == 0) {
            return 1;
          }

          if (strcmp(a_left, "false") == 0) {
            if (strcmp(a_right, "false") == 0) {
              return 0;
            } else {
              return -1;
            }
          } else if (strcmp(a_right, "false") == 0) {
            return 1;
          }

          if (strcmp(a_left, "true") == 0) {
            if (strcmp(a_right, "true") == 0) {
              return 0;
            } else {
              return -1;
            }
          } else if (strcmp(a_right, "true") == 0) {
            return 1;
          }

          bool first = false;
          bool second = false;

          long long l1;
          unsigned long long l2;
          double l3;
          bool lm;
          bool ld;

          const char* error = 0;
          Details::NConvert::ConstResolver<const char*> lr(a_left);
          parseNumber(lr, false, &lm, &ld, &l1, &l2, &l3, &error);
          if (!error) {
            first = true;
          }

          long long r1;
          unsigned long long r2;
          double r3;
          bool rm;
          bool rd;
          error = 0;
          Details::NConvert::ConstResolver<const char*> rr(a_right);
          parseNumber(rr, false, &rm, &rd, &r1, &r2, &r3, &error);
          if (!error) {
            second = true;
          }

          if (first && second) {
            if (rd || ld) {
              return l3 == r3 ? 0 :
                     l3 < r3  ? -1 :
                                1;
            }
            if (!lm || !rm) {
              return l2 == r2 ? 0 :
                     l2 < r2  ? -1 :
                                1;
            }
            return l1 == r1 ? 0 :
                   l1 < r1  ? -1 :
                              1;
          }
          if (!first && !second) {
            return strcmp(a_left, a_right);
          }

          if (first) {
            return -1;
          } else {
            return 1;
          }
        }

        template <typename TLeft, typename  TRight>
        bool Cmp::lessStr(TLeft& a_left, TRight a_right) {
          typedef typename TypeHelper<TLeft>::far_type  left_far_type;
          typedef typename TypeHelper<TRight>::far_type right_far_type;
          int leftTypeIndex  = TypeHelper<left_far_type>::type_index;
          int rightTypeIndex = TypeHelper<right_far_type>::type_index;
          if (leftTypeIndex == UT_STRING || rightTypeIndex == UT_STRING) {
            const char* leftCStr = TypeHelper<TLeft>::cstr(a_left);
            const char* rightCStr = TypeHelper<TRight>::cstr(a_right);
            if (leftCStr && rightCStr){
              return cmpStr(leftCStr, rightCStr) < 0;
            } else if (leftCStr && !rightCStr){
              UnionStringifyOptions so;
              std::string rstr;
              fcf::Details::NConvert::ConstResolver< right_far_type > r{a_right};
              fcf::Details::NConvert::Receiver<std::string, UnionStringifyOptions> rc{rstr, so};
              const char* error = 0;
              fcf::Details::NConvert::Converter<right_far_type, std::string, TNOP>()(r, rc, false, &error);
              if (error) {
                return false;
              }
              return cmpStr(leftCStr, rstr.c_str()) < 0;
            } else {
              UnionStringifyOptions so;
              std::string lstr;
              fcf::Details::NConvert::ConstResolver< left_far_type > r{a_left};
              fcf::Details::NConvert::Receiver<std::string, UnionStringifyOptions> rc{lstr, so};
              const char* error = 0;
              fcf::Details::NConvert::Converter<left_far_type, std::string, TNOP>()(r, rc, false, &error);
              if (error) {
                return true;
              }
              return cmpStr(lstr.c_str(), rightCStr) < 0;
            }
          }
          if (leftTypeIndex == UT_UNDEFINED) {
            if (rightTypeIndex == UT_UNDEFINED) {
              return false;
            } else {
              return true;
            }
          } else if (rightTypeIndex == UT_UNDEFINED) {
            return false;
          }
          if (leftTypeIndex == UT_NULL) {
            if (rightTypeIndex == UT_NULL) {
              return false;
            } else {
              return true;
            }
          } else if (rightTypeIndex == UT_NULL) {
            return false;
          }
          if (leftTypeIndex == UT_BOOL && !(*(bool*)&a_left)) {
            if (rightTypeIndex == UT_BOOL&& !(*(bool*)&a_right)) {
              return false;
            } else {
              return true;
            }
          } else if (rightTypeIndex == UT_BOOL && !(*(bool*)&a_right)) {
            return false;
          }
          if (leftTypeIndex == UT_BOOL && (*(bool*)&a_left)) {
            if (rightTypeIndex == UT_BOOL&& (*(bool*)&a_right)) {
              return false;
            } else {
              return true;
            }
          } else if (rightTypeIndex == UT_BOOL && (*(bool*)&a_right)) {
            return false;
          }
          bool isLeftNum = leftTypeIndex >= FIRST_NUMBER_TYPE && leftTypeIndex <= LAST_NUMBER_TYPE;
          bool isRightNum = rightTypeIndex >= FIRST_NUMBER_TYPE && rightTypeIndex <= LAST_NUMBER_TYPE;
          if (isLeftNum && isRightNum) {
            bool leftDouble = TypeHelper<TLeft>::isdouble();
            bool rightDouble = TypeHelper<TRight>::isdouble();
            if (leftDouble || rightDouble){
              return TypeHelper<TLeft>::todouble(a_left) < TypeHelper<TRight>::todouble(a_right);
            } else {
              bool leftUnsigned = TypeHelper<TLeft>::isunsigned();
              bool rightUnsigned = TypeHelper<TRight>::isunsigned();
              if (leftUnsigned == rightUnsigned){
                return TypeHelper<TLeft>::tonumber(a_left) < TypeHelper<TRight>::tonumber(a_right);
              } else if (!leftUnsigned && rightUnsigned) {
                long long left = (long long)TypeHelper<TLeft>::tonumber(a_left);
                unsigned long long right = (unsigned long long)TypeHelper<TRight>::tonumber(a_right);
                if (left < 0) {
                  return true;
                } else {
                  return left < right;
                }
              } else {
                long long right = (long long)TypeHelper<TRight>::tonumber(a_right);
                unsigned long long left = (unsigned long long)TypeHelper<TLeft>::tonumber(a_left);
                if (right < 0) {
                  return false;
                } else {
                  return left < right;;
                }
              }
            }
          }
          if (isLeftNum && !isRightNum) {
            return true;
          }
          if (!isLeftNum && isRightNum) {
            return false;
          }
          if (leftTypeIndex < rightTypeIndex) {
            return true;
          }
          return false;
        }

        template <typename TLeft, typename  TRight>
        bool Cmp::equal(TLeft& a_left, TRight a_right, bool a_strict) {
          typedef typename TypeHelper<TLeft>::far_type  left_far_type;
          typedef typename TypeHelper<TRight>::far_type right_far_type;
          int leftTypeIndex  = TypeHelper<left_far_type>::type_index;
          int rightTypeIndex = TypeHelper<right_far_type>::type_index;
          if (a_strict && leftTypeIndex != rightTypeIndex){
            return false;
          }
          if (leftTypeIndex == UT_STRING || rightTypeIndex == UT_STRING) {
            const char* leftCStr = TypeHelper<TLeft>::cstr(a_left);
            const char* rightCStr = TypeHelper<TRight>::cstr(a_right);
            if (leftCStr && rightCStr){
              return strcmp(leftCStr, rightCStr) == 0;
            } else if (leftCStr && !rightCStr) {
              Union u;
              std::stringstream ss(leftCStr);
              fcf::Details::NConvert::ConstResolver< std::basic_istream<char> > r{ss};
              const char* error = 0;
              if (!Details::NConvert::parseValue(r, u, &error) || error) {
                return false;
              }
              if (u.isCompatible<right_far_type>()){
                return Cmp::equal(u.ref<right_far_type>(), a_right, a_strict);
              }
              return false;
            } else if (!leftCStr && rightCStr) {
              Union u;
              std::stringstream ss(rightCStr);
              fcf::Details::NConvert::ConstResolver< std::basic_istream<char> > r{ss};
              const char* error = 0;
              if (!Details::NConvert::parseValue(r, u, &error) || error) {
                return false;
              }
              if (u.isCompatible<left_far_type>()){
                return Cmp::equal(u.ref<left_far_type>(), a_left, a_strict);
              }
              return false;
            }
            return false;
          } else if (leftTypeIndex == UT_BOOL || rightTypeIndex == UT_BOOL){
            return TypeHelper<TLeft>::isfalse(a_left) == TypeHelper<TRight>::isfalse(a_right);
          } else if (leftTypeIndex == UT_UNDEFINED || rightTypeIndex == UT_UNDEFINED) {
            return leftTypeIndex == rightTypeIndex;
          } else if (leftTypeIndex == UT_NULL || rightTypeIndex == UT_NULL) {
            return leftTypeIndex == rightTypeIndex;
          } else {
            bool leftNumber = TypeHelper<TLeft>::isnumber();
            bool rightNumber = TypeHelper<TRight>::isnumber();
            if (leftNumber || rightNumber) {
              if (leftNumber && rightNumber) {
                bool leftDouble = TypeHelper<TLeft>::isdouble();
                bool rightDouble = TypeHelper<TRight>::isdouble();
                if (leftDouble || rightDouble){
                  return TypeHelper<TLeft>::todouble(a_left) == TypeHelper<TRight>::todouble(a_right);
                } else {
                  bool leftUnsigned = TypeHelper<TLeft>::isunsigned();
                  bool rightUnsigned = TypeHelper<TRight>::isunsigned();
                  if (leftUnsigned == rightUnsigned){
                    return TypeHelper<TLeft>::tonumber(a_left) == TypeHelper<TRight>::tonumber(a_right);
                  } else if (!leftUnsigned && rightUnsigned) {
                    long long left = (long long)TypeHelper<TLeft>::tonumber(a_left);
                    unsigned long long right = (unsigned long long)TypeHelper<TRight>::tonumber(a_right);
                    if (left < 0) {
                      return false;
                    } else {
                      return left == right;
                    }
                  } else {
                    long long right = (long long)TypeHelper<TRight>::tonumber(a_right);
                    unsigned long long left = (unsigned long long)TypeHelper<TLeft>::tonumber(a_left);
                    if (right < 0) {
                      return false;
                    } else {
                      return left == right;
                    }
                  }
                }
              } else {
                return false;
              }
            }
            return false;
          }
        }

      } // NUnion namespace
    } // Details namespace
  #endif // #ifdef FCF_UNION_IMPLEMENTATION

  //////////////////////////////////////////////
  // Impementation Details::NConvert
  ////////////////////////!//////////////////////
  #ifdef FCF_UNION_IMPLEMENTATION
    namespace Details {
      namespace NConvert {

        FCF_UNION_DECL_VISIBILITY_HIDDEN bool isDouble(const std::string& a_str, bool a_unsigned) {
          size_t dt = 0;
          size_t m = 0;
          std::string::const_iterator it = a_str.begin();
          if (it != a_str.end() && *it == '-'){
            m = 1;
            ++it;
          }
          for(; it != a_str.end(); ++it){
            if (!std::isdigit(*it)) {
              if (*it == '.' && dt == 0){
                ++dt;
              } else {
                break;
              }
            }
          };
          if (a_unsigned && m) {
            return false;
          }
          bool isEmpty = dt + m >= a_str.length();
          return !isEmpty && it == a_str.end();
        }

      } // NConvert namespace
    } // Details namespace
  #endif // #ifdef FCF_UNION_IMPLEMENTATION



  //////////////////////////////////////////////
  // Union::base_iterator impementation
  ////////////////////////!//////////////////////

  FCF_UNION_DECL_TEMPLATE_EXTERN template struct Union::basic_iterator<Union*, Union&, UnionMap::iterator, UnionVector::iterator>;
  FCF_UNION_DECL_TEMPLATE_EXTERN template struct Union::basic_iterator<const Union*, const Union&, UnionMap::const_iterator, UnionVector::const_iterator>;

  #ifdef FCF_UNION_IMPLEMENTATION
    Union::base_iterator::base_iterator()
      : _vitype(IT_UNDEFINED)
      , _owner(0)
      {
    }

    template <typename TPointer, typename TRef, typename TMapIterator, typename TVectorIterator>
    Union::basic_iterator<TPointer, TRef, TMapIterator, TVectorIterator>::basic_iterator(TVectorIterator a_iterator, const UnionVector* a_owner) {
      _vitype = IT_VECTOR;
      _owner = (void*)a_owner;
      new ((void*)&_viiterator.vivector[0])TVectorIterator(a_iterator);
    }

    Union::base_iterator::base_iterator(oiterators_type a_iterators, std::vector< UnionMap::iterator >::iterator a_iterator, const UnionMap* a_owner)
      : _vitype(IT_OMAP)
      , _owner((void*)a_owner)
      {
      oiterator_type* oit = (oiterator_type*)&_viiterator.voiterators[0];
      new (oit) oiterator_type{a_iterators, a_iterator};
    }

    template <typename TPointer, typename TRef, typename TMapIterator, typename TVectorIterator>
    Union::basic_iterator<TPointer, TRef, TMapIterator, TVectorIterator>::basic_iterator(TMapIterator a_iterator, const UnionMap* a_owner) {
      _vitype = IT_MAP;
      _owner = (void*)a_owner;
      new ((void*)&_viiterator.vimap[0])TMapIterator(a_iterator);
    }

    Union::base_iterator::base_iterator(const base_iterator& a_iterator) {
      _vitype = IT_UNDEFINED;
      operator=(a_iterator);
    }

    Union::base_iterator::~base_iterator() {
      if (_vitype == IT_OMAP) {
        Details::NUnion::callDestructor((oiterator_type*)&_viiterator.vivector[0]);
      } else if (_vitype == IT_MAP) {
        Details::NUnion::callDestructor(((UnionMap::iterator*)&_viiterator.vimap[0]));
      } else if (_vitype == IT_VECTOR) {
        Details::NUnion::callDestructor(((UnionVector::iterator*)&_viiterator.voiterators[0]));
      }
      _vitype = IT_UNDEFINED;
    }

    Union::base_iterator& Union::base_iterator::operator=(const base_iterator& a_iterator) {
      if (_vitype == IT_OMAP) {
        Details::NUnion::callDestructor((oiterator_type*)&_viiterator.voiterators[0]);
      }
      else if (_vitype == IT_MAP) {
        Details::NUnion::callDestructor(((UnionMap::iterator*)&_viiterator.vimap[0]));
      } else if (_vitype == IT_VECTOR) {
        Details::NUnion::callDestructor(((UnionVector::iterator*)&_viiterator.vivector[0]));
      }
      _vitype = a_iterator._vitype;
      _owner = a_iterator._owner;
      if (a_iterator._vitype == IT_OMAP) {
        oiterator_type& oit = *(oiterator_type*)&a_iterator._viiterator.voiterators[0];
        new ((void*)&_viiterator.voiterators[0]) oiterator_type(oit);
      } else if (a_iterator._vitype == IT_MAP) {
        const UnionMap::iterator& source = *(const UnionMap::iterator*)&a_iterator._viiterator.vimap[0];
        new ((void*)&_viiterator.vimap[0])UnionMap::iterator(source);
      } else if (a_iterator._vitype == IT_VECTOR) {
        const UnionVector::iterator& source = *(const UnionVector::iterator*)&a_iterator._viiterator.vivector[0];
        new ((void*)&_viiterator.vivector[0])UnionVector::iterator(source);
      }
      return *this;
    }

    bool Union::base_iterator::operator ==(const base_iterator& a_it) const{
      bool e = _isend();
      bool ae = a_it._isend();
      if (e || ae){
        return e == ae;
      }
      if (_vitype == IT_VECTOR || a_it._vitype == IT_VECTOR) {
        UnionVector::const_iterator& it = *(UnionVector::const_iterator*)(void*)&_viiterator.vivector[0];
        UnionVector::const_iterator& ait = *(UnionVector::const_iterator*)(void*)&a_it._viiterator.vivector[0];
        return it == ait;
      } else if (_vitype == IT_MAP || a_it._vitype == IT_MAP) {
        UnionMap::const_iterator& it = *(UnionMap::const_iterator*)(void*)&_viiterator.vimap[0];
        UnionMap::const_iterator& ait = *(UnionMap::const_iterator*)(void*)&a_it._viiterator.vimap[0];
        return it == ait;
      } else if (_vitype == IT_OMAP || a_it._vitype == IT_OMAP) {
        oiterator_type& it = *(oiterator_type*)(void*)&_viiterator.voiterators[0];
        oiterator_type& ait = *(oiterator_type*)(void*)&a_it._viiterator.voiterators[0];
        return *it.iterator == *ait.iterator;
      } else if (_vitype == IT_MAP || a_it._vitype == IT_OMAP) {
        UnionMap::const_iterator& it = *(UnionMap::const_iterator*)(void*)&_viiterator.vimap[0];
        oiterator_type& ait = *(oiterator_type*)(void*)&a_it._viiterator.voiterators[0];
        return it == *ait.iterator;
      } else if (_vitype == IT_OMAP || a_it._vitype == IT_MAP) {
        return a_it.operator==(*this);
      }
      return true;
    }

    bool Union::base_iterator::operator !=(const base_iterator& a_it) const{
      return !(operator==(a_it));
    }

    Union::base_iterator& Union::base_iterator::operator++(){
      if (_vitype == IT_VECTOR){
        ++(*(UnionVector::const_iterator*)(void*)&_viiterator.vivector[0]);
      } else if (_vitype == IT_MAP) {
        ++(*(UnionMap::const_iterator*)(void*)&_viiterator.vimap[0]);
      } else if (_vitype == IT_OMAP) {
        oiterator_type& oit = *(oiterator_type*)&_viiterator.voiterators[0];
        ++oit.iterator;
      }
      return *this;
    }

    Union::base_iterator Union::base_iterator::operator++(int){
      base_iterator it(*this);
      operator++();
      return it;
    }

    template <typename TPointer, typename TRef, typename TMapIterator, typename TVectorIterator>
    TPointer Union::basic_iterator<TPointer, TRef, TMapIterator, TVectorIterator>::operator->() {
      if (_vitype == IT_VECTOR) {
        return ((TVectorIterator*)(void*)&_viiterator.vivector[0])->operator->();
      } else if (_vitype == IT_MAP) {
        return &(((TMapIterator*)(void*)&_viiterator.vimap[0])->operator->()->second);
      } else if (_vitype == IT_OMAP) {
        oiterator_type& oit = *(oiterator_type*)&_viiterator.voiterators[0];
        UnionMap::iterator mapIt = *oit.iterator;
        return &mapIt->second;
      } else {
        return 0;
      }
    }

    template <typename TPointer, typename TRef, typename TMapIterator, typename TVectorIterator>
    TRef& Union::basic_iterator<TPointer, TRef, TMapIterator, TVectorIterator>::operator*() {
      return *operator->();
    }

    template <typename TPointer, typename TRef, typename TMapIterator, typename TVectorIterator>
    TRef& Union::basic_iterator<TPointer, TRef, TMapIterator, TVectorIterator>::value() {
      return *operator->();
    }

    Union Union::base_iterator::key() const {
      Union u;
      if (_vitype == IT_VECTOR) {
        UnionVector::const_iterator& it = *(UnionVector::const_iterator*)(void*)&_viiterator.vivector[0];
        size_t index = it - ((const UnionVector*)_owner)->cbegin();
        return Union(index);
      } else if (_vitype == IT_MAP) {
        return ((UnionMap::const_iterator*)(void*)&_viiterator.vimap[0])->operator->()->first;
      } else if (_vitype == IT_OMAP) {
        oiterator_type& oit = *(oiterator_type*)&_viiterator.voiterators[0];
        UnionMap::iterator mapIt = *oit.iterator;
        return mapIt->first;
      } else {
        return u;
      }
    }

    bool Union::base_iterator::_isend() const{
      if (_vitype == IT_VECTOR) {
        const UnionVector* uv = (const UnionVector*)_owner;
        UnionVector::const_iterator& it = *(UnionVector::const_iterator*)(void*)&_viiterator.vivector[0];
        size_t i = it - uv->cbegin();
        return i >= (uv->size());
      } else if (_vitype == IT_MAP) {
        const UnionMap* um = (const UnionMap*)_owner;
        UnionMap::const_iterator& it = *(UnionMap::const_iterator*)(void*)&_viiterator.vimap[0];
        return um->cend() == it;
      } else if (_vitype == IT_OMAP) {
        const UnionMap* um = (const UnionMap*)_owner;
        oiterator_type& oit = *(oiterator_type*)&_viiterator.voiterators[0];
        if (oit.iterators->cend() == oit.iterator) {
          return true;
        } else if (*oit.iterator == um->cend()) {
          return true;
        }
        return false;
      }
      return true;
    }
  #endif // #ifdef FCF_UNION_IMPLEMENTATION

  //////////////////////////////////////////////
  // UnionException impementation
  ////////////////////////!//////////////////////
  #ifdef FCF_UNION_IMPLEMENTATION
    std::string UnionException::_tostr(const char* a_message, size_t a_line, size_t a_col) {
      std::string result = std::string(a_message);
      if (a_line != SIZE_MAX || a_col != SIZE_MAX) {
        result += " [";
        if (a_line != SIZE_MAX){
          result += "line: ";
          result += std::to_string(a_line);
          if (a_col != SIZE_MAX) {
            result += "; ";
          }
        }
        if (a_col != SIZE_MAX){
          result += "column: ";
          result += std::to_string(a_col);
        }
        result += "]";
      }
      return result;
    }
  #endif


  //////////////////////////////////////////////
  // Union impementation
  ////////////////////////!//////////////////////

  #ifdef FCF_UNION_IMPLEMENTATION
    Union::Union()
      : type(UT_UNDEFINED), orderc(0), order(0) {
    }
  #endif // #ifdef FCF_UNION_IMPLEMENTATION

  #ifdef FCF_UNION_IMPLEMENTATION
    Union::Union(const Union& a_union)
      : type(UT_UNDEFINED), orderc(a_union.orderc), order(a_union.order)
    {
      type = a_union.type;
      struct CallData{
        const UnionValue& source;
        UnionValue&       destination;
      };
      CallData cd{a_union.value, value};
      Details::NUnion::Selector::select<void, Details::NUnion::EI_COPY, TNOP>(type, cd);
    }
  #endif // #ifdef FCF_UNION_IMPLEMENTATION

  #ifdef FCF_UNION_IMPLEMENTATION
    Union::Union(UnionType a_type)
      : type(UT_UNDEFINED), orderc(0), order(0)
    {
      type = a_type;
      struct CallData{
        UnionValue& destination;
      };
      CallData cd{value};
      Details::NUnion::Selector::select<void, Details::NUnion::EI_INITIALIZE, TNOP>(type, cd);
    }
  #endif // #ifdef FCF_UNION_IMPLEMENTATION

  #ifdef FCF_UNION_IMPLEMENTATION
    template <typename Ty>
    Union::Union(const Ty& a_value)
        : type(UT_UNDEFINED), orderc(0), order(0) {
        typedef typename Details::NUnion::TypeHelper<Ty>::far_type far_type;
        Details::NUnion::ExecutorSetArgs<Ty> esa{type, value, a_value};
        Details::NUnion::Executor<fcf::Details::NUnion::EI_SET, far_type, TNOP>()(esa);
    }
  #endif // #ifdef FCF_UNION_IMPLEMENTATION
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT Union::Union(const Undefined& a_value);
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT Union::Union(const Null& a_value);
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT Union::Union(const bool& a_value);
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT Union::Union(const int& a_value);
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT Union::Union(const unsigned int& a_value);
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT Union::Union(const long& a_value);
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT Union::Union(const unsigned long& a_value);
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT Union::Union(const long long& a_value);
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT Union::Union(const unsigned long long& a_value);
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT Union::Union(const float& a_value);
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT Union::Union(const double& a_value);
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT Union::Union(const std::string& a_value);
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT Union::Union(const ConstCharPtr& a_value);
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT Union::Union(const CharPtr& a_value);
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT Union::Union(const UnionVector& a_value);
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT Union::Union(const UnionMap& a_value);

  #ifdef FCF_UNION_IMPLEMENTATION
    Union::Union(const char* a_value)
      : type(UT_UNDEFINED), orderc(0), order(0) {
      typedef typename Details::NUnion::TypeHelper<const char*>::far_type far_type;
      Details::NUnion::ExecutorSetArgs<const char*> esa{type, value, a_value};
      Details::NUnion::Executor<fcf::Details::NUnion::EI_SET, far_type, TNOP>()(esa);
    }
  #endif // #ifdef FCF_UNION_IMPLEMENTATION

  #ifdef FCF_UNION_IMPLEMENTATION
    Union::~Union(){
      Details::NUnion::Selector::select<void, Details::NUnion::EI_DESTROY, TNOP>(type, value);
    }
  #endif // #ifdef FCF_UNION_IMPLEMENTATION

  #ifdef FCF_UNION_IMPLEMENTATION
    Union::iterator Union::obegin(){
      if (type == UT_VECTOR) {
        UnionVector* uv = (UnionVector*)(void*)&value.vvector[0];
        return iterator(uv->begin(), uv);
      } else if (type == UT_MAP) {
        UnionMap* um = (UnionMap*)(void*)&value.vmap[0];
        base_iterator::oiterators_type iterators( new std::vector< UnionMap::iterator >(um->size()) );
        size_t i = 0;
        for(UnionMap::iterator it = um->begin(); it != um->end(); ++it){
          (*iterators)[i] = it;
          ++i;
        }
        std::sort(iterators->begin(), iterators->end(), Details::NUnion::orderLess<UnionMap::iterator>);
        return iterator(iterators, iterators->begin(), um);
      } else {
        return iterator();
      }
    }
  #endif // #ifdef FCF_UNION_IMPLEMENTATION

  #ifdef FCF_UNION_IMPLEMENTATION
    Union::iterator Union::oend(){
      return iterator();
    }
  #endif // #ifdef FCF_UNION_IMPLEMENTATION

  #ifdef FCF_UNION_IMPLEMENTATION
    Union::const_iterator Union::cobegin() const {
      if (type == UT_VECTOR) {
        const UnionVector* uv = (const UnionVector*)(void*)&value.vvector[0];
        return const_iterator(uv->cbegin(), uv);
      } else if (type == UT_MAP) {
        UnionMap* um = (UnionMap*)(void*)&value.vmap[0];
        std::shared_ptr< std::vector< UnionMap::iterator > > iterators( new std::vector< UnionMap::iterator >(um->size()) );
        size_t i = 0;
        for(UnionMap::iterator it = um->begin(); it != um->end(); ++it){
          (*iterators)[i] = it;
          ++i;
        }
        std::sort(iterators->begin(), iterators->end(), Details::NUnion::orderLess<UnionMap::const_iterator>);
        return const_iterator(iterators, iterators->begin(), um);
      } else {
        return const_iterator();
      }
    }
  #endif // #ifdef FCF_UNION_IMPLEMENTATION

  #ifdef FCF_UNION_IMPLEMENTATION
    Union::const_iterator Union::coend() const {
      return const_iterator();
    }
  #endif // #ifdef FCF_UNION_IMPLEMENTATION

  #ifdef FCF_UNION_IMPLEMENTATION
    Union::iterator Union::begin(){
      if (type == UT_VECTOR) {
        UnionVector* uv = (UnionVector*)(void*)&value.vvector[0];
        return iterator(uv->begin(), uv);
      } else if (type == UT_MAP) {
        UnionMap* um = (UnionMap*)(void*)&value.vmap[0];
        return iterator(um->begin(), um);
      } else {
        return iterator();
      }
    }
  #endif // #ifdef FCF_UNION_IMPLEMENTATION

  #ifdef FCF_UNION_IMPLEMENTATION
    Union::iterator Union::end(){
      return iterator();
    }
  #endif // #ifdef FCF_UNION_IMPLEMENTATION

  #ifdef FCF_UNION_IMPLEMENTATION
    Union::const_iterator Union::cbegin() const {
      if (type == UT_VECTOR) {
        const UnionVector* uv = (const UnionVector*)(void*)&value.vvector[0];
        return const_iterator(uv->cbegin(), uv);
      } else if (type == UT_MAP) {
        const UnionMap* um = (const UnionMap*)(void*)&value.vmap[0];
        return const_iterator(um->cbegin(), um);
      } else {
        return const_iterator();
      }
    }
  #endif // #ifdef FCF_UNION_IMPLEMENTATION

  #ifdef FCF_UNION_IMPLEMENTATION
    Union::const_iterator Union::cend() const {
      return const_iterator();
    }
  #endif // #ifdef FCF_UNION_IMPLEMENTATION

  #ifdef FCF_UNION_IMPLEMENTATION
    size_t Union::size() const{
      if (type == UT_VECTOR) {
        return ((UnionVector*)(void*)&value.vvector[0])->size();
      } else if (type == UT_MAP) {
        return ((UnionMap*)(void*)&value.vmap[0])->size();
      } else {
        return 0;
      }
    }
  #endif // #ifdef FCF_UNION_IMPLEMENTATION

  #ifdef FCF_UNION_IMPLEMENTATION
    Union::iterator Union::find(Union a_key) {
      if (type == UT_VECTOR) {
        if (a_key.isCompatible<size_t>(true)){
          UnionVector& uv = *((UnionVector*)(void*)&value.vvector[0]);
          size_t key = a_key.get<size_t>();
          if (key < uv.size()) {
            return iterator(uv.begin() + key, &uv);
          }
        }
      } else if (type == UT_MAP){
        UnionMap& um = *((UnionMap*)(void*)&value.vmap[0]);
        UnionMap::iterator it = um.find(a_key);
        if (it != um.end()) {
          return iterator(it, &um);
        }
      }
      return end();
    }
  #endif // #ifdef FCF_UNION_IMPLEMENTATION

  #ifdef FCF_UNION_IMPLEMENTATION
    const Union& Union::cat(Union a_key) const{
      try {
        if (type == UT_VECTOR) {
          UnionVector* p = (UnionVector*)(void*)&value.vvector[0];
          size_t key = (size_t)a_key;
          if (key >= p->size()) {
            throw UnionException("Key not found");
          }
          return (*p)[key];
        } else if (type == UT_MAP) {
          UnionMap* p = (UnionMap*)(void*)&value.vmap[0];
          UnionMap::const_iterator it = p->find(a_key);
          if (it != p->end()){
            return it->second;
          } else {
            throw UnionException("Key not found");
          }
       }
      } catch(std::exception&){
      }
      throw UnionException("The object is not a container");
    }
  #endif // #ifdef FCF_UNION_IMPLEMENTATION

  #ifdef FCF_UNION_IMPLEMENTATION
    Union& Union::at(Union a_key){
      try {
        if (type == UT_VECTOR) {
          UnionVector* p = (UnionVector*)(void*)&value.vvector[0];
          size_t key = (size_t)a_key;
          if (key >= p->size()) {
            p->resize(key + 1);
          }
          return (*p)[key];
        } else if (type == UT_MAP) {
          UnionMap* p = (UnionMap*)(void*)&value.vmap[0];
          UnionMap::iterator it = p->find(a_key);
          if (it != p->end()){
            return it->second;
          } else {
            return insert(a_key, Union()).value();
          }
       }
      } catch(std::exception&){
      }
      throw UnionException("The object is not a container");
    }
  #endif // #ifdef FCF_UNION_IMPLEMENTATION

  #ifdef FCF_UNION_IMPLEMENTATION
    Union& Union::operator[](Union a_key){
      return at(a_key);
    }
  #endif // #ifdef FCF_UNION_IMPLEMENTATION

  #ifdef FCF_UNION_IMPLEMENTATION
    Union::iterator Union::insert(Union a_value){
      return insert(size(), a_value);
    }
  #endif // #ifdef FCF_UNION_IMPLEMENTATION

  #ifdef FCF_UNION_IMPLEMENTATION
    Union::iterator Union::insert(Union a_key, Union a_value) {
      if (type == UT_VECTOR) {
        UnionVector* p = (UnionVector*)(void*)&value.vvector[0];
        size_t key = (size_t)a_key;
        if (key >= p->size()) {
          p->resize(key + 1);
        }
        (*p)[key] = a_value;
        return iterator(p->begin() + key, p);
      } else if (type == UT_MAP) {
        UnionMap* p = (UnionMap*)(void*)&value.vmap[0];
        std::pair<UnionMap::iterator, bool> insertInfo = p->insert( std::pair<Union, Union>(a_key, a_value) );
        if (!insertInfo.second) {
          insertInfo.first->second = a_value;
        } else {
          insertInfo.first->second.order = ++orderc;
        }
        return iterator(insertInfo.first, p);
      }
      throw UnionException("The object is not a container");
    }
  #endif // #ifdef FCF_UNION_IMPLEMENTATION

  #ifdef FCF_UNION_IMPLEMENTATION
    Union::iterator Union::erase(const Union& a_key){
      if (type == UT_VECTOR){
        UnionVector& uv = *(UnionVector*)(void*)&value.vvector[0];
        if (a_key.isCompatible<size_t>(true)){
          size_t key = a_key.get<size_t>();
          if (key < uv.size()){
            uv.erase(uv.begin() + key);
            return iterator(uv.begin() + key, &uv);
          }
        }
      } else if (type == UT_MAP) {
        UnionMap& um = *(UnionMap*)(void*)&value.vmap[0];
        UnionMap::iterator it = um.find(a_key);
        if (it != um.end()) {
          UnionMap::iterator rit = it;
          ++rit;
          um.erase(it);
          return iterator(rit, &um);
        }
      }
      return end();
    }
  #endif // #ifdef FCF_UNION_IMPLEMENTATION


  #ifdef FCF_UNION_IMPLEMENTATION
    Union::iterator Union::erase(const iterator& a_iterator){
      if (type == UT_VECTOR) {
        if (a_iterator._vitype == base_iterator::IT_VECTOR) {
          UnionVector::iterator& vi = *(UnionVector::iterator*)(void*)&a_iterator._viiterator.vivector[0];
          UnionVector& uv = *(UnionVector*)(void*)&value.vvector[0];
          size_t index = vi - uv.begin();
          if (index < uv.size()){
            uv.erase(vi);
            return iterator(uv.begin() + index, &uv);
          } else {
            end();
          }
        }
      } else if (type == UT_MAP){
        if (a_iterator == end()) {
          return end();
        }
        if (a_iterator._vitype == base_iterator::IT_MAP) {
          UnionMap::iterator mi = *(UnionMap::iterator*)(void*)&a_iterator._viiterator.vimap[0];
          UnionMap&          um = *(UnionMap*)(void*)&value.vmap[0];
          iterator rit(a_iterator);
          ++rit;
          um.erase(mi);
          return rit;
        } else if (a_iterator._vitype == base_iterator::IT_OMAP) {
          base_iterator::oiterator_type& itIt = *(base_iterator::oiterator_type*)(void*)&a_iterator._viiterator.voiterators[0];
          UnionMap::iterator mi               = *itIt.iterator;
          UnionMap&          um               = *(UnionMap*)(void*)&value.vmap[0];
          iterator rit(a_iterator);
          ++rit;
          um.erase(mi);
          return rit;
        }
      }
      return end();
    }
  #endif // #ifdef FCF_UNION_IMPLEMENTATION

  #ifdef FCF_UNION_IMPLEMENTATION
    void Union::swap(Union& a_union){
      Union u(a_union);
      a_union = *this;
      *this = u;
    }
  #endif // #ifdef FCF_UNION_IMPLEMENTATION

  #ifdef FCF_UNION_IMPLEMENTATION
    template <typename Ty>
    bool Union::is() const{
      int ti = Details::NUnion::TypeHelper<Ty>::type_index;
      return ti  == type;
    }
  #endif // #ifdef FCF_UNION_IMPLEMENTATION
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::is<Undefined>() const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::is<Null>() const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::is<bool>() const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::is<int>() const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::is<unsigned int>() const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::is<long>() const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::is<unsigned long>() const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::is<long long>() const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::is<unsigned long long>() const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::is<double>() const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::is<float>() const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::is<std::string>() const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::is<char*>() const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::is<const char*>() const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::is<UnionVector>() const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::is<UnionMap>() const;

  #ifdef FCF_UNION_IMPLEMENTATION
    bool Union::isCompatible(UnionType a_type, bool a_stringifyMode) const {
      if ((a_type == UT_BOOL || ((int)a_type >= (int)FIRST_NUMBER_TYPE && (int)a_type <= (int)LAST_NUMBER_TYPE)) &&
          (type == UT_BOOL || ((int)type >= (int)FIRST_NUMBER_TYPE  && (int)type <= (int)LAST_NUMBER_TYPE))
          ) {
        return true;
      } else {
        if (a_type== type) {
          return true;
        }
        if (a_stringifyMode) {
          if (a_type == UT_STRING) {
            return true;
          }
          if (type == UT_STRING) {
            Union u;
            const char* error = 0;
            std::string& string = *(std::string*)(const std::string*)&value.vstring[0];
            fcf::Details::NConvert::ConstResolver<std::string> r{string, false};
            if (Details::NConvert::parseValue(r, u, &error) && !error) {
              return u.isCompatible(a_type, true);
            }
          }
        }
        return false;
      }
    }
  #endif

  #ifdef FCF_UNION_IMPLEMENTATION
    void Union::parse(const std::string& a_source) {
      {
        Union u;
        fcf::Details::NConvert::ConstResolver<std::string> r{a_source, true};
        if (!Details::NConvert::parseValue(r, u, 0)) {
          throw UnionException("Incorrect source data format", 1, 1, false);
        } else {
          swap(u);
        }
      }
    }
  #endif // #ifdef FCF_UNION_IMPLEMENTATION

  #ifdef FCF_UNION_IMPLEMENTATION
    void Union::parse(std::basic_istream<char>& a_source) {
      Union u;
      fcf::Details::NConvert::ConstResolver< std::basic_istream<char> > r{a_source, true};
      if (!Details::NConvert::parseValue(r, u, 0)) {
        throw UnionException("Incorrect source data format", 1, 1, false);
      } else {
        this->swap(u);
      }
      a_source.seekg(0);
    }
  #endif // #ifdef FCF_UNION_IMPLEMENTATION

  #ifdef FCF_UNION_IMPLEMENTATION
    void Union::stringify(std::string& a_dest, const UnionStringifyOptions& a_options) const {
      typedef std::string destination_type;
      Details::NUnion::StringCleaner<destination_type>()(a_dest);
      struct CallData{
        typedef UnionStringifyOptions options_type;
        const UnionValue&        value;
        destination_type&        destination;
        const UnionStringifyOptions&  options;
        const char**             error;
      };
      CallData cd{value, a_dest, a_options, 0};
      if (a_options.mode == SF_JSON) {
        Details::NUnion::Selector::select<void, Details::NUnion::EI_TO_JSON, destination_type>(type, cd);
      } else {
        Details::NUnion::Selector::select<void, Details::NUnion::EI_CONST_GET, destination_type>(type, cd);
      }
    }
  #endif // #ifdef FCF_UNION_IMPLEMENTATION

  #ifdef FCF_UNION_IMPLEMENTATION
    void Union::stringify(std::basic_ostream<char>& a_dest, const UnionStringifyOptions& a_options) const {
      typedef std::basic_ostream<char> destination_type;
      Details::NUnion::StringCleaner<destination_type>()(a_dest);
      struct CallData{
        typedef UnionStringifyOptions options_type;
        const UnionValue&        value;
        destination_type&        destination;
        const UnionStringifyOptions&  options;
        const char**             error;
      };
      CallData cd{value, a_dest, a_options, 0};
      if (a_options.mode == SF_JSON) {
        Details::NUnion::Selector::select<void, Details::NUnion::EI_TO_JSON, destination_type>(type, cd);
      } else {
        Details::NUnion::Selector::select<void, Details::NUnion::EI_CONST_GET, destination_type>(type, cd);
      }
    }
  #endif // #ifdef FCF_UNION_IMPLEMENTATION

  #ifdef FCF_UNION_IMPLEMENTATION
  template <typename Ty>
  Ty Union::get() const {
    struct CallData{
      typedef UnionStringifyOptions options_type;
      const UnionValue&             value;
      const char**                  error;
      Ty                            destination;
      const UnionStringifyOptions   options;
    };
    CallData cd{value, 0};
    Details::NUnion::Selector::select<void, Details::NUnion::EI_CONST_GET, Ty>(type, cd);
    return cd.destination;
  }
  #endif // #ifdef FCF_UNION_IMPLEMENTATION
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT Undefined Union::get<Undefined>() const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT Null Union::get<Null>() const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::get<bool>() const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT int Union::get<int>() const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT unsigned int Union::get<unsigned int>() const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT long Union::get<long>() const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT unsigned long Union::get<unsigned long>() const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT long long Union::get<long long>() const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT unsigned long long Union::get<unsigned long long>() const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT double Union::get<double>() const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT float Union::get<float>() const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT std::string Union::get<std::string>() const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT UnionVector Union::get<UnionVector>() const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT UnionMap Union::get<UnionMap>() const;

  #ifdef FCF_UNION_IMPLEMENTATION
  template <typename Ty>
  typename Details::NUnion::TypeHelper<Ty>::far_type& Union::ref() {
    typedef typename Details::NUnion::TypeHelper<Ty>::far_type far_type;
    typedef far_type& TRef;
    struct CallData{
      UnionType&    type;
      UnionValue&   value;
      const char**  error;
    };
    CallData cd{type, value, 0};
    return Details::NUnion::Selector::select<TRef, Details::NUnion::EI_GET, far_type>(type, cd);
  }
  #endif // #ifdef FCF_UNION_IMPLEMENTATION
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT Undefined& Union::ref<Undefined>();
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT Null& Union::ref<Null>();
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool& Union::ref<bool>();
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT int& Union::ref<int>();
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT unsigned int& Union::ref<unsigned int>();
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT long long& Union::ref<long long>();
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT unsigned long long& Union::ref<unsigned long long>();
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT double& Union::ref<double>();
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT std::string& Union::ref<std::string>();
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT UnionVector& Union::ref<UnionVector>();
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT UnionMap& Union::ref<UnionMap>();

  #ifdef FCF_UNION_IMPLEMENTATION
  template <typename Ty>
  void Union::set(const Ty& a_value) {
    typedef typename Details::NUnion::TypeHelper<Ty>::far_type far_type;
    Details::NUnion::Selector::select<void, Details::NUnion::EI_DESTROY, TNOP>(type, value);
    type = UT_UNDEFINED;
    far_type newValue = Details::NUnion::TypeHelper<Ty>::farValue(a_value);
    Details::NUnion::ExecutorSetArgs<far_type> esa{type, value, newValue};
    Details::NUnion::Executor<fcf::Details::NUnion::EI_SET, far_type, TNOP>()(esa);
  }
  #endif // #ifdef FCF_UNION_IMPLEMENTATION
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT void Union::set<Undefined>(const Undefined&);
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT void Union::set<Null>(const Null&);
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT void Union::set<bool>(const bool&);
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT void Union::set<int>(const int&);
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT void Union::set<unsigned int>(const unsigned int&);
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT void Union::set<long>(const long&);
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT void Union::set<unsigned long>(const unsigned long&);
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT void Union::set<long long>(const long long&);
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT void Union::set<unsigned long long>(const unsigned long long&);
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT void Union::set<double>(const double&);
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT void Union::set<std::string>(const std::string&);
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT void Union::set<ConstCharPtr>(const ConstCharPtr&);
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT void Union::set<CharPtr>(const CharPtr&);
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT void Union::set<UnionVector>(const UnionVector&);
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT void Union::set<UnionMap>(const UnionMap&);

  #ifdef FCF_UNION_IMPLEMENTATION
    void Union::set(const Union& a_union){
      Details::NUnion::Selector::select<void, Details::NUnion::EI_DESTROY, TNOP>(type, value);
      type = UT_UNDEFINED;
      struct CallData{
        const UnionValue& source;
        UnionValue&       destination;
      };
      CallData cd{a_union.value, value};
      Details::NUnion::Selector::select<void, Details::NUnion::EI_COPY, TNOP>(a_union.type, cd);
      if (!order) {
        order = a_union.order;
      }
      if (orderc < a_union.orderc) {
        orderc = a_union.orderc;
      }
      type = a_union.type;
    }
  #endif // #ifdef FCF_UNION_IMPLEMENTATION

  #ifdef FCF_UNION_IMPLEMENTATION
    void Union::set(UnionType a_type) {
      Details::NUnion::Selector::select<void, Details::NUnion::EI_DESTROY, TNOP>(type, value);
      type = UT_UNDEFINED;
      struct CallData{
        UnionValue& destination;
      };
      CallData cd{value};
      Details::NUnion::Selector::select<void, Details::NUnion::EI_INITIALIZE, TNOP>(a_type, cd);
      type = a_type;
    }
  #endif

  #ifdef FCF_UNION_IMPLEMENTATION
    template <typename Ty>
    bool Union::equal(const Ty& a_value, bool a_strict) const {
      struct CallData {
        bool              strict;
        const UnionValue& left;
        const Ty&         right;
      };
      CallData cd = {a_strict, value, a_value};
      return Details::NUnion::Selector::select<bool, Details::NUnion::EI_EQUAL, TNOP>(type, cd);
    }
  #endif // #ifdef FCF_UNION_IMPLEMENTATION
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::equal<Undefined>(const Undefined&, bool) const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::equal<Null>(const Null&, bool) const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::equal<bool>(const bool&, bool) const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::equal<int>(const int&, bool) const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::equal<unsigned int>(const unsigned int&, bool) const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::equal<long>(const long&, bool) const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::equal<unsigned long>(const unsigned long&, bool) const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::equal<long long>(const long long&, bool) const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::equal<unsigned long long>(const unsigned long long&, bool) const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::equal<double>(const double&, bool) const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT  bool Union::equal<std::string>(const std::string&, bool) const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::equal<ConstCharPtr>(const ConstCharPtr&, bool) const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::equal<CharPtr>(const CharPtr&, bool) const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::equal<UnionVector>(const UnionVector&, bool) const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::equal<UnionMap>(const UnionMap&, bool) const;

  #ifdef FCF_UNION_IMPLEMENTATION
    bool Union::equal(const Union& a_value, bool a_strict) const {
      struct CallData {
        bool              strict;
        const UnionValue& left;
        UnionType         leftType;
        const UnionValue& right;
      };
      CallData cd = {a_strict, value, type, a_value.value};
      return Details::NUnion::Selector::select<bool, Details::NUnion::EI_EQUAL2, TNOP>(a_value.type, cd);
    }
  #endif // #ifdef FCF_UNION_IMPLEMENTATION

  #ifdef FCF_UNION_IMPLEMENTATION
  template <typename Ty>
  bool Union::lessStr(const Ty& a_value) const {
    struct CallData {
      const UnionValue& left;
      const Ty&         right;
    };
    CallData cd = {value, a_value};
    return Details::NUnion::Selector::select<bool, Details::NUnion::EI_LESS, TNOP>(type, cd);
  }
  #endif // #ifdef FCF_UNION_IMPLEMENTATION
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::lessStr<Undefined>(const Undefined&) const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::lessStr<Null>(const Null&) const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::lessStr<bool>(const bool&) const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::lessStr<int>(const int&) const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::lessStr<unsigned int>(const unsigned int&) const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::lessStr<long>(const long&) const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::lessStr<unsigned long>(const unsigned long&) const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::lessStr<long long>(const long long&) const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::lessStr<unsigned long long>(const unsigned long long&) const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::lessStr<double>(const double&) const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::lessStr<std::string>(const std::string&) const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::lessStr<ConstCharPtr>(const ConstCharPtr&) const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::lessStr<CharPtr>(const CharPtr&) const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::lessStr<UnionVector>(const UnionVector&) const;
  FCF_UNION_DECL_TEMPLATE_EXTERN template FCF_UNION_DECL_EXPORT bool Union::lessStr<UnionMap>(const UnionMap&) const;

  #ifdef FCF_UNION_IMPLEMENTATION
    bool Union::lessStr(Union& a_value) const {
      return lessStr((const Union&)a_value);
    }
  #endif // #ifdef FCF_UNION_IMPLEMENTATION

  #ifdef FCF_UNION_IMPLEMENTATION
    bool Union::lessStr(const Union& a_value) const {
      struct CallData {
        const UnionValue& left;
        UnionType         leftType;
        const UnionValue& right;
      };
      CallData cd = {value, type, a_value.value};
      return Details::NUnion::Selector::select<bool, Details::NUnion::EI_LESS2, TNOP>(a_value.type, cd);
    }
  #endif // #ifdef FCF_UNION_IMPLEMENTATION

}

#endif // #ifndef ___FCF__UNION__UNION_HPP___
