#ifndef __VPUSH_CODE_HPP__
#define __VPUSH_CODE_HPP__

#include <boost/function_types/result_type.hpp>
#include <boost/function_types/parameter_types.hpp>

namespace vpush {
namespace detail {

namespace ft = ::boost::function_types;

typedef void(*void_fptr_t)();

struct code_base {
	code_base(void_fptr_t f) : _fptr(f) {}
	virtual void exec() const = 0;
private:
	void_fptr_t _fptr;
};

template <typename FPTR>
struct code : code_base {
	code(FPTR f) : code_base(reinterpret_cast<void_fptr_t>(f)) {}
	typedef ft::return_type return_type;
	typedef ft::parameter_types parameter_types;
	void exec() const {
		// TODO: Boost fusion code to build a container based on parameter types,
		//		check that enough of each exist on the stacks, pop them all,
		//		pass them to the function, collect the return value, and push it
		reinterpret_cast<void_fptr_t>(_fptr)();
	}
	
};

// TODO: Function wrapper for code that wants to manipulate the stacks directly
// TODO: Function *template* wrapper, with associated type lists for instantiation

} // namespace detail
} // namespace vpush

#endif // __VPUSH_CODE_HPP__

