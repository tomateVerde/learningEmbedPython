#include <array>
#include <Python.h>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

namespace pie
{

namespace
{
  //////////////////////////////////////////////////////
  /// @brief Create PyObjects* from C++ types
  inline PyObject* const _add_arg(PyObject* const pyObj)
  {
    return pyObj;
  }

  template<typename T,
          std::enable_if_t<std::is_integral<T>::value &&
                          std::is_signed<T>::value, bool> = true>
  PyObject* const _add_arg(const T value)
  {
    return PyLong_FromLongLong(value);
  }

  template<typename T,
          std::enable_if_t<std::is_integral<T>::value &&
                          !std::is_signed<T>::value, bool> = true>
  PyObject* const _add_arg(const T value)
  {
    return PyLong_FromUnsignedLongLong(value);
  }

  template<typename T, 
          std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
  PyObject* const _add_arg(const T value)
  {
    return PyFloat_FromDouble(value);
  }

  inline PyObject* const _add_arg(const bool value)
  {
    return PyBool_FromLong(value);
  }

  inline PyObject* const _add_arg(const char* value)
  {
    return PyBytes_FromString(value);
  }

  inline PyObject* const _add_arg(const std::string& str)
  {
    return PyByteArray_FromStringAndSize(str.c_str(), str.size());
  }

  PyObject* const _add_arg(const void* ptr, const uint32_t size)
  {
    return PyByteArray_FromStringAndSize(reinterpret_cast<const char*>(ptr), size);
  }

  template<typename T, std::size_t N>
  PyObject* const _add_arg(std::array<T, N>& arr)
  {
    return _add_arg(arr.data(), arr.size() * sizeof(T));
  }

  template<typename T>
  PyObject* const _add_arg(std::vector<T>& vec)
  {
    return _add_arg(vec.data(), vec.size() * sizeof(T));
  }

  //////////////////////////////////////////////////////
  /// @brief Recursive calls to set PyTuple
  template<class T>
  PyObject* _create_PyArgs(PyObject* pArgs, const uint32_t index, T arg)
  {
    PyTuple_SET_ITEM(pArgs, index, _add_arg(arg));

    return pArgs;
  }

  template<class T, class... Args>
  PyObject* _create_PyArgs(PyObject* pArgs, const uint32_t index, T arg, Args... args)
  {
    PyTuple_SET_ITEM(pArgs, index, _add_arg(arg));

    return _create_PyArgs(pArgs, index + 1, args...);
  }

} // anonymous namespace

//////////////////////////////////////////////////////
template<class T, class... Args>
PyObject* createPyArgs(T arg, Args... args)
{
  return _create_PyArgs(PyTuple_New(sizeof...(args) + 1), 0, arg, args...);;
}

} // namespace pie
