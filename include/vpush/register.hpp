#ifndef __VPUSH_REGISTER_HPP__
#define __VPUSH_REGISTER_HPP__

#include <vpush/code.hpp>
#include <vpush/codes.hpp>
#include <vpush/literal.hpp>
#include <vpush/exception.hpp>

namespace vpush {

namespace ft = ::boost::function_types;
namespace mpl = ::boost::mpl;

void check_for_dupes(const std::string& name, detail::basic_code* ptr) {
	if(codes.find(name) != codes.end())
		throw duplicate_code(name);
	if(codenames.find(ptr) != codes.end())
		throw duplicate_code(name);
}

template <typename FPTR>
void register_(FPTR f, std::string name) {
	typename detail::basic_code<FPTR>* new_code
		= new typename detail::basic_code<FPTR>(f);
	check_for_dupes(name, new_code);
	codes[name] = new_code;
	codenames[new_code] = name;
}

void register_adv(detail::void_fptr_t f, const detail::type_checker& tc, std::string name) {
	detail::code_base* new_code = new detail::code_base(f, tc);
	check_for_dupes(name, new_code);
	codes[name] = new_code;
	codenames[new_code] = name;
}

template <typename STATE, typename RET = STATE>
void register_literal(const STATE& i, std::string name) {
	typename detail::literal<STATE>* new_literal
		= new typename detail::literal<STATE>(i);
	check_for_dupes(name, new_literal);
	codes[name] = new_literal;
	codenames[new_literal] = name;
}

std::string get_name(const code& c) {
	return codenames[c.get_base_ptr()];
}

code get_code(const std::string& name) {
	return *codes[name];
}

} // namespace vpush

#endif // __VPUSH_REGISTER_HPP__

