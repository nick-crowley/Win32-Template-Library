//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\ClientRectProperty.hpp
//! \brief Separate implementation for 'ClientRect' window property (resolves circular dependency)
//! \date 29 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CLIENT_RECT_PROPERTY_HPP
#define WTL_CLIENT_RECT_PROPERTY_HPP

#include <wtl/WTL.hpp>
#include <wtl/windows/properties/ClientRectProperty.h>        //!< ClientRectProperty
#include <wtl/windows/Window.hpp>                         //!< Window

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
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
  template <Encoding ENC>
  typename ClientRectPropertyImpl<ENC>::value_t  ClientRectPropertyImpl<ENC>::get() const 
  {
    RectL rc;    //!< New client rectangle

    // [EXISTS] Return current window rectangle
    if (this->Window.exists())
    {
      // Query client rectangle
      if (!::GetClientRect(this->Window, rc))
        throw platform_error(HERE, "Unable to query window rectangle");
    }
    else
    {
      // [�EXISTS] Ensure size/position not 'default'
      if (this->Window.Size == window_t::DefaultSize || this->Window.Position == window_t::DefaultPosition)
        throw logic_error(HERE, "Cannot generate a window rectangle from default co-ordinates");

      // Calculate client from window rectangle 
      rc = RectL(this->Window.Position(), this->Window.Size());
      if (!::AdjustWindowRectEx(rc, 
                                enum_cast(this->Window.Style.get()), 
                                boolean_cast(!this->Window.Menu.empty()), 
                                enum_cast(this->Window.StyleEx.get())))
        throw platform_error(HERE, "Unable to calculate window rectangle from client");
    }

    // Return client rectangle
    return rc;
  }

  // ----------------------------------- MUTATOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // ClientRectPropertyImpl::set 
  //! Set the current client rectangle iff window exists, otherwise 'initial' rectangle
  //! 
  //! \param[in] rc - New client rectangle
  //! 
  //! \throw wtl::platform_error - Unable to set client rectangle 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  void  ClientRectPropertyImpl<ENC>::set(value_t rc) 
  {
    RectL wnd(rc);   //!< New window rectangle

    // Calculate window rectangle 
    if (!::AdjustWindowRectEx(wnd, 
                              enum_cast(this->Window.Style.get()), 
                              boolean_cast(!this->Window.Menu.empty()), 
                              enum_cast(this->Window.StyleEx.get())))
      throw platform_error(HERE, "Unable to calculate window rectangle from client");

    // Set window rectangle
    this->Window.WindowRect = wnd;
  }

      
} // namespace wtl

#endif // WTL_CLIENT_RECT_PROPERTY_HPP


