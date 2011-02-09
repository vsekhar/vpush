#include <map>
#include <utility>

#include <boost/foreach.hpp>

#include <vpush/detail/stacks.hpp>
#include <vpush/util/typeinfo.hpp>

namespace vpush {
namespace detail {

stacks_t::stacks_t() : _stacks() {
	// make code and exec stacks
}

stacks_t::stacks_t(const stacks_t& s) : _stacks(s._stacks) {
	// make code and exec stacks
}

void stacks_t::check(const detail::type_container& t) {
	std::map<util::TypeInfo, std::size_t> type_count;
	BOOST_FOREACH(const util::TypeInfo& type, t._types)
		++type_count[type];

	typedef std::pair<util::TypeInfo, std::size_t> map_element;
	BOOST_FOREACH(const map_element& e, type_count) {
		stack_container_t::const_iterator i = _stacks.find(e.first);
		if(i == _stacks.end())
			throw detail::no_such_stack(e.first);
		if(i->second->size() < e.second)
			throw detail::stack_underflow(e.first);
	}
}

} // namespace detail
} // namespace vpush

