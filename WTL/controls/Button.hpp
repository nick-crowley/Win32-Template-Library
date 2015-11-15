//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\controls\Button.hpp
//! \brief Encapsulates standard button controls
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_BUTTON_HPP
#define WTL_BUTTON_HPP

#include <wtl/WTL.hpp>
#include <wtl/windows/WindowBase.hpp>                     //!< WindowBase
#include <wtl/controls/events/ButtonEvents.hpp>           //!< ButtonClickEvent
#include <wtl/controls/properties/ButtonIconProperty.h>   //!< ButtonIconProperty
#include <wtl/controls/properties/ButtonStateProperty.h>  //!< ButtonStateProperty
#include <wtl/gdi/Theme.hpp>                              //!< Theme

//! \namespace wtl - Windows template library
namespace wtl 
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Button - Encapsulates a button control
  //! 
  //! \tparam ENC - Character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct Button : WindowBase<ENC>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias type - Define own type
    using type = Button<ENC>;
  
    //! \alias base - Define base type
    using base = WindowBase<ENC>;
    
    //! \alias wndclass_t - Inherit class type
    using wndclass_t = typename base::wndclass_t;
    
    //! \var encoding - Inherit character encoding
    static constexpr Encoding  encoding = base::encoding;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
    
    ButtonClickEvent<encoding>        Click;         //!< Button click
    //ButtonGainFocusEvent<encoding>    GainFocus;     //!< Button gained input focus
    //ButtonLoseFocusEvent<encoding>    LoseFocus;     //!< Button lost input focus
    OwnerDrawCtrlEvent<encoding>      OwnerDraw;     //!< Owner draw button
    OwnerMeasureCtrlEvent<encoding>   OwnerMeasure;  //!< Measure button for owner draw
    //CustomDrawEvent<encoding>         CustomDraw;    //!< Custom draw

    // Properties
    ButtonIconProperty<encoding>      Icon;          //!< Icon
    ButtonStateProperty<encoding>     State;         //!< State

    // ------------------------------------ CONSTRUCTION ------------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Button::Button
    //! Creates the window object for a standard button control (without creating the window handle)
    //! 
    //! \param[in] instance - Owning instance
    //! 
    //! \throw wtl::platform_error - Unrecognised system window class
    /////////////////////////////////////////////////////////////////////////////////////////
    Button(::HINSTANCE instance) : base(getClass(instance)),
                                   Icon(*this),
                                   State(*this)
    {
      // Set properties
      this->Style = WindowStyle::ChildWindow | ButtonStyle::Centre|ButtonStyle::Notify|ButtonStyle::OwnerDraw;

      // Clear paint handlers (Painting handled by system window class)
      this->Paint.clear();

      // Owner draw handler
      OwnerDraw += new OwnerDrawCtrlEventHandler<encoding>(this, &Button::onOwnerDraw);
      OwnerMeasure += new OwnerMeasureCtrlEventHandler<encoding>(this, &Button::onOwnerMeasure);

      // Focus
      //this->GainFocus += new GainFocusEventHandler<encoding>(this, &Button::onGainFocus);
      //this->LoseFocus += new LoseFocusEventHandler<encoding>(this, &Button::onLoseFocus);

      // Invalidate button on mouse enter/leave
      this->MouseEnter += new MouseEnterEventHandler<encoding>(this, &Button::onMouseEnter);
      this->MouseLeave += new MouseLeaveEventHandler<encoding>(this, &Button::onMouseLeave);

      // Subclass prior to creation
      using WindowType = typename base::WindowType;
      this->SubClasses.push_back(typename base::SubClass(WindowType::Native, getSystemWndProc()));
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Button::Button
    //! Creates the window object for a custom button control (without creating the window handle)
    //! 
    //! \param[in] &custom - Custom window class
    //! 
    //! \throw wtl::platform_error - Unrecognised system window class
    /////////////////////////////////////////////////////////////////////////////////////////
    Button(wndclass_t& custom) : base(custom)
    {
      // Set properties
      this->Style = WindowStyle::ChildWindow | ButtonStyle::Centre|ButtonStyle::Notify|ButtonStyle::OwnerDraw;

      // Remove paint handlers
      this->Paint.clear();
    }

    // -------------------------------- COPY, MOVE & DESTROY  -------------------------------
  public:
    DISABLE_COPY(Button);     //!< Cannot be copied
    ENABLE_MOVE(Button);      //!< Can be moved
    ENABLE_POLY(Button);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------
  
    /////////////////////////////////////////////////////////////////////////////////////////
    // Button::getClass 
    //! Get the window class for this button
    //! 
    //! \param[in] instance - Module handle
    //! \return wndclass_t& - Window class 
    /////////////////////////////////////////////////////////////////////////////////////////
    static wndclass_t& getClass(HINSTANCE instance) 
    {
      static wndclass_t  std(SystemClass::Button);  //!< Standard system button class
      
      static wndclass_t  btn(instance,
                             std.Name,
                             std.Style,
                             base::WndProc,   
                             std.Menu,
                             std.Cursor,
                             std.Background,
                             std.SmallIcon,
                             std.LargeIcon,
                             std.ClassStorage,
                             std.WindowStorage);    //!< Compile-time button subclass

      // Return custom button class
      return btn;
    }
    
  protected:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Button::getSystemWndProc 
    //! Get the window procedure for the standard button
    //! 
    //! \return ::WNDPROC - System window procedure
    /////////////////////////////////////////////////////////////////////////////////////////
    static ::WNDPROC getSystemWndProc() 
    {
      static wndclass_t  std(SystemClass::Button);  //!< Standard button system window class
      
      // Return window proc
      return std.WndProc;
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
      return send_message<encoding,window_msg(BM)>(this->Handle, w, l);
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
        LResult ret;       //!< Message result, defaults to unhandled

        // Examine message
        switch (message)
        {
        // [COMMAND (REFLECTED)] Raise associated event
        case WindowMessage::ReflectCommand:  
          // Extract notification
          switch (static_cast<ButtonNotification>(ControlEventArgs<encoding,WindowMessage::Command>(w,l).Message))
          {
          case ButtonNotification::Click:      ret = Click.raise(ButtonClickEventArgs<encoding>(w,l));            break;
          /*case ButtonNotification::SetFocus:   ret = GainFocus.raise(ButtonGainFocusEventArgs<encoding>(w,l));    break;
          case ButtonNotification::KillFocus:  ret = LoseFocus.raise(ButtonLoseFocusEventArgs<encoding>(w,l));    break;*/
          }
          break;

        // [OWNER-DRAW (REFLECTED)] Raise 'Owner Draw' or 'Owner Measure'
        case WindowMessage::ReflectDrawItem:     { OwnerDrawCtrlEventArgs<encoding> args(w,l);  ret = OwnerDraw.raise(args);                    }  break;
        case WindowMessage::ReflectMeasureItem:  { OwnerMeasureCtrlEventArgs<encoding> args(this->Handle,w,l);  ret = OwnerMeasure.raise(args); }  break;
        }

        // [UNHANDLED] Pass to default window procedure
        return base::route(message, w, l);
      }
      catch (std::exception& e)
      {
        cdebug << caught_exception("Unable to route message", HERE, e);
        
        // [ERROR] Unhandled
        return MsgRoute::Unhandled;
      }
    }
    
  private:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Button::onCreate
    //! Called during window creation to modify window parameters and create child windows
    //! 
    //! \param[in,out] &args - Message arguments 
    //! \return LResult - Message result and routing
    /////////////////////////////////////////////////////////////////////////////////////////
    //virtual LResult  onCreate(CreateWindowEventArgs<encoding>& args) 
    //{ 
    //  // Pass to base
    //  return base::onCreate(args); 
    //}
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Button::onGainFocus
    //! Called when gaining keyboard focus
    //! 
    //! \param[in] args - Message arguments 
    //! \return LResult - Routing indicating message was handled
    /////////////////////////////////////////////////////////////////////////////////////////
    LResult  onGainFocus(const GainFocusEventArgs<encoding>& args) 
    {
      cdebug << __func__ << endl;
      
      // Handle message
      return {MsgRoute::Handled, 0};
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Button::onLoseFocus
    //! Called when losing keyboard focus
    //! 
    //! \param[in] args - Message arguments 
    //! \return LResult - Routing indicating message was handled
    /////////////////////////////////////////////////////////////////////////////////////////
    LResult  onLoseFocus(const LoseFocusEventArgs<encoding>& args) 
    {
      cdebug << __func__ << endl;
      
      // Handle message
      return {MsgRoute::Handled, 0};
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Button::onMouseEnter
    //! Invalidate the button when the cursor enters the button
    //! 
    //! \param[in] args - Message arguments 
    //! \return LResult - Routing indicating message was handled
    /////////////////////////////////////////////////////////////////////////////////////////
    LResult  onMouseEnter(MouseEnterEventArgs<encoding> args) 
    {
      cdebug << __func__ << endl;
      
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
      cdebug << __func__ << endl;

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
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual LResult  onOwnerDraw(OwnerDrawCtrlEventArgs<encoding>& args) 
    { 
      // debug
      cdebug << object_info(__func__, "Ident", args.Ident, "Action",args.Action, "State",args.State) << endl;

      try
      {        
        Theme theme(this->handle(), L"Button");
        RectL rc = args.Rect;

        // Determine state
        PUSHBUTTONSTATES state = (!this->Enabled                          ? PBS_DISABLED 
                                 : args.State && OwnerDrawState::Selected ? PBS_PRESSED 
                                 : this->isMouseOver()                    ? PBS_HOT : PBS_NORMAL);
        
        // Draw background
        theme.drawBackground(args.Graphics, BP_PUSHBUTTON, state, args.Rect);

        // Pressed: Offset drawing rect
        if (state == PBS_PRESSED)
          rc += PointL(1,1);

        // Draw icon
        if (Icon.exists()) 
        {
          args.Graphics.draw(Icon, rc.topLeft(), SizeL(32,32));
          rc.Left += 32;
        }

        // Draw text
        DrawTextFlags flags = DrawTextFlags::SingleLine|DrawTextFlags::VCentre|DrawTextFlags::Centre;
        theme.write(args.Graphics, BP_PUSHBUTTON, state, this->Text(), rc, flags);
      }
      catch (const std::exception&)
      {
      }

      // Draw button background
      //args.Graphics.fill(args.Rect, StockBrush::BtnFace);

      // Draw button text
      //args.Graphics.setTextColour(SystemColour::BtnText);
      //args.Graphics.template write<encoding>(this->Text(), args.Rect, DrawTextFlags::Centre|DrawTextFlags::VCentre);

      // Handle message
      return {MsgRoute::Handled, 0};
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Button::onOwnerMeasure
    //! Called in response to a reflected 'owner measure' message to 
    //! 
    //! \param[in,out] &args - Message arguments 
    //! \return LResult - Routing indicating message was handled
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual LResult  onOwnerMeasure(OwnerMeasureCtrlEventArgs<encoding>& args) 
    { 
      // Measure button text
      args.Size = args.Graphics.measure(this->Text());

      // Handle message
      return {MsgRoute::Handled, 0};
    }
  };
} // namespace wtl

#include <wtl/controls/properties/ButtonIconProperty.hpp>      //!< IconProperty
#include <wtl/controls/properties/ButtonStateProperty.hpp>     //!< StateProperty

#endif // WTL_BUTTON_HPP
