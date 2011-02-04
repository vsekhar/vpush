#include <vpush/env.hpp>

namespace vpush {

void Env::register_(std::string name, detail::op_func_t f) {
	detail::functions_t::const_iterator i = functions.find(name);
	BOOST_ASSERT(i == functions.end());
	functions[name] = f;
}

void Env::check_stacks(const detail::types& t) {
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

