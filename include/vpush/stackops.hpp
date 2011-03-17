#ifndef __VPUSH_STACKOPS_HPP__
#define __VPUSH_STACKOPS_HPP__

#include <vpush/detail/stack.hpp>
#include <vpush/detail/codestack.hpp>

namespace vpush {

template <> inline detail::stack<bool>& get_stack(Protein& p) { return p.bool_stack; }
template <> inline detail::stack<int>& get_stack(Protein& p) { return p.int_stack; }
template <> inline detail::stack<double>& get_stack(Protein& p) { return p.double_stack; }
template <> inline detail::stack<detail::Code>& get_stack(Protein& p) { return p.code_stack; }
template <> inline detail::stack<detail::Exec>& get_stack(Protein& p) { return p.exec_stack; }
// template <> inline detail::stack<Name>& get_stack(Protein& p) { return p.name_stack; }

/* stack-wide operations queries & operations */
template <typename T> inline std::size_t size(Protein& p) { return get_stack<T>(p).size(); }
template <typename T> inline bool empty(Protein& p) { return get_stack<T>(p).empty(); }
template <typename T> inline void clear(Protein& p) { get_stack<T>(p).clear(); }

/* non-modifying element access */
template <typename T> inline T& top(Protein& p) { return get_stack<T>(p).back(); }
template <class T>    inline T& first(Protein& p) { return top<T>(p); }
template <class T>    inline T& second(Protein& p) { return *(++get_stack<T>(p).rbegin()); }

/* modifying element access (pushing and popping) */
template <typename T> inline void push(Protein& p, const T& v) { get_stack<T>(p).push_back(v); }

template <typename T>
inline T pop(Protein& p) {
	detail::stack<T>& stack = get_stack<T>(p);
	if(stack.empty()) throw detail::stack_underflow(typeid(T));
	T ret(stack.back());
	stack.pop_back();
	return ret;
}

/* non-standard stack operations (for combinatorics operators) */
template <typename T>
void inline push_second(Protein& p, const T& t) {
	detail::stack<T>& stack = get_stack<T>(p);
	typename detail::stack<T>::reverse_iterator itr = stack.rbegin();
	if(!stack.empty()) itr++;
	stack.insert(itr.base(), t);
}

template <typename T>
inline T pop_second(Protein& p) {
	detail::stack<T>& stack = get_stack<T>(p);
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

