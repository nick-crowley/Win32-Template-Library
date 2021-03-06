//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\traits\ThemeTraits.hpp
//! \brief Defines theme traits 
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_THEME_TRAITS_HPP
#define WTL_THEME_TRAITS_HPP

#include <wtl/WTL.hpp>
#include <wtl/utils/Handle.hpp>               //!< Handle
#include <wtl/utils/String.hpp>               //!< String
#include <wtl/traits/WindowTraits.hpp>        //!< HWnd
#include <Uxtheme.h>                          //!< Visual styles

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct htheme_allocator - Encapsulates allocating ::HTHEME handles  (which are only typedefs for ::HANDLE)
  /////////////////////////////////////////////////////////////////////////////////////////
  struct htheme_allocator
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \var npos - Invalid handle sentinel value
    static constexpr ::HTHEME npos = defvalue<::HTHEME>(); 
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  
    // ------------------------------------ CONSTRUCTION ------------------------------------
	
    DISABLE_CTOR(htheme_allocator);     //!< Cannot instantiate

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    DISABLE_COPY(htheme_allocator);     //!< Cannot instantiate
    DISABLE_MOVE(htheme_allocator);     //!< Cannot instantiate
    DISABLE_DTOR(htheme_allocator);     //!< Cannot instantiate

    // ----------------------------------- STATIC METHODS -----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // htheme_allocator::create
    //! Create solid theme
    //! 
    //! \param[in] const& wnd - Owner window
    //! \param[in] const& name - Wide-character string containing a semicolon-separated list of classes.
    //! \return NativeHandle<::HTHEME> - Created handle
    //! 
    //! \throw wtl::platform_error - Failed to allocate handle
    /////////////////////////////////////////////////////////////////////////////////////////
    static NativeHandle<::HTHEME> create(const HWnd& wnd, const String<Encoding::UTF16>& name) 
    { 
      // Create solid colour theme handle
      if (::HTHEME theme = ::OpenThemeData(wnd, name.c_str()))
        return { theme, AllocType::Create };

      // Error: Failed  
      throw platform_error(HERE, "Unable to create visual style");
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // htheme_allocator::clone
    //! Clone handle
    //! 
    //! \param[in] theme - Theme handle
    //! \return NativeHandle<::HTHEME> - Duplicate of handle
    //! 
    //! \throw wtl::platform_error - Failed to clone handle
    /////////////////////////////////////////////////////////////////////////////////////////
    static NativeHandle<::HTHEME>  clone(NativeHandle<::HTHEME> theme) { return theme; }

    /////////////////////////////////////////////////////////////////////////////////////////
    // htheme_allocator::destroy noexcept
    //! Release theme handle
    //! 
    //! \param[in] theme - Theme handle
    //! \return bool - True iff closed successfully
    /////////////////////////////////////////////////////////////////////////////////////////
    static bool destroy(NativeHandle<::HTHEME> theme) noexcept
    {
      // Delete without checking if handle is valid
      switch (theme.Method)
      {
      case AllocType::Accquire: return true;
      case AllocType::Create:   return SUCCEEDED( ::CloseThemeData(theme.Handle) );
      case AllocType::WeakRef:  return true;
      }
      return false;
    }
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };


  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias HTheme - Shared theme handle
  //!
  //! \remarks Custom allocator is provided to disambiguate ::HTHEME, which is merely a typedef for ::HANDLE
  /////////////////////////////////////////////////////////////////////////////////////////
  using HTheme = Handle<::HTHEME, htheme_allocator>;  


  
} //namespace wtl
#endif // WTL_THEME_TRAITS_HPP

