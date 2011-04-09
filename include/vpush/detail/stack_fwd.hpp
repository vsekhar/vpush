#ifndef __VPUSH_DETAIL_STACK_FWD_HPP__
#define __VPUSH_DETAIL_STACK_FWD_HPP__

#include <vpush/exception.hpp>
#include <vpush/protein_fwd.hpp>

namespace vpush {

namespace detail {

template <typename T>
struct stack;

}

template <typename T> inline detail::stack<T>& stack(Protein&) {
	throw detail::no_such_stack(typeid(T));
}

}

#endif

