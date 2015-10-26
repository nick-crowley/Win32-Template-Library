//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\ExceptionLog.hpp
//! \brief Encapsulates logging different exception types in a consistent manner
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_EXCEPTION_LOG_HPP
#define WTL_EXCEPTION_LOG_HPP

#include "wtl/WTL.hpp"
#include "wtl/io/Console.hpp"         //!< Console

//! \namespace wtl - Windows template library
namespace wtl
{
  
  //////////////////////////////////////////////////////////////////////////////////////////
  //! \struct exception_log_t - Encapsulates exception meta-data for logging
  //////////////////////////////////////////////////////////////////////////////////////////
  struct exception_log_t
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    // ----------------------------------- REPRESENTATION -----------------------------------
    
    const char  *Source,        //!< Throw location
                *Sink,          //!< Catch location
                *Error,         //!< Error 
                *Context;       //!< Context

    // ------------------------------------ CONSTRUCTION ------------------------------------
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // exception_log_t::exception_log_t
    //! Create from no exception 
    //! 
    //! \param[in] const* sink - Catch block
    //! \param[in] const* message - [optional] Catch message
    //////////////////////////////////////////////////////////////////////////////////////////
    exception_log_t(const char* sink, const char* message = nullptr) : Source("Unspecified"), 
                                                                       Sink(sink), 
                                                                       Error("Unknown error"), 
                                                                       Context(message ? message : "An error has occurred")
    {}

    //////////////////////////////////////////////////////////////////////////////////////////
    // exception_log_t::exception_log_t
    //! Create from STL exception 
    //! 
    //! \param[in] const* sink - Catch block
    //! \param[in] const& e - Exception
    //! \param[in] const* message - [optional] Catch message
    //////////////////////////////////////////////////////////////////////////////////////////
    exception_log_t(const char* sink, const std::exception& e, const char* message = nullptr) : Source("Unspecified"), 
                                                                                                Sink(sink), 
                                                                                                Error(e.what()), 
                                                                                                Context(message ? message : "An error has occurred")
    {}
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // exception_log_t::exception_log_t
    //! Create from WTL exception 
    //! 
    //! \param[in] const* sink - Catch block
    //! \param[in] const& e - Exception
    //! \param[in] const* message - [optional] Catch message
    //////////////////////////////////////////////////////////////////////////////////////////
    exception_log_t(const char* sink, const exception& e, const char* message = nullptr) : Source(e.location()), 
                                                                                           Sink(sink), 
                                                                                           Error(e.what()),
                                                                                           Context(message ? message : "An error has occurred")
    {}

    // -------------------------------- COPY, MOVE & DESTROY  -------------------------------
    
    ENABLE_COPY(exception_log_t);     //!< Can be copied  (No move optimization)
    ENABLE_POLY(exception_log_t);     //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };

  
  //////////////////////////////////////////////////////////////////////////////////////////
  // std::exception_log
  //! Create an exception log object
  //!
  //! \param[in] const* sink - Catch block
  //! \param[in] const* message - [optional] Catch message console
  //! \return exception_log_t - Exception log
  //////////////////////////////////////////////////////////////////////////////////////////
  exception_log_t  exception_log(const char* sink, const char* message = nullptr);

  //////////////////////////////////////////////////////////////////////////////////////////
  // std::exception_log
  //! Create an exception log object
  //!
  //! \param[in] const* sink - Catch block
  //! \param[in] const& e - Exception
  //! \param[in] const* message - [optional] Catch message console
  //! \return exception_log_t - Exception log
  //////////////////////////////////////////////////////////////////////////////////////////
  exception_log_t  exception_log(const char* sink, const std::exception& e, const char* message = nullptr);


  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Writes an exception to the debug console
  //!
  //! \param[in,out] &c - Debug console
  //! \param[in] const& - Exception details
  //! \return Console& - Reference to 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  Console& operator << (Console& c, const exception_log_t& ex);
  
  

} // WTL namespace



#endif // WTL_EXCEPTION_LOG_HPP


