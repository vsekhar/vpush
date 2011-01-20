#ifndef __VPUSH_REGISTER_HPP__
#define __VPUSH_REGISTER_HPP__

#include <boost/function_types/result_type.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/mpl/for_each.hpp>

#include <vpush/stack.hpp>
#include <vpush/stacks.hpp>
#include <vpush/code.hpp>

namespace vpush {

// Functor for boost::mpl::for_each to iterate through function parameter types
struct instantiate_stack {
	template <class T> inline void operator()(T) { stack<T>(); }
};

template <typename FPTR>
void register_(const FPTR f, std::string) {
	// Ensure appropriate type stacks are created
	namespace ft = ::boost::function_types;
	namespace mpl = ::boost::mpl;
	typedef typename ft::result_type<FPTR>::type result_type;
	typedef typename ft::parameter_types<FPTR>::type parameter_types;
	stack<result_type>();
	mpl::for_each<parameter_types> (instantiate_stack());
	
	// Register function
	
}

} // namespace vpush

#endif // __VPUSH_REGISTER_HPP__

