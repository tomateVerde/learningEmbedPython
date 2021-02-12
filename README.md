# Description

Recently learned a little bit about Python's C API. Decided to make an example code for me to reference in the future.

```c++
int main(int argc, char** argv)
{
  // Singleton to avoid calling `Py_FinalizeEx` twice
  PyModule& foo(PyModule::getInstance());

  foo.importModule("example");

  const std::string aString("This is a string");
  const uint32_t aInt = 246;
  const double aFloat = 3.1415;

  // Calling a function without some args
  PyObject* pValue = 
    foo.callFunction("print_args", "Hello world!!", aString, aInt, aFloat);
  
  if (pValue)
  {
    Py_XDECREF(pValue);
  }

  // Calling a function without args but returns a byte array
  pValue = foo.callFunction("no_args", NULL);

  if (pValue && PyByteArray_Check(pValue) > 0)
  {
    std::string output(PyByteArray_AsString(pValue));
    std::cout << "From Python: " << output << std::endl;
  }

  Py_XDECREF(pValue);

  return 0;
}
```