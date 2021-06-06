
#ifndef PIE_MODULE_HPP
#define PIE_MODULE_HPP

/// Standard
#include <string>

/// Libraries
#include <Python.h>

/// Local
#include "pie_args.hpp"

namespace pie
{

class Module
{
public:

  ////////////////////////////////////////////////////////////////////////////
  Module(const char * const module_name) :
    module_(PyImport_ImportModule(module_name)),
    module_name_(module_name)
  {
    Py_XINCREF(module_);
  }

  ~Module()
  {
    Py_XDECREF(module_);
  }

  ////////////////////////////////////////////////////////////////////////////
  Module& operator= (const Module& other)
  {
    this->module_name_ = other.module_name_;
    this->module_ = other.module_;

    Py_XINCREF(this->module_);

    return *this;
  }

  Module (const Module& other)
  {
    this->module_name_ = other.module_name_;
    this->module_ = other.module_;

    Py_XINCREF(this->module_);
  }

  ////////////////////////////////////////////////////////////////////////////
  /// @brief Returns name of module
  const std::string name() const
  {
    return module_name_;
  }

  ////////////////////////////////////////////////////////////////////////////
  /// @brief Calls the function `func_name` from this module
  /// @return `nullptr` on failure
  template<class... Args>
  PyObject* function(const char* const func_name, Args... args) const
  {
    PyObject* func = PyObject_GetAttrString(module_, func_name);

    PyObject* py_return = nullptr;

    if (func && PyCallable_Check(func)) 
    {
      PyObject* py_args = create_PyArgs(std::forward<Args>(args)...);

      py_return = PyObject_CallObject(func, py_args);

      Py_XDECREF(py_args);
    }

    Py_XDECREF(func);

    return py_return;
  }

  ////////////////////////////////////////////////////////////////////////////
  /// @brief Calls the function `func_name` from this module
  /// @return `nullptr` on failure
  PyObject* function(const char* const func_name) const
  {
    PyObject* func = PyObject_GetAttrString(module_, func_name);

    PyObject* py_return = nullptr;

    if (func && PyCallable_Check(func)) 
    {
      py_return = PyObject_CallObject(func, nullptr);
    }

    Py_XDECREF(func);

    return py_return;
  }

private:

  PyObject * module_;
  std::string module_name_;

};

} // end namespace pie

#endif // PIE_MODULE_HPP
