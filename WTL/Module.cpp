////////////////////////////////////////////////////////////////////////////////
//! \file wtl\library\Module.cpp
//! \brief Provides module implementation
//! \date 7 March 2015
//! \author Nick Crowley
//! \copyright � Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////

#include "WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  //! \var LoadedModules - Loaded modules collection
  ModuleCollection  LoadedModules;

  ///////////////////////////////////////////////////////////////////////////////
  // Module::Module
  //! Create from native module handle. Adds module to 'Loaded Modules' collection.
  //!
  //! \param[in] m - Native module handle
  ///////////////////////////////////////////////////////////////////////////////
  Module::Module(::HMODULE m) : Handle(m, AllocType::WeakRef)
  {
    LoadedModules.add(*this);
  }
  
  ///////////////////////////////////////////////////////////////////////////////
  // Module::Module
  //! Virtual d-tor. Removes module from 'Loaded Modules' collection.
  ///////////////////////////////////////////////////////////////////////////////
  Module::~Module()
  {
    LoadedModules.remove(*this);
  }

}