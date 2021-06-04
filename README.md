# Description

Recently learned a little bit about Python's C API. Decided to make an example code for me to reference in the future.

Here is the example:

```python
def print_args(a, b , c, d):

  print("Hello World from python")
  print("--- Received {}".format(a))
  print("--- Received {}".format(b))
  print("--- Received {}".format(c))
  print("--- Received {}".format(d))

def no_args():

  return bytearray(b'This is a function with no args')
```

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
Output: 
```
Hello World from python
--- Received bytearray(b'Hello world!!')
--- Received bytearray(b'This is a string')
--- Received 246
--- Received 3.1415
From Python: This is a function with no args
```
## Dependencies

+ C++ 17
+ CMake version `3.20.3`
+ MinGW 32bit version
    + Make sure the bin and lib folder are added to the path. E.g. `C:\MinGW\bin` and `C:\MinGW\lib`
    + MinGw GCC version `(MinGW.org GCC Build-2) 9.2.0`
+ Python `3.9 32bit`

## Troubleshooting

+ Having trouble linking?
    + Make sure MinGW and the Python lib are the same 32 bits or 64 bits version
+ Running into `windows error while loading shared libraries: ?: cannot open shared object file: No such file or directory`
    + Make sure MinGW is part of your path