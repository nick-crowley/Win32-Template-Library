//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\DynamicArray.hpp
//! \brief Statically allocated, fixed capacity array, with dynamic runtime quantity.
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_DYNAMIC_ARRAY_HPP
#define WTL_DYNAMIC_ARRAY_HPP

#include <wtl/WTL.hpp>
#include <wtl/utils/Array.hpp>              //!< Array
#include <wtl/utils/Adapters.hpp>           //!< if_then
#include <wtl/utils/Exception.hpp>          //!< Exceptions
#include <wtl/io/Console.hpp>               //!< Debug console
#include <iterator>                         //!< std::iterator
#include <functional>                       //!< std::
#include <algorithm>                        //!< std::equals,std::find_if,etc.
#include <initializer_list>                 //!< std::initializer_list

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias DynamicArray - Statically allocated fixed capacity array with variable runtime length
  //!
  //! \tparam DATA - Type of each element
  //! \tparam LENGTH - Maximum number of elements within the array
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename DATA, uint32_t LENGTH>
  using DynamicArray = Array<DATA,LENGTH,true>;


  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Array<DATA,LENGTH,true> - Partial specialization for dynamic arrays
  //!
  //! \tparam DATA - Type of each element
  //! \tparam LENGTH - Maximum number of elements within the array
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename DATA, uint32_t LENGTH>
  struct Array<DATA,LENGTH,true>
  {
    template <typename V, uint32_t L, bool D>
    friend struct Array;                     // Unbound friend of itself

    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias cast - Defines dynamic array of equal length & dissimilar type
    template <typename D>
		using cast = Array<D,LENGTH,true>;

    //! \alias ptrdiff_t - Pointer difference type
    using ptrdiff_t = intptr_t;

    //! \alias other - Defines fixed array of equal type & length
		using other = Array<DATA,LENGTH,false>;

    //! \alias resize - Defines dynamic array of equal type & dissimilar length
    template <unsigned L>
		using resize = Array<DATA,L,true>;

    //! \alias traits - Traits type
    using traits = array_traits<DATA>;

    //! \alias value_type - Element data type
    using value_type = DATA;

    //! \alias reference/pointer - Mutable element reference/pointer
    using reference = value_type&;
    using pointer   = value_type*;

    //! \alias array_t/array_ref/array_ptr - Mutable array value/reference/pointer
    using array_t   = value_type [LENGTH];
    using array_ref = value_type (&)[LENGTH];
    using array_ptr = value_type (*)[LENGTH];

    //! \alias const_reference/const_pointer - Immutable element reference/pointer
    using const_reference = const value_type&;
    using const_pointer   = const value_type*;

    //! \alias const_array_t/const_array_ref/const_array_ptr - Immutable array value/reference/pointer
    using const_array_t   = const value_type [LENGTH];
    using const_array_ref = const value_type (&)[LENGTH];
    using const_array_ptr = const value_type (*)[LENGTH];

    //! \var dynamic - Whether array can be resized
    static constexpr bool dynamic = true;

    //! \var length - Capacity of the array
    static constexpr uint32_t length = LENGTH;

    //! \var npos - Sentinel value for 'item not found'
    static constexpr uint32_t npos = static_cast<uint32_t>(-1L);

//!\ if DEVELOPMENT_MODE - Use iterators instead of pointers to aide development
#ifdef DEVELOPMENT_MODE
    /////////////////////////////////////////////////////////////////////////////////////////
    //! \struct iterator_t - Random access element iterator
    //! \tparam VALUE - Element type
    //! \tparam CONTAINER - Array type
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename VALUE = value_type, typename CONTAINER = Array>
    struct iterator_t : public std::iterator<std::random_access_iterator_tag, VALUE>
    {
      friend struct Array;   // Unbound friend of array

      // ---------------------------------- TYPES & CONSTANTS ---------------------------------

      // ------------------------------------ CONSTRUCTION ------------------------------------

      /////////////////////////////////////////////////////////////////////////////////////////
      //! iterator_t::iterator_t
      //! Create iterator at any position
      //!
      //! \param[in,out] &a - Array over which to iterate
      //! \param[in] n - Zero-based Initial index
      /////////////////////////////////////////////////////////////////////////////////////////
      iterator_t(CONTAINER& a, uint32_t n) : Container(&a),
                                           Index(n)
      {}

      /////////////////////////////////////////////////////////////////////////////////////////
      //! iterator_t::iterator_t
      //! Copy-create from another iterator for array of equal type
      //!
      //! \param[in] const& it - Another iterator for array of equal type
      /////////////////////////////////////////////////////////////////////////////////////////
      iterator_t(const iterator_t& it)
      {
        *this = it;
      }

      // ----------------------------------- STATIC METHODS -----------------------------------

      // ---------------------------------- ACCESSOR METHODS ----------------------------------

      /////////////////////////////////////////////////////////////////////////////////////////
      //! iterator_t::getIndex const
      //! Get the index of the current element
      //!
      //! \return uint32_t - Zero based index of the current element
      /////////////////////////////////////////////////////////////////////////////////////////
      uint32_t getIndex() const
      {
        return Index;
      }

      /////////////////////////////////////////////////////////////////////////////////////////
      //! iterator_t::operator == const
      //! Logical equality operator
      //!
      //! \param[in] &r - Another iterator for array of equal type
      //! \return bool - True if equal, otherwise false
      /////////////////////////////////////////////////////////////////////////////////////////
      bool operator == (const iterator_t& r) const
      {
        return Index == r.Index && Container == r.Container;    // Match index first
      }

      /////////////////////////////////////////////////////////////////////////////////////////
      //! iterator_t::operator != const
      //! Logical inequality operator
      //!
      //! \param[in] &r - Another iterator from array of equal type
      //! \return bool - True if not equal, otherwise false
      /////////////////////////////////////////////////////////////////////////////////////////
      bool operator != (const iterator_t& r) const
      {
        return Index != r.Index || Container != r.Container;    // Check index first
      }

      /////////////////////////////////////////////////////////////////////////////////////////
      //! iterator_t::operator < const
      //! Lesser comparison operator
      //!
      //! \param[in] &r - Another iterator for array of equal type
      //! \return bool - True if equal, otherwise false
      /////////////////////////////////////////////////////////////////////////////////////////
      bool operator < (const iterator_t& r) const
      {
        return Index < r.Index && Container == r.Container;    // Match index first
      }

      /////////////////////////////////////////////////////////////////////////////////////////
      //! iterator_t::operator <= const
      //! Lesser-or-equal comparison operator
      //!
      //! \param[in] &r - Another iterator for array of equal type
      //! \return bool - True if equal, otherwise false
      /////////////////////////////////////////////////////////////////////////////////////////
      bool operator <= (const iterator_t& r) const
      {
        return Index <= r.Index && Container == r.Container;    // Match index first
      }

      /////////////////////////////////////////////////////////////////////////////////////////
      //! iterator_t::operator > const
      //! Greater comparison operator
      //!
      //! \param[in] &r - Another iterator for array of equal type
      //! \return bool - True if equal, otherwise false
      /////////////////////////////////////////////////////////////////////////////////////////
      bool operator > (const iterator_t& r) const
      {
        return Index > r.Index && Container == r.Container;    // Match index first
      }

      /////////////////////////////////////////////////////////////////////////////////////////
      //! iterator_t::operator >= const
      //! Greater-or-equal comparison operator
      //!
      //! \param[in] &r - Another iterator for array of equal type
      //! \return bool - True if equal, otherwise false
      /////////////////////////////////////////////////////////////////////////////////////////
      bool operator >= (const iterator_t& r) const
      {
        return Index >= r.Index && Container == r.Container;    // Match index first
      }

      /////////////////////////////////////////////////////////////////////////////////////////
      //! iterator_t::operator* const
      //! Dereferencing operator
      //!
      //! \throw wtl::out_of_range - [Debug only] Position is invalid
      //! \return reference - Reference to the current element
      /////////////////////////////////////////////////////////////////////////////////////////
      reference operator* () const
      {
        CHECKED_INDEX(Index, 0, Container->size());

        // Retrieve
        return Container->Data[Index];
      }

      /////////////////////////////////////////////////////////////////////////////////////////
      //! iterator_t::operator-> const
      //! Dereferencing operator
      //!
      //! \throw wtl::out_of_range - [Debug only] Position is invalid
      //! \return pointer - Pointer to the current element
      /////////////////////////////////////////////////////////////////////////////////////////
      pointer operator-> () const
      {
        CHECKED_INDEX(Index, 0, Container->size());

        // Retrieve
        return &Container->Data[Index];
      }

      /////////////////////////////////////////////////////////////////////////////////////////
      //! iterator_t::operator+ const
      //! Add this position to another
      //!
      //! \param[in] const &r - Another position
      //! \return ptrdiff_t - Sum of indicies
      /////////////////////////////////////////////////////////////////////////////////////////
      ptrdiff_t operator + (const iterator_t& r) const
      {
        return Index + r.Index;
      }

      /////////////////////////////////////////////////////////////////////////////////////////
      //! iterator_t::operator- const
      //! Substract this position from another
      //!
      //! \param[in] const &r - Another position
      //! \return ptrdiff_t - Difference between indicies
      /////////////////////////////////////////////////////////////////////////////////////////
      ptrdiff_t operator - (const iterator_t& r) const
      {
        return Index - r.Index;
      }

      /////////////////////////////////////////////////////////////////////////////////////////
      //! iterator_t::operator[] const
      //! Access the element at position +/- N elements
      //!
      //! \param[in] i - Zero-based offset from current position of element to retrieve
      //! \throw wtl::out_of_range - [Debug only] Position is invalid
      //! \return const_reference - Immutable reference to element
      /////////////////////////////////////////////////////////////////////////////////////////
      const_reference operator[] (int32_t i) const
      {
        CHECKED_INDEX(Index, 0, Container->size());

        // Retrieve
        return Container->Data[Index+i];
      }

      /////////////////////////////////////////////////////////////////////////////////////////
      //! iterator_t::operator + const
      //! Calculate a position N elements ahead
      //!
      //! \param[in] i - Number of elements to advance
      //! \return iterator_t - New position
      /////////////////////////////////////////////////////////////////////////////////////////
      iterator_t operator + (uint32_t i) const
      {
        return iterator_t(*Container, Index+i);
      }

      /////////////////////////////////////////////////////////////////////////////////////////
      //! iterator_t::operator- const
      //! Calculate a position N elements behind
      //!
      //! \param[in] i - Number of elements to retreat
      //! \return iterator_t - New position
      /////////////////////////////////////////////////////////////////////////////////////////
      iterator_t operator- (uint32_t i) const
      {
        return iterator_t(*Container, Index-i);
      }

      // ----------------------------------- MUTATOR METHODS ----------------------------------

      /////////////////////////////////////////////////////////////////////////////////////////
      //! iterator_t::operator=
      //! Assignment operator
      //!
      //! \param[in] &r - Another iterator from array of equal type
      //! \return iterator_t& - Reference to self
      /////////////////////////////////////////////////////////////////////////////////////////
      iterator_t& operator= (const iterator_t& r)
      {
        Container = r.Container;
        Index = r.Index;
        return *this;
      }

      /////////////////////////////////////////////////////////////////////////////////////////
      //! iterator_t::operator +=
      //! Advance the iterator by N elements
      //!
      //! \param[in] i - Number of elements to advance
      //! \return iterator_t - Reference to self at new position
      /////////////////////////////////////////////////////////////////////////////////////////
      iterator_t& operator += (uint32_t n)
      {
        Index += n;
        return *this;
      }

      /////////////////////////////////////////////////////////////////////////////////////////
      //! iterator_t::operator++
      //! Prefix increment operator
      //!
      //! \return iterator_t& - Reference to self at new position
      /////////////////////////////////////////////////////////////////////////////////////////
      iterator_t& operator++()
      {
        ++Index;
        return *this;
      }

      /////////////////////////////////////////////////////////////////////////////////////////
      //! iterator_t::operator++
      //! Postfix increment operator
      //!
      //! \return iterator_t - Position before increment
      /////////////////////////////////////////////////////////////////////////////////////////
      iterator_t operator++(int)
      {
        iterator_t tmp(*this);
        operator++();
        return tmp;
      }

      /////////////////////////////////////////////////////////////////////////////////////////
      //! iterator_t::operator--
      //! Prefix decrement operator
      //!
      //! \return iterator_t& - Reference to self at new position
      /////////////////////////////////////////////////////////////////////////////////////////
      iterator_t& operator--()
      {
        --Index;
        return *this;
      }

      /////////////////////////////////////////////////////////////////////////////////////////
      //! iterator_t::operator--
      //! Postfix decrement operator
      //!
      //! \return iterator_t - Position before increment
      /////////////////////////////////////////////////////////////////////////////////////////
      iterator_t operator--(int)
      {
        iterator_t tmp(*this);
        operator--();
        return tmp;
      }

      // ----------------------------------- REPRESENTATION -----------------------------------
    protected:
      CONTAINER*  Container;     //!< Array over which to iterate
      uint32_t      Index;         //!< Zero-based index of the element representing the current position
    };

    //! \typedef iterator - Friendly name for a mutable iterator
    using iterator = iterator_t<value_type, Array>;

    //! \typedef const_iterator - Friendly name for an immutable iterator
    using const_iterator = iterator_t<const value_type, const Array>;

    // Bound friend of iterators of matching type
    friend struct iterator_t<value_type, Array>;
    friend struct iterator_t<const value_type, const Array>;

//! \ifnot DEVELOPMENT_MODE - Use pointers as iterators to increase performance
#else
    //! \typedef iterator - Friendly name for a mutable iterator
    using iterator = pointer;

    //! \typedef const_iterator - Friendly name for an immutable iterator
    using const_iterator = const_pointer;
#endif

    //! \typedef reverse_iterator - Friendly name for a mutable reverse iterator
    using reverse_iterator = std::reverse_iterator<iterator>;

    //! \typedef const_reverse_iterator - Friendly name for an immutable reverse iterator
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  private:
    //! \enum Unique - Distinct dummy type
    enum class Unique { Signature };

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    array_ref  Data;    //!< Provides strongly typed access to element storage
    uint32_t     Count;   //!< Number of valid elements

  private:
    byte       Storage[sizeof(DATA)*LENGTH];   //!< Actual element storage

    // ------------------------------------ CONSTRUCTION ------------------------------------
  private:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::Array
    //! Initialise empty array
    //!
    //! \param[in] - Ignored
    /////////////////////////////////////////////////////////////////////////////////////////
    Array(Unique) : Data(reinterpret_cast<array_ref>(Storage)),     // Safe to use address of 'Storage' before init
                    Count(0UL)
    {}

  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::Array
    //! Create empty array
    /////////////////////////////////////////////////////////////////////////////////////////
    Array() : Array(Unique::Signature)
    {
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::Array
    //! Copy-create with elements copy-constructed from another array of different type
    //!
    //! \tparam V - Foreign array element data type
    //! \tparam L - Foreign array length
    //! \tparam D - Whether foreign array can be resized
    //!
    //! \param[in] &r - Foreign array
    //!
    //! \throw wtl::length_error - [Debug only] Number of elements exceeds capacity
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename V, uint32_t L, bool D>
    Array(const Array<V,L,D>& r) : Array(Unique::Signature)
    {
      assign(r);
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // Array::Array
    //! Create with (up to 'length') elements copy-constructed from a static array
    //!
    //! \tparam ELEMENT - Element type
    //! \tparam LEN - Array length
    //!
    //! \param[in] r - Array
    //////////////////////////////////////////////////////////////////////////////////////////
    template <typename ELEMENT, unsigned LEN>
    Array(ELEMENT (&r)[LEN]) : Array(Unique::Signature)
    {
      assign( &r[0], &r[std::min(LEN,length)] );
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // Array::Array
    //! Create with elements copy-constructed from a range of elements
    //!
    //! \tparam INPUT - Input iterator type
    //!
    //! \param[in] start - First position in input range
    //! \param[in] end - Position immediately beyond last element in input range
    //! \remarks If there are insufficient elements in the input range, the results are undefined
    //////////////////////////////////////////////////////////////////////////////////////////
    template <typename INPUT>
    Array(INPUT start, INPUT end) : Array(Unique::Signature)
    {
      assign(start,end);
    }

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::Array
    //! Copy-create with elements copy-constructed from another array of equal type
    //!
    //! \param[in] &r - Another array
    /////////////////////////////////////////////////////////////////////////////////////////
    Array(const Array& r) : Array(Unique::Signature)
    {
      assign(r);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::Array
    //! Copy/move-create with elements copy-constructed from another array of equal type
    //!
    //! \param[in] &&r - Another array
    /////////////////////////////////////////////////////////////////////////////////////////
    Array(Array&& r) : Array(Unique::Signature)
    {
      assign(r);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::operator=
    //! Replace elements with those copy-constructed from an array of same type
    //!
    //! \param[in] &r - Another array
    //! \return Array& - Reference to self
    /////////////////////////////////////////////////////////////////////////////////////////
    Array& operator=(const Array& r)
    {
      assign(r);
      return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::operator=
    //! Replace elements with those copy-constructed from an array of different type
    //!
    //! \tparam E - Foreign element type
    //! \tparam L - Foreign array length
    //! \tparam D - Whether foreign array is dynamic
    //!
    //! \param[in] &r - Foreign array
    //! \return Array& - Reference to self
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename E, uint32_t L, bool D>
    Array& operator=(const Array<E,L,D>& r)
    {
      assign(r);
      return *this;
    }

    ENABLE_POLY(Array);       //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

//!\ if DEVELOPMENT_MODE - Use iterators instead of pointers to aide development
#ifdef DEVELOPMENT_MODE
    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::begin() const
    //! Get immutable iterator positioned at first element
    //!
    //! \return const_iterator - Iterator positioned at first element
    /////////////////////////////////////////////////////////////////////////////////////////
    const_iterator begin() const
    {
      return const_iterator(*this, 0);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::end() const
    //! Get immutable iterator positioned immediately beyond final element
    //!
    //! \return const_iterator - Iterator positioned immediately beyond array bounds
    /////////////////////////////////////////////////////////////////////////////////////////
    const_iterator end() const
    {
      return const_iterator(*this, size());
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::rbegin() const
    //! Get immutable reverse iterator positioned at final element
    //!
    //! \return const_reverse_iterator - Iterator positioned at final element
    /////////////////////////////////////////////////////////////////////////////////////////
    const_reverse_iterator rbegin() const
    {
      return const_reverse_iterator( const_iterator(*this, size()) );
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::rend() const
    //! Get immutable reverse iterator positioned immediately before first element
    //!
    //! \return const_reverse_iterator - Iterator positioned immediately before first element
    /////////////////////////////////////////////////////////////////////////////////////////
    const_reverse_iterator rend() const
    {
      return const_reverse_iterator( const_iterator(*this, 0UL) );
    }

//! \ifnot DEVELOPMENT_MODE - Use pointers instead to increase performance
#else
    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::begin() const
    //! Get immutable iterator positioned at first element
    //!
    //! \return const_iterator - Iterator positioned at first element
    /////////////////////////////////////////////////////////////////////////////////////////
    const_iterator begin() const
    {
      return const_iterator(Data);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::end() const
    //! Get immutable iterator positioned immediately beyond final element
    //!
    //! \return const_iterator - Iterator positioned immediately beyond array bounds
    /////////////////////////////////////////////////////////////////////////////////////////
    const_iterator end() const
    {
      return const_iterator(Data + size());
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::rbegin() const
    //! Get immutable reverse iterator positioned at final element
    //!
    //! \return const_reverse_iterator - Iterator positioned at final element
    /////////////////////////////////////////////////////////////////////////////////////////
    const_reverse_iterator rbegin() const
    {
      return const_reverse_iterator( end() );
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::rend() const
    //! Get immutable reverse iterator positioned immediately before first element
    //!
    //! \return const_reverse_iterator - Iterator positioned immediately before first element
    /////////////////////////////////////////////////////////////////////////////////////////
    const_reverse_iterator rend() const
    {
      return const_reverse_iterator( begin() );
    }
#endif


    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::all_of const
    //! Query whether a predicate is satisfied by all elements
    //!
    //! \tparam PRED - Unary predicate function object
    //!
    //! \param[in] &p - Element predicate
    //! \return bool - True if all elements satisfy the predicate, False if otherwise. True if the array is empty.
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename PRED>
    bool all_of(const PRED& p) const
    {
      return all_of(begin(), end(), p);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::all_of const
    //! Query whether a predicate is satisfied by all elements within a range
    //!
    //! \tparam PRED - Unary predicate function object
    //!
    //! \param[in] first - Position of first element in range
    //! \param[in] last - Position beyond last element in range
    //! \param[in] &p - Element predicate
    //! \return bool - True if all elements satisfy the predicate, False if otherwise. True if the subset is empty.
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename PRED>
    bool all_of(const_iterator first, const_iterator last, const PRED& p) const
    {
      // Query all
      return std::all_of(first, last, p);   //return any_of(first, last, std::unary_negate<PRED>(p)) == false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::any_of const
    //! Query whether a predicate is satisfied by any element
    //!
    //! \tparam PRED - Unary predicate function object
    //!
    //! \param[in] const &p - Element predicate
    //! \return bool - True if any elements satisfy the predicate, False if otherwise. False if the array is empty.
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename PRED>
    bool any_of(const PRED& p) const
    {
      return any_of(begin(), end(), p);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::any_of const
    //! Query whether a predicate is satisfied by any element within a range
    //!
    //! \tparam PRED - Unary predicate function object
    //!
    //! \param[in] first - Position of first element in range
    //! \param[in] last - Position beyond last element in range
    //! \param[in] const &p - Element predicate
    //! \return bool - True if any elements satisfy the predicate, False if otherwise. False if the subset is empty.
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename PRED>
    bool any_of(const_iterator first, const_iterator last, const PRED& p) const
    {
      return find_if(first, last, p) != last;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::at() const
    //! Provides read-only access to array elements without bounds checking
    //!
    //! \param[in] index - Zero-based index of the element to access
    //! \return const_reference - Immutable reference to the desired element
    //!
    //! \throw wtl::out_of_range - [Debug only] Index out of bounds
    /////////////////////////////////////////////////////////////////////////////////////////
    const_reference at(uint32_t index) const
    {
      CHECKED_INDEX(index, 0, size());

      // Retrieve data
      return Data[index];
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::back
    //! Get a immutable reference to the last element
    //!
    //! \return const_reference - Immutable reference to the last element
    //!
    //! \throw wtl::logic_error - [Debug only] Array is empty
    /////////////////////////////////////////////////////////////////////////////////////////
    const_reference back() const
    {
      LOGIC_INVARIANT(size() > 0L);

      return Data[size()-1];
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::c_arr() const
    //! Get an immutable pointer to the entire array
    //!
    //! \return const_array_ptr - Immutable pointer to the entire array
    /////////////////////////////////////////////////////////////////////////////////////////
    const_array_ptr c_arr() const
    {
      return &Data;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::capacity() const
    //! Get the maximum number of elements allowed in the array
    //!
    //! \return uint32_t - Maximum number of elements
    //////////////////////////////////////////////////////////////////////////////////////////
    uint32_t capacity() const
    {
      return length;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::contains() const
    //! Query whether array contains a value
    //!
    //! \param[in] value - Value to find
    //! \return bool - True if found, False otherwise
    //////////////////////////////////////////////////////////////////////////////////////////
    bool contains(const_reference value) const
    {
      return find(value) != npos;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::copy const
    //! Copy all elements to an output range of sufficient capacity.
    //! If there is insufficient capacity, the results are undefined
    //!
    //! \tparam OUTPUT - Type of output iterator
    //!
    //! \param[in] dest - Position of beginning of destination range
    //! \return iterator - Output iterator positioned one element beyond last element copied
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename OUTPUT>
    OUTPUT copy(OUTPUT dest) const
    {
      return std::copy(begin(), end(), dest);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::empty() const
    //! Query whether array is empty
    //!
    //! \return bool - True if empty, otherwise false
    //////////////////////////////////////////////////////////////////////////////////////////
    bool empty() const
    {
      return Count == 0UL;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::find() const
    //! Searches for a value using a forward linear search
    //!
    //! \param[in] value - Value to find
    //! \return uint32_t - Zero-based index of first matching item if found, otherwise npos
    //////////////////////////////////////////////////////////////////////////////////////////
    uint32_t find(const_reference value) const
    {
      // Forward linear search
      for (uint32_t i = 0UL; i < length; ++i)
        if (Data[i] == value)
          return i;

      // Not found
      return npos;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::find_if const
    //! Find the position of the first element that satisfies a predicate
    //!
    //! \tparam PRED - Unary predicate function object
    //!
    //! \param[in] const &p - Element predicate
    //! \return const_iterator - Position of matching element if found, otherwise 'end' position
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename PRED>
    const_iterator find_if(const PRED& p) const
    {
      return find_if(begin(), end(), p);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::find_if
    //! Find the position of the first element that satisfies a predicate
    //!
    //! \tparam PRED - Unary predicate function object
    //!
    //! \param[in] first - Position of first element in range
    //! \param[in] last - Position beyond last element in range
    //! \param[in] const &p - Element predicate
    //! \return const_iterator - Position of matching element if found, otherwise 'end' position
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename PRED>
    const_iterator find_if(const_iterator first, const_iterator last, const PRED& p) const
    {
      // Forward linear search
      return std::find_if<const_iterator,const PRED&>(first, last, p);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::for_each
    //! Executes a function upon each element in the array
    //!
    //! \tparam FUNC - Unary function object
    //!
    //! \param[in,out] &&f - Unary function object
    //! \return FUNC - Copy of input function object
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename FUNC>
    FUNC for_each(FUNC&& f) const
    {
      return for_each(begin(), end(), f);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::for_each
    //! Executes a function upon each element in a subset of the array
    //!
    //! \tparam FUNC - Unary function object
    //!
    //! \param[in] first - Position of first element in range
    //! \param[in] last - Position beyond last element in range
    //! \param[in,out] &f - Unary function object
    //! \return FUNC - Copy of input function object
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename FUNC>
    FUNC for_each(const_iterator first, const_iterator last, FUNC& f) const
    {
      return std::for_each<const_iterator, FUNC&>(first, last, f);   // Pass function by-ref
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::for_each_if
    //! Executes a function upon each element in the array that satisfies a predicate
    //!
    //! \tparam PRED - Unary predicate function object
    //! \tparam FUNC - Unary function object
    //!
    //! \param[in] first - Position of first element in range
    //! \param[in] last - Position beyond last element in range
    //! \param[in,out] &p - Unary predicate function object
    //! \param[in,out] &f - Unary function object
    //! \return FUNC - Copy of unary function object 'f'
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename PRED, typename FUNC>
    FUNC for_each_if(const_iterator first, const_iterator last, const PRED& p, const FUNC& f) const
    {
      return std::for_each<const_iterator,FUNC&>(first, last, wtl::if_then(p,f)).fx();
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::for_each_if
    //! Executes a function upon each element in the array that satisfies a predicate
    //!
    //! \tparam PRED - Unary predicate function object
    //! \tparam FUNC - Unary function object
    //!
    //! \param[in,out] &p - Unary predicate function object
    //! \param[in,out] &f - Unary function object
    //! \return FUNC - Copy of Unary function object 'f'
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename PRED, typename FUNC>
    FUNC for_each_if(const PRED& p, FUNC& f) const
    {
      return for_each_if(begin(), end(), p, f);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::full() const
    //! Query whether array is full
    //!
    //! \return bool - True if full, otherwise false
    //////////////////////////////////////////////////////////////////////////////////////////
    bool full() const
    {
      return Count == length;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::front
    //! Get a immutable reference to the first element
    //!
    //! \return const_reference - Immutable reference to the first element
    //!
    //! \throw wtl::logic_error - [Debug only] Array is empty
    /////////////////////////////////////////////////////////////////////////////////////////
    const_reference front() const
    {
      LOGIC_INVARIANT(size() > 0L);

      // Access
      return Data[0];
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::none_of const
    //! Query whether a predicate is not satisfied by any elements
    //!
    //! \tparam PRED - Unary predicate function object
    //!
    //! \param[in] const &p - Element predicate
    //! \return bool - True if no elements satisfy the predicate, False if otherwise. True if the array is empty.
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename PRED>
    bool none_of(const PRED& p) const
    {
      return none_of(begin(), end(), p);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::none_of const
    //! Query whether a predicate is not satisfied by any elements within a range
    //!
    //! \tparam INPUT - Input iterator type
    //! \tparam PRED - Unary predicate function object
    //!
    //! \param[in] first - Position of first element in range
    //! \param[in] last - Position beyond last element in range
    //! \param[in] const &p - Element predicate
    //! \return bool - True if no elements satisfy the predicate, False if otherwise. True if the subset is empty.
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename INPUT, typename PRED>
    bool none_of(INPUT first, INPUT last, const PRED& p) const
    {
      // Check all fail the predicate
      return find_if(first, last, p) == last;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::size
    //! Get number of elements currently in the array
    //!
    //! \return uint32_t - Element count
    /////////////////////////////////////////////////////////////////////////////////////////
    uint32_t size() const
    {
      return Count;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::tranform
    //! Transforms each element within a range into an output container
    //!
    //! \tparam OUTPUT - Output iterator type
    //! \tparam FUNC - Transformation function type
    //!
    //! \param[in] first - First element in input range
    //! \param[in] last - Position beyond last element in input range
    //! \param[in] output - First element in output range
    //! \param[in] fn - Unary transformation function
    //! \return OUTPUT - Position in output range immediately beyond last element transformed
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename OUTPUT, typename FUNC>
    OUTPUT transform(const_iterator first, const_iterator last, OUTPUT output, const FUNC fn) const
    {
      return std::transform(first, last, output, fn);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::tranform
    //! Transforms each element within a range into an output container
    //!
    //! \tparam OUTPUT - Output iterator type
    //! \tparam FUNC - Transformation function type
    //!
    //! \param[in] output - First element in output range
    //! \param[in] fn - Unary transformation function
    //! \return OUTPUT - Position in output range immediately beyond last element transformed
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename OUTPUT, typename FUNC>
    OUTPUT transform(OUTPUT output, const FUNC fn) const
    {
      return transform(begin(), end(), output, fn);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::operator == const
    //! Equality operator
    //!
    //! \tparam V - Array element data type
    //! \tparam L - Array length
    //! \tparam D - Whether array can be resized
    //!
    //! \param[in] const &r - Array of another type
    //! \return bool - True iff all elements are equal
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename V, unsigned L, bool D>
    bool operator== (const Array<V,L,D>& r) const
    {
      // Check all elements are equal
      return std::equal(begin(), end(), r.begin());
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::operator != const
    //! Inequality operator
    //!
    //! \tparam V - Array element data type
    //! \tparam L - Array length
    //! \tparam D - Whether array can be resized
    //!
    //! \param[in] const &r - Array of another type
    //! \return bool - True iff one or more elements are unequal
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename V, unsigned L, bool D>
    bool operator!= (const Array<V,L,D>& r) const
    {
      // Check at least one element is unequal
      return !std::equal(begin(), end(), r.begin());
    }


    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::operator[] const
    //! Provides read-only access to array elements without bounds checking
    //!
    //! \tparam INDEX - Type of the index parameter (Must be implicitly convertible to uint32_t)
    //!
    //! \param[in] index - Zero-based index of the element to access. If the index is invalid the result is undefined.
    //! \return const_reference - Immutable reference to the desired element
    //!
    //! \throw wtl::out_of_range - [Debug only] Index out of bounds
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename INDEX>
    const_reference operator[] (INDEX index) const
    {
      CHECKED_INDEX((int)index, 0L, (int)size());

      // Retrieve element
      return Data[static_cast<uint32_t>(index)];
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::operator const_array_ref() const
    //! Get an immutable reference to the entire array
    //!
    //! \return const_array_ref - Immutable reference to the entire array
    /////////////////////////////////////////////////////////////////////////////////////////
    operator const_array_ref() const
    {
      return Data;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::operator array_ref()
    //! Get an mutable reference to the entire array
    //!
    //! \return array_ref - Mutable reference to the entire array
    /////////////////////////////////////////////////////////////////////////////////////////
    operator array_ref()
    {
      return Data;
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------

//!\ if DEVELOPMENT_MODE - Use iterators instead of pointers to aide development
#ifdef DEVELOPMENT_MODE
    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::begin
    //! Get mutable iterator positioned at first element
    //!
    //! \return iterator - Iterator positioned at first element
    /////////////////////////////////////////////////////////////////////////////////////////
    iterator begin()
    {
      return iterator(*this, 0);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::end
    //! Get mutable iterator positioned immediately beyond final element
    //!
    //! \return iterator - Iterator positioned immediately beyond final element
    /////////////////////////////////////////////////////////////////////////////////////////
    iterator end()
    {
      return iterator(*this, size());
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::rbegin()
    //! Get mutable reverse iterator positioned at final element
    //!
    //! \return reverse_iterator - Iterator positioned at final element
    /////////////////////////////////////////////////////////////////////////////////////////
    reverse_iterator rbegin()
    {
      return reverse_iterator( iterator(*this, size()) );
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::rend()
    //! Get mutable reverse iterator positioned immediately before first element
    //!
    //! \return reverse_iterator - Iterator positioned immediately before first element
    /////////////////////////////////////////////////////////////////////////////////////////
    reverse_iterator rend()
    {
      return reverse_iterator( iterator(*this, 0UL) );
    }

//! \ifnot DEVELOPMENT_MODE - Use pointers as iterators to increase performance
#else
    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::begin
    //! Get mutable iterator positioned at first element
    //!
    //! \return iterator - Iterator positioned at first element
    /////////////////////////////////////////////////////////////////////////////////////////
    iterator begin()
    {
      return iterator(Data);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::end
    //! Get mutable iterator positioned immediately beyond final element
    //!
    //! \return iterator - Iterator positioned immediately beyond final element
    /////////////////////////////////////////////////////////////////////////////////////////
    iterator end()
    {
      return iterator(Data + size());
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::rbegin()
    //! Get mutable reverse iterator positioned at final element
    //!
    //! \return reverse_iterator - Iterator positioned at final element
    /////////////////////////////////////////////////////////////////////////////////////////
    reverse_iterator rbegin()
    {
      return reverse_iterator( iterator(Data + size()) );
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::rend()
    //! Get mutable reverse iterator positioned immediately before first element
    //!
    //! \return reverse_iterator - Iterator positioned immediately before first element
    /////////////////////////////////////////////////////////////////////////////////////////
    reverse_iterator rend()
    {
      return reverse_iterator( iterator(Data) );
    }
#endif

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::accumulate
    //! Accumulates all the elements in the array
    //!
    //! \param[in] seed - Initial seed value
    //! \return value_type - Sum of all elements
    /////////////////////////////////////////////////////////////////////////////////////////
    value_type accumulate(value_type seed /*= defvalue<value_type>()*/)
    {
      for_each([&seed](const value_type& v) { seed += v; });
      return seed;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::accumulate
    //! Accumulates the results of a function applied to all elements in the array
    //!
    //! \tparam RESULT - Type of resultant sum
    //! \tparam FUNC - Unary function object type
    //!
    //! \param[in,out] fn - Unary function object
    //! \param[in] seed - Initial seed value
    //! \return RESULT - Sum of all elements
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename RESULT, typename FUNC>
    RESULT accumulate(FUNC&& fn, RESULT seed) // = zero_t<RESULT>)
    {
      for_each([&](const value_type& v) { seed += fn(v); });
      return seed;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::assign
    //! Replace elements with those copy-constructed from elements from another container
    //!
    //! \tparam INPUT - Input iterator type
    //!
    //! \param[in] first - First element in input range
    //! \param[in] last - Position immediately beyond last element in input range
    //!
    //! \throw wtl::length_error - [Debug only] Number of elements in input range exceeds capacity
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename INPUT>
    void assign(INPUT first, INPUT last)
    {
      LENGTH_INVARIANT((last-first) <= length);
      static_assert(std::is_convertible<decltype(*first),value_type>::value, "Cannot convert between element types");

      uint32_t i(0UL);

      // Clear existing
      clear();

      // Copy-construct (up to) LENGTH elements
      for (Count = std::min<uint32_t>(length, last-first); i < Count; ++i)
        traits::alloc_t::construct(Data+i, *(first++));
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::assign
    //! Assign from an array of different (but convertible) element types
    //!
    //! \tparam V - Array element data type
    //! \tparam L - Array length
    //! \tparam D - Whether array can be resized
    //!
    //! \param[in] &r - Foreign array
    //!
    //! \throw wtl::length_error - [Debug only] Number of elements exceeds capacity
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename V, uint32_t L, bool D>
    void assign(const Array<V,L,D>& r)
    {
      static_assert(std::is_convertible<V,value_type>::value, "Cannot convert between element types");

      // Assign from foreign array
      assign(r.begin(), r.end());
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::all_of
    //! Query whether a predicate is satisfied by all elements
    //!
    //! \tparam PRED - Unary predicate function object
    //!
    //! \param[in,out] &p - Element predicate
    //! \return bool - True if all elements satisfy the predicate, False if otherwise. True if the array is empty.
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename PRED>
    bool all_of(PRED& p)
    {
      return all_of(begin(), end(), p);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::all_of
    //! Query whether a predicate is satisfied by all elements within a range
    //!
    //! \tparam PRED - Unary predicate function object
    //!
    //! \param[in] first - Position of first element in range
    //! \param[in] last - Position beyond last element in range
    //! \param[in,out] &p - Element predicate
    //!
    //! \return bool - True if all elements satisfy the predicate, False if otherwise. True if the subset is empty.
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename PRED>
    bool all_of(iterator first, iterator last, PRED& p)
    {
      // Check for none that fail the predicate
      return any_of(first, last, std::unary_negate<PRED>(p)) == false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::any_of
    //! Query whether a predicate is satisfied by any element
    //!
    //! \tparam PRED - Unary predicate function object
    //!
    //! \param[in,out] &p - Element predicate
    //! \return bool - True if any elements satisfy the predicate, False if otherwise. False if the array is empty.
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename PRED>
    bool any_of(PRED& p)
    {
      return any_of(begin(), end(), p);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::any_of
    //! Query whether a predicate is satisfied by any element within a range
    //!
    //! \tparam PRED - Unary predicate function object
    //!
    //! \param[in] first - Position of first element in range
    //! \param[in] last - Position beyond last element in range
    //! \param[in,out] &p - Element predicate
    //! \return bool - True if any elements satisfy the predicate, False if otherwise. False if the subset is empty.
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename PRED>
    bool any_of(iterator first, iterator last, PRED& p)
    {
      return find_if(first, last, p) != last;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::at
    //! Provides access to array elements without bounds checking
    //!
    //! \param[in] index - Zero-based index of the element to access
    //! \return reference - Mutable reference to the desired element
    //!
    //! \throw wtl::out_of_range - [Debug only] Index out of bounds
    /////////////////////////////////////////////////////////////////////////////////////////
    reference at(uint32_t index)
    {
      CHECKED_INDEX(index, 0, size());

      // Retrieve data
      return Data[index];
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::back
    //! Get a mutable reference to the last element
    //!
    //! \return reference - Mutable reference to the last element
    //!
    //! \throw wtl::logic_error - [Debug only] Array is empty
    /////////////////////////////////////////////////////////////////////////////////////////
    reference back()
    {
      LOGIC_INVARIANT(size() > 0L);

      return Data[size()-1];
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::c_arr()
    //! Get an mutable pointer to the entire array
    //!
    //! \return array_ptr - Mutable pointer to the entire array
    /////////////////////////////////////////////////////////////////////////////////////////
    array_ptr c_arr()
    {
      return &Data;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::clear
    //! Empties the array
    /////////////////////////////////////////////////////////////////////////////////////////
    void clear()
    {
      // Destroy elements in descending order, updating count in real-time
      for (int32_t i = Count-1; i >= 0; --i)
        traits::alloc_t::destroy(Data + (Count=i));
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::copy_if
    //! Copy all elements within the array that satisfy a predicate to another container
    //!
    //! \tparam OUTPUT - Type of Output iterator
    //! \tparam PRED - Unary predicate function object
    //!
    //! \param[in] dest - Position of beginning of destination range
    //! \param[in,out] &p - Element predicate
    //! \return iterator - Output iterator positioned one element beyond last element copied
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename OUTPUT, typename PRED>
    iterator copy_if(OUTPUT dest, PRED& p)
    {
      return copy_if(begin(), end(), dest, p);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::copy_if
    //! Copy elements from a subset that satisfy a predicate to another container
    //!
    //! \tparam OUTPUT - Type of Output iterator
    //! \tparam PRED - Unary predicate function object
    //!
    //! \param[in] first - Position of first element in range
    //! \param[in] last - Position beyond last element in range
    //! \param[in] dest - Position of beginning of destination range
    //! \param[in,out] &p - Element predicate
    //! \return iterator - Output iterator positioned one element beyond last element copied
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename OUTPUT, typename PRED>
    iterator copy_if(iterator first, iterator last, OUTPUT dest, PRED& p)
    {
      return std::copy_if<iterator,OUTPUT,PRED&>(first, last, dest, p);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::count_if
    //! Count the number of elements that satisfy a predicate
    //!
    //! \tparam PRED - Unary predicate function object
    //!
    //! \param[in,out] &p - Element predicate
    //! \return uint32_t - Number of matching elements
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename PRED>
    uint32_t count_if(PRED& p)
    {
      return count_if(begin(), end(), p);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::count_if
    //! Count the number of elements within a range that satisfy a predicate
    //!
    //! \tparam PRED - Unary predicate function object
    //!
    //! \param[in] first - Position of first element in range
    //! \param[in] last - Position beyond last element in range
    //! \param[in,out] &p - Element predicate
    //! \return uint32_t - Number of matching elements
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename PRED>
    uint32_t count_if(iterator first, iterator last, PRED& p)
    {
      return std::count_if<iterator,PRED&>(first, last, p);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::emplace_back
    //! Constructs a new elements in place at the back of the array
    //!
    //! \param[in,out] && args - [optional] Constructor arguments
    //!
    //! \throw wtl::length_error - [Debug only] Array is full
    /////////////////////////////////////////////////////////////////////////////////////////
    template< class... PARAMS>
    void emplace_back(PARAMS&&... args)
    {
      CHECKED_LENGTH(size(), capacity());

      // Attempt in-place variadic construction
      traits::alloc_t::construct(&Data[Count], std::forward<PARAMS>(args)...);

      // Success!
      ++Count;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::fill
    //! Fill all elements with a value
    //!
    //! \param[in] const& value - Value
    /////////////////////////////////////////////////////////////////////////////////////////
    void fill(const_reference value)
    {
      std::fill(begin(), end(), value);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::find_if
    //! Find the position of the first element that satisfies a predicate
    //!
    //! \tparam PRED - Unary predicate function object
    //!
    //! \param[in,out] &p - Element predicate
    //! \return iterator - Position of matching element if found, otherwise 'end' position
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename PRED>
    iterator find_if(PRED& p)
    {
      return find_if(begin(), end(), p);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::find_if
    //! Find the position of the first element that satisfies a predicate
    //!
    //! \tparam PRED - Unary predicate function object
    //!
    //! \param[in] first - Position of first element in range
    //! \param[in] last - Position beyond last element in range
    //! \param[in,out] &p - Element predicate
    //! \return iterator - Position of matching element if found, otherwise 'end' position
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename PRED>
    iterator find_if(iterator first, iterator last, PRED& p)
    {
      return std::find_if<iterator,PRED&>(first, last, p);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::for_each
    //! Executes a function upon each element in the array
    //!
    //! \tparam FUNC - Unary function object
    //!
    //! \param[in,out] &&f - Unary function object
    //! \return FUNC - Copy of input function object
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename FUNC>
    FUNC for_each(FUNC&& f)
    {
      return for_each(begin(), end(), f);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::for_each
    //! Executes a function upon each element in a subset of the array
    //!
    //! \tparam FUNC - Unary function object
    //!
    //! \param[in] first - Position of first element in range
    //! \param[in] last - Position beyond last element in range
    //! \param[in,out] &f - Unary function object
    //! \return FUNC - Copy of input function object
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename FUNC>
    FUNC for_each(iterator first, iterator last, FUNC& f)
    {
      return std::for_each<iterator, FUNC>(first, last, f);   // Pass function by-ref
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::for_each_if
    //! Executes a function upon each element in the array that satisfies a predicate
    //!
    //! \tparam PRED - Unary predicate function object
    //! \tparam FUNC - Unary function object
    //!
    //! \param[in] first - Position of first element in range
    //! \param[in] last - Position beyond last element in range
    //! \param[in,out] &p - Unary predicate function object
    //! \param[in,out] &f - Unary function object
    //! \return FUNC - Copy of input Unary function object 'f'
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename PRED, typename FUNC>
    FUNC for_each_if(iterator first, iterator last, PRED& p, FUNC& f)
    {
      return std::for_each<iterator,FUNC>(first, last, wtl::if_then(p,f)).fx();
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::for_each_if
    //! Executes a function upon each element in the array that satisfies a predicate
    //!
    //! \tparam PRED - Unary predicate function object
    //! \tparam FUNC - Unary function object
    //!
    //! \param[in] const &p - Unary predicate function object
    //! \param[in,out] &f - Unary function object
    //! \return FUNC - Copy of Unary function object 'f'
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename PRED, typename FUNC>
    FUNC for_each_if(const PRED& p, FUNC& f)
    {
      return for_each_if(begin(), end(), p, f);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::front
    //! Get a mutable reference to the first element
    //!
    //! \return reference - Mutable reference to the first element
    //!
    //! \throw wtl::logic_error - [Debug only] Array is empty
    /////////////////////////////////////////////////////////////////////////////////////////
    reference front()
    {
      LOGIC_INVARIANT(size() > 0L);

      return Data[0];
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::none_of
    //! Query whether a predicate is not satisfied by any elements
    //!
    //! \tparam PRED - Unary predicate function object
    //!
    //! \param[in,out] &p - Element predicate
    //! \return bool - True if no elements satisfy the predicate, False if otherwise. True if the array is empty.
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename PRED>
    bool none_of(PRED& p)
    {
      return none_of(begin(), end(), p);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::none_of
    //! Query whether a predicate is not satisfied by any elements within a range
    //!
    //! \tparam INPUT - Input iterator type
    //! \tparam PRED - Unary predicate function object
    //!
    //! \param[in] first - Position of first element in range
    //! \param[in] last - Position beyond last element in range
    //! \param[in,out] &p - Element predicate
    //! \return bool - True if no elements satisfy the predicate, False if otherwise. True if the subset is empty.
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename INPUT, typename PRED>
    bool none_of(INPUT first, INPUT last, PRED& p)
    {
      // Check all fail the predicate
      return find_if(first, last, p) == last;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::push_back
    //! Appends an element to the back of the array, without bounds checking.
    //! If the array is full, the results are undefined
    //!
    //! \param[in] const &d - Element to append
    //!
    //! \throw wtl::length_error - [Debug only] Array is full
    /////////////////////////////////////////////////////////////////////////////////////////
    bool push_back(const_reference d)
    {
      CHECKED_LENGTH(size(), capacity());

      // Copy-construct in place
      traits::alloc_t::construct(&Data[Count], d);

      // Success!
      ++Count;
      return true;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::pop_back
    //! Removes and return an element from the back of the array, without bounds checking.
    //! If the array is empty, the results are undefined
    //!
    //! \throw wtl::logic_error - [Debug only] Array is empty
    /////////////////////////////////////////////////////////////////////////////////////////
    value_type pop_back()
    {
      LOGIC_INVARIANT(size() > 0UL);

      // Copy and destroy
      value_type vt(back());
      traits::alloc_t::destroy(Data[Count-1]);
      --Count;

      // Return copy
      return vt;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::tranform
    //! Transforms each element within an input range into an output range
    //!
    //! \tparam OUTPUT - Output iterator
    //! \tparam FUNC - Transformation function
    //!
    //! \param[in] first - First element in input range
    //! \param[in] last - Position beyond last element in input range
    //! \param[in] output - First element in output range
    //! \param[in] fn - Transformation function
    //! \return OUTPUT - Position in output range immediately beyond last element transformed
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename OUTPUT, typename FUNC>
    OUTPUT transform(iterator first, iterator last, OUTPUT output, FUNC fn)
    {
      return std::transform(first, last, output, fn);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::tranform
    //! Transforms each element within an input range into an output range
    //!
    //! \tparam OUTPUT - Output iterator
    //! \tparam FUNC - Transformation function
    //!
    //! \param[in] output - First element in output range
    //! \param[in] fn - Transformation function
    //! \return OUTPUT - Position in output range immediately beyond last element transformed
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename OUTPUT, typename FUNC>
    OUTPUT transform(OUTPUT output, FUNC fn)
    {
      return std::transform(begin(), end(), output, fn);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::tranform_n
    //! Transforms up to N elements within an input range into an output range
    //!
    //! \tparam INPUT - Input iterator type
    //! \tparam OUTPUT - Output iterator
    //! \tparam FUNC - Transformation function
    //!
    //! \param[in] first - First element in input range
    //! \param[in] last - Position beyond last element in input range
    //! \param[in] n - Maximum number of elements to transform
    //! \param[in] output - First element in output range
    //! \param[in] fn - Transformation function
    //! \return OUTPUT - Position in output range immediately beyond last element transformed
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename INPUT, typename OUTPUT, typename FUNC>
    OUTPUT transform_n(INPUT first, INPUT last, uint32_t n, OUTPUT output, FUNC fn)
    {
      // Transform inputs into outputs while n > 0
      while (n-- > 0L && first != last)
        *(output++) = fn(*(first++));

      return output;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::tranform_n
    //! Transforms up to first N elements into an output range
    //!
    //! \tparam OUTPUT - Output iterator
    //! \tparam FUNC - Transformation function
    //!
    //! \param[in] n - Maximum number of elements to transform
    //! \param[in] output - First element in output range
    //! \param[in] fn - Transformation function
    //! \return OUTPUT - Position in output range immediately beyond last element transformed
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename OUTPUT, typename FUNC>
    OUTPUT transform_n(uint32_t n, OUTPUT output, FUNC fn)
    {
      return transform_n(begin(), end(), n, output, fn);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::operator[]
    //! Provides access to array elements without bounds checking
    //!
    //! \tparam INDEX - Type of the index parameter (Must be implicitly convertible to uint32_t)
    //!
    //! \param[in] index - Zero-based index of the element to access. If the index is invalid the result is undefined.
    //! \return reference - Reference to the desired element
    //!
    //! \throw wtl::out_of_range - [Debug only] Index out of range
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename INDEX>
    reference operator[](INDEX index)
    {
      CHECKED_INDEX(static_cast<uint32_t>(index),0,size());

      return Data[static_cast<uint32_t>(index)];
    }
  };



} // WTL namespace



#endif // WTL_DYNAMIC_ARRAY_HPP
