#ifndef __VPUSH_EXCEPTION_HPP__
#define __VPUSH_EXCEPTION_HPP__

#include <stdexcept>
#include <string>
#include <vpush/util/typeinfo.hpp>

namespace vpush {
namespace detail {

struct stack_underflow : public std::underflow_error {
	stack_underflow(util::TypeInfo t)
		: std::underflow_error (std::string("Stack underflow: ") + t.get().name())
		, _typeinfo(t) {}
	
	util::TypeInfo _typeinfo;
};

struct duplicate_code : public std::runtime_error {
	duplicate_code(std::string name)
		: std::runtime_error(std::string("Duplicate code name: ") + name) {}
};

struct no_such_stack : public std::runtime_error {
	no_such_stack(util::TypeInfo t)
		: std::runtime_error(std::string("No such stack: ") + t.get().name() + " (use env.make_stack())") {}
};

struct no_such_function : public std::runtime_error {
	no_such_function(const std::string name)
		: std::runtime_error(std::string("No such function: ") + name + " (check for env.register_(...))") {}
};

struct no_functions : public std::runtime_error {
	no_functions()
		: std::runtime_error("No functions") {}
};

} // namespace detail
} // namespace vpush


#endif // __VPUSH_EXCEPTION_HPP__

