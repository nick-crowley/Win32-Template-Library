//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\commands\NewDocumentCommand.hpp
//! \brief Encapsulates the 'New Document' Gui Command
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_NEW_DOCUMENT_HPP
#define WTL_NEW_DOCUMENT_HPP

#include <wtl/WTL.hpp>
#include <wtl/windows/Command.hpp>             //!< Command
#include <wtl/windows/Window.hpp>         //!< Window
#include <wtl/windows/MessageBox.hpp>         //!< errorBox

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct NewDocumentCommand - Encapsulates displaying the 'New Document' dialog
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct NewDocumentCommand : Command<ENC>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias type - Define own type
    using type = NewDocumentCommand<ENC>;
  
    //! \alias base - Define base type
    using base = Command<ENC>;

    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;
    
    //! \alias window_t - Define window base type
    using window_t = Window<ENC>;

    //! \var encoding - Define encoding type
    static constexpr Encoding encoding = ENC;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  
    // ------------------------------------- CONSTRUCTION -----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // NewDocumentCommand::NewDocumentCommand
    //! Create command
    //! 
    //! \param[in] appWnd - Main application window
    /////////////////////////////////////////////////////////////////////////////////////////
    NewDocumentCommand(window_t& appWnd)  
      : base(CommandId::File_New, [&appWnd] () { errorBox<encoding>(appWnd, "Program Error", "Command not implemented"); })
    {}
    
	  // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    ENABLE_COPY(NewDocumentCommand);      //!< Can be shallow copied
    ENABLE_MOVE(NewDocumentCommand);      //!< Can be moved 
    ENABLE_POLY(NewDocumentCommand);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // NewDocumentCommand::clone const
    //! Create a new instance of the command
    //! 
    //! \return type* - New instance of command
    /////////////////////////////////////////////////////////////////////////////////////////
    type*  clone() const override
    {
      return new NewDocumentCommand(*this);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // NewDocumentCommand::state const
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

#endif // WTL_NEW_DOCUMENT_HPP
