////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\event\PaintWindowEvent.hpp
//! \brief Encapsulates the WM_PAINT message in the 'PaintWindow' event
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_PAINT_WINDOW_EVENT_HPP
#define WTL_PAINT_WINDOW_EVENT_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  

  ///////////////////////////////////////////////////////////////////////////////
  //! \struct EventArgs<WindowMessage::PAINT> - Event arguments for Win32 message 'WM_PAINT'
  //! 
  //! \tparam ENC - Message character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct EventArgs<ENC,WindowMessage::PAINT>
  {  
    // ------------------- TYPES & CONSTANTS -------------------
  
    //! \struct PaintCycle - Encapsulates the paint cycle for a window's client area
    struct PaintCycle : PAINTSTRUCT
    {
      //! \alias base - Define base type
      using base = PAINTSTRUCT;

      ///////////////////////////////////////////////////////////////////////////////
      // PaintCycle::PaintCycle
      //! Initiates the paint cycle for a window
      //! 
      //! \param[in] const& wnd - Window being painted
      ///////////////////////////////////////////////////////////////////////////////
      PaintCycle(const HWnd& wnd) : Window(wnd)
      {
        // Begin paint cycle
        ::BeginPaint(Window, static_cast<base*>(this));
      }

      ///////////////////////////////////////////////////////////////////////////////
      // PaintCycle::~PaintCycle
      //! Ends the paint cycle for a window
      ///////////////////////////////////////////////////////////////////////////////
      virtual ~PaintCycle()
      {
        // End paint cycle (Validate update region)
        ::EndPaint(Window, static_cast<base*>(this));
      }

      const HWnd&   Window;     //!< Handle of window being painted
    };
    
    // --------------------- CONSTRUCTION ----------------------

    ///////////////////////////////////////////////////////////////////////////////
    // EventArgs<WindowMessage::PAINT>::EventArgs
    //! Creates arguments for 'PaintWindow' Event (ie. WM_PAINT)
    //! 
    //! \param[in] const& wnd - Window receiving message
    //! \param[in] w - Not used
    //! \param[in] l - Not used
    ///////////////////////////////////////////////////////////////////////////////
    EventArgs(const HWnd& wnd, ::WPARAM w, ::LPARAM l) : Data(wnd), 
                                                         Graphics(Data.hdc),
                                                         Rect(Data.rcPaint),
                                                         EraseBackground(Data.fErase != FALSE), 
                                                         IncUpdate(Data.fIncUpdate != FALSE), 
                                                         Restore(Data.fRestore != FALSE)
    {}

    ///////////////////////////////////////////////////////////////////////////////
    // EventArgs::~EventArgs
    //! Ends the paint cycle
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~EventArgs()
    {}
    
    // ------------------------ STATIC -------------------------

    // ---------------------- ACCESSORS ------------------------			

    // ----------------------- MUTATORS ------------------------

    // -------------------- REPRESENTATION ---------------------
  protected:
    PaintCycle     Data;             //!< Encapsulates the paint cycle (NB: Must be initialized before following fields)

  public:
    DeviceContext  Graphics;         //!< Device context clipped to non-client area 
    RectL          Rect;             //!< Drawing/update rectangle
    bool           EraseBackground;  //!< Whether to erase window background 
    
  protected:
    bool           IncUpdate,        //!< Reserved
                   Restore;          //!< Reserved
  };
  
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \alias PaintWindowEventArgs - Arguments for 'PaintWindow' Event (ie. WM_PAINT)
  //! 
  //! \tparam ENC - Message character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using PaintWindowEventArgs = EventArgs<ENC,WindowMessage::PAINT>;
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \alias PaintWindowEventDelegate - Delegate for 'PaintWindow' event (ie. WM_PAINT)
  //! 
  //! \tparam ENC - Window character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using PaintWindowEventDelegate = EventArgsDelegate<ENC,WindowMessage::PAINT>;

  ///////////////////////////////////////////////////////////////////////////////
  //! \alias PaintWindowEventHandler - Handler for 'PaintWindow' event (ie. WM_PAINT)
  //! 
  //! \tparam ENC - Window character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using PaintWindowEventHandler = EventHandler<ENC,WindowMessage::PAINT,PaintWindowEventDelegate<ENC>>;

}

#endif // WTL_PAINT_WINDOW_EVENT_HPP