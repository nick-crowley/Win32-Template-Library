////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\event\OwnerMeasureEvent.hpp
//! \brief Encapsulates the WM_MEASUREITEM message in the 'OwnerMeasure' event
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_OWNER_MEASURE_EVENT_HPP
#define WTL_OWNER_MEASURE_EVENT_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct EventArgs<WindowMessage::MEASUREITEM> - Event arguments for Win32 message 'WM_MEASUREITEM'
  //! 
  //! \tparam ENC - Message character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct EventArgs<ENC,WindowMessage::MEASUREITEM>
  {  
    // ------------------- TYPES & CONSTANTS -------------------
  
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

    //! \union Identifier - Measure item identifier
    union Identifier
    {
      WindowId    Control;      //!< Control id
      CommandId   Action;       //!< Action Id
    };
    
    // -------------------- REPRESENTATION ---------------------
  protected:
    MeasureData&      Data;             //!< Encapsulates the input measure data (NB: Must be initialized before following fields)

  public:
    OwnerDrawControl  CtrlType;         //!< Control type
    DeviceContext     Graphics;         //!< Device context
    Identifier        Ident;            //!< Control/Menu identifier
    int32             Item;             //!< [CONTROL] Zero-based item index
    SizeL             Size;             //!< Item size
    
    // --------------------- CONSTRUCTION ----------------------

    ///////////////////////////////////////////////////////////////////////////////
    // EventArgs<MEASUREITEM>::EventArgs
    //! Creates arguments for 'OwnerMeasure' Event (ie. WM_MEASUREITEM)
    //! 
    //! \param[in] w - Not used
    //! \param[in] l - Not used
    ///////////////////////////////////////////////////////////////////////////////
    EventArgs(const HWnd& wnd, ::WPARAM w, ::LPARAM l) : Data(*opaque_cast<MeasureData>(l)), 
                                                         Graphics(HDeviceContext(wnd)),
                                                         Size(Data.itemWidth, Data.itemHeight),
                                                         Item(Data.itemID), 
                                                         CtrlType(enum_cast<OwnerDrawControl>(Data.CtlType))
    {
      // Extract identifier
      if (CtrlType == OwnerDrawControl::Menu)
        Ident.Action = static_cast<CommandId>(Data.itemID);
      else
        Ident.Control = static_cast<WindowId>(Data.CtlID);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // EventArgs::~EventArgs
    //! Save values
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~EventArgs()
    {
      Data.itemWidth = Size.width;
      Data.itemHeight = Size.height;
    }
    
    // ------------------------ STATIC -------------------------

    // ---------------------- ACCESSORS ------------------------			
    
    ///////////////////////////////////////////////////////////////////////////////
    // EventArgs<MEASUREITEM>::reflect const
    //! Reflects the event back to the originator control
    //! 
    //! \param[in] const& ctrl - Originator control
    //! \return LResult - Message result and routing
    ///////////////////////////////////////////////////////////////////////////////
    LResult reflect(const HWnd& ctrl) const
    {
      // Reflect message
      return send_message<encoding,message+WindowMessage::REFLECT>(ctrl, opaque_cast(Data.CtlID), opaque_cast(Data));
    }
    
    // ----------------------- MUTATORS ------------------------
  };
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \alias OwnerMeasureEvent - Defines 'OwnerMeasure' event (ie. WM_MEASUREITEM)
  //! 
  //! \tparam ENC - Window character encoding
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using OwnerMeasureEvent = MessageEvent<ENC, WindowMessage::MEASUREITEM>;
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \alias OwnerMeasureEventArgs - Arguments for 'OwnerMeasure' Event (ie. WM_MEASUREITEM)
  //! 
  //! \tparam ENC - Message character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using OwnerMeasureEventArgs = EventArgs<ENC,WindowMessage::MEASUREITEM>;

  ///////////////////////////////////////////////////////////////////////////////
  //! \alias OwnerMeasureEventHandler - Handler for 'OwnerMeasure' event (ie. WM_MEASUREITEM)
  //! 
  //! \tparam ENC - Window character encoding
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using OwnerMeasureEventHandler = typename OwnerMeasureEvent<ENC>::delegate_t;

}

#endif // WTL_OWNER_MEASURE_EVENT_HPP