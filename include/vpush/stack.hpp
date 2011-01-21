#ifndef __VPUSH_STACK_HPP__
#define __VPUSH_STACK_HPP__

#include <ostream>
#include <list>

#include <boost/foreach.hpp>
#include <vpush/stacks.hpp>
#include <vpush/util/typeinfo.hpp>
#include <vpush/util/singleton.hpp>

namespace vpush {
namespace detail {

struct stack_base {
	stack_base(const util::TypeInfo &t) : _typeinfo(t) {
		stacks()[t] = this;
	}
	virtual std::size_t size() const = 0;
private:
	util::TypeInfo _typeinfo;
};

template <typename> struct stack;

template <typename T>
std::ostream& operator<<(std::ostream& o, const stack<T>& s){
	bool first = true;
	o << '[';
	BOOST_FOREACH(const T& t, s) {
		o << t;
		if(!first) o << ", ";
		first = false;
	}
	return o;
}

template <typename T>
struct stack : stack_base {
	stack() : stack_base(typeid(T)) {}
	inline void push(T t) { _stack.push_back(t); }
	inline T pop() { T ret = _stack.back(); _stack.pop_back(); return ret; }
	inline T top() const { return _stack.back(); }
	inline T second() const { return *(++_stack.rbegin()); }
	inline std::size_t size() const { return _stack.size(); }
	inline bool empty() const { return _stack.empty(); }
	void clear() { _stack.clear(); }

	// for combinatorial operators
	// warning: these are undefined if the stack underflows. caller is responsible
	// for checking stack<>.size() >= 1;
	inline void push_second(T t) { _stack.insert(++_stack.rbegin(), t); }
	inline T pop_second() {
		std::list<T>::const_iterator second = ++_stack.rbegin();
		T ret = *second;
		_stack.erase(second);
		return ret;
	}

	friend std::ostream& operator<< <>(std::ostream&, const stack<T>&);
private:
	std::list<T> _stack;
};

template <typename T>
struct make_stack_t { typedef stack<T> type; };

template <typename T>
struct make_stack_singleton_t {
	typedef util::singleton<typename make_stack_t<T>::type > type;
};

} // namespace detail

// Short-hand for compile-type stacks by type, e.g. vpush::stack<int>().pop()
template <typename T>
inline typename detail::make_stack_t<T>::type& stack() {
	typedef typename detail::make_stack_singleton_t<T>::type singleton_t;
	return static_cast<typename detail::make_stack_t<T>::type&>(singleton_t::instance());
}

// Simple access
//	push(5);				// auto-detect type int
//	push<int>('a');			// perform conversion char --> int
//	int i = top<int>();		// value remains on stack
//	assert(size<int>() == 1);
//	i = pop<int>();			// value removed from stack
//	assert(size<int>() == 0);

template <typename T> inline void push(T t) { stack<T>().push(t); }
template <typename T> inline T pop() { return stack<T>().pop(); }
template <typename T> inline T top() { return stack<T>().top(); }
template <typename T> inline std::size_t size() { return stack<T>().size(); }
template <typename T> inline bool empty() { return stack<T>().empty(); }
template <typename T> inline void clear() { stack<T>().clear(); }

// Non-standard stack manipulation
//	push(42);
//	assert(!empty<int>());	// caller is responsible for checking
//	push_second(24);			// push under top element, undefined if stack is empty
//	assert(top<int>() == 42);
//	assert(second<int>() == 24);

template <typename T> inline void push_second(T t) { stack<T>().push_second(t); }
template <typename T> inline T pop_second() { return stack<T>().pop_second(); }
template <typename T> inline T second() { return stack<T>().second(); }
} // namespace vpush

#endif // __VPUSH_STACK_HPP__

