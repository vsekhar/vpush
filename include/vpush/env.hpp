#ifndef __vpush_env_hpp__
#define __vpush_env_hpp__

#include <string>
#include <sstream>
#include <ostream>

#include <boost/assert.hpp>
#include <boost/assign/ptr_map_inserter.hpp>
#include <boost/serialization/access.hpp>

#include <vpush/env_fwd.hpp>
#include <vpush/exception.hpp>
#include <vpush/detail/stack.hpp>
#include <vpush/detail/types.hpp>
#include <vpush/util/typeinfo.hpp>


namespace vpush {

struct Env {
	Env() : stacks() {}
	Env(const Env& e) : stacks(e.stacks) {}

	template <typename T>
	inline void make_stack() {
		boost::assign::ptr_map_insert<typename detail::stack<T> >(stacks)(typeid(T));
	}

	template <typename T>
	inline void push(const T& t) { get_stack<T>().push_back(t); }

	template <typename T>
	inline void push_second(const T& t) {
		detail::stack<T>& cur_stack = get_stack<T>();
		typename detail::stack<T>::reverse_iterator itr = cur_stack.rbegin();
		if(!cur_stack.empty()) itr++;
		cur_stack.insert(itr.base(), t);
	}

	template <typename T>
	inline T pop() {
		detail::stack<T>& stack = get_stack<T>();
		if(stack.empty()) throw detail::stack_underflow(typeid(T));
		T ret(stack.back());
		stack.pop_back();
		return ret;
	}

	template <typename T>
	inline T pop_second() {
		detail::stack<T>& stack = get_stack<T>();
		if(stack.size() < 2) throw detail::stack_underflow(typeid(T));

		// some funny iterator arithmetic for converting between
		// reverse and forward iterators
		typename detail::stack<T>::reverse_iterator second
			= ++stack.rbegin();
		T ret = *second;
		stack.erase(--second.base());
		return ret;
	}

	template <typename T>
	inline T top() const { return get_stack<T>().back(); }

	template <typename T>
	inline T second() const { return *(++get_stack<T>().rbegin()); }

	template <typename T>
	inline std::size_t size() const { return get_stack<T>().size(); }

	template <typename T>
	inline bool empty() const { return get_stack<T>().empty(); }

	template <typename T>
	inline void clear() { get_stack<T>().clear(); }
	
	template <typename T>
	std::ostream& list_stack(std::ostream& o) const {
		bool first = true;
		const typename detail::stack<T>& stack = get_stack<T>();
		typename detail::stack<T>::const_reverse_iterator i = stack.rbegin();
		for(typename detail::stack<T>::const_reverse_iterator i = stack.rbegin();
			i != stack.rend();
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
	std::string list_stack() const {
		std::stringstream ss;
		list_stack<T>(ss);
		return ss.str();
	}
	
	void check_stacks(const detail::type_container&);


private:
//	friend ::boost::serialization::access;
	
	template <typename ARCHIVE>
	void serialize(ARCHIVE & ar, const unsigned int) {
		ar & stacks;
	}
	template <typename T>
	inline detail::stack<T>& get_stack() {
		detail::stacks_t::iterator stack = stacks.find(typeid(T));
	#ifdef _DEBUG
		if(stack == stacks.end())
			throw detail::no_such_stack(typeid(T));
		BOOST_ASSERT((stack->first) == util::TypeInfo(typeid(T)));
	#endif
		return static_cast<typename detail::stack<T>& >(*stack->second);
	}

	template <typename T>
	inline const detail::stack<T>& get_stack() const {
		detail::stacks_t::const_iterator stack = stacks.find(typeid(T));
	#ifdef _DEBUG
		if(stack == stacks.end())
			throw detail::no_such_stack(typeid(T));
		BOOST_ASSERT((stack->first) == util::TypeInfo(typeid(T)));
	#endif
		return static_cast<const typename detail::stack<T>& >(*stack->second);
	}
	
	detail::stacks_t stacks;
};

} // namespace vpush

#endif

