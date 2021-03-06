//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\ClientRectProperty.h
//! \brief Separate class declaration for the 'ClientRect' window property
//! \date 29 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CLIENT_RECT_PROPERTY_H
#define WTL_CLIENT_RECT_PROPERTY_H

#include <wtl/WTL.hpp>
#include <wtl/utils/Rectangle.hpp>           //!< RectL
#include <wtl/traits/EncodingTraits.hpp>     //!< Encoding
#include <wtl/windows/PropertyImpl.hpp>      //!< PropertyImpl

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct ClientRectPropertyImpl - Provides the getters and setters for the 'ClientRect' window property
  //! 
  //! \tparam ENC - Window encoding
  //!
  //! \remarks [WINDOW NOT EXISTS] Values derived from 'Size' and 'Position' window properties
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct ClientRectPropertyImpl : PropertyImpl<ENC,RectL>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Define own type
    using type = ClientRectPropertyImpl<ENC>;

    //! \alias base - Define base type
    using base = PropertyImpl<ENC,RectL>;
      
    //! \alias value_t - Inherit value type
    using value_t = typename base::value_t;
    
    //! \alias window_t - Inherit window type
    using window_t = typename base::window_t;
    
    // ----------------------------------- REPRESENTATION -----------------------------------

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // ClientRectPropertyImpl::ClientRectPropertyImpl
    //! Create window property
    //! 
    //! \param[in,out] &wnd - Owner window
    //!
    //! \remarks Initial value is determined from offline 'Size' and 'Position'
    /////////////////////////////////////////////////////////////////////////////////////////
    ClientRectPropertyImpl(window_t& wnd) : base(wnd, defvalue<value_t>())
    {}

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // ClientRectPropertyImpl::get const
    //! Get the client rectangle
    //! 
    //! \return value_t - Current rectangle if window exists, otherwise 'initial' rectangle
    //!
    //! \throw wtl::logic_error - Window is using default size or location
    //! \throw wtl::platform_error - Unable to query client rectangle
    /////////////////////////////////////////////////////////////////////////////////////////
    value_t  get() const;

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // ClientRectPropertyImpl::set 
    //! Set the current client rectangle iff window exists, otherwise 'initial' rectangle
    //! 
    //! \param[in] rectangle - Window rectangle
    //! 
    //! \throw wtl::platform_error - Unable to set client rectangle 
    /////////////////////////////////////////////////////////////////////////////////////////
    void  set(value_t rectangle);
  };

  
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias ClientRectProperty - Define client rectangle property type 
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using ClientRectProperty = Property<ClientRectPropertyImpl<ENC>>;

      
} // namespace wtl

#endif // WTL_CLIENT_RECT_PROPERTY_H
