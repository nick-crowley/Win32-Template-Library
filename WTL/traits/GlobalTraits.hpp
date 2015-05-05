//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\traits\GlobalTraits.hpp
//! \brief Defines global memory handle traits
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_GLOBAL_MEM_TRAITS_HPP
#define WTL_GLOBAL_MEM_TRAITS_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct handle_alloc<::HGLOBAL> - Encapsulates allocating global memory handles
  /////////////////////////////////////////////////////////////////////////////////////////
  template <>
  struct handle_alloc<::HGLOBAL>
  {
    //! \var npos - Invalid handle sentinel value
    static const ::HGLOBAL npos; 

    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HGLOBAL>::create
    //! Create global memory handle to resource
    //! 
    //! \param[in] const& module - Module containing resource
    //! \param[in] const& resource - Resource handle
    //! \return HAlloc<::HGLOBAL> - Accquired handle
    //! 
    //! \throw wtl::platform_error - Failed to allocate handle
    /////////////////////////////////////////////////////////////////////////////////////////
    static HAlloc<::HGLOBAL> create(const HModule& module, const HResource& resource) 
    { 
      // Load resource
      if (::HGLOBAL res = ::LoadResource(module, resource))
        return { res, AllocType::Accquire };

      // Error: Failed  
      throw platform_error(HERE, "Unable to load resource");
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HGLOBAL>::clone
    //! Clone handle
    //! 
    //! \param[in] mem - Global memory handle
    //! \return HAlloc<::HGLOBAL> - Duplicate of handle
    //! 
    //! \throw wtl::platform_error - Failed to clone handle
    /////////////////////////////////////////////////////////////////////////////////////////
    static HAlloc<::HGLOBAL> clone(HAlloc<::HGLOBAL> mem);

    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HGLOBAL>::destroy noexcept
    //! Release handle
    //! 
    //! \param[in] mem - Global memory handle
    //! \return bool - True iff closed successfully
    /////////////////////////////////////////////////////////////////////////////////////////
    static bool destroy(HAlloc<::HGLOBAL> mem) noexcept
    {
      // Delete without checking if handle is valid
      switch (mem.Method)
      {
      case AllocType::Accquire: return true;
      case AllocType::Create:   return false;
      case AllocType::WeakRef:  return true;
      }
      return false;
    }
  };

  
  //! \alias HGlobal - Shared global memory handle
  using HGlobal = Handle<::HGLOBAL>;

} //namespace wtl
#endif // WTL_GLOBAL_MEM_TRAITS_HPP

