#include <string>

#include "pie/pie_interpreter.hpp"

/////
#include <iostream>
/////

int main(int argc, char** argv)
{
  std::unique_ptr<pie::Module> foo;
  
  // Singleton to avoid calling `Py_FinalizeEx` twice
  pie::Interpreter& interpreter(pie::Interpreter::instance());

  auto example_mod = interpreter.module("example");

  const std::string aString("This is a string");
  const size_t aInt = 246;


  std::array<float, 7> arr = {3.14, 3.45, 4.65, 3, 4,5 ,6};

  // Calling a function without some args
  PyObject* pValue = 
    // example_mod->function("print_args", false);
    // example_mod->function("print_args", "Hello world!!");
    // example_mod->function("print_args", "Hello world!!", aString, aInt, aFloat);
    example_mod->function("print_args", "Hello world!!", aString, aInt, arr);
  
  if (pValue)
  {
    Py_XDECREF(pValue);
  }

  interpreter.module("example")->function("print_args", "Hello world!! NUMERO 2", aString, aInt, arr);

  // Calling a function without args but returns a byte array
  pValue = example_mod->function("no_args");

  if (pValue && PyByteArray_Check(pValue) > 0)
  {
    std::string output(PyByteArray_AsString(pValue));
    std::cout << "From Python: " << output << std::endl;
  }

  if (pValue)
  {
    Py_XDECREF(pValue);
  }

  pValue = interpreter.module("base64")->function("b64encode", "foobar");
  // pValue = interpreter.module("base64")->function("b64encode")("foobar");
  // pValue = interpreter.module("base64")->function("b64encode")(); // @TODO? Maybe looks kinda kool TM

  if (pValue && PyBytes_Check(pValue) > 0)
  {
    std::string output(PyBytes_AsString(pValue));
    std::cout << "From Python: " << output << std::endl;
  }

  Py_XDECREF(pValue);

  return 0;
}
