#ifndef __VPUSH_EXCEPTION_HPP__
#define __VPUSH_EXCEPTION_HPP__

#include <stdexcept>
#include <string>
#include <vpush/util/typeinfo.hpp>

namespace vpush {

struct stack_underflow : public std::underflow_error {
	stack_underflow(util::TypeInfo t)
		: std::underflow_error (std::string("Stack underflow: ") + t.get().name())
		, _typeinfo(t) {}
	
	util::TypeInfo _typeinfo;
};

} // namespace vpush


#endif // __VPUSH_EXCEPTION_HPP__

