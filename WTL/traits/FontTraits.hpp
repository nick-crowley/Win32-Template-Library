//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\traits\FontTraits.hpp
//! \brief Defines font traits + helper global operators
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_FONT_TRAITS_HPP
#define WTL_FONT_TRAITS_HPP

#include <wtl/WTL.hpp>
#include <wtl/utils/Handle.hpp>               //!< Handle
#include <wtl/utils/String.hpp>               //!< String
#include <wtl/platform/DrawingFlags.hpp>      //!< StockObject, FontStyles
#include <wtl/casts/BooleanCast.hpp>          //!< BooleanCast
#include <wtl/casts/EnumCast.hpp>             //!< EnumCast

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias HFont - Shared font handle
  /////////////////////////////////////////////////////////////////////////////////////////
  using HFont = Handle<::HFONT>;
  
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct handle_alloc<::HFONT> - Encapsulates creating device context fonts
  /////////////////////////////////////////////////////////////////////////////////////////
  template <>
  struct handle_alloc<::HFONT>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \var npos - Invalid handle sentinel value
    static constexpr ::HFONT npos = defvalue<::HFONT>(); 
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  
    // ------------------------------------ CONSTRUCTION ------------------------------------
	
    DISABLE_CTOR(handle_alloc);     //!< Cannot instantiate

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    DISABLE_COPY(handle_alloc);     //!< Cannot instantiate
    DISABLE_MOVE(handle_alloc);     //!< Cannot instantiate
    DISABLE_DTOR(handle_alloc);     //!< Cannot instantiate

    // ----------------------------------- STATIC METHODS -----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HFONT>::create
    //! Create font handle
    //! 
    //! \tparam ENC - Encoding type
    //! \tparam LEN - Name buffer capacity
    //! 
    //! \param[in] const& name - Font name
    //! \param[in] height - Height in twips
    //! \param[in] weight - Weight
    //! \param[in] italic - Italics
    //! \param[in] underline - Underline
    //! \param[in] strike - Strikeout
    //! \param[in] charSet - Character set
    //! \param[in] quality - Quality
    //! \param[in] family - Font pitch and family
    //! \param[in] precision - Output precision
    //! \param[in] clipping - Clipping precision
    //! \param[in] width - Width in twips
    //! \param[in] escape - Escapement
    //! \param[in] orient - Orientation
    //! \return NativeHandle<::HFONT> - Created handle
    //! 
    //! \throw wtl::platform_error - Failed to allocate handle
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC>
    static NativeHandle<::HFONT> create(const String<ENC>& name, 
                                        int32_t       height,  
                                        FontWeight    weight = FontWeight::Normal, 
                                        bool          italic = false, 
                                        bool          underline = false, 
                                        bool          strike = false, 
                                        FontCharSet   charSet = FontCharSet::Default, 
                                        FontQuality   quality = FontQuality::Default,
                                        FontFamily    family = FontFamily::Default, 
                                        FontPrecision precision = FontPrecision::Default, 
                                        FontClipping  clipping = FontClipping::Default,
                                        int32_t       width = 0,  
                                        int32_t       escape = 0, 
                                        int32_t       orient = 0) 
    { 
      // Create font 
      if (::HFONT font = WinAPI<ENC>::createFont(height, width, 
                                                 escape, orient, 
                                                 enum_cast(weight), 
                                                 boolean_cast(italic), boolean_cast(underline), boolean_cast(strike), 
                                                 enum_cast(charSet), 
                                                 enum_cast(precision), enum_cast(clipping), enum_cast(quality), 
                                                 enum_cast(family), name.c_str()))
        return { font, AllocType::Create };

      // Error: Failed  
      throw platform_error(HERE, "Unable to create font");
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HFONT>::create
    //! Accquire stock font handle
    //! 
    //! \param[in] obj - Stock object
    //! \return NativeHandle<::HFONT> - Accquired handle
    //! 
    //! \throw wtl::platform_error - Failed to allocate handle
    /////////////////////////////////////////////////////////////////////////////////////////
    static NativeHandle<::HFONT> create(StockObject obj) 
    { 
      switch (obj)
      {
      case StockObject::OemFixedFont:
      case StockObject::AnsiFixedFont:
      case StockObject::AnsiVarFont:
      case StockObject::SystemFont:
      case StockObject::DeviceDefaultFont:
      case StockObject::SystemFixedFont:
      case StockObject::DefaultGuiFont:
        // Accquire stock font
        if (::HFONT pen = (::HFONT)::GetStockObject(enum_cast(obj)))
          return { pen, AllocType::WeakRef };
      }
      
      // Error: Failed  
      throw platform_error(HERE, "Unable to accquire stock font");
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HFONT>::clone
    //! Clone handle
    //! 
    //! \param[in] font - Handle
    //! \return NativeHandle<::HFONT> - Duplicate of handle
    //! 
    //! \throw wtl::platform_error - Failed to clone handle
    /////////////////////////////////////////////////////////////////////////////////////////
    static NativeHandle<::HFONT> clone(NativeHandle<::HFONT> font);

    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HFONT>::destroy noexcept
    //! Release handle 
    //! 
    //! \param[in] font - Handle
    //! \return bool - True iff closed successfully
    /////////////////////////////////////////////////////////////////////////////////////////
    static bool destroy(NativeHandle<::HFONT> font) noexcept
    {
      // Delete without checking if handle is valid
      switch (font.Method)
      {
      case AllocType::Accquire: return ::DeleteObject(font.Handle) != False;
      case AllocType::Create:   return false;
      case AllocType::WeakRef:  return true;
      }
      return false;
    }
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };

  
} //namespace wtl
#endif // WTL_FONT_TRAITS_HPP

