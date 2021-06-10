#ifndef PIE_INTERPRETER_HPP
#define PIE_INTERPRETER_HPP

// Standard
#include <memory>
#include <string_view>

// Local
#include "pie_args.hpp"
#include "pie_module.hpp"

namespace pie
{

class Interpreter
{
public:
  /// @TODO make this guy the only one that can create modules
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

  std::unique_ptr<pie::Module> module(const char * const name)
  {
    return std::make_unique<pie::Module>(name);
  }

private:

  ////////////////////////////////////////////////////////////////////////////
  Interpreter()
  {
    Py_Initialize();
  }
};

} // end namespace pie

#endif // PIE_INTERPRETER_HPP