//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Array.hpp
//! \brief Static array declaration file
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_ARRAY_HPP
#define WTL_ARRAY_HPP

#include "wtl/WTL.hpp"
#include "wtl/utils/Allocator.hpp"          //!< StaticAlloc
#include "wtl/io/Console.hpp"               //!< Debug console
#include "wtl/utils/DebugInfo.hpp"          //!< DebugInfo
#include "wtl/utils/NameValuePair.hpp"      //!< NameValuePair

//! \namespace wtl - Windows template library
namespace wtl
{
  //////////////////////////////////////////////////////////////////////////////////////////
  //! \struct array_traits - Defines array traits
  //! 
  //! \tparam DATA - Type used to specialize traits
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename DATA>
  struct array_traits
  {
    //! \alias alloc_t - Element allocator 
    using alloc_t = static_alloc<DATA>;
  };


  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Array - Statically allocated, variable length, fixed capacity array. 
  //! This container is designed to be compatible with the standard template library algorithms.
  //! 
  //! \tparam DATA - Type of each element
  //! \tparam LENGTH - Maximum number of elements within the array
  //! \tparam DYNAMIC - Whether array can have a variable number of elements
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename DATA, uint32 LENGTH, bool DYNAMIC>
  struct Array
  { /*Undefined*/ };

  
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator << 
  //! Write an array of elements not of class type to the debug console
  //!
  //! \tparam E - Array element type
  //! \tparam L - Array capacity
  //! \tparam D - Whether array can be resized
  //!
  //! \param[in,out] &c - Console
  //! \param[in] const &r - Array
  //! \return Console& : Reference to console
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename E, uint32 L, bool D>
  enable_if_class_t<E,Console&> operator << (Console& c, const Array<E,L,D>& r) 
  {
    // Write comma separated elements
    return c << debug_info("Array", name_value_pair(" Size", r.size()),
                                    name_value_pair(" Elements", delimited_range(r, ',')));
  }

  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator << 
  //! Write an array of elements of class type to the debug console
  //!
  //! \tparam E - Array element type
  //! \tparam L - Array capacity
  //! \tparam D - Whether array can be resized
  //!
  //! \param[in,out] &s - Console
  //! \param[in] const &r - Array
  //! \return Console& : Reference to console
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename E, uint32 L, bool D>
  enable_if_not_class_t<E,Console&> operator << (Console& c, const Array<E,L,D>& r) 
  {
    // Write tag + size
    c << debug_info("Array")
      << name_value_pair(" Size", r.size())
      << endl;

    // Write N index/elements pairs
    for (uint32 i = 0UL, count = r.size(); i < count; ++i) 
      c << name_value_pair(" Index", i)
        << name_value_pair(" Element", r[i])
        << Cons::Break;
    
    // Close
    return c << Cons::Yellow << '}';
  }
  
  
} // WTL namespace



#endif // WTL_ARRAY_HPP