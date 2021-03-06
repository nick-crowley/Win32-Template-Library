//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\button\Button.hpp
//! \brief Encapsulates the standard button control
//! \date 1 December 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_BUTTON_HPP
#define WTL_BUTTON_HPP

#include <wtl/WTL.hpp>
#include <wtl/windows/Control.hpp>                                //!< Control
#include <wtl/windows/controls/button/ButtonConstants.hpp>        //!< (Constants)
#include <wtl/windows/controls/button/ButtonClickEvent.hpp>       //!< ButtonClickEvent
#include <wtl/windows/controls/button/ButtonIconProperty.h>       //!< ButtonIconProperty
#include <wtl/windows/controls/button/ButtonStateProperty.h>      //!< ButtonStateProperty
#include <wtl/gdi/Theme.hpp>                                      //!< Theme

//! \namespace wtl - Windows template library
namespace wtl 
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Button - Encapsulates a standard button control and acts as the basis of all custom button types
  //! 
  //! \tparam ENC - Character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct Button : Control<ENC>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias type - Define own type
    using type = Button<ENC>;
  
    //! \alias base - Define base type
    using base = Control<ENC>;
    
    //! \var encoding - Inherit character encoding
    static constexpr Encoding  encoding = base::encoding;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
    
    // Events
    ButtonClickEvent<encoding>        Click;         //!< Button click
    //CustomDrawEvent<encoding>       CustomDraw;    //!< Custom draw
    OwnerDrawCtrlEvent<encoding>      OwnerDraw;     //!< Owner draw button
    OwnerMeasureCtrlEvent<encoding>   OwnerMeasure;  //!< Measure button for owner draw

    // Properties
    ButtonIconProperty<encoding>      Icon;          //!< Icon
    ButtonStateProperty<encoding>     State;         //!< State

    // ------------------------------------ CONSTRUCTION ------------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Button::Button
    //! Creates the window object for a button control without creating the window handle
    //! 
    //! \param[in] id - Control identifier
    //! 
    //! \throw wtl::platform_error - Unrecognised system window class
    /////////////////////////////////////////////////////////////////////////////////////////
    Button(WindowId id) : base(id),
                          Icon(*this),
                          State(*this)
    {
      static const WindowClass<encoding>  std(SystemClass::Button);    //!< Lookup standard button window-class

      // Set properties
      this->Style = WindowStyle::ChildWindow|WindowStyle::TabStop | ButtonStyle::PushButton|ButtonStyle::Centre|ButtonStyle::Notify|ButtonStyle::OwnerDraw;
      
      // Clear paint handlers (Handled by subclass)
      this->Paint.clear();

      // Compile-time subclass the standard button control
      this->SubClasses += std.WndProc;

      // Owner draw handlers
      OwnerDraw += new OwnerDrawCtrlEventHandler<encoding>(this, &Button::onOwnerDraw);
      OwnerMeasure += new OwnerMeasureCtrlEventHandler<encoding>(this, &Button::onOwnerMeasure);

      // Mouse handlers (Handles 'hot' notification)
      this->MouseEnter += new MouseEnterEventHandler<encoding>(this, &Button::onMouseEnter);
      this->MouseLeave += new MouseLeaveEventHandler<encoding>(this, &Button::onMouseLeave);
    }

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
  public:
    DISABLE_COPY(Button);     //!< Cannot be copied
    ENABLE_MOVE(Button);      //!< Can be moved
    ENABLE_POLY(Button);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Button::registerClass 
    //! Registers the window-class on the first call, retrieves the pre-registered class upon subsequent calls
    //! 
    //! \param[in] instance - [optional] Module for which to register the window class [Necessary on first call]
    //! \return const WindowClass<encoding>& - Registered window class 
    //! 
    //! \throw wtl::platform_error - Unable to register window class
    /////////////////////////////////////////////////////////////////////////////////////////
    static const WindowClass<encoding>&  registerClass(::HINSTANCE instance = nullptr) 
    {
      static const String<encoding> name("WTL.Button");
      
      // Define WTL button window-class
      static const WindowClass<encoding>  std(SystemClass::Button);    //!< Lookup standard button windowclass
      static const WindowClass<encoding>  btn(instance,
                                              name.c_str(),
                                              std.Style,
                                              base::WndProc,           //!< Replace the window procedure 'Compile-time subclass'
                                              std.Menu,
                                              std.Cursor,
                                              std.Background,
                                              std.SmallIcon,
                                              std.LargeIcon,
                                              std.ClassStorage,
                                              std.WindowStorage);    

      // Return WTL button class
      return btn;
    }
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Button::send
    //! Sends a message to the window
    //! 
    //! \tparam WM - Window Message 
    //!
    //! \param[in] w- [optional] First parameter
    //! \param[in] l - [optional] Second parameter
    //! \return LResult - Message result and routing
    /////////////////////////////////////////////////////////////////////////////////////////
    using base::send;

    /////////////////////////////////////////////////////////////////////////////////////////
    // Button::send
    //! Sends a button message to the window
    //! 
    //! \tparam BM - Button Message 
    //!
    //! \param[in] w- [optional] First parameter
    //! \param[in] l - [optional] Second parameter
    //! \return LResult - Message result and routing
    /////////////////////////////////////////////////////////////////////////////////////////
    template <ButtonMessage BM> 
    LResult send(::WPARAM w = 0, ::LPARAM l = 0)
    {
      return send_message<encoding>(BM, this->Handle, w, l);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Button::wndclass const
    //! Get the window class
    //! 
    //! \return const WindowClass<encoding>& - Shared window class
    /////////////////////////////////////////////////////////////////////////////////////////
    const WindowClass<encoding>& wndclass() const override
    {
      return registerClass();
    }
    
  protected:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Button::route
    //! Routes messages to an instance's handlers (This is the 'Instance window procedure')
    //!
    //! \param[in] message - Window message identifier
    //! \param[in] w - [optional] First message parameter
    //! \param[in] l - [optional] Second message parameter
    //! \return LResult - Message routing and result 
    /////////////////////////////////////////////////////////////////////////////////////////
    LResult route(WindowMessage message, WPARAM w, LPARAM l) override
    {
      try
      {
        LResult  ret;       //!< Message result, defaults to unhandled
        uint16_t code;      //!< Notification code

        // Examine message
        switch (message)
        {
        // [COMMAND (REFLECTED)] Raise associated event
        case WindowMessage::ReflectCommand:  
          // Extract notification
          code = ControlEventArgs<encoding,WindowMessage::Command>(w,l).Message;
          switch (static_cast<ButtonNotification>(code))
          {
          case ButtonNotification::Click:       ret = Click.raise(ButtonClickEventArgs<encoding>(w,l));   break;
          case ButtonNotification::DoubleClick:       /* TODO: Raise notification */                      break;
          case ButtonNotification::HotItemChange:     /* TODO: Raise notification */                      break;
          }
          break;

        // [OWNER-DRAW (REFLECTED)] Raise 'Owner Draw' 
        case WindowMessage::ReflectDrawItem: 
          if (!OwnerDraw.empty())
            ret = OwnerDraw.raise( OwnerDrawCtrlEventArgs<encoding>(w,l) ); 
          break;

        // [OWNER-DRAW (REFLECTED)] Raise 'Owner Measure'
        case WindowMessage::ReflectMeasureItem:  
          if (!OwnerMeasure.empty())
            ret = OwnerMeasure.raise( OwnerMeasureCtrlEventArgs<encoding>(this->Handle,w,l) ); 
          break;
        }

        // [UNHANDLED] Pass to default window procedure
        return base::route(message, w, l);
      }
      catch (std::exception& e)
      {
        cdebug << caught_exception("Unable to route message", HERE, e);
        
        // [ERROR] Unhandled
        return {MsgRoute::Unhandled, -1};
      }
    }
    
  private:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Button::onMouseEnter
    //! Invalidate the button when the cursor enters the button
    //! 
    //! \param[in] args - Message arguments 
    //! \return LResult - Routing indicating message was handled
    /////////////////////////////////////////////////////////////////////////////////////////
    LResult  onMouseEnter(MouseEnterEventArgs<encoding> args) 
    {
      //cdebug << __func__ << endl;
      
      // Redraw
      this->invalidate();

      // Handle message
      return {MsgRoute::Handled, 0};
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Button::onMouseLeave
    //! Invalidate the button when the cursor enters the button
    //! 
    //! \param[in] args - Message arguments 
    //! \return LResult - Routing indicating message was handled
    /////////////////////////////////////////////////////////////////////////////////////////
    LResult  onMouseLeave(MouseLeaveEventArgs<encoding> args) 
    {
      //cdebug << __func__ << endl;

      // Redraw
      this->invalidate();
      
      // Handle message
      return {MsgRoute::Handled, 0};
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Button::onOwnerDraw
    //! Called in response to a reflected 'owner draw' message to draw the button
    //! 
    //! \param[in,out] &args - Message arguments 
    //! \return LResult - Routing indicating message was handled
    //!
    //! \throw wtl::platform_error - Unable to draw button
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual LResult  onOwnerDraw(OwnerDrawCtrlEventArgs<encoding>& args) 
    { 
      // debug
      //cdebug << object_info(__func__, "Ident", args.Ident, 
      //                                "Action",args.Action, 
      //                                "State",args.State) << std::endl;

      // Draw control using current window skin
      SkinFactory<encoding>::get()->draw(*this, args.Graphics, args.Rect);

      // Handle message
      return {MsgRoute::Handled, 0};
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Button::onOwnerMeasure
    //! Called in response to a reflected 'owner measure' message to 
    //! 
    //! \param[in,out] &args - Message arguments 
    //! \return LResult - Routing indicating message was handled
    //!
    //! \throw wtl::platform_error - Unable to measure button
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual LResult  onOwnerMeasure(OwnerMeasureCtrlEventArgs<encoding>& args) 
    { 
      // Measure control using current window skin
      args.Size = SkinFactory<encoding>::get()->measure(*this, args.Graphics);

      // Handle message
      return {MsgRoute::Handled, 0};
    }
  };


} // namespace wtl

#include <wtl/windows/controls/button/ButtonIconProperty.hpp>      //!< IconProperty
#include <wtl/windows/controls/button/ButtonStateProperty.hpp>     //!< StateProperty

#endif // WTL_BUTTON_HPP
