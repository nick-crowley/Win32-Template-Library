//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\ButtonIconProperty.hpp
//! \brief Separate class declaration for the Button control 'Icon' property
//! \date 29 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_BUTTON_ICON_PROPERTY_H
#define WTL_BUTTON_ICON_PROPERTY_H

#include <wtl/WTL.hpp>
#include <wtl/traits/EncodingTraits.hpp>     //!< Encoding
#include <wtl/traits/IconTraits.hpp>         //!< HIcon
#include <wtl/windows/PropertyImpl.hpp>      //!< PropertyImpl

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  //! Forward declaration
  template <Encoding ENC>
  struct Button;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct ButtonIconPropertyImpl - Provides the getters and setters for the 'Icon' window property
  //! 
  //! \tparam ENC - Window encoding
  //!
  //! \remarks All windows are created using the default system font, therefore this property does not define the 'initial' font.
  //! \remarks The font is stored as a shared-handle which is not necessarily released when the window is destroyed.
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct ButtonIconPropertyImpl : PropertyImpl<ENC,HIcon,Button<ENC>>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Define own type
    using type = ButtonIconPropertyImpl;

    //! \alias base - Define base type
    using base = PropertyImpl<ENC,HIcon,Button<ENC>>;
      
    //! \alias value_t - Inherit value type
    using value_t = typename base::value_t;
    
    //! \alias window_t - Inherit window type
    using window_t = typename base::window_t;

    // ----------------------------------- REPRESENTATION -----------------------------------

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // ButtonIconPropertyImpl::ButtonIconPropertyImpl
    //! Create window property 
    //! 
    //! \param[in,out] &wnd - Owner window
    /////////////////////////////////////////////////////////////////////////////////////////
    ButtonIconPropertyImpl(window_t& wnd) : base(wnd)
    {}

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ButtonIconPropertyImpl::exist const
    //! Query whether button has an icon
    //! 
    //! \return bool - True iff icon assigned
    /////////////////////////////////////////////////////////////////////////////////////////
    bool exists() const
    {
      return this->Value.exists();
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // ButtonIconPropertyImpl::get const
    //! Get the button icon
    //! 
    //! \return value_t - Current icon if button exists, otherwise initial icon
    /////////////////////////////////////////////////////////////////////////////////////////
    value_t  get() const;

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // ButtonIconPropertyImpl::set 
    //! Set the icon iff button exists, otherwise sets the initial icon
    //! 
    //! \param[in] icon - Button icon
    /////////////////////////////////////////////////////////////////////////////////////////
    void  set(value_t font);
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ButtonIconPropertyImpl::set 
    //! Set the previously assigned icon
    /////////////////////////////////////////////////////////////////////////////////////////
    void  set();

  };

  
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias ButtonIconProperty - Define button icon property type 
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using ButtonIconProperty = Property<ButtonIconPropertyImpl<ENC>>;

      
} // namespace wtl

#endif // WTL_BUTTON_ICON_PROPERTY_H