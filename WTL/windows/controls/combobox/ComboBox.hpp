//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\combobox\ComboBox.hpp
//! \brief Encapsulates standard combobox control
//! \date 23 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_COMBOBOX_HPP
#define WTL_COMBOBOX_HPP

#include <wtl/WTL.hpp>
#include <wtl/windows/Control.hpp>                                      //!< Control
#include <wtl/windows/controls/combobox/ComboBoxConstants.hpp>          //!< (Constants)
#include <wtl/windows/controls/combobox/ComboBoxItemsCollection.hpp>    //!< ComboBoxItemsCollection
#include <wtl/windows/controls/combobox/ComboBoxMinVisibleProperty.h>   //!< ComboBoxMinVisibleProperty

//! \namespace wtl - Windows template library
namespace wtl 
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct ComboBox - Encapsulates a standard ComboBox control
  //! 
  //! \tparam ENC - Character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct ComboBox : Control<ENC>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias type - Define own type
    using type = ComboBox<ENC>;
  
    //! \alias base - Define base type
    using base = Control<ENC>;
    
    //! \var encoding - Inherit character encoding
    static constexpr Encoding  encoding = base::encoding;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
    
    // Data
    ComboBoxItemsCollection<encoding>     Items;          //!< Items 

    // Properties
    ComboBoxMinVisibleProperty<encoding>  MinVisible;     //!< Minimum number of visible items

    // Events
    OwnerDrawCtrlEvent<encoding>          OwnerDraw;      //!< OwnerDraw 
    OwnerMeasureCtrlEvent<encoding>       OwnerMeasure;   //!< OwnerMeasure 
    
    // ------------------------------------ CONSTRUCTION ------------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ComboBox::ComboBox
    //! Creates the window object for a combobox control without creating the window handle
    //! 
    //! \param[in] id - Control identifier
    //! 
    //! \throw wtl::platform_error - Unrecognised system window class
    /////////////////////////////////////////////////////////////////////////////////////////
    ComboBox(WindowId id) : base(id), 
                            Items(*this),
                            MinVisible(*this)
    {
      static const WindowClass<encoding>  std(SystemClass::ComboBox);    //!< Lookup standard combobox window-class

      // Set properties
      this->Style = WindowStyle::ChildWindow | ComboBoxStyle::DropDownList|ComboBoxStyle::HasStrings;
      
      // Clear paint handlers (Handled by subclass)
      this->Paint.clear();

      // Compile-time subclass the standard combobox control
      this->SubClasses += std.WndProc;

      // Owner draw handlers
      OwnerDraw += new OwnerDrawCtrlEventHandler<encoding>(this, &ComboBox::onOwnerDraw);
      OwnerMeasure += new OwnerMeasureCtrlEventHandler<encoding>(this, &ComboBox::onOwnerMeasure);
    }

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
  public:
    DISABLE_COPY(ComboBox);     //!< Cannot be copied
    ENABLE_MOVE(ComboBox);      //!< Can be moved
    ENABLE_POLY(ComboBox);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // ComboBox::registerClass 
    //! Registers the window-class on the first call, retrieves the pre-registered class upon subsequent calls
    //! 
    //! \param[in] instance - [optional] Module for which to register the window class [Necessary on first call]
    //! \return const WindowClass<encoding>& - Registered window class 
    //! 
    //! \throw wtl::platform_error - Unable to register window class
    /////////////////////////////////////////////////////////////////////////////////////////
    static const WindowClass<encoding>&  registerClass(::HINSTANCE instance = nullptr)
    {
      static const String<encoding> name("WTL.ComboBox");
      
      // Define WTL combobox window-class
      static const WindowClass<encoding>  std(SystemClass::ComboBox);    //!< Lookup standard combobox windowclass
      static const WindowClass<encoding>  btn(instance,
                                              name.c_str(),
                                              std.Style,
                                              base::WndProc,            //!< Replace the window procedure 'Compile-time subclass'
                                              std.Menu,
                                              std.Cursor,
                                              std.Background,
                                              std.SmallIcon,
                                              std.LargeIcon,
                                              std.ClassStorage,
                                              std.WindowStorage);    

      // Return WTL combobox class
      return btn;
    }
        
    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // ComboBox::send
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
    // ComboBox::send
    //! Sends a combobox message to the window
    //! 
    //! \tparam CM - ComboBox Message 
    //!
    //! \param[in] w- [optional] First parameter
    //! \param[in] l - [optional] Second parameter
    //! \return LResult - Message result and routing
    /////////////////////////////////////////////////////////////////////////////////////////
    template <ComboBoxMessage CM> 
    LResult send(::WPARAM w = 0, ::LPARAM l = 0)
    {
      return send_message<encoding>(CM, this->Handle, w, l);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ComboBox::wndclass const
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
    // ComboBox::route
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
          case ButtonNotification::Click:   /* TODO ret = Click.raise(ButtonClickEventArgs<encoding>(w,l));  */            break;
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
    // ComboBox::onOwnerDraw
    //! Called in response to a reflected 'owner draw' message to draw the combobox
    //! 
    //! \param[in,out] &args - Message arguments 
    //! \return LResult - Routing indicating message was handled
    //!
    //! \throw wtl::platform_error - Unable to draw combobox
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual LResult  onOwnerDraw(OwnerDrawCtrlEventArgs<encoding>& args) 
    { 
      
      // Handle message
      return {MsgRoute::Handled, 0};
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // ComboBox::onOwnerMeasure
    //! Called in response to a reflected 'owner measure' message to 
    //! 
    //! \param[in,out] &args - Message arguments 
    //! \return LResult - Routing indicating message was handled
    //!
    //! \throw wtl::platform_error - Unable to measure combobox
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual LResult  onOwnerMeasure(OwnerMeasureCtrlEventArgs<encoding>& args) 
    { 
      
      // Handle message
      return {MsgRoute::Handled, 0};
    }
  };
  
} // namespace wtl


#include <wtl/windows/controls/combobox/ComboBoxMinVisibleProperty.hpp>   //!< ComboBoxMinVisibleProperty

#endif // WTL_COMBOBOX_HPP
