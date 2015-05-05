//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\event\OwnerMeasureEvent.hpp
//! \brief Encapsulates the WM_MEASUREITEM message in the 'OwnerMeasure' event
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_OWNER_MEASURE_EVENT_HPP
#define WTL_OWNER_MEASURE_EVENT_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct ControlEventArgs<WindowMessage::MEASUREITEM> - Event arguments for Win32 message 'WM_MEASUREITEM' raised by controls
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct ControlEventArgs<ENC,WindowMessage::MEASUREITEM,uint16,default<uint16>()>
  {  
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;

    //! \alias resource_t - Define resource id type
    using resource_t = ResourceId<ENC>;

    //! \alias MeasureData - Define measure data type
    using MeasureData = ::MEASUREITEMSTRUCT;
    
    //! \var encoding - Define character encoding
    static constexpr Encoding encoding = ENC;
    
    //! \var message - Define message identifier
    static constexpr WindowMessage message = WindowMessage::MEASUREITEM;

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    MeasureData&      Data;             //!< Encapsulates the input measure data (NB: Must be initialized before following fields)

  public:
    OwnerDrawControl  CtrlType;         //!< Control type
    WindowId          Ident;            //!< Originator identifier
    int32             Item;             //!< Zero-based item index
    DeviceContext     Graphics;         //!< Device context
    HWnd              Sender;           //!< Originator handle
    SizeL             Size;             //!< Item size
    
    // ------------------------------------ CONSTRUCTION ------------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // ControlEventArgs<MEASUREITEM>::ControlEventArgs
    //! Creates arguments for 'OwnerMeasure' Event (ie. WM_MEASUREITEM)
    //! 
    //! \param[in] const& wnd - Originator window handle
    //! \param[in] w - Originator window id
    //! \param[in] l - Measure data
    //! 
    //! \throw wtl::invalid_argument - [Debug only] Event is not for a control
    /////////////////////////////////////////////////////////////////////////////////////////
    ControlEventArgs(const HWnd& wnd, ::WPARAM w, ::LPARAM l) : Data(*opaque_cast<MeasureData>(l)), 
                                                                CtrlType(enum_cast<OwnerDrawControl>(Data.CtlType)),
                                                                Ident(static_cast<WindowId>(Data.CtlID)),
                                                                Item(Data.itemID), 
                                                                Graphics(HDeviceContext(wnd)),
                                                                Sender(wnd),
                                                                Size(Data.itemWidth, Data.itemHeight) 
    {
      PARAM_INVARIANT(Data,enum_cast<OwnerDrawControl>(Data.CtlType) != OwnerDrawControl::Menu);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // ControlEventArgs::~ControlEventArgs
    //! Save values
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual ~ControlEventArgs()
    {
      Data.itemWidth = Size.width;
      Data.itemHeight = Size.height;
    }
    
    // --------------------------------------- STATIC ---------------------------------------

    // -------------------------------------- ACCESSORS --------------------------------------			
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ControlEventArgs<MEASUREITEM>::reflect const
    //! Reflects the event back to the originator control
    //! 
    //! \return LResult - Message result and routing
    /////////////////////////////////////////////////////////////////////////////////////////
    LResult reflect() const
    {
      // Reflect message
      return send_message<encoding,message+WindowMessage::REFLECT>(Sender, opaque_cast(Data.CtlID), opaque_cast(Data));
    }
    
    // --------------------------------------- MUTATORS --------------------------------------
  };
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias OwnerMeasureCtrlEvent - Defines 'OwnerMeasure' event from controls (ie. WM_MEASUREITEM)
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using OwnerMeasureCtrlEvent = ControlEvent<ENC, WindowMessage::MEASUREITEM>;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias OwnerMeasureCtrlEventArgs - Arguments for 'OwnerMeasure' Event from controls (ie. WM_MEASUREITEM)
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using OwnerMeasureCtrlEventArgs = ControlEventArgs<ENC,WindowMessage::MEASUREITEM>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias OwnerMeasureCtrlEventHandler - Handler for 'OwnerMeasure' event from controls (ie. WM_MEASUREITEM)
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using OwnerMeasureCtrlEventHandler = ControlEventHandler<ENC,WindowMessage::MEASUREITEM>;



  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct EventArgs<WindowMessage::MEASUREITEM> - Event arguments for Win32 message 'WM_MEASUREITEM'
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct EventArgs<ENC,WindowMessage::MEASUREITEM>
  {  
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;

    //! \alias resource_t - Define resource id type
    using resource_t = ResourceId<ENC>;

    //! \alias MeasureData - Define measure data type
    using MeasureData = ::MEASUREITEMSTRUCT;
    
    //! \var encoding - Define character encoding
    static constexpr Encoding encoding = ENC;
    
    //! \var message - Define message identifier
    static constexpr WindowMessage message = WindowMessage::MEASUREITEM;

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    MeasureData&      Data;             //!< Encapsulates the input measure data (NB: Must be initialized before following fields)

  public:
    DeviceContext     Graphics;         //!< Device context
    CommandId         Ident;            //!< Action identifier
    SizeL             Size;             //!< Item size
    
    // ------------------------------------ CONSTRUCTION ------------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // EventArgs<MEASUREITEM>::EventArgs
    //! Creates arguments for 'OwnerMeasure' Event (ie. WM_MEASUREITEM)
    //! 
    //! \param[in] const& wnd - Menu owner window
    //! \param[in] w - Originator window id
    //! \param[in] l - Measure data
    //! 
    //! \throw wtl::invalid_argument - [Debug only] Event is not for a control
    /////////////////////////////////////////////////////////////////////////////////////////
    EventArgs(const HWnd& wnd, ::WPARAM w, ::LPARAM l) : Data(*opaque_cast<MeasureData>(l)), 
                                                         Graphics(HDeviceContext(wnd)),
                                                         Ident(static_cast<CommandId>(Data.itemID)),
                                                         Size(Data.itemWidth, Data.itemHeight)
    {
      PARAM_INVARIANT(Data,enum_cast<OwnerDrawControl>(Data.CtlType) == OwnerDrawControl::Menu);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // EventArgs::~EventArgs
    //! Save values
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual ~EventArgs()
    {
      Data.itemWidth = Size.width;
      Data.itemHeight = Size.height;
    }
    
    // --------------------------------------- STATIC ---------------------------------------

    // -------------------------------------- ACCESSORS --------------------------------------			
    
    // --------------------------------------- MUTATORS --------------------------------------
  };
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias OwnerMeasureMenuEvent - Defines 'OwnerMeasure' event (ie. WM_MEASUREITEM)
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using OwnerMeasureMenuEvent = MessageEvent<ENC, WindowMessage::MEASUREITEM>;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias OwnerMeasureMenuEventArgs - Arguments for 'OwnerMeasure' Event (ie. WM_MEASUREITEM)
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using OwnerMeasureMenuEventArgs = EventArgs<ENC,WindowMessage::MEASUREITEM>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias OwnerMeasureMenuEventHandler - Handler for 'OwnerMeasure' event (ie. WM_MEASUREITEM)
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using OwnerMeasureMenuEventHandler = MessageEventHandler<ENC,WindowMessage::MEASUREITEM>;

}

#endif // WTL_OWNER_MEASURE_EVENT_HPP
