#ifndef __VPUSH_DETAIL_FUNCTIONS_HPP__
#define __VPUSH_DETAIL_FUNCTIONS_HPP__

#include <string>
#include <map>
#include <utility>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/member.hpp>

#include <vpush/env_fwd.hpp>
#include <vpu

namespace vpush {
namespace detail {

typedef int(*op_func_t)(Env&);

struct function_entry {
	function_entry(const std::string& n, op_func_t f) : name(n), func(f) {}
	std::string name;
	op_func_t func;
private:
	function_entry();
};

struct byName;
struct by

typedef std::map<std::string, op_func_t> functions_t;
typedef functions_t::const_iterator function_t;

} // namespace detail
} // namespace vpush

#endif // __VPUSH_DETAIL_FUNCTIONS_HPP__

