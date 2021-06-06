#ifndef PIE_ARGS_HPP
#define PIE_ARGS_HPP

/// Standard
#include <array>
#include <string_view>
#include <type_traits>
#include <vector>

/// Libraries
#include <Python.h>

namespace pie
{

namespace
{
  //////////////////////////////////////////////////////
  /// @brief Create PyObjects* from C++ types
  inline PyObject* const _add_item(PyObject* const pyObj)
  {
    return pyObj;
  }

  template<typename T,
          std::enable_if_t<std::is_integral<T>::value &&
                          std::is_signed<T>::value, bool> = true>
  PyObject* const _add_item(const T value)
  {
    return PyLong_FromLongLong(value);
  }

  template<typename T,
          std::enable_if_t<std::is_integral<T>::value &&
                          !std::is_signed<T>::value, bool> = true>
  PyObject* const _add_item(const T value)
  {
    return PyLong_FromUnsignedLongLong(value);
  }

  template<typename T, 
          std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
  PyObject* const _add_item(const T value)
  {
    return PyFloat_FromDouble(value);
  }

  inline PyObject* const _add_item(const bool value)
  {
    return PyBool_FromLong(value);
  }

  inline PyObject* const _add_item(const char* value)
  {
    return PyBytes_FromString(value);
  }

  inline PyObject* const _add_item(const std::string_view& str)
  {
    return PyByteArray_FromStringAndSize(str.data(), str.size());
  }

  PyObject* const _add_item(const void* ptr, const uint32_t size)
  {
    return PyByteArray_FromStringAndSize(reinterpret_cast<const char*>(ptr), size);
  }

  template<typename T, std::size_t N>
  PyObject* const _add_item(std::array<T, N>& arr)
  {
    return _add_item(arr.data(), arr.size() * sizeof(T));
  }

  template<typename T>
  PyObject* const _add_item(std::vector<T>& vec)
  {
    return _add_item(vec.data(), vec.size() * sizeof(T));
  }

  //////////////////////////////////////////////////////
  /// @brief Recursive calls to set PyTuple
  template<class T>
  PyObject* _create_PyArgs(PyObject* pArgs, const uint32_t index, T item)
  {
    PyTuple_SET_ITEM(pArgs, index, _add_item(item));

    return pArgs;
  }

  template<class T, class... Args>
  PyObject* _create_PyArgs(PyObject* pArgs, const uint32_t index, T item, Args... args)
  {
    PyTuple_SET_ITEM(pArgs, index, _add_item(item));

    return _create_PyArgs(pArgs, index + 1, args...);
  }

} // anonymous namespace

//////////////////////////////////////////////////////
template<class T, class... Args>
PyObject* create_PyArgs(T item, Args... args)
{
  return _create_PyArgs(PyTuple_New(sizeof...(args) + 1), 0, item, args...);;
}

} // namespace pie

#endif // PIE_ARGS_HPP
