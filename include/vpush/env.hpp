#ifndef __vpush_env_hpp__
#define __vpush_env_hpp__

#include <string>

#include <boost/serialization/access.hpp>

#include <vpush/env_fwd.hpp>
#include <vpush/exception.hpp>
#include <vpush/detail/stacks.hpp>
#include <vpush/detail/types.hpp>
#include <vpush/util/typeinfo.hpp>


namespace vpush {

struct Env {
	Env() : stacks() {}
	Env(const Env& e) : stacks(e.stacks) {}

	///////////////////////////////////////////////////////////////////////////
	// Stack functions (for convenience in user code)
	///////////////////////////////////////////////////////////////////////////
	
	template <typename T>
	inline void push(const T& t) { stacks.get<T>().push_back(t); }

	template <typename T>
	inline void push_second(const T& t) {
		detail::stack<T>& cur_stack = stacks.get<T>();
		typename detail::stack<T>::reverse_iterator itr = cur_stack.rbegin();
		if(!cur_stack.empty()) itr++;
		cur_stack.insert(itr.base(), t);
	}

	template <typename T>
	inline T pop() {
		detail::stack<T>& stack = stacks.get<T>();
		if(stack.empty()) throw detail::stack_underflow(typeid(T));
		T ret(stack.back());
		stack.pop_back();
		return ret;
	}

	template <typename T>
	inline T pop_second() {
		detail::stack<T>& stack = stacks.get<T>();
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
	inline T top() const { return stacks.get<T>().back(); }

	template <typename T>
	inline T second() const { return *(++stacks.get<T>().rbegin()); }

	template <typename T>
	inline std::size_t size() const { return stacks.get<T>().size(); }

	template <typename T>
	inline bool empty() const { return stacks.get<T>().empty(); }

	template <typename T>
	inline void clear() { stacks.get<T>().clear(); }
	
	detail::stacks_t stacks;

private:
	friend class ::boost::serialization::access;
	template <typename ARCHIVE>
	void serialize(ARCHIVE & ar, const unsigned int) {
		ar & stacks;
	}
	
};

} // namespace vpush

#endif

