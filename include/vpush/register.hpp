#ifndef __VPUSH_REGISTER_HPP__
#define __VPUSH_REGISTER_HPP__

#include <boost/function_types/function_arity.hpp>
#include <boost/static_assert.hpp>

#include <vpush/code.hpp>
#include <vpush/codes.hpp>

namespace vpush {

namespace ft = ::boost::function_types;
namespace mpl = ::boost::mpl;

template <typename FPTR>
void register_(FPTR f, std::string name) {
	codes[name] = new typename detail::basic_code<FPTR>(f);
}

template <typename FPTR>
void register_adv(FPTR f, const detail::type_checker& tc, std::string name) {
	BOOST_STATIC_ASSERT(!ft::function_arity<FPTR>::value);
	codes[name] = new typename detail::stack_code<FPTR>(f);
}

} // namespace vpush

#endif // __VPUSH_REGISTER_HPP__

