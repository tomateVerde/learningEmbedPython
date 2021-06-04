#include <filesystem>
#include <Python.h>
#include <string>
#include <utility>
#include <vector>

#include "helper.hpp"

/////
#include <iostream>
/////

class PyModule
{
public:
  
  ///////////////////////////////////////////////
  static class PyModule& getInstance()
  {
    static PyModule instance;

    return instance;
  }

  ///////////////////////////////////////////////
  PyModule ( const PyModule & )     = delete;
  PyModule ( PyModule && )          = delete;
  void operator=(PyModule const&)   = delete;
  void operator=(PyModule const&&)  = delete;

  ///////////////////////////////////////////////
  ~PyModule()
  {
    for (auto pModule :  py_modules)
    {
      Py_XDECREF(pModule);
    }

    Py_FinalizeEx();
  }

  ///////////////////////////////////////////////
  template<class... Args>
  PyObject* callFunction(const char* const functionName, Args... args)
  {
    for (auto pModule :  py_modules)
    {
      PyObject* pFunc = PyObject_GetAttrString(pModule, functionName);

      if (pFunc && PyCallable_Check(pFunc)) 
      {
        
        PyObject* pArgs = pie::createPyArgs(std::forward<Args>(args)...);

        PyObject* pValue = PyObject_CallObject(pFunc, pArgs);

        Py_XDECREF(pArgs);
        Py_XDECREF(pFunc);

        if (pValue)
        {
          return pValue;
        }
      }
    }

    return NULL;
  }

  PyObject* callFunction(const char* const functionName)
  {
    for (auto pModule :  py_modules)
    {
      PyObject* pFunc = PyObject_GetAttrString(pModule, functionName);

      if (pFunc && PyCallable_Check(pFunc)) 
      {
        PyObject* pValue = PyObject_CallObject(pFunc, nullptr);

        Py_XDECREF(pFunc);

        if (pValue)
        {
          return pValue;
        }
      }
    }

    return NULL;
  }

  ///////////////////////////////////////////////
  bool importModule(const char * const py_module)
  {
    const std::filesystem::path module_path(py_module);

    // Need to add the module to the system path
    const std::string sysPathAppend = 
      std::string("sys.path.append(\"") + 
      module_path.parent_path().string() +
      std::string("\")");
    
    PyRun_SimpleString(sysPathAppend.c_str());

    const std::string moduleName = module_path.filename().string();

    PyObject* pModule = PyImport_ImportModule(moduleName.c_str());

    if (!pModule) 
    {
      Py_XDECREF(pModule);
      return false;
    }

    py_modules.push_back(pModule);

    return true;
  }

private:

  ///////////////////////////////////////////////
  PyModule()
  {
    Py_Initialize();

    PyRun_SimpleString("import sys");
  }

  std::vector<PyObject *> py_modules;
};

int main(int argc, char** argv)
{
  // Singleton to avoid calling `Py_FinalizeEx` twice
  PyModule& foo(PyModule::getInstance());

  foo.importModule("example");

  const std::string aString("This is a string");
  const size_t aInt = 246;
  // const int32_t aInt = 246;
  const double aFloat = 3.1415;

  // Calling a function without some args
  PyObject* pValue = 
    foo.callFunction("print_args", "Hello world!!");
    // foo.callFunction("print_args", "Hello world!!", aString, aInt, aFloat);
  
  if (pValue)
  {
    Py_XDECREF(pValue);
  }

  // Calling a function without args but returns a byte array
  pValue = foo.callFunction("no_args");

  if (pValue && PyByteArray_Check(pValue) > 0)
  {
    std::string output(PyByteArray_AsString(pValue));
    std::cout << "From Python: " << output << std::endl;
  }

  Py_XDECREF(pValue);

  return 0;
}
