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
	stack_underflow(const stack_underflow& e, std::string a)
		: std::underflow_error (std::string(e.what()) + a)
		, _typeinfo(e._typeinfo) {}
	
	util::TypeInfo _typeinfo;
};

struct duplicate_code : public std::runtime_error {
	duplicate_code(std::string name)
		: std::runtime_error(std::string("Duplicate code name: ") + name) {}
};

struct no_such_stack : public std::runtime_error {
	no_such_stack(util::TypeInfo t)
		: std::runtime_error(std::string("No such stack: ") + t.get().name()) {}
};

struct no_such_function : public std::runtime_error {
	no_such_function(const std::string name)
		: std::runtime_error(std::string("No such function: ") + name) {}
};

struct unmatched_brackets : public std::runtime_error {
	unmatched_brackets()
		: std::runtime_error(std::string("Unmatched brackets")) {}
};

} // namespace detail
} // namespace vpush


#endif // __VPUSH_EXCEPTION_HPP__

