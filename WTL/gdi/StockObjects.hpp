//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\gdi\StockObjects.hpp
//! \brief Provides extended stock drawing objects
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_STOCK_OBJECTS_HPP
#define WTL_STOCK_OBJECTS_HPP

#include "wtl/WTL.hpp"
#include "wtl/utils/Handle.hpp"
#include "wtl/traits/BrushTraits.hpp"
#include "wtl/traits/FontTraits.hpp"
#include "wtl/traits/PenTraits.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct StockBrush - Provides stock brushes
  /////////////////////////////////////////////////////////////////////////////////////////
  struct StockBrush final
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    // Define solid brushes
    static const HBrush Black,       //!< Black brush
                        Blue,        //!< Blue brush
                        DarkBlue,    //!< DarkBlue brush
                        SkyBlue,     //!< SkyBlue brush
                        Cyan,        //!< Cyan brush
                        Teal,        //!< Teal brush
                        Lime,        //!< Lime brush
                        Green,       //!< Green brush
                        Leaves,      //!< Leaves brush
                        Forest,      //!< Forest brush
                        Yellow,      //!< Yellow brush
                        Gold,        //!< Gold brush
                        Orange,      //!< Orange brush
                        Honey,       //!< Honey brush
                        Brown,       //!< Brown pen
                        Red,         //!< Red brush
                        Rose,        //!< Rose brush
                        Pink,        //!< Pink brush
                        Purple,      //!< Purple brush
                        Magenta,     //!< Magenta brush
                        Beige,       //!< Beige brush
                        Wheat,       //!< Wheat brush
                        Snow,        //!< Snow brush
                        White,       //!< White brush
                        Null;        //!< Special: Null brush
	
    // ------------------------------------ CONSTRUCTION ------------------------------------
	
    DISABLE_CTOR(StockBrush);       //!< Cannot instantiate
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    DISABLE_COPY(StockBrush);       //!< Cannot instantiate
    DISABLE_MOVE(StockBrush);       //!< Cannot instantiate
    DISABLE_DTOR(StockBrush);       //!< Cannot instantiate
  };


  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct StockFont - Provides stock fonts
  /////////////////////////////////////////////////////////////////////////////////////////
  struct StockFont final
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    // Define common fonts
    static const HFont  System,        //!< System font
                        Tooltip,       //!< Tooltip font
                        Window;        //!< Window font

    // ------------------------------------ CONSTRUCTION ------------------------------------
	
    DISABLE_CTOR(StockFont);       //!< Cannot instantiate
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    DISABLE_COPY(StockFont);       //!< Cannot instantiate
    DISABLE_MOVE(StockFont);       //!< Cannot instantiate
    DISABLE_DTOR(StockFont);       //!< Cannot instantiate
  };

  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct StockPen - Provides stock pens
  /////////////////////////////////////////////////////////////////////////////////////////
  struct StockPen final
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    // Define solid pens
    static const HPen   Black,       //!< Black pen
                        Blue,        //!< Blue pen
                        DarkBlue,    //!< DarkBlue pen
                        SkyBlue,     //!< SkyBlue pen
                        Cyan,        //!< Cyan pen
                        Teal,        //!< Teal pen
                        Lime,        //!< Lime pen
                        Green,       //!< Green pen
                        Leaves,      //!< Leaves pen
                        Forest,      //!< Forest pen
                        Yellow,      //!< Yellow pen
                        Gold,        //!< Gold pen
                        Orange,      //!< Orange pen
                        Honey,       //!< Honey pen
                        Brown,       //!< Brown pen
                        Red,         //!< Red pen
                        Rose,        //!< Rose pen
                        Pink,        //!< Pink pen
                        Purple,      //!< Purple pen
                        Magenta,     //!< Magenta pen
                        Beige,       //!< Beige pen
                        Wheat,       //!< Wheat pen
                        Snow,        //!< Snow pen
                        White,       //!< White pen
                        Null;        //!< Special: Null pen

    // ------------------------------------ CONSTRUCTION ------------------------------------
	
    DISABLE_CTOR(StockPen);       //!< Cannot instantiate
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    DISABLE_COPY(StockPen);       //!< Cannot instantiate
    DISABLE_MOVE(StockPen);       //!< Cannot instantiate
    DISABLE_DTOR(StockPen);       //!< Cannot instantiate
  };

}

#endif // WTL_STOCK_OBJECTS_HPP
  