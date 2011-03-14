#ifndef __VPUSH_STACKOPS_HPP__
#define __VPUSH_STACKOPS_HPP__

#include <vpush/detail/stack.hpp>
#include <vpush/detail/codestack.hpp>

namespace vpush {

template <> inline detail::stack<bool>& get_stack(Protein& e) { return e.bool_stack; }
template <> inline detail::stack<int>& get_stack(Protein& e) { return e.int_stack; }
template <> inline detail::stack<double>& get_stack(Protein& e) { return e.double_stack; }
template <> inline detail::stack<detail::Code>& get_stack(Protein& e) { return e.code_stack; }
template <> inline detail::stack<detail::Exec>& get_stack(Protein& e) { return e.exec_stack; }
// template <> inline detail::stack<Name>& get_stack(Protein& e) { return e.name_stack; }

/* stack-wide operations queries & operations */
template <typename T> inline std::size_t size(Protein& e) { return get_stack<T>(e).size(); }
template <typename T> inline bool empty(Protein& e) { return get_stack<T>(e).empty(); }
template <typename T> inline void clear(Protein& e) { get_stack<T>(e).clear(); }

/* non-modifying element access */
template <typename T> inline T& top(Protein& e) { return get_stack<T>(e).back(); }
template <class T>    inline T& first(Protein& e) { return top<T>(e); }
template <class T>    inline T& second(Protein& e) { return *(++get_stack<T>(e).rbegin()); }

/* modifying element access (pushing and popping) */
template <typename T> inline void push(Protein& e, const T& v) { get_stack<T>(e).push_back(v); }

template <typename T>
inline T pop(Protein& e) {
	detail::stack<T>& stack = get_stack<T>(e);
	if(stack.empty()) throw detail::stack_underflow(typeid(T));
	T ret(stack.back());
	stack.pop_back();
	return ret;
}

/* non-standard stack operations (for combinatorics operators) */
template <typename T>
void inline push_second(Protein& e, const T& t) {
	detail::stack<T>& stack = get_stack<T>(e);
	typename detail::stack<T>::reverse_iterator itr = stack.rbegin();
	if(!stack.empty()) itr++;
	stack.insert(itr.base(), t);
}

template <typename T>
inline T pop_second(Protein& e) {
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

