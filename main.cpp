#include <string>

#include "pie/pie_interpreter.hpp"

/////
#include <iostream>
/////

int main(int argc, char** argv)
{
  // Singleton to avoid calling `Py_FinalizeEx` twice
  pie::Interpreter& foo(pie::Interpreter::instance());

  foo.import_module("example");

  const std::string aString("This is a string");
  const size_t aInt = 246;
  // const int32_t aInt = 246;
  // const double aFloat = 3.1415;

  std::array<float, 7> arr = {3.14, 3.45, 4.65, 3, 4,5 ,6};

  // Calling a function without some args
  PyObject* pValue = 
    // foo.call_function("print_args", false);
    // foo.call_function("print_args", "Hello world!!");
    // foo.call_function("print_args", "Hello world!!", aString, aInt, aFloat);
    foo.call_function({"example", "print_args"}, "Hello world!!", aString, aInt, arr);
  
  if (pValue)
  {
    Py_XDECREF(pValue);
  }

  // Calling a function without args but returns a byte array
  pValue = foo.call_function({"example", "no_args"});

  if (pValue && PyByteArray_Check(pValue) > 0)
  {
    std::string output(PyByteArray_AsString(pValue));
    std::cout << "From Python: " << output << std::endl;
  }

  if (pValue)
  {
    Py_XDECREF(pValue);
  }

  foo.import_module("base64");

  pValue = foo.call_function({"base64", "b64encode"}, "foobar");

  if (pValue && PyBytes_Check(pValue) > 0)
  {
    std::string output(PyBytes_AsString(pValue));
    std::cout << "From Python: " << output << std::endl;
  }

  Py_XDECREF(pValue);

  return 0;
}
