#include <vpush/detail/functions.hpp>
#include <vpush/exception.hpp>
#include <vpush/env.hpp>

namespace vpush {
namespace detail {

void functions_t::add(const std::string& name, op_func_t f, const type_container& t) {
	functions_by_name_mutable& funcs = _container.get<detail::byName>();
	functions_by_name_mutable::const_iterator i = funcs.find(name);
	BOOST_ASSERT(i == funcs.end());
	funcs.insert(function_entry(name, f, t));
}

function functions_t::get(const std::string& name) const {
	functions_by_name& funcs = _container.get<detail::byName>();
	functions_by_name::const_iterator i = funcs.find(name);
	if(i == funcs.end())
		throw detail::no_such_function(name);
	function f;
	f.fptr = i->func;
	f.types = i->func_types;
	return f;
}

function functions_t::get(op_func_t fptr) const {
	functions_by_fptr& funcs = _container.get<detail::byFptr>();
	functions_by_fptr::const_iterator i = funcs.find(fptr);
	if(i == funcs.end())
		throw detail::no_such_function("(binary fptr)");
	function f;
	f.fptr = i->func;
	f.types = i->func_types;
	return f;
}

int functions_t::run(const std::string& name, Env& env) const {
	function f = get(name);
	env.stacks.check(f.types);
	return f.fptr(env);
}

int functions_t::run(op_func_t fptr, Env& env) const {
	function f = get(fptr);
	env.stacks.check(f.types);
	return f.fptr(env);
}


} // namespace detail
} // namespace vpush

