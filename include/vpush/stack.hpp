#ifndef __VPUSH_STACK_HPP__
#define __VPUSH_STACK_HPP__

#include <ostream>
#include <boost/foreach.hpp>
#include "vpush/util/typeinfo.hpp"
#include "vpush/stacks.hpp"

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
	inline T pop() {T ret = _stack.back(); _stack.pop_back(); return ret;}
	inline void push(T t) {_stack.push_back(t);}
	inline std::size_t size() const { return _stack.size(); }
	void reserve(std::size_t s) { _stack.reserve(s); }
	std::size_t capacity() const { return _stack.capacity(); }
	void clear() { _stack.clear(); }
	friend std::ostream& operator<< <>(std::ostream&, const stack<T>&);
private:
	std::vector<T> _stack;
};

template <typename T>
struct make_stack_t { typedef stack<T> type; };

template <typename T>
struct make_stack_singleton_t {
	typedef util::singleton<typename make_stack_t<T>::type > type;
};

} // namespace detail

// Enable simplified access to the stack, e.g. vpush::stack<int>().pop()
template <typename T>
inline typename detail::make_stack_t<T>::type& stack() {
	typedef typename detail::make_stack_singleton_t<T>::type singleton_t;
	return static_cast<typename detail::make_stack_t<T>::type&>(singleton_t::instance());
}

// Enable even simpler access for pushing and popping, e.g. int i = vpush::pop<int>();
template <typename T>
inline T pop() { return stack<T>().pop(); }

template <typename T>
inline void push(T t) { stack<T>().push(t); }

} // namespace vpush

#endif // __VPUSH_STACK_HPP__

