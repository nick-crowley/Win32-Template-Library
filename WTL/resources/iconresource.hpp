//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\resources\IconResource.hpp
//! \brief Encapsulates reading icon resources
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_ICON_RESOURCES_HPP
#define WTL_ICON_RESOURCES_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct IconResource - Encapsulates loading an icon from the resource table
  /////////////////////////////////////////////////////////////////////////////////////////
  struct IconResource : Resource
  {      
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias base - Define base type
    using base = Resource;

    // ----------------------------------- REPRESENTATION -----------------------------------
  public:
    HIcon   Handle;     //!< Shared icon handle

    // ------------------------------------ CONSTRUCTION ------------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // IconResource::IconResource
    //! Loads an icon resource
    //! 
    //! \tparam ENC - Character encoding type
    //! 
    //! \param[in] id - Icon identifier
    //! \param[in] lang - [optional] Resource language (If unspecified, neutral is used)
    //! 
    //! \throw wtl::logic_error - Missing icon
    //! \throw wtl::platform_error - Unable to load resource
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC> explicit 
    IconResource(ResourceId<ENC> id, LanguageId lang = LanguageId::Neutral) 
        // Load appropriate icon from resources, then create handle
      : base(LoadedModules.findResource(ResourceType::Icon,findIconId(id,SizeL::EMPTY,lang),lang)),
        Handle(const_cast<byte*>(get<byte>()), base::size(), true)
    {}
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // IconResource::IconResource
    //! Loads an icon resource with a custom size
    //! 
    //! \tparam ENC - Character encoding type
    //! 
    //! \param[in] id - Icon identifier
    //! \param[in] size - Desired size 
    //! \param[in] lang - [optional] Resource language (If unspecified, neutral is used)
    //! 
    //! \throw wtl::logic_error - Missing icon
    //! \throw wtl::platform_error - Unable to load resource
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC> explicit 
    IconResource(ResourceId<ENC> id, SizeL size, LanguageId lang = LanguageId::Neutral) 
        // Load appropriate icon from resources, then create handle
      : base(LoadedModules.findResource(ResourceType::Icon,findIconId(id,size,lang),lang)),
        Handle(const_cast<byte*>(get<byte>()), base::size(), size)
    {}
    
    // --------------------------------------- STATIC ---------------------------------------
  protected:
    /////////////////////////////////////////////////////////////////////////////////////////
    // IconResource::IconResource
    //! Loads an icon resource
    //! 
    //! \param[in] id - Icon identifier
    //! \param[in] size - Desired size (If zero, SM_CXICON is used)
    //! \param[in] lang - Resource language (If unspecified, neutral is used)
    //! 
    //! \throw wtl::logic_error - Missing icon
    //! \throw wtl::platform_error - Unable to load resource
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC>
    static ResourceId<ENC> findIconId(ResourceId<ENC> id, SizeL size, LanguageId lang)
    {
      Resource group(LoadedModules.findResource(ResourceType::GroupIcon,id,lang));    //!< Find icon group

      // Lookup icon that is most appropriate for current display
      return ::LookupIconIdFromDirectoryEx(const_cast<byte*>(group.get<byte>()), TRUE, size.width, size.height, LR_DEFAULTCOLOR);
    }

    // -------------------------------------- ACCESSORS --------------------------------------			

    // --------------------------------------- MUTATORS --------------------------------------

  };

  
} //namespace wtl
#endif // WTL_ICON_RESOURCES_HPP

