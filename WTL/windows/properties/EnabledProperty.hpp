//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\EnabledProperty.hpp
//! \brief Separate implementation for 'Enabled' window property (resolves circular dependency)
//! \date 29 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_ENABLED_PROPERTY_HPP
#define WTL_WINDOW_ENABLED_PROPERTY_HPP

#include "wtl/WTL.hpp"
#include "wtl/casts/BooleanCast.hpp"                      //!< BooleanCast
#include "wtl/windows/properties/EnabledProperty.h"       //!< EnabledPropertyImpl
#include "wtl/windows/WindowBase.hpp"                     //!< WindowBase

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  // ---------------------------------- ACCESSOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // EnabledPropertyImpl::get const
  //! Get the window state
  //! 
  //! \return value_t - Current state if window exists, otherwise 'initial' state
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  typename EnabledPropertyImpl<ENC>::value_t  EnabledPropertyImpl<ENC>::get() const 
  {
    // [EXISTS] Query window state
    if (this->Window.exists())
      return boolean_cast(::IsWindowVisible(this->Window));

    // Return cached
    return base::get();
  }

  // ----------------------------------- MUTATOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // EnabledPropertyImpl::set 
  //! Set the current window state iff window exists, otherwise 'initial' state
  //! 
  //! \param[in] state - Window state
  //! 
  //! \throw wtl::platform_error - Unable to set window state
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  void  EnabledPropertyImpl<ENC>::set(value_t state) 
  {
    // Set window state
    if (this->Window.exists() && !::EnableWindow(this->Window, boolean_cast(state)))
      throw platform_error(HERE, "Unable to set window state");

    // Update value
    base::set(state);
  }

      
} // namespace wtl

#endif    // WTL_WINDOW_ENABLED_PROPERTY_HPP
