//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\gdi\DeviceContext.hpp
//! \brief Provides static storage
//! \date 7 March 2015
//! \author Nick Crowley
//! \copyright � Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////

#include "DeviceContext.hpp"
#include "wtl/utils/Default.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
  // ----------------------------------- REPRESENTATION -----------------------------------
  
  //! \var DeviceContext::ScreenDC - Screen device context
  DeviceContext  DeviceContext::ScreenDC = default<HDeviceContext>();
  
  // ------------------------------------ CONSTRUCTION ------------------------------------
	
  // -------------------------------- COPY, MOVE & DESTROY --------------------------------

  // ----------------------------------- STATIC METHODS -----------------------------------

  // ---------------------------------- ACCESSOR METHODS ----------------------------------

  // ----------------------------------- MUTATOR METHODS ----------------------------------
}
