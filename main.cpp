#include <filesystem>
#include <Python.h>
#include <string>
#include <utility>
#include <vector>


#include "pie/pie_args.hpp"
#include "pie/pie_module.hpp"

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
    Py_FinalizeEx();
  }

  ///////////////////////////////////////////////
  template<class... Args>
  PyObject* callFunction(const char* const functionName, Args... args)
  {
    for (const auto& pie_mod : pie_modules)
    {
      return pie_mod.function(functionName, std::forward<Args>(args)...);
    }

    return NULL;
  }

  PyObject* callFunction(const char* const functionName)
  {
    for (const auto& pie_mod : pie_modules)
    {
      return pie_mod.function(functionName);
    }

    return NULL;
  }

  ///////////////////////////////////////////////
  bool importModule(const char * const py_module)
  {
    pie_modules.emplace_back(py_module);

    return true;
  }

private:

  ///////////////////////////////////////////////
  PyModule()
  {
    Py_Initialize();
  }

  std::vector<pie::Module> pie_modules;
};

int main(int argc, char** argv)
{
  // Singleton to avoid calling `Py_FinalizeEx` twice
  PyModule& foo(PyModule::getInstance());

  foo.importModule("example");

  const std::string aString("This is a string");
  const size_t aInt = 246;
  // const int32_t aInt = 246;
  // const double aFloat = 3.1415;

  std::array<float, 7> arr = {3.14, 3.45, 4.65, 3, 4,5 ,6};

  // Calling a function without some args
  PyObject* pValue = 
    // foo.callFunction("print_args", false);
    // foo.callFunction("print_args", "Hello world!!");
    // foo.callFunction("print_args", "Hello world!!", aString, aInt, aFloat);
    foo.callFunction("print_args", "Hello world!!", aString, aInt, arr);
  
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

  if (pValue)
  {
    Py_XDECREF(pValue);
  }

  foo.importModule("base64");

  pValue = foo.callFunction("b64encode", "foo");

  if (pValue)
    std::cout << "Good pValue" << std::endl;

  if (pValue && PyBytes_Check(pValue) > 0)
  {
    std::string output(PyBytes_AsString(pValue));
    std::cout << "From Python: " << output << std::endl;
  }

  Py_XDECREF(pValue);

  return 0;
}
