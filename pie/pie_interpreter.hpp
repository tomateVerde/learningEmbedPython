#ifndef PIE_INTERPRETER_HPP
#define PIE_INTERPRETER_HPP

// Standard
#include <memory>
#include <iostream>
#include <string_view>
#include <utility>
#include <vector>

// Local
#include "pie_args.hpp"
#include "pie_module.hpp"

namespace pie
{

class Interpreter
{
public:
  
  ////////////////////////////////////////////////////////////////////////////
  static class Interpreter& instance()
  {
    static Interpreter instance;

    return instance;
  }

  ////////////////////////////////////////////////////////////////////////////
  Interpreter ( const Interpreter & )  = delete;
  Interpreter ( Interpreter && )       = delete;
  void operator=(Interpreter const&)   = delete;
  void operator=(Interpreter const&&)  = delete;

  ////////////////////////////////////////////////////////////////////////////
  ~Interpreter()
  {
    Py_FinalizeEx();
  }

/// @TODO instead of std::pair, maybe make my own class so i can do something like
/// "foo"."func"
/// {"foo"}.("func")... I don't know, think about it
  ////////////////////////////////////////////////////////////////////////////
  template<class... Args>
  PyObject* call_function(const std::pair<std::string_view, std::string_view>& mod_func, Args... args)
  {
    for (const auto& module_ptr : modules)
    {
      if (module_ptr->name() == mod_func.first)
      {
        return module_ptr->function(mod_func.second.data(), std::forward<Args>(args)...);
      }
    }

    return nullptr;
  }

  ////////////////////////////////////////////////////////////////////////////
  PyObject* call_function(const std::pair<std::string_view, std::string_view>& mod_func)
  {
    for (const auto& module_ptr : modules)
    {
      if (module_ptr->name() == mod_func.first)
      {
        return module_ptr->function(mod_func.second.data());
      }
    }

    return nullptr;
  }

  ////////////////////////////////////////////////////////////////////////////
  bool import_module(const char * const str)
  {
    try
    {
      modules.emplace_back(std::make_unique<pie::Module>(str));

      return true;
    }
    catch(const std::exception& e)
    {
      std::cerr << e.what() << '\n';
      return false;
    }
  }

private:

  ////////////////////////////////////////////////////////////////////////////
  Interpreter()
  {
    Py_Initialize();
  }

  std::vector<std::unique_ptr<pie::Module>> modules;
};

} // end namespace pie

#endif // PIE_INTERPRETER_HPP