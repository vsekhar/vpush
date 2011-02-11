#include <map>
#include <utility>

#include <boost/foreach.hpp>
#include <boost/assign/ptr_map_inserter.hpp>

#include <vpush/detail/stacks.hpp>
#include <vpush/util/typeinfo.hpp>

namespace vpush {
namespace detail {

stacks_t::stacks_t() {
	const stack_factory_container_t& sc = stack_factories::instance();
	BOOST_FOREACH(const stack_base& s, sc)
		_stacks.insert(s.construct());
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

