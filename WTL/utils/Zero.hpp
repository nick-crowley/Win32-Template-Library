//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Zero.hpp
//! \brief Defines the default value for any type
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_ZERO_HPP
#define WTL_ZERO_HPP

#include "wtl/WTL.hpp"
#include "wtl/utils/SFINAE.hpp"
#include "wtl/utils/Default.hpp"
#include "wtl/utils/Constant.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct zero_t - Defines zero for any numeric type
  //! 
  //! \tparam T - Any type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename = void> 
  struct zero_t; 
  /* Undefined */

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct zero_t<integer> - Integer zero is zero
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T> 
  struct zero_t<T,enable_if_integer_t<T>> : integral_constant<T, static_cast<T>(0)>
  {};

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct zero_t<boolean> - Boolean zero is false
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T> 
  struct zero_t<T,enable_if_boolean_t<T>> : integral_constant<bool, false>
  {};
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct zero_t<enumeration> - Enumeration zero is (possibly invalid) strongly typed zero
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T> 
  struct zero_t<T,enable_if_enum_t<T>> : integral_constant<T, static_cast<T>(0)>
  {};
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct zero_t<float> - Floating-point zero is precisely zero 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T> 
  struct zero_t<T,enable_if_floating_t<T>> 
  {
    //! \var value - Defines zero value
    static constexpr T  value = static_cast<T>(0.0);
  };
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct zero_t<pointer> - Pointer zero is nullptr
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename U>
  struct zero_t<T*,U> 
  {
    //! \var value - Defines zero value
    static constexpr T*  value = nullptr;
  };
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct zero_t<reference> - Null reference is not permitted
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename U>
  struct zero_t<T&,U>;
  /*Undefined*/
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct zero_t<array> - Array types are not permitted
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  struct zero_t<T,enable_if_array_t<T>>;
  /*Undefined*/
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct zero_t<class> - Class types are not permitted
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  struct zero_t<T,enable_if_class_t<T>>;
  /*Undefined*/




  /////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::zero constexpr
  //! Get the zero-value for any type
  //! 
  //! \tparam T - Any type
  //!
  //! \return zero_t<T> - Default value
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T> constexpr
  T  zero() noexcept
  {
    // Return zero value
    return zero_t<T>::value;   
  }

  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::zero constexpr
  //! Get the zero-value for any type via ATD
  //! 
  //! \tparam T - Any type
  //!
  //! \param[in] const& - Value used to deduce T
  //! \return zero_t<T> - Default value
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T> constexpr
  T  zero(const T&) noexcept
  {
    // Return zero value
    return zero_t<T>::value;   
  }
}



#endif // WTL_ZERO_HPP

