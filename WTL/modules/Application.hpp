//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\modules\Application.hpp
//! \brief Encapsulates a program
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_APPLICATION_HPP
#define WTL_APPLICATION_HPP

#include <wtl/WTL.hpp>
#include <wtl/modules/Module.h>                         //!< Module
#include <wtl/threads/MessagePump.hpp>                  //!< MessagePump
#include <wtl/platform/DateTime.hpp>                    //!< DateTime
#include <wtl/platform/SystemVersion.hpp>               //!< SystemVersion
#include <wtl/utils/String.hpp>                         //!< String
#include <wtl/windows/controls/button/Button.hpp>       //!< Button
#include <wtl/windows/controls/combobox/ComboBox.hpp>   //!< ComboBox
#include <wtl/windows/controls/edit/Edit.hpp>           //!< Edit
#include <wtl/windows/controls/static/Static.hpp>       //!< Static
#include <wtl/windows/MessageWindow.hpp>                //!< MessageWindow

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Application - Encapsulates the executing module and the top-level application window.
  //! 
  //! \tparam WINDOW - Main window type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename WINDOW>
  struct Application : Module, 
                       MessagePump<WINDOW>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \var encoding - Inherit window character encoding
    static constexpr Encoding encoding = WINDOW::encoding;
    
    //! \alias type - Define own type
    using type = Application<WINDOW>;
  
    //! \alias module_base - Define module base type
    using module_base = Module;
    
    //! \alias msgpump_base - Define message pump base type
    using msgpump_base = MessagePump<WINDOW>;
    
    //! \alias char_t - Define character type
    using char_t = encoding_char_t<encoding>;

    //! \alias resource_t - Define resource id type
    using resource_t = ResourceId<encoding>;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  
    // ------------------------------------ CONSTRUCTION ------------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // Application::Application
    //! Create application from handle supplied by WinMain(..)
    //!
    //! \param[in] inst - Module instance
    /////////////////////////////////////////////////////////////////////////////////////////
    Application(::HINSTANCE inst) : module_base(inst), 
                                    msgpump_base(inst)
    {}
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
  public:
    DISABLE_COPY(Application);       //!< Cannot be copied
    ENABLE_MOVE(Application);        //!< Move-only type
    
    // ----------------------------------- STATIC METHODS -----------------------------------
  private:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Application::registerCommonCtrls const 
    //! Registers the window-classes of the various common controls
    //!
    //! \throw wtl::platform_error - Unable to register common controls
    /////////////////////////////////////////////////////////////////////////////////////////
    static void registerCommonCtrls()
    {
      ::INITCOMMONCONTROLSEX cmnCtrls { sizeof(::INITCOMMONCONTROLSEX), ICC_STANDARD_CLASSES | ICC_WIN95_CLASSES | ICC_USEREX_CLASSES | ICC_LINK_CLASS | ICC_PROGRESS_CLASS };
      
      // Register controls
      ::InitCommonControlsEx(&cmnCtrls);
      //if (!::InitCommonControlsEx(&cmnCtrls))
      //  throw platform_error(HERE, "Unable to register common controls");
    }

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Application::name const 
    //! Get the application name
    //!
    //! \return String<encoding> - Full application name
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual String<encoding> name() const = 0;
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Application::version const 
    //! Get the application version
    //!
    //! \return String<encoding> - Version string
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual String<encoding> version() const = 0;

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // Application::run
    //! Executes the application
    //! 
    //! \param[in] const* cmdLine - Command line
    //! \param[in] mode - Initial display mode
    //! \return int32_t - Return code
    /////////////////////////////////////////////////////////////////////////////////////////
    int32_t  run(const char_t* cmdLine, ShowWindowFlags mode = ShowWindowFlags::ShowNormal) 
    {
      // Feedback: AppName + Operating System
      cdebug << textcol::cyan                 << "--------------------------------------------------------------------------" << std::endl
             << (textcol::cyan|textcol::bold) << "\t\t\t"       << this->name() << "\t" << this->version()                    << std::endl  
             << textcol::cyan                 << "--------------------------------------------------------------------------" << std::endl 
             << std::endl
             << name_value_pair("Date/Time", LongDateString<encoding>().c_str())          << std::endl
             << name_value_pair("Command Line", cmdLine)                                  << std::endl
             << name_value_pair("Operating System", SystemVersion<encoding>().fullname()) << std::endl
             << name_value_pair("Module Path", this->path<encoding>().c_str())            << std::endl;

      // Register common-controls window classes
      registerCommonCtrls();

      // Register library window classes
      Button<encoding>::registerClass(this->handle());
      ComboBox<encoding>::registerClass(this->handle());
      Static<encoding>::registerClass(this->handle());
      Edit<encoding>::registerClass(this->handle());
      MessageWindow<encoding>::registerClass(this->handle());

      // Execute
      return msgpump_base::run(mode);
    }

  };
}

#endif // WTL_APPLICATION_HPP
