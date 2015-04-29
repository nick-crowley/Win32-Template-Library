////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\EventMessage.hpp
//! \brief Encapsulates handling a window message in an observeable event
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_EVENT_MESSAGE_HPP
#define WTL_EVENT_MESSAGE_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct EventArgs - Encapsulates decoding win32 message arguments
  //! 
  //! \tparam ENC - Message character encoding 
  //! \tparam WM - Window message
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, WindowMessage WM> 
  struct EventArgs
  {    
    // ------------------- TYPES & CONSTANTS -------------------

    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;
    
    //! \alias resource_t - Define resource id type
    using resource_t = ResourceId<ENC>;
    
    //! \var encoding - Define message character encoding 
    static constexpr Encoding  encoding = ENC;
    
    //! \var message - Define message identifier
    static constexpr WindowMessage  message = WM;
    
    //! \var unhandled - Define unhandled result
    static constexpr ::LRESULT  unhandled = unhandled_result<WM>::value;
    
    // -------------------- REPRESENTATION ---------------------

    // --------------------- CONSTRUCTION ----------------------
    
    ///////////////////////////////////////////////////////////////////////////////
    // EventArgs::EventArgs
    //! Create argument decoder for messages with zero arguments
    ///////////////////////////////////////////////////////////////////////////////
    EventArgs()
    {}

    ///////////////////////////////////////////////////////////////////////////////
    // EventArgs::EventArgs
    //! Create from parameters
    //! 
    //! \param[in] w - First message parameter
    //! \param[in] l - Second message parameter
    ///////////////////////////////////////////////////////////////////////////////
    EventArgs(::WPARAM w, ::LPARAM l)
    {}

    ///////////////////////////////////////////////////////////////////////////////
    // EventArgs::~EventArgs
    //! Virtual d-tor
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~EventArgs()
    {}
    
    // ------------------------ STATIC -------------------------

    // ---------------------- ACCESSORS ------------------------			
    
    // ----------------------- MUTATORS ------------------------

  };
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \alias MessageEvent - Defines an event encapsulating a window message 
  //! 
  //! \tparam ENC - Window character encoding
  //! \tparam WM - Window message
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, WindowMessage WM>
  using MessageEvent = Event<ENC, LResult, EventArgs<ENC,WM>&>;

  ///////////////////////////////////////////////////////////////////////////////
  //! \alias MessageEventArgs - Defines arguments for any 'Message' Event 
  //! 
  //! \tparam ENC - Message character encoding 
  //! \tparam WM - Window message
  ///////////////////////////////////////////////////////////////////////////////
  //template <Encoding ENC, WindowMessage WM, template <unsigned> class ARGS = MessageEvent<ENC,WM>::argument_t>
  //using MessageEventArgs = ARGS<0>; // typename MessageEvent<ENC,WM>::argument_t<0>;

  ///////////////////////////////////////////////////////////////////////////////
  //! \alias MessageEventHandler - Defines handler for any 'Message' event 
  //! 
  //! \tparam ENC - Window character encoding
  //! \tparam WM - Window message
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, WindowMessage WM>
  using MessageEventHandler = typename MessageEvent<ENC,WM>::delegate_t;

  
}

#endif // WTL_EVENT_MESSAGE_HPP