#ifndef __vpush_env_hpp__
#define __vpush_env_hpp__

#include <vector>

#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>

#include <vpush/detail/stack.hpp>
#include <vpush/exception.hpp>

namespace vpush {

namespace mpl = ::boost::mpl;

template <typename> struct wrap {};

struct Env {

	void reset() {
		bool_stack.clear();
		int_stack.clear();
		double_stack.clear();
		// code_stack.clear();
		// exec_stack.clear();
		// name_stack.clear();
	}

	// stacks
	detail::stack<bool> bool_stack;
	detail::stack<int> int_stack;
	detail::stack<double> double_stack;
	// detail::stack<Code> code_stack;
	// detail::stack<Exec> exec_stack;
	// detail::stack<Name> name_stack;
private:
	friend class ::boost::serialization::access;
	template <class A> void serialize(A& a, unsigned int) {
		a & bool_stack;
		a & int_stack;
		a & double_stack;
		// ar & code_stack;
		// ar & exec_stack;
		// ar & name_stack;
	}

}; // struct Env

// stack access
template <typename T> inline detail::stack<T>& get_stack(Env&) {
	throw detail::no_such_stack(typeid(T));
}
template <> inline detail::stack<bool>& get_stack(Env& e) { return e.bool_stack; }
template <> inline detail::stack<int>& get_stack(Env& e) { return e.int_stack; }
template <> inline detail::stack<double>& get_stack(Env& e) { return e.double_stack; }
// template <> inline detail::stack<Code>& get_stack(Env& e) { return e.code_stack; }
// template <> inline detail::stack<Exec>& get_stack(Env& e) { return e.exec_stack; }
// template <> inline detail::stack<Name>& get_stack(Env& e) { return e.name_stack; }

/* stack-wide operations queries & operations */
template <typename T> inline std::size_t size(Env& e) { return get_stack<T>(e).size(); }
template <typename T> inline bool empty(Env& e) { return get_stack<T>(e).empty(); }
template <typename T> inline void clear(Env& e) { get_stack<T>(e).clear(); }

/* non-modifying element access */
template <typename T> inline T& top(Env& e) { return get_stack<T>(e).back(); }
template <class T>    inline T& first(Env& e) { return top<T>(e); }
template <class T>    inline T& second(Env& e) { return *(++get_stack<T>(e).rbegin()); }

/* modifying element access (pushing and popping) */
template <typename T> inline void push(Env& e, const T& v) { get_stack<T>(e).push_back(v); }

template <typename T>
inline T pop(Env& e) {
	detail::stack<T>& stack = get_stack<T>(e);
	if(stack.empty()) throw detail::stack_underflow(typeid(T));
	T ret(stack.back());
	stack.pop_back();
	return ret;
}

/* non-standard stack operations (for combinatorics operators) */
template <typename T>
void inline push_second(Env& e, const T& t) {
	detail::stack<T>& stack = get_stack<T>(e);
	typename detail::stack<T>::reverse_iterator itr = stack.rbegin();
	if(!stack.empty()) itr++;
	stack.insert(itr.base(), t);
}

template <typename T>
inline T pop_second(Env& e) {
	detail::stack<T>& stack = get_stack<T>(e);
	if(stack.size() < 2) throw detail::stack_underflow(typeid(T));

	// some funny iterator arithmetic for converting between
	// reverse and forward iterators
	typename detail::stack<T>::reverse_iterator second
		= ++stack.rbegin();
	T ret = *second;
	stack.erase(--second.base());
	return ret;
}

} // namespace vpush

#endif

