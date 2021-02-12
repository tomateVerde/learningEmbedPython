#include <Python.h>
#include <string>
#include <type_traits>

namespace
{
//////////////////////////////////////////////////////
inline PyObject* const _addArg(std::string str)
{
  return PyByteArray_FromStringAndSize(str.c_str(), str.size());
}

/////////////////////////////////////////////////////
inline PyObject* const _addArg(PyObject* pyObj)
{
  return pyObj;
}

//////////////////////////////////////////////////////
template<typename T,
        std::enable_if_t<std::is_integral<T>::value, bool> = true>
PyObject* const _addArg(const T value)
{
  return PyLong_FromLong(value);
}

//////////////////////////////////////////////////////
template<typename T, 
        std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
PyObject* const _addArg(const T value)
{
  return PyFloat_FromDouble(value);
}

//////////////////////////////////////////////////////
template<class Arg>
PyObject* _createPyArgs(PyObject* pArgs, const uint32_t argIndex, Arg arg)
{
  PyObject* pValue = _addArg(arg);
  
  PyTuple_SetItem(pArgs, argIndex, pValue);

  return pArgs;
}

//////////////////////////////////////////////////////
template<class Arg, class... Args>
PyObject* _createPyArgs(PyObject* pArgs, const uint32_t argIndex, Arg arg, Args... otherArgs)
{
  PyObject* pValue = _addArg(arg);
  
  PyTuple_SetItem(pArgs, argIndex, pValue);

  _createPyArgs(pArgs, argIndex + 1, otherArgs...);

  return pArgs;
}

} // anonymous namespace

//////////////////////////////////////////////////////
template<class Arg, class... Args>
PyObject* createPyArgs(Arg arg, Args... otherArgs)
{
  if (sizeof...(otherArgs) > 0)
  {
    PyObject* pArgs = PyTuple_New(sizeof...(otherArgs) + 1);

    _createPyArgs(pArgs, 0, arg, otherArgs...);

    return pArgs;
  }

  return nullptr;
}
