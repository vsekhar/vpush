#ifndef __VPUSH_DETAIL_STACKS_HPP__
#define __VPUSH_DETAIL_STACKS_HPP__

#include <ostream>
#include <sstream>

#include <boost/assert.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <boost/ptr_container/serialize_ptr_map.hpp>
#include <boost/assign/ptr_map_inserter.hpp>
#include <boost/serialization/access.hpp>

#include <vpush/detail/stack.hpp>
#include <vpush/detail/types.hpp>
#include <vpush/exception.hpp>
#include <vpush/util/singleton.hpp>

namespace vpush {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
// Run-time stack containers (embedded in Env/Org objects)
///////////////////////////////////////////////////////////////////////////////

struct stacks_t {

	template <typename T>
	inline void make() {
		boost::assign::ptr_map_insert<stack<T> >(_stacks)(typeid(T));
	}
	
	std::size_t count() const { return _stacks.size(); }
	
	template <typename T>
	std::ostream& list(std::ostream& o) const {
		bool first = true;
		const stack<T>& s = get<T>();
		for(typename stack<T>::const_reverse_iterator i = s.rbegin();
			i != s.rend();
			i++)
		{
			if(!first)
				o << " ";
			else
				first = false;
			o << *i;
		}
		return o;
	}

	template <typename T>
	std::string list() const {
		std::stringstream ss;
		list<T>(ss);
		return ss.str();
	}

	template <typename T>
	inline stack<T>& get() {
		stack_container_t::iterator s = _stacks.find(typeid(T));
	#ifdef _DEBUG
		if(s == _stacks.end())
			throw detail::no_such_stack(typeid(T));
		BOOST_ASSERT((s->first) == util::TypeInfo(typeid(T)));
	#endif
		return static_cast<stack<T>& >(*s->second);
	}
	
	template <typename T>
	inline const stack<T>& get() const {
		stack_container_t::const_iterator s = _stacks.find(typeid(T));
	#ifdef _DEBUG
		if(s == _stacks.end())
			throw detail::no_such_stack(typeid(T));
		BOOST_ASSERT((s->first) == util::TypeInfo(typeid(T)));
	#endif
		return static_cast<const detail::stack<T>& >(*s->second);
	}

	void check(const detail::type_container&);

private:
	typedef boost::ptr_map<util::TypeInfo, stack_base> stack_container_t;
	stack_container_t _stacks;
	//stack<exec_type> _exec_stack;
	//stack<code_type> _code_stack;
	
	friend class ::boost::serialization::access;
	
	template <typename ARCHIVE>
	void save(ARCHIVE & ar, const unsigned int) const {
		const std::size_t stack_count = _stacks.size();
		ar << stack_count;
		for(stack_container_t::const_iterator i = _stacks.begin();
			i != _stacks.end();
			i++
		) {
			const stack_base* s = i->second;
			ar << s;
		}
		
	}
	
	template <typename ARCHIVE>
	void load(ARCHIVE & ar, const unsigned int) {
		std::size_t stack_count;
		ar >> stack_count;
		for(std::size_t i = 0; i < stack_count; i++) {
			stack_base* s;
			ar >> s;
			_stacks.insert(s->get_type(), s);
		}
	}
	
	BOOST_SERIALIZATION_SPLIT_MEMBER()
};

} // namespace detail
} // namespace vpush

#endif

