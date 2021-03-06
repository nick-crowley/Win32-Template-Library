//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\String.hpp
//! \brief Dynamically allocated string
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_STRING_HPP
#define WTL_STRING_HPP

#include <wtl/WTL.hpp>
#include <wtl/traits/EncodingTraits.hpp>        //!< Character encoding
#include <wtl/utils/Exception.hpp>              //!< Exceptions
#include <wtl/utils/Encoding.hpp>               //!< string_encoder
//#include <wtl/io/Console.hpp>                   //!< Console
#include <string>                               //!< std::basic_string
#include <vector>                               //!< std::vector
#include <ostream>                              //!< std::basic_ostream

//! \namespace wtl - Windows template library
namespace wtl
{

  /////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::strlen
  //! Get length of narrow character string
  //!
  //! \param[in] const* str - Narrow character string
  //! \return size_t - Length in characters
  /////////////////////////////////////////////////////////////////////////////////////////
  inline size_t strlen(const char* str)
  {
    return ::strlen(str);
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::strlen
  //! Get length of wide character string
  //!
  //! \param[in] const* str - Wide character string
  //! \return size_t - Length in characters
  /////////////////////////////////////////////////////////////////////////////////////////
  inline size_t strlen(const wchar_t* str)
  {
    return ::wcslen(str);
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::strcmp
  //! Case sensitive narrow character comparison
  //!
  //! \param[in] const* a - Narrow char string
  //! \param[in] const* b - Another narrow char string
  //! \return int32_t - -1 if less, 0 if equal, 1 if greater
  /////////////////////////////////////////////////////////////////////////////////////////
  inline int32_t strcmp(const char* a, const char* b)
  {
    return ::strcmp(a,b);
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::strcmp
  //! Case sensitive wide character comparison
  //!
  //! \param[in] const* a - Wide char string
  //! \param[in] const* b - Another wide char string
  //! \return int32_t - -1 if less, 0 if equal, 1 if greater
  /////////////////////////////////////////////////////////////////////////////////////////
  inline int32_t strcmp(const wchar_t* a, const wchar_t* b)
  {
    return ::wcscmp(a,b);
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::strcpy
  //! Copy narrow character string
  //!
  //! \param[in] *dest - Narrow char string
  //! \param[in] const* src - Another narrow char string
  //! \return char* - Returns 'dest'
  /////////////////////////////////////////////////////////////////////////////////////////
  template <unsigned LEN>
  inline char* strcpy(char (&dest)[LEN], const char* src)
  {
    return ::strcpy(dest,src);
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::strcpy
  //! Copy wide character string
  //!
  //! \param[in] *dest - Wide char string
  //! \param[in] const* src - Another wide char string
  //! \return wchar_t* - Returns 'dest'
  /////////////////////////////////////////////////////////////////////////////////////////
  template <unsigned LEN>
  inline wchar_t* strcpy(wchar_t (&dest)[LEN], const wchar_t* src)
  {
    return ::wcscpy(dest,src);
  }
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::snprintf
  //! Formats a narrow character string
  //!
  //! \tparam LENGTH - Length of output buffer in characters
  //! \tparam ARGS - Argument types 
  //! 
  //! \param[in,out] *buffer - Narrow character output buffer
  //! \param[in] const* format - Narrow formatting string
  //! \param[in] ... - [optional] Formatting arguments
  //! \return int32_t - Number of character written.
  //!
  //! \remarks If result is less than zero then an error occurred. If the result exceeds the capacity then there was insufficient space.
  /////////////////////////////////////////////////////////////////////////////////////////
  template <uint32_t LENGTH, typename... ARGS>
  inline int32_t snprintf(char (&buffer)[LENGTH], const char* format, ARGS&&... args)
  {
    return std::snprintf(buffer, LENGTH, format, std::forward<ARGS>(args)...);
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::snprintf
  //! Formats a wide character string
  //!
  //! \tparam LENGTH - Length of output buffer in characters
  //! \tparam ARGS - Argument types 
  //! 
  //! \param[in,out] *buffer - Wide character output buffer
  //! \param[in] capacity - Buffer capacity
  //! \param[in] const* format - Wide formatting string
  //! \param[in] ... - [optional] Formatting arguments
  //! \return int32_t - Number of character written.
  //!
  //! \remarks If result is less than zero then an error occurred. If the result exceeds the capacity then there was insufficient space.
  /////////////////////////////////////////////////////////////////////////////////////////
  template <uint32_t LENGTH, typename... ARGS>
  inline int32_t snprintf(wchar_t (&buffer)[LENGTH], const wchar_t* format, ARGS&&... args)
  {
    return std::swprintf(buffer, LENGTH, format, std::forward<ARGS>(args)...);
  }



  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct String - Character string with an unlimited capacity and variable runtime length. In essence, this is a hollow type upon a std::string.
  //!
  //! \tparam ENC - Character encoding
  //! 
  //! \remarks Although one should not derive from classes without polymorphic destructors, I have done so here because there
  //! \remarks is insufficient ROI for implementing this class through composition and delegating so many methods.
  //! \remarks 
  //! \remarks Allocating an instance upon the stack will always lead to well-defined behaviour, and so would deleting a dynamically
  //! \remarks allocated instance iff it is destroyed through a pointer to its dynamic type. However, if destroyed through a 
  //! \remarks base pointer, it will technically invoke undefined behaviour. In practice, however, the derived destructor is trivial
  //! \remarks so the object will be properly destroyed by the base destructor
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct String : std::basic_string<encoding_char_t<ENC>>
  {
    template <Encoding E2>
    friend struct String;   // Unbound friend of itself

    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias base - Defines base type
    using base = std::basic_string<encoding_char_t<ENC>>;
  
    //! \alias type - Defines own type
    using type = String<ENC>;
  
    //! \alias char_t - Character type
    using char_t = encoding_char_t<ENC>;
    
    //! \alias size_type - Inherit character index type
    using size_type = typename base::size_type;

    //! \var encoding - Character encoding
    static constexpr Encoding encoding = ENC;

    //! \var null_t - Null terminator
    static constexpr char_t null_t = defvalue<char_t>();
  
    // ----------------------------------- REPRESENTATION -----------------------------------
    
    // ------------------------------------ CONSTRUCTION ------------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // String::String
    //! Constructs empty string
    /////////////////////////////////////////////////////////////////////////////////////////
    String()
    {}

    /////////////////////////////////////////////////////////////////////////////////////////
    // String::String
    //! Constructs and fills the string with 'n' copies character 'ch'
    //! 
    //! \param[in] count - Number of copies
    //! \param[in] ch - Fill character
    /////////////////////////////////////////////////////////////////////////////////////////
    String(size_type count, char_t ch) : base(count, ch)
    {}

    /////////////////////////////////////////////////////////////////////////////////////////
    // String::String
    //! Constructs the string from a substring of a std::string
    //! 
    //! \param[in] const& s - String
    //! \param[in] pos - Position of sub-string
    //! \param[in] count - [optional] Length of sub-string. If unspecified, the remainder of the input string is used
    //! 
    //! \throw std::out_of_range - Position 'pos' exceeds length of input string 's'
    /////////////////////////////////////////////////////////////////////////////////////////
    String(const base& other, size_type pos, size_type count = base::npos) : base(other, pos, count)
    {}
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // String::String
    //! Constructs the string from the first 'n' characters of a null-terminated string
    //! 
    //! \param[in] const* s - Null terminated string
    //! \param[in] count - Number of characters to copy
    //! 
    //! \remarks If 's' does not contain 'count' characters the behaviour is undefined
    /////////////////////////////////////////////////////////////////////////////////////////
    String(const char_t* s, size_type count) : base(s, count)
    {}
	  
    /////////////////////////////////////////////////////////////////////////////////////////
    // String::String
    //! Constructs the string from a null-terminated string
    //! 
    //! \param[in] const* s - Null terminated string
    /////////////////////////////////////////////////////////////////////////////////////////
    String(const char_t* s) : base(s)
    {}
	  
    /////////////////////////////////////////////////////////////////////////////////////////
    // String::String
    //! Constructs the string from a null-terminated string of different character encoding
    //! 
    //! \tparam CHR - Foreign character type
    //! 
    //! \param[in] const* s - Null terminated string
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename CHR>
    String(const CHR* s) : String(std::basic_string<CHR>(s))
    {}
	  
    /////////////////////////////////////////////////////////////////////////////////////////
    // String::String
    //! Constructs the string from a std::string of different character encoding
    //! 
    //! \tparam CHR - Foreign character type
    //! 
    //! \param[in] s - Null terminated string
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename CHR>
    String(const std::basic_string<CHR>& s) : base(translate<default_encoding<CHR>::value>(s))
    {}
	  
    /////////////////////////////////////////////////////////////////////////////////////////
    // String::String
    //! Constructs the string from a string of different character encoding
    //! 
    //! \tparam E2 - Foreign character encoding
    //! 
    //! \param[in] const& s - String of different character encoding
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding E2>
    String(const String<E2>& s) : base(translate<E2>(s))
    {}
	    
    /////////////////////////////////////////////////////////////////////////////////////////
    // String::String
    //! Constructs the string with the contents of the range of characters
    //! 
    //! \param[in] first - First character in the range
    //! \param[in] last - Position immediately following last character in the range
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename InputIt>
    String(InputIt first, InputIt last) : base(first,last)
    {}
	  
    /////////////////////////////////////////////////////////////////////////////////////////
    // String::String
    //! Constructs the string with the contents of the initializer list 
    //! 
    //! \param[in] init - Initial characters
    /////////////////////////////////////////////////////////////////////////////////////////
    String(std::initializer_list<char_t> init) : base(init)
    {}

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
    
    ENABLE_COPY(String);			//!< Can be copied
		ENABLE_MOVE(String);			//!< Can be moved
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // String::operator=
    //! Overwrites contents with characters from a string of different character encoding
    //! 
    //! \param[in] const& s - String of different character encoding
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding E2>
    type& operator=(const String<E2>& s) 
    {
      base::operator=(translate<E2>(s));
      return *this;
    }

    // ----------------------------------- STATIC METHODS -----------------------------------
  private:
    /////////////////////////////////////////////////////////////////////////////////////////
    // String::translate
    //! Converts a String in a foreign character encoding into a std::string in native character encoding
    //! 
    //! \tparam E2 - Foreign character encoding
    //! \tparam CHR - Foreign character type
    //! 
    //! \param[in] const& s - Foreign string to convert
    //! \return std::basic_string<char_t> - std::string in native encoding
    //!
    //! \throw wtl::platform_error - Unable to convert string
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding E2> 
    static base  translate(const String<ENC>& s)
    {
      // Delegate to std::string overload
      return translate<ENC, typename String<ENC>::char_t>(s);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // String::translate
    //! Converts a std::string in a foreign character encoding into a std::string in native character encoding
    //! 
    //! \tparam E2 - Foreign character encoding
    //! \tparam CHR - Foreign character type
    //! 
    //! \param[in] const& s - Foreign std::string to convert
    //! \return std::basic_string<char_t> - std::string in native encoding
    //!
    //! \throw wtl::platform_error - Unable to convert string
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding E2, typename CHR = encoding_char_t<E2>> 
    static base  translate(const std::basic_string<CHR>& s)
    {
      std::vector<char_t> buf(s.length()+1, null_t);      //!< Temporary (dynamic) storage

      // Convert from 's' into 'buf'
      string_encoder<E2,encoding>::convert(s.c_str(), s.c_str()+s.length(), buf.data(), buf.data()+buf.capacity());
      return buf.data();
    }

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // String::contains const
    //! Query whether the string contains a character
    //! 
    //! \param[in] ch - Character 
    //! \return bool - True iff string contains character
    /////////////////////////////////////////////////////////////////////////////////////////
    bool contains(char_t ch) const
    {
      return this->find(ch) != base::npos;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // String::translate const
    //! Translate string into another encoding
    //! 
    //! \tparam E2 - Foreign character encoding
    //! 
    //! \return String<E2> - Current text in character encoding E2
    //!
    //! \throw wtl::platform_error - Unable to convert text
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding E2> 
    String<E2>  translate() const
    {
      return {*this};
    }
    
	  type operator + (const type& r) const
		{	
      type s2;
      s2.reserve(this->length() + r.length());
		  s2 += *this;
      s2 += r;
      return s2;
		}

	  type operator + (const char_t* str) const
		{	
		  type s2;
      s2.reserve(this->length() + strlen(str));
		  s2 += *this;
      s2 += str;
      return s2;
		}
    
    template <typename CHR>
	  type operator + (const CHR* str) const
		{	
      type s2;
      s2.reserve(this->length() + strlen(str));
		  s2 += *this;
      s2 += str;
      return s2;
		}

    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
	  type& operator += (const type& r)
		{	
		  this->append(r);
      return *this;
		}

	  type& operator += (const char_t* str)
		{	
		  this->append(str);
      return *this;
		}
    
    template <typename CHR>
	  type& operator += (const CHR* str)
		{	
		  this->append(type(str).c_str());
      return *this;
		}

	  type& operator += (char_t ch)
		{	
      this->append(1, ch);
      return *this;
		}
  };


  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::c_str
  //! Creates a dynamic string from a character array or string literal
  //!
  //! \tparam char_t - Character type
  //! \tparam encoding - [optional] Character encoding (if unspecified the default encoding for the character type is used)
  //!
  //! \param[in] const* str - Null terminated string
  //! \return String<ENC> - String containing 'str'
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename char_t, Encoding encoding = default_encoding<char_t>::value>
  String<encoding>  c_str(const char_t* str)
  {
    return String<encoding>(str);
  }

  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Prints a string (of character type dissimilar to that of an output stream) to an output stream
  //!
  //! \tparam CHAR - Output stream character type
  //! \tparam TRAITS - Output stream character traits
  //! \tparam ENC - String character encoding
  //!
  //! \param[in,out] &c - Output stream
  //! \param[in] const& str - String
  //! \return std::basic_ostream<CHAR,TRAITS>& - Reference to 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename CHAR, typename TRAITS, Encoding ENC, typename = enable_if_not_same_t<encoding_char_t<ENC>,CHAR>>
  std::basic_ostream<CHAR,TRAITS>& operator << (std::basic_ostream<CHAR,TRAITS>& c, const String<ENC>& str)
  {
    // Convert string to same encoding as stream before output
    return c << '\'' << str.template translate<default_encoding<CHAR>::value>() << '\'';
  }

  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Prints a null-terminated string (of character type dissimilar to that of an output stream) to an output stream
  //!
  //! \tparam OS_CHAR - Output stream character type
  //! \tparam TRAITS - Output stream character traits
  //! \tparam STR_CHAR - String character type
  //!
  //! \param[in,out] &c - Output stream
  //! \param[in] const* str - Null-terminated string
  //! \return std::basic_ostream<CHAR,TRAITS>& - Reference to 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename OS_CHAR, typename TRAITS, typename STR_CHAR, typename = enable_if_character_t<STR_CHAR>, typename = enable_if_not_same_t<OS_CHAR,STR_CHAR>>
  std::basic_ostream<OS_CHAR,TRAITS>& operator << (std::basic_ostream<OS_CHAR,TRAITS>& c, const STR_CHAR* str)
  {
    // Convert string to same encoding as stream before output
    return c << String<default_encoding<OS_CHAR>::value>(str);
  }

} // WTL namespace



#endif // WTL_STRING_HPP

