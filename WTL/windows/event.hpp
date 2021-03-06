//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\Event.hpp
//! \brief Provides an observeable event
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_EVENT_HPP
#define WTL_EVENT_HPP

#include <wtl/WTL.hpp>
#include <wtl/casts/OpaqueCast.hpp>           //!< OpaqueCast
#include <wtl/windows/Delegate.hpp>           //!< Delegate
#include <tuple>                              //!< std::tuple
#include <utility>                            //!< std::tuple_element
#include <memory>                             //!< std::shared_ptr
#include <list>                               //!< std::list

//! \namespace wtl - Windows template library
namespace wtl 
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Event - Provides an observeable event pattern with multiple subscribers
  //! 
  //! \tparam RET - [optional] Handler function return type (If unspecified, no return)
  //! \tparam ARGS... - [optional] Handler function signature (If unspecified, no arguments)
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename RET = void, typename... ARGS>
  struct Event 
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    /////////////////////////////////////////////////////////////////////////////////////////
    //! \alias argument_t - Delegate argument type accessor
    //!
    //! \tparam IDX - Zero-based argument index
    /////////////////////////////////////////////////////////////////////////////////////////
    template <unsigned IDX>
    using argument_t = typename std::tuple_element<IDX, std::tuple<ARGS...>>::type;
  
    //! \alias delegate_t - Define delegate type
    using delegate_t = Delegate<RET,ARGS...>;
    
    //! \alias result_t - Define delegate return type
    using result_t = RET;

    //! \alias signature_t - Define delegate signature
    using signature_t = result_t (ARGS...);
  
    //! \var arguments - Number of arguments
    static constexpr uint32_t  arguments = sizeof...(ARGS);

  protected:
    //! \alias storage_t - Define delegate storage type
    using storage_t = std::shared_ptr<delegate_t>;

    //! \alias collection_t - Define delegate collection type
    using collection_t = std::list<storage_t>;

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    collection_t  Subscribers;          //!< Subscribers collection (delegates to handler functions)

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Event::Event
    //! Create event with no subscribers
    /////////////////////////////////////////////////////////////////////////////////////////
    Event() = default;
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    ENABLE_COPY(Event);      //!< Can be deep copied
		ENABLE_MOVE(Event);      //!< Can be moved
    ENABLE_POLY(Event);      //!< Can be polymorphic
    
    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Event::empty() const
    //! Query whether event has any subscribers
    //!
    //! \return bool - True iff event has no subscribers
    //////////////////////////////////////////////////////////////////////////////////////////
    bool empty() const
    {
      return Subscribers.empty();
    }

  private:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Event::invoke()
    //! Raises the event, notifying each subscriber in the order in which they subscribed
    //! 
    //! \tparam CALL_ARGS... - Argument types provided when raised
    //!
    //! \param[in] &&... args - [optional] Arguments for event handler
    //! \return result_t - Result of call to final subscriber. If no subscribers then a default constructed 'result_t'
    //! 
    //! \remarks This overload is selected if the event handler has a return type other than void
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename = enable_if_not_same_t<RET,void>, typename... CALL_ARGS>
    result_t invoke(CALL_ARGS&&... args) const
    {
      result_t r(defvalue<result_t>());

      // Forward arguments to each subscriber
      for (auto& fn : Subscribers)
        r = (*fn)(std::forward<CALL_ARGS>(args)...);

      // Return result
      return r;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Event::invoke() const
    //! Raises the event, notifying each subscriber in the order in which they subscribed
    //! 
    //! \tparam CALL_ARGS... - Argument types provided when raised
    //!
    //! \param[in] &&... args - [optional] Arguments for event handler
    //! 
    //! \remarks This overload is selected if the event handler has no return type
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename = enable_if_same_t<result_t,void>, typename = void, typename... CALL_ARGS>
    void invoke(CALL_ARGS&&... args) const
    {
      // Forward arguments to each subscriber
      for (auto& fn : Subscribers)
        (*fn)(std::forward<CALL_ARGS>(args)...);
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Event::clear
    //! Removes all subscribers 
    /////////////////////////////////////////////////////////////////////////////////////////
    void clear()
    {
      Subscribers.clear();
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Event::raise()
    //! Raises the event, notifying each subscriber in the order in which they subscribed
    //! 
    //! \tparam CALL_ARGS... - Argument types provided when raised
    //!
    //! \param[in] &&... args - [optional] Arguments for event handler
    //! \return result_t - [Returns value] Result of call to final subscriber. If no subscribers then a default constructed 'result_t'
    //!                    [Returns void] Nothing
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename... CALL_ARGS>
    result_t raise(CALL_ARGS&&... args) 
    {
      // Forward arguments to each subscriber, and capture return value (iff function signature has a return type)
      return invoke(std::forward<CALL_ARGS>(args)...);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Event::operator +=
    //! Adds a subscriber to the collection. Also ensures it possesses the correct signature.
    //! 
    //! \tparam DGT_RET - Delegate return type
    //! \tparam DGT_ARGS... - [optional] Delegate argument types
    //!
    //! \param[in] *ptr - Pointer to subscriber (Transfers ownership to the event)
    //! \return LPARAM - Unique subscriber identifier
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename DGT_RET, typename... DGT_ARGS>
    LPARAM operator += (Delegate<DGT_RET,DGT_ARGS...>* ptr) 
    {
      concept_check(DGT_RET (DGT_ARGS...),MatchingSignature<signature_t>);

      // Append to subscriber list and return address as cookie
      Subscribers.emplace_back(ptr);
      return opaque_cast(ptr);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Event::operator -=
    //! Removes a subscriber from the collection
    //! 
    //! \param[in] cookie - Unique subscriber identifier
    /////////////////////////////////////////////////////////////////////////////////////////
    void operator -= (LPARAM cookie) 
    {
      auto findByAddress = [cookie] (const storage_t& ptr) { return ptr.get() == opaque_cast<delegate_t>(cookie); };

      // Remove by delegate address
      Subscribers.remove_if( findByAddress );
    }
  };


  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias handler_t - Defines the handler type for any event
  //! 
  //! \tparam EVENT - Any event type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename EVENT>
  using handler_t = typename EVENT::delegate_t;
        
} // namespace wtl

#endif // WTL_EVENT_HPP
