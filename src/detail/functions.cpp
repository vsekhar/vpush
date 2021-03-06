#include <sstream>

#include <boost/foreach.hpp>
#include <boost/assert.hpp>
#include <boost/functional/hash.hpp>

#include <vpush/detail/functions.hpp>
#include <vpush/detail/code.hpp>
#include <vpush/detail/types.hpp>
#include <vpush/exception.hpp>
#include <vpush/util/random.hpp>

namespace vpush {
namespace detail {

function_entry::function_entry(const std::string& n, op_func_t f, const type_container& t)
	: name(n), func(f), func_types(t) {}

void functions_t::add(const std::string& n, op_func_t f) {
	add(n, f, type_container());
}

void functions_t::add(const std::string& name, op_func_t f, const type_container& t) {
	functions_by_name_mutable& funcs = _container.get<byName>();
	functions_by_name_mutable::const_iterator i = funcs.find(name);
	if(i != funcs.end())
		throw duplicate_code(name);
	funcs.insert(function_entry(name, f, t));
}

op_func_t functions_t::get_fptr(const std::string& name) const {
	functions_by_name& funcs = _container.get<byName>();
	functions_by_name::const_iterator i = funcs.find(name);
	if(i == funcs.end())
		throw no_such_function(name);
	return i->func;
}

type_container functions_t::get_types(const op_func_t& fptr) const {
	functions_by_fptr& funcs = _container.get<byFptr>();
	functions_by_fptr::const_iterator i = funcs.find(fptr);
	if(i == funcs.end()) {
		std::stringstream ss;
		ss << fptr;
		throw no_such_function(ss.str());
	}
	return i->func_types;
}

std::string functions_t::get_name(const Code& c) const {
	if(c.type == Code::OPEN)
		return "(";
	else if (c.type == Code::CLOSE)
		return ")";
	else {
		functions_by_fptr& funcs = _container.get<byFptr>();
		functions_by_fptr::const_iterator i = funcs.find(c.fptr);
		if(i == funcs.end()) {
			std::stringstream ss;
			ss << c.fptr;
			throw no_such_function(ss.str());
		}
		return i->name;
	}
}

Exec functions_t::get_code(const std::string& name) const {
	if(name == "(")
		return Exec(Exec::OPEN);
	else if(name == ")")
		return Exec(Exec::CLOSE);
	else
		return Exec(get_fptr(name));
}

Exec functions_t::get_random() const {
#ifdef _DEBUG
	BOOST_ASSERT(_container.size());
#endif
	util::RandomInt ri(0, _container.size()-1);
	return _container.get<byRAC>()[ri()].func;
}

bool functions_t::contains(const std::string& name) const {
	functions_by_name& funcs = _container.get<byName>();
	return funcs.find(name) != funcs.end();
}

bool functions_t::contains(op_func_t fptr) const {
	functions_by_fptr& funcs = _container.get<byFptr>();
	return funcs.find(fptr) != funcs.end();
}

bool functions_t::is_superset_of(const functions_t& other) const {
	functions_by_seq& f2 = other._container.get<bySeq>();
	BOOST_FOREACH(const function_entry& fe, f2)
		if(!contains(fe.name) || !contains(fe.func))
			return false;
	return true;
}

std::size_t functions_t::hash() const {
	std::size_t seed = 0;
	functions_by_ordered_name& funcs = _container.get<byOrderedName>();
	BOOST_FOREACH(const function_entry& fe, funcs)
		boost::hash_combine(seed, fe.name);
	return seed;
}

} // namespace detail

detail::functions_t functions;

} // namespace vpush

