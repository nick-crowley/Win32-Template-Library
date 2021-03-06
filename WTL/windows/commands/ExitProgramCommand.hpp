//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\commands\ExitProgramCommand.hpp
//! \brief Encapsulates the 'Exit program' command
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CLOSE_PROGRAM_HPP
#define WTL_CLOSE_PROGRAM_HPP

#include <wtl/WTL.hpp>
#include <wtl/windows/Command.hpp>             //!< Command
#include <wtl/windows/Window.hpp>         //!< Window

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct ExitProgramCommand - Base for all Gui commands
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct ExitProgramCommand : Command<ENC>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias type - Define own type
    using type = ExitProgramCommand<ENC>;
  
    //! \alias base - Define base type
    using base = Command<ENC>;

    //! \alias window_t - Define window base type
    using window_t = Window<ENC>;

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    window_t&  AppWnd;        //!< Application window

    // ------------------------------------- CONSTRUCTION -----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // ExitProgramCommand::ExitProgramCommand
    //! Create command
    //! 
    //! \param[in] appWnd - Main application window
    /////////////////////////////////////////////////////////////////////////////////////////
    ExitProgramCommand(window_t& appWnd) 
      : base(CommandId::App_Exit, [&appWnd] () { appWnd.post(WindowMessage::Close); } ),
        AppWnd(appWnd)
    {}
    
	  // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    ENABLE_COPY(ExitProgramCommand);      //!< Can be shallow copied
    ENABLE_MOVE(ExitProgramCommand);      //!< Can be moved 
    ENABLE_POLY(ExitProgramCommand);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ExitProgramCommand::clone const
    //! Create a new instance of the command
    //! 
    //! \return type* - New instance of command
    /////////////////////////////////////////////////////////////////////////////////////////
    type*  clone() const override
    {
      return new ExitProgramCommand(AppWnd);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // ExitProgramCommand::state const
    //! Query the current state of the command 
    //! 
    //! \return CommandState - Current state of command
    /////////////////////////////////////////////////////////////////////////////////////////
    CommandState state() const override
    {
      // Always enabled
      return CommandState::Enabled;
    }
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
  };
  
  
}

#endif // WTL_CLOSE_PROGRAM_HPP
