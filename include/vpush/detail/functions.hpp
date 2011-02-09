#ifndef __VPUSH_DETAIL_FUNCTIONS_HPP__
#define __VPUSH_DETAIL_FUNCTIONS_HPP__

#include <string>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>

#include <vpush/env_fwd.hpp>
#include <vpush/detail/types.hpp>

namespace vpush {
namespace detail {

typedef signed int(*op_func_t)(Env&);

struct function_entry {
	function_entry(const std::string& n, op_func_t f, const type_container& t)
		: name(n), func(f), func_types(t) {}
	std::string name;
	op_func_t func;
	type_container func_types;
private:
	function_entry();
};

struct byName;
struct byFptr;

using namespace ::boost::multi_index;

typedef multi_index_container <
	function_entry,
	indexed_by<
		hashed_unique<
			tag<byName>,
			member<function_entry, std::string, &function_entry::name>
		>,
		hashed_unique<
			tag<byFptr>,
			member<function_entry, op_func_t, &function_entry::func>
		>
	>
> functions_container;

struct function {
	op_func_t fptr;
	type_container types;
};

class functions_t {
public:
	inline void add(const std::string& n, op_func_t f) { add(n, f, type_container()); }
	void add(const std::string&, op_func_t, const type_container&);
	function get(const std::string&);
	function get(op_func_t);
	int run(const std::string&, Env&);
	int run(op_func_t, Env&);

private:
	typedef functions_container::index<byName>::type functions_by_name;
	typedef functions_container::index<byFptr>::type functions_by_fptr;
	functions_container _container;
};

} // namespace detail
} // namespace vpush

#endif // __VPUSH_DETAIL_FUNCTIONS_HPP__

