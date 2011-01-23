#ifndef __VPUSH_POPPER_HPP__
#define __VPUSH_POPPER_HPP__

#include <vpush/stack.hpp>

namespace vpush {
namespace detail {

//Function object for popping the stack
template <typename T>
struct popper {
	operator T() {return pop<T>();}
};

} // namespace detail
} // namespace vpush

#endif

