#include <Python.h>
#include <string>
#include <type_traits>

#include <iostream>

namespace pie
{

namespace
{
//////////////////////////////////////////////////////
/// @brief Create PyObjects* from C++ types
inline PyObject* const _add_arg(const std::string& str)
{
  return PyByteArray_FromStringAndSize(str.c_str(), str.size());
}

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

//////////////////////////////////////////////////////
/// @brief Recursive calls to set PyTuple
template<class Arg>
PyObject* _create_PyArgs(PyObject* pArgs, const uint32_t index, Arg arg)
{
  PyTuple_SetItem(pArgs, index, _add_arg(arg));

  return pArgs;
}

template<class Arg, class... Args>
PyObject* _create_PyArgs(PyObject* pArgs, const uint32_t index, Arg arg, Args... otherArgs)
{
  PyTuple_SetItem(pArgs, index, _add_arg(arg));

  _create_PyArgs(pArgs, index + 1, otherArgs...);

  return pArgs;
}

} // anonymous namespace


//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
template<class Arg, class... Args>
PyObject* createPyArgs(Arg arg, Args... otherArgs)
{
  PyObject* pArgs = PyTuple_New(sizeof...(otherArgs) + 1);

  _create_PyArgs(pArgs, 0, arg, otherArgs...);

  return pArgs;
}



} // namespace pie
