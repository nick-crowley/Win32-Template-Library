//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\Button.hpp
//! \brief Encapsulates button controls
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_BUTTON_HPP
#define WTL_BUTTON_HPP

#include "wtl/WTL.hpp"
#include "wtl/windows/WindowBase.hpp"                 //!< WindowBase
#include "wtl/windows/events/StandardControls.hpp"    //!< ButtonClickEvent

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
    
    //! \var encoding - Inherit character encoding
    static constexpr Encoding  encoding = base::encoding;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
    
    ButtonClickEvent<encoding>        Click;         //!< Button click
    ButtonGainFocusEvent<encoding>    GainFocus;     //!< Button gained input focus
    ButtonLoseFocusEvent<encoding>    LoseFocus;     //!< Button lost input focus
    OwnerDrawCtrlEvent<encoding>      OwnerDraw;     //!< Owner draw button
    OwnerMeasureCtrlEvent<encoding>   OwnerMeasure;  //!< Measure button for owner draw
    //CustomDrawEvent<encoding>         CustomDraw;    //!< Custom draw

    // ------------------------------------ CONSTRUCTION ------------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Button::Button
    //! Creates a standard button control
    //! 
    //! \param[in] instance - Owning instance
    //! 
    //! \throw wtl::platform_error - Unrecognised system window class
    /////////////////////////////////////////////////////////////////////////////////////////
    Button(::HINSTANCE instance) : base(getClass(instance))
    {
      // Set properties
      this->Style = WindowStyle::ChildWindow | ButtonStyle::Centre|ButtonStyle::Notify|ButtonStyle::OwnerDraw;

      // Clear paint handlers (Painting handled by system window class)
      this->Paint.clear();

      // Owner draw handler
      OwnerDraw += new OwnerDrawCtrlEventHandler<encoding>(this, &Button::onOwnerDraw);
      OwnerMeasure += new OwnerMeasureCtrlEventHandler<encoding>(this, &Button::onOwnerMeasure);

      // Subclass prior to creation
      SubClasses.push_back(SubClass(WindowType::Native, getSystemWndProc()));
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Button::Button
    //! Creates a custom button control
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

    /////////////////////////////////////////////////////////////////////////////////////////
    // Button::~Button
    //! Can be polymorphic
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual ~Button()
    {}

    // ----------------------------------- STATIC METHODS -----------------------------------
  
    /////////////////////////////////////////////////////////////////////////////////////////
    // Button::getClass 
    //! Get the window class
    //! 
    //! \param[in] instance - Module handle
    //! \return wndclass_t& - Window class 
    /////////////////////////////////////////////////////////////////////////////////////////
    static wndclass_t& getClass(HINSTANCE instance) 
    {
      static WindowClass<encoding>  std(SystemClass::Button);  //!< Standard system button class
      
      static WindowClass<encoding>  btn(instance,
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
    //! Get the standard button window procedure
    //! 
    //! \return ::WNDPROC - System window procedure
    /////////////////////////////////////////////////////////////////////////////////////////
    static ::WNDPROC getSystemWndProc() 
    {
      static WindowClass<encoding>  std(SystemClass::Button);  //!< Standard button system window class
      
      // Return window proc
      return std.WndProc;
    }
    
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

        // 
        switch (message)
        {
        // [COMMAND (REFLECTED)] Raise associated event
        case WindowMessage::REFLECT_COMMAND:  
          // Extract notification
          switch (static_cast<ButtonNotification>(CtrlCommandEventArgs<encoding>(w,l).Message))
          {
          case ButtonNotification::Click:      ret = Click.raise(ButtonClickEventArgs<encoding>(w,l));            break;
          case ButtonNotification::SetFocus:   ret = GainFocus.raise(ButtonGainFocusEventArgs<encoding>(w,l));    break;
          case ButtonNotification::KillFocus:  ret = LoseFocus.raise(ButtonLoseFocusEventArgs<encoding>(w,l));    break;
          }
          break;

        // [OWNER-DRAW (REFLECTED)] Raise 'Owner Draw' or 'Owner Measure'
        case WindowMessage::REFLECT_DRAWITEM:     ret = OwnerDraw.raise(OwnerDrawCtrlEventArgs<encoding>(w,l));                      break;
        case WindowMessage::REFLECT_MEASUREITEM:  ret = OwnerMeasure.raise(OwnerMeasureCtrlEventArgs<encoding>(this->Handle,w,l));   break;
        }

        // [UNHANDLED] Return result & routing
        return base::route(message, w, l);
      }
      catch (exception& e)
      {
        cdebug << exception_log(HERE,e,"Unable to route message") << endl;
        
        // [ERROR] Unhandled
        return MsgRoute::Unhandled;
      }
    }
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Button::onOwnerDraw
    //! Called in response to a reflected 'owner draw' message to draw the button
    //! 
    //! \param[in,out] &args - Message arguments 
    //! \return LResult - Message result and routing
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual LResult  onOwnerDraw(OwnerDrawCtrlEventArgs<encoding>& args) 
    { 
      HBrush background(SystemColour::BtnFace);

      // Draw button background
      args.Graphics.fill(args.Rect, background);

      // Draw button text
      args.Graphics.setTextColour(Colour::Black);
      args.Graphics.write<encoding>(this->Text().c_str(), this->TextLength, args.Rect, DrawTextFlags::Centre|DrawTextFlags::VCentre);

      // Handled
      return 0;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Button::onOwnerMeasure
    //! Called in response to a reflected 'owner measure' message to 
    //! 
    //! \param[in,out] &args - Message arguments 
    //! \return LResult - Message result and routing
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual LResult  onOwnerMeasure(OwnerMeasureCtrlEventArgs<encoding>& args) 
    { 
      // Measure button text
      args.Size = args.Graphics.measure(this->Text().c_str());

      // Handled
      return 0;
    }
  };


} // namespace wtl

#endif // WTL_BUTTON_HPP
