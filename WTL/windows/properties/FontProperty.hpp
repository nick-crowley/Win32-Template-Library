//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\FontProperty.hpp
//! \brief Encapsulates the basic window font in a class-type property
//! \date 5 July 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_FONT_PROPERTY_HPP
#define WTL_WINDOW_FONT_PROPERTY_HPP

#include "wtl/WTL.hpp"
#include "wtl/traits/EncodingTraits.hpp"                  //!< Encoding
#include "wtl/traits/FontTraits.hpp"                      //!< HFont
#include "wtl/windows/properties/WindowProperty.hpp"      //!< WindowPropertyImpl

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct FontPropertyImpl - Encapsulates the window font in a read/write ref-counted shared-handle property
  //! 
  //! \tparam ENC - Window encoding
  //!
  //! \remarks All windows are created using the default system font, therefore this property does not define the 'initial' font.
  //! \remarks The font is stored as a shared window handle, it is not necessarily destroyed, therefore, when the window is destroyed.
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct FontPropertyImpl : WindowPropertyImpl<ENC,HFont,PropertyAccess::ReadWrite>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Define own type
    using type = FontPropertyImpl;

    //! \alias base - Define base type
    using base = WindowPropertyImpl<ENC,HFont,PropertyAccess::ReadWrite>;
      
    //! \alias value_t - Inherit value type
    using value_t = typename base::value_t;

    // ----------------------------------- REPRESENTATION -----------------------------------

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // FontPropertyImpl::FontPropertyImpl
    //! Create with initial font used by all window-classes (System font)
    //! 
    //! \param[in,out] &wnd - Owner window
    //! \param[in] init - Initial font
    /////////////////////////////////////////////////////////////////////////////////////////
    FontPropertyImpl(WindowBase<ENC>& wnd, StockObject init) : base(wnd, init)
    {}

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // FontPropertyImpl::get const
    //! Get the window font
    //! 
    //! \return value_t - Current font if window exists, otherwise 'initial' font
    /////////////////////////////////////////////////////////////////////////////////////////
    value_t  get() const;

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // FontPropertyImpl::set 
    //! Set the current window font iff window exists, otherwise 'initial' font
    //! 
    //! \param[in] font - Window font
    /////////////////////////////////////////////////////////////////////////////////////////
    void  set(value_t font);
  };

  
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias FontProperty - Define window font property type 
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using FontProperty = Property<FontPropertyImpl<ENC>>;

      
} // namespace wtl

#endif // WTL_WINDOW_FONT_PROPERTY_HPP