#ifndef __VPUSH_DETAIL_STACK_FACTORY_HPP__
#define __VPUSH_DETAIL_STACK_FACTORY_HPP__

#include <vpush/detail/stack.hpp>

namespace vpush {
namespace detail {

struct stack_factory_base {
	virtual stack_base* construct() const = 0;
};

template <typename T>
struct stack_factory : stack_factory_base {
	stack_base* construct() const { return new stack<T>(); }
};

} // namespace detail
} // namespace vpush

#endif // __VPUSH_DETAIL_STACK_FACTORY_HPP__

