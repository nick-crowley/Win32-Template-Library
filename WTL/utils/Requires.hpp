//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Requires.hpp
//! \brief Custom 'Concept' checking mechanism
//! \date 28 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_REQUIRES_HPP
#define WTL_REQUIRES_HPP

#include <wtl/WTL.hpp>

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Requires - Checks whether a type models a concept
  //!
  //! \tparam T - Type to be tested
  //! \tparam CONCEPT - Concept to be modelled
  //! 
  //! \remarks Compatible with GCC-5.2, Clang-3.8, and MSVC-14
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename CONCEPT>  
  struct requires 
  {
  protected:
    //! \struct child - Encapsulates the fallback candidate
    struct child 
    {
      template <typename C> static char  test(...);
    };
    
    //! \struct derived - Encapsulates both candidates with equal precedence for name lookup
    struct derived : CONCEPT, child
    {
      using CONCEPT::test;
      using child::test;
    };

  public:
    //! \var value - Indicates whether type 'T' models the concept 'CONCEPT'
    static constexpr bool value = sizeof(derived::template test<T>(nullptr)) > 1;     //!< Test via overload resolution
  };

  
  //////////////////////////////////////////////////////////////////////////////////////////
  //! \def CONCEPT_ERROR_MSG - Generates a string literal containing a 'concept violation' error message
  //! 
  //! \tparam TYPE - Name of the type being tested
  //! \tparam CONCEPT - Name of the concept required  
  //! 
  //! \remarks 'CONCEPT' parameter is ellipsis only because preprocessor confuses template parameter pack expansion with multiple arguments (?!)
  //////////////////////////////////////////////////////////////////////////////////////////
  #define CONCEPT_ERROR_MSG(TYPE,...)  "*ERROR*: Template parameter " #TYPE " does not model the '" #__VA_ARGS__ "' concept...\n"
  

  //////////////////////////////////////////////////////////////////////////////////////////
  //! \def concept_check - Inserts a static assertion that checks whether a type models a concept
  //! 
  //! \tparam TYPE - Name of the type being tested
  //! \tparam CONCEPT - Name of the concept required  [Must be an unqualified name of a type residing in the 'concepts' namespace]
  //! 
  //! \remarks 'CONCEPT' parameter is ellipsis only because preprocessor confuses template parameter pack expansion with multiple arguments (?!)
  //////////////////////////////////////////////////////////////////////////////////////////
  #define concept_check(TYPE,...)  static_assert(requires<TYPE,::wtl::concepts::__VA_ARGS__>::value, CONCEPT_ERROR_MSG(TYPE, __VA_ARGS__))
  

}



#endif // WTL_REQUIRES_HPP


