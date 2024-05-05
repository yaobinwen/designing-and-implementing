#pragma once

#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <memory>
#include <utility>

namespace ywen
{

/// A simple vector implementation. This vector does not try to implement the
/// standard C++ vector behavior (so some member functions do not match the
/// signatures of the standard vector). Instead, this vector is mainly for
/// study purpose.
///
/// Some outstanding differences than the standard C++ vector:
/// - No iterator.
/// - No allocator.
/// - Some member functions (e.g., `at`) do not throw exceptions.
/// - Prefer exception safety over complexity.
template<typename _Ty>
class vector
{
public:
  /// Construct an empty vector.
  constexpr vector() noexcept;

  /// Construct a vector using the initialization list.
  ///
  /// Throws:
  /// - `std::bad_alloc`: When out of memory.
  /// - Exceptions thrown by _Ty's copy assignment operator. This `vector` does
  ///   not catch them so the `vector` users must deal with them.
  constexpr vector(std::initializer_list<_Ty> init);

  /// Destructor (noexcept by default but I want it to be explicit).
  ~vector() noexcept;

  /// Append the given value to the end of the vector.
  ///
  /// Throws: see `insert`.
  constexpr void
  push_back(_Ty const & value);

  /// Remove the last element of the vector.
  ///
  /// Throws: see `erase`.
  constexpr void
  pop_back();

  /// Insert the given value at the specified location.
  ///
  /// Throws:
  /// - `std::bad_alloc`: When out of memory.
  /// - Exceptions thrown by _Ty's copy assignment operator. This `vector` does
  ///   not catch them so the `vector` users must deal with them.
  constexpr void
  insert(const size_t index, _Ty const & value);

  /// Erase the value at the specified location.
  ///
  /// Throws:
  /// - `std::bad_alloc`: When out of memory.
  /// - Exceptions thrown by _Ty's copy assignment operator. This `vector` does
  ///   not catch them so the `vector` users must deal with them.
  constexpr void
  erase(const size_t index);

  /// Get vector's size.
  constexpr size_t
  size() const noexcept;

  /// Get vector's capacity.
  constexpr size_t
  capacity() const noexcept;

  /// Check if the vector has no elements.
  constexpr bool
  empty() const noexcept;

  /// Return the reference to the ith (0-based) element, with bounds checking.
  constexpr _Ty &
  at(const size_t i) noexcept;

  /// Return the constant reference to the ith (0-based) element, with bounds
  /// checking.
  constexpr _Ty const &
  at(const size_t i) const noexcept;

  /// Return the reference to the ith (0-based) element, with bounds checking.
  constexpr _Ty &
  operator[](const size_t i) noexcept;

  /// Return the constant reference to the ith (0-based) element, with bounds
  /// checking.
  constexpr _Ty const &
  operator[](const size_t i) const noexcept;

  /// Return the raw pointer to the underlying array.
  constexpr _Ty *
  data() noexcept;

  /// Return the raw pointer to the underlying array.
  constexpr const _Ty *
  data() const noexcept;

private:
  /// Return the new capacity based on the given capacity.
  static size_t
  _get_new_capacity(const size_t capacity) noexcept;

private:
  /// The current number of elements inside the vector.
  ///
  /// Invariants:
  /// - `m_size <= m_capacity`.
  unsigned int m_size;

  /// The number of total slots that the vector can use to store elements
  /// having to allocate more slots.
  ///
  /// Invariants:
  /// - `m_size <= m_capacity`
  unsigned int m_capacity;

  /// The raw pointer to the underlying memory storage.
  ///
  /// Invariants:
  /// - `0 == m_size && nullptr == m_vec`, OR
  /// - `0 < m_size && nullptr != m_vec`.
  _Ty * m_vec;
};

template<class _Ty>
constexpr vector<_Ty>::vector() noexcept
  : m_size(0), m_capacity(0), m_vec(nullptr)
{
  // Empty
}

template<class _Ty>
constexpr vector<_Ty>::vector(std::initializer_list<_Ty> init)
{
  const size_t count = init.size();  // `size()` does not throw.

  if (0 == count)
  {
    assert((nullptr == m_vec));
    assert((0 == m_size));
    assert((0 == m_capacity));

    return;
  }

  // `new` may throw `std::bad_alloc`
  std::unique_ptr<_Ty[]> new_vec(new _Ty[count]);

  size_t i = 0;
  for (typename std::initializer_list<_Ty>::iterator it = init.begin();
       it != init.end();
       ++it)
  {
    // _Ty's copy assignment operator may throw.
    new_vec[i] = *it;
    ++i;
  }

  _Ty * tmp_vec = new_vec.release();  // `release()` does not throw.

  std::swap(tmp_vec, m_vec);  // `std::swap()` does not throw.
  m_size = count;
  m_capacity = count;

  assert((nullptr != m_vec));
  assert((count == m_size));
  assert((count == m_capacity));
}

template<class _Ty>
vector<_Ty>::~vector() noexcept
{
  // NOTE(ywen): Ideally, _Ty's destructor should not throw. In reality, it
  // may throw. Because this is library code, we want to propagate the
  // exception to the caller. The caller may or may not be able to catch the
  // exception and handle it, but it's up to them.
  if (m_vec != nullptr)
  {
    delete[] m_vec;
    m_vec = nullptr;
  }

  m_size = 0;
  m_capacity = 0;

  assert((nullptr == m_vec));
  assert((0 == m_size));
  assert((0 == m_capacity));
}

template<class _Ty>
constexpr void
vector<_Ty>::push_back(_Ty const & value)
{
  this->insert(m_size, value);
}

template<class _Ty>
constexpr void
vector<_Ty>::pop_back()
{
  this->erase(m_size - 1);
}

template<class _Ty>
constexpr void
vector<_Ty>::insert(const size_t index, _Ty const & value)
{
  assert((0U <= index));
  assert((index <= m_size));

  const size_t prev_size = m_size;
  const size_t prev_capacity = m_capacity;

  const size_t new_capacity =
    (m_size + 1 > m_capacity ? _get_new_capacity(m_capacity) : m_capacity);
  const size_t new_size = m_size + 1;

  // `new` may throw `std::bad_alloc`.
  std::unique_ptr<_Ty[]> new_vec(new _Ty[m_capacity]);

  // Copy the first half (i.e., before the position that `index` points at) to
  // the same location in the new vector.
  for (size_t i = 0; i < index; ++i)
  {
    // If _Ty's copy assignment throws, `new_vec` will de-allocate the
    // temporary vector so no resource leaks.
    new_vec[i] = m_vec[i];
  }

  // Copy the second half (i.e., after the position that `index` points at) to
  // the location with 1 offset in the new vector.
  for (size_t i = m_size; i > index; --i)
  {
    // If _Ty's copy assignment throws, `new_vec` will de-allocate the
    // temporary vector so no resource leaks.
    new_vec[i] = m_vec[i - 1];
  }

  // If _Ty's copy assignment throws, `new_vec` will de-allocate the temporary
  // vector so no resource leaks.
  new_vec[index] = value;

  // Now the temporary vector has been initialized successfully, we can
  // manipulate the raw pointer without worrying about memory leak as long as
  // we make sure no exception is thrown.
  _Ty * tmp_vec = new_vec.release();  // release() doesn't throw.
  std::swap(tmp_vec, m_vec);          // std::swap() doesn't throw.

  // Ideally, deleting the array should not throw. If it throws because the
  // destructor throws, we can't handle it gracefully and have to terminate
  // anyway. Should that happen, we still wouldn't have resource leak and the
  // size and capacity would still be correct.
  delete[] tmp_vec;

  // Set capacity before size to make sure capacity is always >= size, which
  // is a valid state. (In contrast, capacity < size is an invalid state.)
  m_capacity = new_capacity;
  m_size = new_size;

  assert((nullptr != m_vec));
  assert((prev_size + 1 == m_size));
  assert((m_size <= m_capacity));
  assert((prev_capacity <= m_capacity));
}

template<class _Ty>
constexpr void
vector<_Ty>::erase(const size_t index)
{
  assert(0U < m_size);
  assert((0U <= index));
  assert((index < m_size));

  const size_t prev_size = m_size;
  const size_t prev_capacity = m_capacity;
  const size_t new_size = m_size - 1;

  if (index == m_size - 1)
  {
    // When we erase at the end, we actually don't need to do anything except
    // adjusting the size.
  }
  else
  {
    // Erase at the head or in the middle, then we need to do more.

    // When we erase an element, we can keep using the existing capacity.
    // `new` may throw `std::bad_alloc`.
    std::unique_ptr<_Ty[]> new_vec(new _Ty[m_capacity]);

    // Copy the first half (i.e., before the position that `index` points at)
    // to the same location in the new vector.
    for (size_t i = 0; i < index; ++i)
    {
      // If _Ty's copy assignment throws, `new_vec` will de-allocate the
      // temporary vector so no resource leaks.
      new_vec[i] = m_vec[i];
    }

    // Copy the second half (i.e., after the position that `index` points at)
    // to the location with 1 offset in the new vector.
    for (size_t i = index; i < new_size; ++i)
    {
      // If _Ty's copy assignment throws, `new_vec` will de-allocate the
      // temporary vector so no resource leaks.
      new_vec[i] = m_vec[i + 1];
    }

    _Ty * tmp_vec = new_vec.release();  // `release()` doesn't throw.
    std::swap(tmp_vec, m_vec);          // `std::swap()` doesn't throw.

    // Ideally, deleting the array should not throw. If it throws because the
    // destructor throws, we can't handle it gracefully and have to terminate
    // anyway. Should that happen, we still wouldn't have resource leak and the
    // size and capacity would still be correct.
    delete[] tmp_vec;
  }

  m_size = new_size;

  assert((nullptr != m_vec));
  assert((m_size == prev_size - 1));
  assert((m_capacity == prev_capacity));
}

template<class _Ty>
constexpr size_t
vector<_Ty>::size() const noexcept
{
  return m_size;
}

template<class _Ty>
constexpr size_t
vector<_Ty>::capacity() const noexcept
{
  return m_capacity;
}

template<class _Ty>
constexpr bool
vector<_Ty>::empty() const noexcept
{
  return 0 == m_size;
}

template<class _Ty>
constexpr _Ty &
vector<_Ty>::at(const size_t i) noexcept
{
  return const_cast<_Ty &>(static_cast<vector<_Ty> const *>(this)->at(i));
}

template<class _Ty>
constexpr _Ty const &
vector<_Ty>::at(const size_t i) const noexcept
{
  assert((0 <= i));
  assert((i < m_size));

  return m_vec[i];
}

template<class _Ty>
constexpr _Ty &
vector<_Ty>::operator[](const size_t i) noexcept
{
  return this->at(i);
}

template<class _Ty>
constexpr _Ty const &
vector<_Ty>::operator[](const size_t i) const noexcept
{
  return this->at(i);
}

template<class _Ty>
constexpr _Ty *
vector<_Ty>::data() noexcept
{
  return m_vec;
}

template<class _Ty>
constexpr const _Ty *
vector<_Ty>::data() const noexcept
{
  return m_vec;
}

template<class _Ty>
size_t
vector<_Ty>::_get_new_capacity(const size_t capacity) noexcept
{
  return static_cast<size_t>(capacity * 2 + 1);
}

}  // namespace ywen
