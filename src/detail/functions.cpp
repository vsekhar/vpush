#include <vpush/detail/functions.hpp>
#include <vpush/exception.hpp>

namespace vpush {
namespace detail {

void functions_t::add(const std::string& name, op_func_t f, const type_container& t) {
	functions_by_name& funcs = _container.get<detail::byName>();
	functions_by_name::const_iterator i = funcs.find(name);
	BOOST_ASSERT(i == funcs.end());
	funcs.insert(function_entry(name, f, t));
}

function functions_t::get(const std::string& name) {
	functions_by_name& funcs = _container.get<detail::byName>();
	functions_by_name::const_iterator i = funcs.find(name);
	if(i == funcs.end())
		throw detail::no_such_function(name);
	function f;
	f.fptr = i->func;
	f.types = i->func_types;
	return f;
}

function functions_t::get(op_func_t fptr) {
	functions_by_fptr& funcs = _container.get<detail::byFptr>();
	functions_by_fptr::const_iterator i = funcs.find(fptr);
	if(i == funcs.end())
		throw detail::no_such_function("(binary fptr)");
	function f;
	f.fptr = i->func;
	f.types = i->func_types;
	return f;
}


} // namespace detail
} // namespace vpush

