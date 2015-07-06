//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\WindowStyleExProperty.cpp
//! \brief Implementation for 'WindowStyleEx' property (avoids circular reference regarding WindowBase template)
//! \date 5 July 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////

#include "wtl/WTL.hpp"
#include "wtl/casts/EnumCast.hpp"                            //!< EnumCast
#include "wtl/windows/properties/WindowStyleExProperty.hpp"    //!< WindowStyleExProperty
#include "wtl/windows/WindowBase.hpp"                        //!< WindowBase

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  // ---------------------------------- ACCESSOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // WindowStyleExPropertyImpl::get const
  //! Get the extended window style
  //! 
  //! \return value_t - Current style if window exists, otherwise 'initial' style
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  typename WindowStyleExPropertyImpl<ENC>::value_t  WindowStyleExPropertyImpl<ENC>::get() const 
  {
    // [EXISTS] Query window StyleEx
    if (this->Window.exists())
      return enum_cast<WindowStyleEx>( getFunc<encoding>(::GetWindowLongPtrA,::GetWindowLongPtrW)(this->Window, GWL_EXSTYLE) );
        
    // Return cached
    return base::get();
  }

  // ----------------------------------- MUTATOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // WindowStyleExPropertyImpl::set 
  //! Set the current extended window style iff window exists, otherwise 'initial' style
  //! 
  //! \param[in] style - Extended window style
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  void  WindowStyleExPropertyImpl<ENC>::set(value_t style) 
  {
    // [EXISTS] Set window StyleEx
    if (this->Window.exists() && !getFunc<encoding>(::SetWindowLongPtrA,::SetWindowLongPtrW)(this->Window, GWL_EXSTYLE, enum_cast(style)))
      throw platform_error(HERE, "Unable to set extended window style");

    // Store value
    base::set(style);
  }

      
} // namespace wtl

