//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\Static.hpp
//! \brief Encapsulates standard static controls
//! \date 6 December 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_STATIC_HPP
#define WTL_STATIC_HPP

#include <wtl/WTL.hpp>
#include <wtl/windows/Control.hpp>                                    //!< Control
#include <wtl/gdi/Theme.hpp>                                          //!< Theme

//! \namespace wtl - Windows template library
namespace wtl 
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Static - Encapsulates the standard static control
  //! 
  //! \tparam ENC - Character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct Static : Control<ENC>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias type - Define own type
    using type = Static<ENC>;
  
    //! \alias base - Define base type
    using base = Control<ENC>;
    
    //! \alias class_t - Inherit class type
    using class_t = typename base::class_t;
    
    //! \var encoding - Inherit character encoding
    static constexpr Encoding  encoding = base::encoding;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
    
    // Events (TODO)
    

    // Properties (TODO)

    // ------------------------------------ CONSTRUCTION ------------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Static::Static
    //! Creates the window object for a static control without creating the window handle
    //! 
    //! \param[in] id - Control identifier
    //! 
    //! \throw wtl::platform_error - Unrecognised system window class
    /////////////////////////////////////////////////////////////////////////////////////////
    Static(WindowId id) : base(id)
    {
      // Set properties
      this->Style = WindowStyle::ChildWindow | StaticStyle::Left;
      
      // Clear paint handlers (Handled by subclass)
      this->Paint.clear();

      // Compile-time subclass the standard static control
      this->SubClasses.push(getNativeSubClass());
    }

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
  public:
    DISABLE_COPY(Static);     //!< Cannot be copied
    ENABLE_MOVE(Static);      //!< Can be moved
    ENABLE_POLY(Static);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Static::registerClass 
    //! Registers the window-class 
    //! 
    //! \param[in] instance - Handle to registering module  [Used only during initial call]
    //! \return const class_t& - Window class 
    //! 
    //! \throw wtl::platform_error - Unable to register window class
    /////////////////////////////////////////////////////////////////////////////////////////
    static const WindowClass<encoding>&  registerClass(::HINSTANCE instance) 
    {
      static String<encoding> name("WTL.Static");
      
      // Define WTL static window-class
      static WindowClass<encoding>  std(SystemClass::Static);    //!< Lookup standard static window-class
      static WindowClass<encoding>  ctrl(instance,
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

      // Return WTL static class
      return ctrl;
    }
    
  protected:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Static::getNativeSubClass 
    //! Get the window procedure for the standard static
    //! 
    //! \return SubClass - SubClass representing the window procedure of the standard static
    /////////////////////////////////////////////////////////////////////////////////////////
    static SubClass getNativeSubClass() 
    {
      static WindowClass<encoding>  std(SystemClass::Static);    //!< Lookup standard static window-class
      
      // Return native window proc
      return { SubClass::WindowType::Native, std.WndProc };
    }
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Static::send
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
    // Static::send
    //! Sends a static message to the window
    //! 
    //! \tparam EM - Static Message 
    //!
    //! \param[in] w- [optional] First parameter
    //! \param[in] l - [optional] Second parameter
    //! \return LResult - Message result and routing
    /////////////////////////////////////////////////////////////////////////////////////////
    /*template <StaticMessage EM> 
    LResult send(::WPARAM w = 0, ::LPARAM l = 0)
    {
      return send_message<encoding>(EM, this->Handle, w, l);
    }*/
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Static::wndclass const
    //! Get the window class
    //! 
    //! \return const class_t& - Shared window class
    /////////////////////////////////////////////////////////////////////////////////////////
    const WindowClass<encoding>& wndclass() const override
    {
      return registerClass(nullptr);
    }
    
  protected:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Static::route
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
          switch (static_cast<StaticNotification>(ControlEventArgs<encoding,WindowMessage::Command>(w,l).Message))
          {
          case StaticNotification::Clicked:      /* TODO: Raise notification */           break;
          case StaticNotification::DblClick:      /* TODO: Raise notification */          break;
          case StaticNotification::Enable:     /* TODO: Raise notification */             break;
          case StaticNotification::Disable:     /* TODO: Raise notification */            break;
          }
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
  };
} // namespace wtl


#endif // WTL_STATIC_HPP