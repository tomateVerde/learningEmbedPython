#ifndef PIE_ACCESSORS_HPP
#define PIE_ACCESSORS_HPP

/// Standard
#include <optional>
#include <string>

/// Libraries
#include <Python.h>

namespace pie
{

template<typename T>
std::optional<T> get(PyObject* const pyObj)
{
  return std::nullopt;
}

template<>
std::optional<std::string> get<std::string>(PyObject* const pyObj)
{
  if (pyObj && PyBytes_Check(pyObj) > 0)
  {
    return PyBytes_AsString(pyObj);
  }

  return std::nullopt;
}

// @TODO Add more specializations

} // namespace pie

#endif // PIE_ACCESSORS_HPP