#ifndef __VPUSH_DETAIL_FUNCTIONS_HPP__
#define __VPUSH_DETAIL_FUNCTIONS_HPP__

#include <string>
#include <map>
#include <utility>

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
> functions_t;

typedef functions_t::index<byName>::type functions_by_name;
typedef functions_t::index<byFptr>::type functions_by_type;

} // namespace detail
} // namespace vpush

#endif // __VPUSH_DETAIL_FUNCTIONS_HPP__

