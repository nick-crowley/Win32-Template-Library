//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\platform\Size.hpp
//! \brief Provides utility size type
//! \date 20 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_SIZE_HPP
#define WTL_SIZE_HPP

#include <wtl/WTL.hpp>
#include <wtl/utils/DebugInfo.hpp>     //!< DebugInfo
#include <wtl/utils/SFINAE.hpp>        //!< enable_if_sizeof_t
#include <type_traits>                 //!< std::enable_if

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Size - Encapsulates a size of any type
  //!
  //! \tparam T - Dimension type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  struct Size
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Defines own type
    using type = Size<T>;

    //! \alias value_t - Defines dimension type
    using value_t = T;

    //! \var EMPTY - Empty sentinel value
    //static constexpr Size<T> EMPTY {0,0};
    static const Size<T> EMPTY;

    // ----------------------------------- REPRESENTATION -----------------------------------

    value_t  Width,        //!< Width extent
             Height;       //!< Height extent

    // ------------------------------------ CONSTRUCTION ------------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // Size::Size constexpr 
    //! Create empty size of zero Width and Height
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr
    Size() : Width(defvalue<T>()),
             Height(defvalue<T>()) 
    {}

    /////////////////////////////////////////////////////////////////////////////////////////
    // Size::Size constexpr 
    //! Create from Win32 size
    //!
    //! \param[in] const& sz - Input size
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr
    Size(const ::SIZE& sz) : Width(static_cast<T>(sz.cx)),
                             Height(static_cast<T>(sz.cy))
    {}

    /////////////////////////////////////////////////////////////////////////////////////////
    // Size::Size constexpr 
    //! Create from dimensions of any type
    //!
    //! \param[in] const w - Width
    //! \param[in] const h - Height
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename U> constexpr
    Size(const U w, const U h) : Width(static_cast<T>(w)),
                                 Height(static_cast<T>(h))
    {}

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    CONSTEXPR_COPY_CTOR(Size);      //!< Can be deep copied at compile-time
    CONSTEXPR_MOVE_CTOR(Size);      //!< Can be moved at compile-time
    ENABLE_COPY_ASSIGN(Size);       //!< Can be assigned
    ENABLE_MOVE_ASSIGN(Size);       //!< Can be move-assigned
    DISABLE_POLY(Size);             //!< Cannot be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Size::empty constexpr 
    //! Query whether size is empty
    //!
    //! \return bool - True iff all fields zero
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr
    bool empty() const 
    {
      return *this == EMPTY;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Size::operator == constexpr
    //! Equality operator
    //!
    //! \param[in] const& r - Another size
    //! \return bool - True iff co-ordinates equal
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr
    bool operator == (const type& r) const 
    {
      return Width  == r.Width
          && Height == r.Height;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Size::operator != constexpr
    //! Inequality operator
    //!
    //! \param[in] const& r - Another size
    //! \return bool - True iff co-ordinates unequal
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr
    bool operator != (const type& r) const 
    {
      return Width != r.Width
         || Height != r.Height;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Size::operator+ constexpr
    //! Calculate the result of enlarging by a given size
    //!
    //! \param[in] const& sz - Another size of any type
    //! \return type - Result of adding 'this' to 'sz'
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename U> constexpr
    type operator + (const Size<U>&  sz) const 
    {
      return type(Width + static_cast<T>(sz.Width),
                  Height + static_cast<T>(sz.Height));
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Size::operator- constexpr
    //! Calculate the result of subtracting a given size
    //!
    //! \param[in] const& sz - Another size of any type
    //! \return type - Result of subtracting 'sz' from 'this'
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename U> constexpr
    type operator - (const Size<U>&  pt) const 
    {
      return type(Width - static_cast<T>(sz.Width),
                  Height - static_cast<T>(sz.Height));
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Size::operator const ::COORD* const noexcept
    //! Implicit user-conversion to native ::COORD pointer
    //!
    //! \return const ::COORD* - Pointer to self as immutable ::COORD
    //! 
    //! \remarks Requires value_t model the Signed16BitFields concept
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename = void>
    operator const ::COORD* () const noexcept
    {
      concept_check(value_t,Signed16BitFields);

      return reinterpret_cast<const ::COORD*>(this);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Size::operator ::COORD const noexcept
    //! Implicit user-conversion to native ::COORD 
    //!
    //! \return ::COORD - Copy of current size as ::COORD
    //! 
    //! \remarks Requires value_t model the Signed16BitFields concept
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename = void>
    operator  ::COORD () const noexcept
    {
      concept_check(value_t,Signed16BitFields);

      return {Width,Height};
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Size::operator const ::SIZE* const noexcept
    //! Implicit user-conversion to native ::SIZE pointer
    //!
    //! \return const ::SIZE* - Pointer to self as immutable ::SIZE
    //! 
    //! \remarks Requires value_t model the Signed32BitFields concept
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename = void>
    operator const ::SIZE* () const noexcept
    {
      concept_check(value_t,Signed32BitFields);

      return reinterpret_cast<const ::SIZE*>(this);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Size::operator ::SIZE const noexcept
    //! Implicit user-conversion to native ::SIZE 
    //!
    //! \return ::SIZE - Copy of current size as ::SIZE
    //! 
    //! \remarks Requires value_t model the Signed32BitFields concept
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename = void>
    operator  ::SIZE () const noexcept
    {
      concept_check(value_t,Signed32BitFields);

      return {Width,Height};
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // Size::clear noexcept
    //! Reset all fields to zero
    /////////////////////////////////////////////////////////////////////////////////////////
    void  clear() noexcept
    {
      *this = EMPTY;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Size::operator += noexcept
    //! Enlarge by a given size
    //!
    //! \param[in] const& sz - Another size of any type
    //! \return type - Reference to self, updated by subtracting 'sz' from 'this'
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename U>
    type& operator += (const Size<U>&  sz) noexcept 
    {
      Width += static_cast<T>(sz.Width);
      Height += static_cast<T>(sz.Height);
      return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Size::operator -= noexcept
    //! Subtract by a given size
    //!
    //! \param[in] const& sz - Another size of any type
    //! \return type - Reference to self, updated by subtracting 'sz' from 'this'
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename U>
    type& operator -= (const Size<U>&  pt) noexcept
    {
      Width -= static_cast<T>(sz.Width);
      Height -= static_cast<T>(sz.Height);
      return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Size::operator ::COORD* noexcept
    //! Implicit user-conversion to native ::COORD pointer
    //!
    //! \return ::COORD* - Pointer to self as mutable ::COORD
    //! 
    //! \remarks Requires value_t model the Signed16BitFields concept
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename = void>
    operator ::COORD* () noexcept
    {
      concept_check(value_t,Signed16BitFields);

      return reinterpret_cast<::COORD*>(this);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Size::operator ::SIZE* noexcept
    //! Implicit user-conversion to native ::SIZE pointer
    //!
    //! \return ::SIZE* - Pointer to self as mutable ::SIZE
    //! 
    //! \remarks Requires value_t model the Signed32BitFields concept
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename = void>
    operator ::SIZE* () noexcept
    {
      concept_check(value_t,Signed32BitFields);

      return reinterpret_cast<::SIZE*>(this);
    }
  };

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \var Size<T>::EMPTY - 'Empty' sentinel value
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  const Size<T>  Size<T>::EMPTY;


  //! \alias SizeL - Size using long32_t fields (binary compatible with Win32 ::RECT)
  using SizeL = Size<long32_t>;

  //! \alias SizeF - Size using floating point fields
  using SizeF = Size<float>;




  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Prints a Size to the debug console
  //!
  //! \tparam T - Size field type
  //!
  //! \param[in,out] &c - Debugging console
  //! \param[in] const& sz - Size
  //! \return Console& - Reference to 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  Console& operator << (Console& c, const Size<T>& sz)
  {
    return c << make_nvpair_tuple("Width",  sz.Width,
                                  "Height", sz.Height);
  }


} // WTL namespace



#endif // WTL_SIZE_HPP


