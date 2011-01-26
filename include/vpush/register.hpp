#ifndef __VPUSH_REGISTER_HPP__
#define __VPUSH_REGISTER_HPP__

#include <vpush/code.hpp>
#include <vpush/codes.hpp>
#include <vpush/literal.hpp>

namespace vpush {

namespace ft = ::boost::function_types;
namespace mpl = ::boost::mpl;

template <typename FPTR>
void register_(FPTR f, std::string name) {
	typename detail::basic_code<FPTR>* new_code
		= new typename detail::basic_code<FPTR>(f);
	// TODO: Check for dupes
	codes[name] = new_code;
	codenames[new_code->get_fptr()] = name;
}

void register_adv(detail::void_fptr_t f, const detail::type_checker& tc, std::string name) {
	detail::code_base* new_code = new detail::code_base(f, tc);
	// TODO: Check for dupes
	codes[name] = new_code;
	codenames[new_code->get_fptr()] = name;
}

template <typename STATE, typename RET = STATE>
void register_literal(const STATE& i, std::string name) {
	typename detail::literal<STATE, RET>* new_literal
		= new typename detail::literal<STATE, RET>(i);
	// TODO: Check for dupes
	codes[name] = new_literal;
	codenames[new_literal->get_fptr()] = name;
}

std::string get_name(const code& c) {
	return codenames[c.get_fptr()];
}

code get_code(const std::string& name) {
	return *codes[name];
}

} // namespace vpush

#endif // __VPUSH_REGISTER_HPP__

