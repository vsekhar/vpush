#ifndef __vpush_env_hpp__
#define __vpush_env_hpp__

#include <string>
#include <sstream>
#include <list>
#include <map>
#include <stdexcept>

#include <boost/assert.hpp>
#include <boost/assign/ptr_map_inserter.hpp>

#include <vpush/env_fwd.hpp>
#include <vpush/exception.hpp>
#include <vpush/detail/stack.hpp>
#include <vpush/detail/functions.hpp>
#include <vpush/detail/types.hpp>
#include <vpush/util/typeinfo.hpp>


namespace vpush {

struct Env {
	Env() : stacks() {}
	Env(const Env& e) : stacks(e.stacks), functions(e.functions) {}

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
	std::string list_stack() const {
		std::stringstream ss;
		bool first = true;
		const typename detail::stack<T>& stack = get_stack<T>();
		typename detail::stack<T>::const_reverse_iterator i = stack.rbegin();
		while(i != stack.rend()) {
			if(!first)
				ss << ", ";
			else
				first = false;
			ss << *i;
			i++;
		}
		return ss.str();
	}

	void register_(std::string, detail::op_func_t);
	void check_stacks(const detail::types&);


private:
	template <typename T>
	inline detail::stack<T>& get_stack() {
		detail::stacks_t::iterator stack = stacks.find(typeid(T));
	#ifdef _DEBUG
		BOOST_ASSERT(stack != stacks.end());
		BOOST_ASSERT((stack->first) == util::TypeInfo(typeid(T)));
	#endif
		return static_cast<typename detail::stack<T>& >(*stack->second);
	}

	template <typename T>
	inline const detail::stack<T>& get_stack() const {
		detail::stacks_t::const_iterator stack = stacks.find(typeid(T));
	#ifdef _DEBUG
		BOOST_ASSERT(stack != stacks.end());
		BOOST_ASSERT((stack->first) == util::TypeInfo(typeid(T)));
	#endif
		return static_cast<const typename detail::stack<T>& >(*stack->second);
	}
	
	detail::stacks_t stacks;
	detail::functions_t functions;
};

} // namespace vpush

#endif

