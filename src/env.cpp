#include <boost/foreach.hpp>

#include <vpush/env.hpp>

namespace vpush {

void Env::check_stacks(const detail::type_container& t) {
	std::map<util::TypeInfo, unsigned int> type_count;
	BOOST_FOREACH(const util::TypeInfo& type, t._types)
		++type_count[type];

	typedef std::pair<util::TypeInfo, std::size_t> map_element;
	BOOST_FOREACH(const map_element& e, type_count) {
		detail::stacks_t::const_iterator i = stacks.find(e.first);
		if(i == stacks.end())
			throw detail::stack_underflow(e.first);
		if(i->second->size() < e.second)
			throw detail::stack_underflow(e.first);
	}
}

} // namespace vpush

