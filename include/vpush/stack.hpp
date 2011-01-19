#ifndef __VPUSH_STACK_HPP__
#define __VPUSH_STACK_HPP__

#include <ostream>
#include <boost/foreach.hpp>
#include "vpush/util/singleton.hpp"

namespace vpush {

struct stack_base {

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
	stack()
	T pop() {return _stack.pop_back();}
	void push(T t) {_stack.push_back(t);}
	std::size_t size() const {return _stack.size();}
	util::TypeInfo get_typeinfo() const {return _typeinfo;}
	friend std::ostream& operator<< <>(std::ostream&, const stack<T>&);
private:
	std::vector<T> _stack;
	util::TypeInfo _typeinfo(typeid(T));
};

namespace detail {

// Functor for boost::mpl::for_each to iterate through function parameter types
struct make_stack {
	template <class T> inline void operator()(T) {
		util::singleton<stack<T> >::instance();
	}
};

} // namespace detail

} // namespace vpush

#endif // __VPUSH_STACK_HPP__

