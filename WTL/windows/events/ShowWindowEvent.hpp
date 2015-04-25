////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\event\ShowWindowEvent.hpp
//! \brief Encapsulates the WM_SHOWWINDOW message in the 'ShowWindow' event
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_SHOW_WINDOW_EVENT_HPP
#define WTL_SHOW_WINDOW_EVENT_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct EventArgs<ENC,WindowMessage::SHOWWINDOW> - Event arguments for Win32 message 'WM_SHOWWINDOW'
  //! 
  //! \tparam ENC - Message character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct EventArgs<ENC,WindowMessage::SHOWWINDOW>
  {  
    // ------------------- TYPES & CONSTANTS -------------------
  
    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;
    
    //! \alias resource_t - Define resource id type
    using resource_t = ResourceId<ENC>;
    
    //! \var encoding - Define message character encoding 
    static constexpr Encoding  encoding = ENC;
    
    //! \var message - Define message identifier
    static constexpr WindowMessage  message = WindowMessage::SHOWWINDOW;
    
    //! \var unhandled - Define unhandled result
    static constexpr ::LRESULT  unhandled = unhandled_result<message>::value;

    // --------------------- CONSTRUCTION ----------------------

    ///////////////////////////////////////////////////////////////////////////////
    // EventArgs::EventArgs
    //! Decode arguments for win32 message 'WM_SHOWWINDOW' 
    //! 
    //! \param[in] w - Whether showing or hiding
    //! \param[in] l - Window status
    ///////////////////////////////////////////////////////////////////////////////
    EventArgs(::WPARAM w, ::LPARAM l) : Showing(w != FALSE), 
                                        Status(opaque_cast<ShowWindowType>(l))
    {}
    
    // ------------------------ STATIC -------------------------

    // ---------------------- ACCESSORS ------------------------			

    // ----------------------- MUTATORS ------------------------

    // -------------------- REPRESENTATION ---------------------

    bool            Showing;      //!< Whether window is being shown or hidden
    ShowWindowType  Status;       //!< Status of window
  };
  
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \alias ShowWindowEvent - Defines 'ShowWindow' event (ie. WM_SHOWWINDOW)
  //! 
  //! \tparam ENC - Window character encoding
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using ShowWindowEvent = MessageEvent<ENC,WindowMessage::SHOWWINDOW>;
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \alias ShowWindowEventArgs - Arguments for 'ShowWindow' Event (ie. WM_SHOWWINDOW)
  //! 
  //! \tparam ENC - Message character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using ShowWindowEventArgs = EventArgs<ENC,WindowMessage::SHOWWINDOW>;

  ///////////////////////////////////////////////////////////////////////////////
  //! \alias ShowWindowEventHandler - Handler for 'ShowWindow' event (ie. WM_SHOWWINDOW)
  //! 
  //! \tparam ENC - Window character encoding
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using ShowWindowEventHandler = typename ShowWindowEvent<ENC>::delegate_t;


}

#endif // WTL_SHOW_WINDOW_EVENT_HPP
