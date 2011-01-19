#ifndef __VPUSH_REGISTER_HPP__
#define __VPUSH_REGISTER_HPP__

#include <boost/function_types/result_type.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/mpl/for_each.hpp>

#include "vpush/stack.hpp"

namespace vpush {

template <typename FPTR>
void register_(const FPTR f, std::string) {
	namespace ft = ::boost::function_types;
	namespace mpl = ::boost::mpl;
	typedef typename ft::result_type<FPTR>::type result_type;
	typedef typename ft::parameter_types<FPTR>::type parameter_types;
	util::singleton<stack<result_type> >::instance();
	mpl::for_each<parameter_types> (detail::make_stack());
}

} // namespace vpush

#endif // __VPUSH_REGISTER_HPP__

