//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Default.hpp
//! \brief Defines the default value for any type
//! \date 26 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_DEFAULT_HPP
#define WTL_DEFAULT_HPP

#include <wtl/WTL.hpp>
#include <wtl/utils/Constant.hpp>           //!< integral_constant
#include <wtl/utils/SFINAE.hpp>             //!< enable_if

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct default_t - Defines the default value for any type
  //! 
  //! \tparam T - Any type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename = void> 
  struct default_t; 
  /* Undefined */

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct default_t<integer> - Integer types default to zero
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T> 
  struct default_t<T,enable_if_integer_t<T>> : integral_constant<T, static_cast<T>(0)>
  {};

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct default_t<boolean> - Boolean types default to false
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T> 
  struct default_t<T,enable_if_boolean_t<T>> : integral_constant<bool, false>
  {};
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct default_t<float> - Floating-point types default to zero
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T> 
  struct default_t<T,enable_if_floating_t<T>> 
  {
    static constexpr T  value = static_cast<T>(0.0);
  };
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct default_t<pointer> - Pointer types default to nullptr
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename U>
  struct default_t<T*,U> 
  {
    static constexpr T*  value = nullptr;
  };
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct default_t<reference> - Reference types are not allowed
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename U>
  struct default_t<T&,U>;
  /*Undefined*/

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct default_t<T,POD> - Zero-initialize POD types  [Excluding scalar typs]
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T> 
  struct default_t<T,std::enable_if_t<std::is_pod<T>::value && !std::is_scalar<T>::value>> 
  {
    static constexpr T  value {};
  };

  //! \namespace <anon> - Utility namespace
  namespace 
  {
    //! \typedef enable_if_default_constructible_t - Defines SFINAE expression requiring default-constructible non-POD class type
    template <typename T>
    using enable_if_default_constructible_t = std::enable_if_t<std::is_class<T>::value 
                                                           && !std::is_pod<T>::value 
                                                           && std::is_default_constructible<T>::value>;
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct default_t<class> - Class types default to a default-constructed instance
  //! 
  //! \remarks Non-default constructible class types require individual specializations
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T> 
  struct default_t<T,enable_if_default_constructible_t<T>> 
  {
    static const T  value;
  };

  //! \var default_t<class>::value - Default class-type storage
  template <typename T> 
  const T default_t<T,enable_if_default_constructible_t<T>>::value;

  

  /////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::defvalue constexpr
  //! Get the default-value for any type
  //! 
  //! \tparam T - Any type
  //!
  //! \return default_t<T> - Default value
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T> constexpr
  T  defvalue() noexcept
  {
    // Return default value
    return default_t<T>::value;   
  }

  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::defvalue constexpr
  //! Get the default-value for any type via ATD
  //! 
  //! \tparam T - Any type
  //!
  //! \param[in] const& - Value used to deduce T
  //! \return default_t<T> - Default value
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T> constexpr
  T  defvalue(const T&) noexcept
  {
    // Return default value
    return default_t<T>::value;   
  }
}



#endif // WTL_DEFAULT_HPP


